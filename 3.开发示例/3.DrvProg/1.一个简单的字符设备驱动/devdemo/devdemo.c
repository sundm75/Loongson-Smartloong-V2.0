#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/timer.h>
#include <asm/atomic.h>
#include <linux/slab.h>
#include <linux/device.h>

#define CDEVDEMO_MAJOR 255  /*预设cdevdemo的主设备号*/

static int cdevdemo_major = CDEVDEMO_MAJOR;
struct class *cdevdemo_class;
static struct device *dev; 
/*设备结构体,此结构体可以封装设备相关的一些信息等
  信号量等也可以封装在此结构中，后续的设备模块一般都
  应该封装一个这样的结构体，但此结构体中必须包含某些
  成员，对于字符设备来说，我们必须包含struct cdev cdev*/
struct cdevdemo_dev	
{
	struct cdev cdev;
};

struct cdevdemo_dev *cdevdemo_devp;	/*设备结构体指针*/

/*文件打开函数，上层对此设备调用open时会执行*/
int cdevdemo_open(struct inode *inode, struct file *filp)	
{
	printk(KERN_NOTICE "======== cdevdemo_open ");
	return 0;
}

/*文件释放，上层对此设备调用close时会执行*/
int cdevdemo_release(struct inode *inode, struct file *filp)	
{
	printk(KERN_NOTICE "======== cdevdemo_release ");	
	return 0;
}

/*文件的读操作，上层对此设备调用read时会执行*/
static ssize_t cdevdemo_read(struct file *filp, char __user *buf, size_t count, loff_t *ppos)
{
	printk(KERN_NOTICE "======== cdevdemo_read ");	
}

/* 文件操作结构体，文中已经讲过这个结构*/
static const struct file_operations cdevdemo_fops =
{
	.owner = THIS_MODULE,
	.open = cdevdemo_open,
	.release = cdevdemo_release,
	.read = cdevdemo_read,
};

/*初始化并注册cdev*/
static void cdevdemo_setup_cdev(struct cdevdemo_dev *dev, int index)
{
	printk(KERN_NOTICE "======== cdevdemo_setup_cdev 1");	
	int err, devno = MKDEV(cdevdemo_major, index);
	printk(KERN_NOTICE "======== cdevdemo_setup_cdev 2");

	/*初始化一个字符设备，设备所支持的操作在cdevdemo_fops中*/	
	cdev_init(&dev->cdev, &cdevdemo_fops);
	printk(KERN_NOTICE "======== cdevdemo_setup_cdev 3");	
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &cdevdemo_fops;
	printk(KERN_NOTICE "======== cdevdemo_setup_cdev 4");	
	err = cdev_add(&dev->cdev, devno, 1);
	printk(KERN_NOTICE "======== cdevdemo_setup_cdev 5");
	if(err)
	{
		printk(KERN_NOTICE "Error %d add cdevdemo %d", err, index);	
	}
}
static char *chardev_devnode(struct device *dev, umode_t *mode)  
{  
    if (mode)  
        *mode = 0666;  
  
    return NULL;  
}  
  

int cdevdemo_init(void)
{
	printk(KERN_NOTICE "======== cdevdemo_init ");	
	int ret;
	dev_t devno = MKDEV(cdevdemo_major, 0);

	/*申请设备号，如果申请失败采用动态申请方式*/
	if(cdevdemo_major)
	{
		printk(KERN_NOTICE "======== cdevdemo_init 1");
		ret = register_chrdev_region(devno, 1, "cdevdemo");
	}else
	{
		printk(KERN_NOTICE "======== cdevdemo_init 2");
		ret = alloc_chrdev_region(&devno,0,1,"cdevdemo");
		cdevdemo_major = MAJOR(devno);
	}
	if(ret < 0)
	{
		printk(KERN_NOTICE "======== cdevdemo_init 3");
		return ret;
	}
	/*动态申请设备结构体内存*/
	cdevdemo_devp = kmalloc(sizeof(struct cdevdemo_dev), GFP_KERNEL);
	if(!cdevdemo_devp)	/*申请失败*/
	{
		ret = -ENOMEM;
		printk(KERN_NOTICE "Error add cdevdemo");	
		goto fail_malloc;
	}

	memset(cdevdemo_devp,0,sizeof(struct cdevdemo_dev));
	printk(KERN_NOTICE "======== cdevdemo_init 3");
	cdevdemo_setup_cdev(cdevdemo_devp, 0);

	/*下面两行是创建了一个总线类型，会在/sys/class下生成cdevdemo目录
	  这里的还有一个主要作用是执行device_create后会在/dev/下自动生成
	  cdevdemo设备节点。而如果不调用此函数，如果想通过设备节点访问设备
	  需要手动mknod来创建设备节点后再访问。*/
	cdevdemo_class = class_create(THIS_MODULE, "cdevdemo");
    if (IS_ERR(cdevdemo_class))  
    {  
        ret = PTR_ERR(cdevdemo_class);  
        printk(KERN_ERR "class create error %d\n", ret);  
        goto fail_malloc;  
    }  
	cdevdemo_class->devnode = chardev_devnode;  
	dev = device_create(cdevdemo_class, NULL, MKDEV(cdevdemo_major, 0), NULL, "cdevdemo");
	
	printk(KERN_NOTICE "======== cdevdemo_init 4");
	return 0;

	fail_malloc:
		unregister_chrdev_region(devno,1);
}

void cdevdemo_exit(void)	/*模块卸载*/
{
    device_destroy(cdevdemo_class, MKDEV(cdevdemo_major,0));  
    class_destroy(cdevdemo_class);  

	printk(KERN_NOTICE "End cdevdemo");	
	cdev_del(&cdevdemo_devp->cdev);	/*注销cdev*/
	kfree(cdevdemo_devp);		/*释放设备结构体内存*/
	unregister_chrdev_region(MKDEV(cdevdemo_major,0),1);	//释放设备号
}
MODULE_AUTHOR("sundm75");  
MODULE_LICENSE("GPL");  
module_param(cdevdemo_major, int, S_IRUGO);
module_init(cdevdemo_init);
module_exit(cdevdemo_exit);

