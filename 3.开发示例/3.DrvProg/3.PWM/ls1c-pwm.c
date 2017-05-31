#include <linux/miscdevice.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/ioctl.h>
#include <linux/platform_device.h>
#include <linux/timer.h>

#define CMD_PWM_GET     _IO('c',0x01)
#define CMD_PWM_START   _IO('c',0x02)
#define CMD_PWM_STOP    _IO('c',0x03)

#define REG_PWM_CNTR	0x00 // 0xBFE5C010
#define REG_PWM_HRC	0x04 // 0xBFE5C014
#define REG_PWM_LRC	0x08 // 0xBFE5C018
#define REG_PWM_CTRL	0x0c  // 0xBFE5C01C
#define REG_GPIO_CFG0	0xbfd010c0//GPIO GPIO[31:0] 配置寄存器 1 表示配置为GPIO， 0 表示无效
#define REG_GPIO_CFG2	0xbfd010c8//GPIO[95:64] 配置寄存器 1 表示配置为GPIO， 0 表示无效
#define SB2F_GPIO_MUX_CTRL1 0xbfd00424//功能复用寄存器

static unsigned char __iomem *pwm_base = NULL;
struct resource *res;
static int ls1f_pwm_probe(struct platform_device *pdev);
struct platform_device *pwm_dev;
struct resource *res1 = NULL;

static int ls1f_pwm_getResourse(struct platform_device *pdev, unsigned int index);

static struct platform_driver ls1f_pwm_driver = {
	.probe = ls1f_pwm_probe,
	.driver = {
			.name = "ls1c-pwm",
	},
};

static int ls1f_pwm_open(struct inode *inode, struct file *file) 
{
    //配置PWM0 PWM1 原始功能没有复用
	//long val = readl(SB2F_GPIO_MUX_CTRL1);
	//val &= 0xfffffffc;
	//writel(val, SB2F_GPIO_MUX_CTRL1);

	long val = readl(REG_GPIO_CFG0);	
	//配置GPIO6引脚为普通功能，而非GPIO功能
	val &= 0xffffffbf;
	writel(val, REG_GPIO_CFG0);

	val = readl(REG_GPIO_CFG0);
	//配置GPIO92引脚为普通功能，而非GPIO功能
	val &= 0xefffffff;
	writel(val, REG_GPIO_CFG2);

	
	return 0;
}

static int ls1f_pwm_close(struct inode *inode, struct file *file)
{
	writel(0x0, pwm_base + REG_PWM_CTRL);
	return 0;
}

static ssize_t ls1f_pwm_read(struct file *file, char __user *buf, size_t count, loff_t *ptr)
{
	unsigned int pwm_val;
	pwm_val = readl(pwm_base);

	if (copy_to_user(buf, &pwm_val, sizeof(unsigned int)))
		return -EFAULT;
	return 4;
}

static ssize_t ls1f_pwm_write(struct file *file, const char __user *buf, size_t count, loff_t *ptr)
{
	unsigned int hrc_val, lrc_val;
	unsigned int data[2] = {0x0};

	if (copy_from_user(data, buf, sizeof(data)))
	{
		printk("Write error!\n");
		return -EIO;
	}

	hrc_val = data[1] - 1;
	lrc_val = data[0] + data[1] -1;

	//设置占空比
	writel(hrc_val, pwm_base + REG_PWM_HRC);
	writel(lrc_val, pwm_base + REG_PWM_LRC);
	printk("hrc:%i ; lrc:%i\n",hrc_val,lrc_val);
	return 0;
}

static int pwm_start_stop(unsigned int cmd, unsigned long arg)
{
	printk("into: %s\n", __FUNCTION__);
	printk("arg: %ld\n", arg);

	if (arg > 3)
		return -1;
	//从platform中获取指定PWM的寄存器基址 pwm_base
	//通过改变arg，实现在pwm0、pwm1、pwm2、pwm3之间切换
	ls1f_pwm_getResourse(pwm_dev, arg);
	switch (cmd) {
	//启动PWM
	case CMD_PWM_START:
		printk("CMD_PWM_START\n");
		writel(0x0, pwm_base + REG_PWM_CNTR);
		writel(0x01, pwm_base + REG_PWM_CTRL);
		break;
	//停止PWM
	case CMD_PWM_STOP:
		printk("CMD_PWM_STOP\n");
		writel(0x0, pwm_base + REG_PWM_CTRL);
		break;
	default:
		break;
	}
	return 0;
}

static long ls1f_pwm_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	printk("into %s\n", __FUNCTION__);
	printk("cmd: %d\n", cmd);
	printk("arg: %ld\n", arg);

	if (arg > 3)
		return -1;

	switch (cmd) {
	case CMD_PWM_GET:
		ls1f_pwm_getResourse(pwm_dev, arg);
		break;
	case CMD_PWM_START:
	case CMD_PWM_STOP:
		return pwm_start_stop(cmd, arg);
	default:
		break;
	}
	return 0;
}


static const struct file_operations ls1f_pwm_ops = {
		.owner = THIS_MODULE,
		.open = ls1f_pwm_open,
		.release = ls1f_pwm_close,
		.read = ls1f_pwm_read,
		.write = ls1f_pwm_write,
		.unlocked_ioctl = ls1f_pwm_ioctl,
};

static struct miscdevice ls1f_pwm_miscdev = {
		MISC_DYNAMIC_MINOR,
		"ls1f-pwm",
		&ls1f_pwm_ops,
};

static int ls1f_pwm_getResourse(struct platform_device *pdev, unsigned int index)
{
	res = platform_get_resource(pdev, IORESOURCE_MEM, index);

	if (res == NULL)
	{
	  printk("Fail to get ls1f_pwm_resource!\n");
	  return -ENOENT;
	}
	printk("Resource start=0x%x, end = 0x%x\n", res->start, res->end);
	if (res1 != NULL)
	{
		release_mem_region(res->start, 0x0f);
	}
	res1 = request_mem_region(res->start, 0x0f, "ls1f-pwm");
	if (res1 == NULL)
	{
	  printk("Fail to request ls1f_pwm region!\n");
	  return -ENOENT;
	}
	pwm_base = ioremap(res->start, res->end - res->start + 1);
	if (pwm_base == NULL)
	{
	  printk("Fail to ioremap ls1f_pwm resource!\n");
	  return -EINVAL;
	}
	return 0;
}

static int __devinit ls1f_pwm_probe(struct platform_device *pdev)
{
	pwm_dev = pdev;
	return ls1f_pwm_getResourse(pdev, 1);
}

static int __init ls1f_pwm_init(void) {
	if (misc_register(&ls1f_pwm_miscdev))
	{
	  printk(KERN_WARNING "pwm: Couldn't register device 10, %d.\n", 255);
	  return -EBUSY;
	}
	return platform_driver_register(&ls1f_pwm_driver);
}

static void __exit ls1f_pwm_exit(void)
{
	misc_deregister(&ls1f_pwm_miscdev);
	release_mem_region(res->start, 0x20);
	platform_driver_unregister(&ls1f_pwm_driver);	
}

module_init(ls1f_pwm_init);
module_exit(ls1f_pwm_exit);
MODULE_AUTHOR("sundm");
MODULE_DESCRIPTION("loongson 1C PWM driver");
MODULE_LICENSE("GPL");
