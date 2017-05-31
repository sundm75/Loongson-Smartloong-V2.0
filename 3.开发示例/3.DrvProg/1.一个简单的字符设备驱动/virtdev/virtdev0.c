#include <linux/module.h>  
#include <linux/fs.h>  
#include <linux/init.h>  
#include <linux/cdev.h>  
#include <linux/slab.h>  
#include <linux/uaccess.h>  
  
#define MAX_SIZE    0x1000  
#define MEM_CLEAR         0x1  
  
  
struct virtdev_dev  
{  
    struct cdev cdev;  
    unsigned char mem[MAX_SIZE];  
};  
  
struct virtdev_dev *devp;  
dev_t devno;  
  
  
static int virtdev_open(struct inode *inode, struct file *filp)  
{  
    filp->private_data = devp;  
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
        printk(KERN_INFO "virtdev is set to zero\n");  
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
  
static int __init virtdev_init(void)  
{  
    int ret;  
    int err;  
      
    ret = alloc_chrdev_region(&devno, 0, 1, "virtdev");  
    if (ret < 0)  
        return ret;  
          
    devp = kzalloc(sizeof(struct virtdev_dev), GFP_KERNEL);  
    if (!devp)  
    {  
        ret = -ENOMEM;  
        goto fail_malloc;  
    }  
      
    cdev_init(&devp->cdev, &virtdev_fops);  
    devp->cdev.owner = THIS_MODULE;  
    err = cdev_add(&devp->cdev, devno, 1);  
    if (err)  
        printk(KERN_NOTICE "Error %d adding virtdev", err);  
          
    return 0;  
      
fail_malloc:  
    unregister_chrdev_region(devno, 1);  
    return ret;  
}  
  
  
static void __exit virtdev_exit(void)  
{  
    cdev_del(&devp->cdev);  
    kfree(devp);  
    unregister_chrdev_region(devno, 1);  
}  
  
module_init(virtdev_init);  
module_exit(virtdev_exit);  
  
MODULE_AUTHOR("lql");  
MODULE_LICENSE("GPL");  