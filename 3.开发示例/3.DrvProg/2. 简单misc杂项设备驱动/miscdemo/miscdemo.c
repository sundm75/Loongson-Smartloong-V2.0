/*miscdemo.c*/
#include  <linux/init.h>
#include  <linux/module.h>
#include  <linux/fs.h>
#include  <linux/miscdevice.h>

int  open_state  =  0;
/////////////////////////////////////////////////
int  miscdemo_open(struct  inode  *inode,  struct  file  *filp)
{
        if  (open_state  ==  0)
        {
                open_state  =  1;
                printk("miscdemo open!\n");
                return  0;
        }
        printk("miscdemo  has  been  open!\n");
        return  -1;
}

int  miscdemo_release(struct  inode  *inode,  struct  file  *filp)
{
        if  (open_state  ==  1)
        {
                open_state  =  0;
                printk("miscdemo  release!\n");
                return  0;
        }
        printk("miscdemo  has  not  been  open  yet!\n");
        return  -1;
}

ssize_t  miscdemo_read(struct  file  *filp,  char  *buf,
                size_t  count,  loff_t  fpos)
{
        printk("miscdemo  read!\n");
        return  0;
}

ssize_t  miscdemo_write(struct  file  *filp,  char  *buf,
                size_t  count,  loff_t  fpos)
{
        printk("miscdemo  write!\n");
        return  0;
}

int  miscdemo_ioctl(struct  inode  *inode,  struct  file  *filp,
                unsigned  int  cmd,  unsigned  long  arg)
{
        printk("ioctl  is  called!\n");
        printk("cmd:%d  arg:%d\n",  cmd,  arg);
        return  0;
}
/////////////////////////////////////////////////
struct  file_operations  fops  =  
{
        .owner            =      THIS_MODULE,
        .open              =      miscdemo_open,
        .release        =      miscdemo_release,
        .write            =      miscdemo_write,
        .read              =      miscdemo_read,
        . unlocked_ioctl	=      miscdemo_ioctl
};

struct  miscdevice  dev  =  
{
        .minor    =      MISC_DYNAMIC_MINOR,
        .fops        =      &fops,
        .name      =      "miscdemo",
        .nodename  =  "miscdemo_node"
};

int  setup_miscdemo(void)
{
        
        return  misc_register(&dev);
}
/////////////////////////////////////////////////
static  int  __init  miscdemo_init(void)
{
        printk("miscdemo  init!\n");
        return  setup_miscdemo();
}

static  void  __exit  miscdemo_exit(void)
{
        printk("miscdemo  exit!\n");
        misc_deregister(&dev);
}

MODULE_AUTHOR("sundm75");  
MODULE_LICENSE("GPL");  
module_init(miscdemo_init);
module_exit(miscdemo_exit);

