
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include <linux/device.h>

#define MAX_SIZE          0x1000
#define MEM_CLEAR         0x1
#define MEM_PRINT         0x2
#define DEVICE_NUM        4

static struct class *class;
static struct device *classdev;


struct virtdev_dev
{
	struct cdev cdev;
	unsigned char mem[MAX_SIZE];
};

struct virtdev_dev *devp;
dev_t devno;
int major;


static int virtdev_open(struct inode *inode, struct file *filp)
{
	struct virtdev_dev *dev = container_of(inode->i_cdev, 
					struct virtdev_dev, cdev);
	filp->private_data = dev;
	
	return 0;
}


static int virtdev_release(struct inode *inode, struct file *filp)
{
	return 0;
}



static long virtdev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct virtdev_dev *dev = filp->private_data;
	
	switch (cmd)
	{
	case MEM_CLEAR:
		memset(dev->mem, 0, MAX_SIZE);
		printk(KERN_INFO "virtdev is set to zero.\n");
		break;
		
	case MEM_PRINT:
		printk(KERN_INFO "copying string OK!\n");
		break;
	
	default:
		return -EINVAL;
	}
	
	return 0;
}

static ssize_t virtdev_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos)
{
	unsigned long p = *ppos;
	unsigned int count = size;
	int ret = 0;
	struct virtdev_dev *dev = filp->private_data;
	
	if (p >= MAX_SIZE)
		return -EINVAL;
	
	if (count > MAX_SIZE - p)
		count = MAX_SIZE - p;
		
	if (copy_to_user(buf, dev->mem + p, count))
		ret = -EINVAL;
	else
	{
		*ppos += count;
		ret = count;
		printk(KERN_INFO "read %u byte(s) from %lu\n", count, p);
	}
	
	return ret;
}


static ssize_t virtdev_write(struct file *filp, const char __user *buf, size_t size,
					loff_t *ppos)
{
	unsigned long p = *ppos;
	unsigned int count = size;
	int ret = 0;
	struct virtdev_dev *dev = filp->private_data;
	
	if (p >= MAX_SIZE)
		return -EINVAL;
	if (count > MAX_SIZE - p)
		count = MAX_SIZE - p;
		
	if (copy_from_user(dev->mem + p, buf, count))
		ret = -EINVAL;
	else
	{
		*ppos += count;
		ret = count;
		printk(KERN_INFO "write %u byte(s) from %lu\n", count, p);
	}
	
	return ret;
}


static loff_t virtdev_llseek(struct file *filp, loff_t offset, int orig)
{
	loff_t ret = 0;
	
	switch (orig)
	{
	case 0:
		if (offset < 0)
		{
			ret = -EINVAL;
			break;
		}
		if (offset > MAX_SIZE)
		{
			ret = -EINVAL;
			break;
		}
		filp->f_pos = offset;
		ret = filp->f_pos;
		break;
		
	case 1:
		if ((filp->f_pos + offset) < 0 )
		{
			ret = -EINVAL;
			break;
		}
		
		if ((filp->f_pos + offset) > MAX_SIZE)
		{
			ret = -EINVAL;
			break;
		}
		filp->f_pos += offset;
		ret = filp->f_pos;
		break;
		
	default:
		ret = -EINVAL;
		break;
	}
	
	return ret;
}


static const struct file_operations virtdev_fops =
{
	.owner  =  THIS_MODULE,
	.llseek =  virtdev_llseek,
	.read   =  virtdev_read,
	.write  =  virtdev_write,
	.unlocked_ioctl = virtdev_ioctl,
	.open   =  virtdev_open,
	.release = virtdev_release,
};

static char *chardev_devnode(struct device *dev, umode_t *mode)
{
    if (mode)
        *mode = 0666;

    return NULL;
}


static void virtdev_setup_cdev(struct virtdev_dev *dev, int index)
{
	int err;
	int ret;
	int devno = MKDEV(major, index);
	
		
	classdev = device_create(class, NULL, devno, NULL, "virtdev%d", index);
	if (IS_ERR(class))
	{
		ret = PTR_ERR(classdev);
		printk(KERN_ERR "device create error %d\n", ret);
		class_destroy(class);
	}

	
	cdev_init(&dev->cdev, &virtdev_fops);
	dev->cdev.owner = THIS_MODULE;
	err = cdev_add(&dev->cdev, devno, 1);
	if (err)
		printk(KERN_NOTICE "Error %d adding virtdev%d", err, index);
}


static int __init virtdev_init(void)
{
	int ret;
	int i;
	
	
	ret = alloc_chrdev_region(&devno, 0, DEVICE_NUM, "virtdev");
	if (ret < 0)
		return ret;
	major = MAJOR(devno);
		
	devp = kzalloc(sizeof(struct virtdev_dev) * DEVICE_NUM, GFP_KERNEL);
	if (!devp)
	{
		ret = -ENOMEM;
		goto fail_malloc;
	}
	
	class = class_create(NULL, "virtdev");
	if (IS_ERR(class))
	{
		ret = PTR_ERR(class);
		printk(KERN_ERR "class create error %d\n", ret);
		goto fail_malloc;
	}
	class->devnode = chardev_devnode;
	
	for (i = 0; i < DEVICE_NUM; i++)
		virtdev_setup_cdev(devp + i, i);
	
	return 0;
	

fail_malloc:
	unregister_chrdev_region(devno, 1);
	return ret;
}


static void __exit virtdev_exit(void)
{
	int i;
	int devno;
	
	for (i = 0; i < DEVICE_NUM; i++)
	{
		devno = MKDEV(major, i);
		device_destroy(class, devno);
		
		cdev_del(&((devp+i)->cdev));
	}
	
	class_destroy(class);
	kfree(devp);
	unregister_chrdev_region(MKDEV(major, 0), DEVICE_NUM);
}

module_init(virtdev_init);
module_exit(virtdev_exit);

MODULE_AUTHOR("lql");
MODULE_LICENSE("GPL");