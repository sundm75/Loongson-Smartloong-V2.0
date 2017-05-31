#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/platform_device.h>
#include<linux/i2c.h>
#include<linux/err.h>
#include<linux/slab.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
#include <loongson1.h>
#include <linux/delay.h>
#include <linux/cdev.h>



/**
在platform.c中入加

ls1x_i2c0_devs加入成员

{	I2C_BOARD_INFO("ds3231", 0x68),},



*/

#define bcd(x)  ( ((x/10)<<4) | (x%10))
#define frombcd(x)   ((x >> 4) * 10 + (x & 0x0f))

static struct i2c_board_info ds3231_info = {    
    I2C_BOARD_INFO("ds3231", 0x68),
};
static struct i2c_client *ds3231_client;

struct ds3231_drv_data{
	struct device *pdev;
	struct i2c_client *i2client;
};

static struct device *newds3231dev=NULL;

//取日期
static ssize_t ds3231_getdate(struct device *pdev,
	struct device_attribute *attr,
	char *buf)
{
	struct ds3231_drv_data *drvdata=(struct ds3231_drv_data *)dev_get_drvdata(pdev);
	u8 regaddr,yy,mm,dd,data[3];
	int result;


	//设置寄存器地址4	
	regaddr = 4;
	result = i2c_master_send( drvdata->i2client,(const char *)&regaddr,1);
	if(!result){
		printk("<4>set ds3231 register addr failured\n");
		return -1;
	}

	//读3个字节
	memset(&data,0,sizeof(data));
	result = i2c_master_recv(drvdata->i2client,data,sizeof(data));
	if(!result){
		printk("<4>read ds3231 failured\n");
		return -1;
	}

	yy = data[2];
	mm = data[1];
	dd = data[0];
	
	return sprintf(buf,"%02d%02d%02d\n",frombcd(yy),frombcd(mm),frombcd(dd));
}

static ssize_t ds3231_gettime(struct device *pdev,
	struct device_attribute *attr,
	char *buf)
{
	struct ds3231_drv_data *drvdata=(struct ds3231_drv_data *)dev_get_drvdata(pdev);
	u8 regaddr,hh,mm,ss,data[3];
	int result;


	//设置寄存器地址0	
	regaddr = 0;
	result = i2c_master_send( drvdata->i2client,(const char *)&regaddr,1);
	if(!result){
		printk("<4>set ds3231 register addr failured\n");
		return -1;
	}

	//读3个字节
	memset(&data,0,sizeof(data));
	result = i2c_master_recv(drvdata->i2client,data,sizeof(data));
	if(!result){
		printk("<4>read ds3231 failured\n");
		return -1;
	}

	hh = data[2];
	mm = data[1];
	ss = data[0];
	
	return sprintf(buf,"%02d%02d%02d\n",frombcd(hh),frombcd(mm),frombcd(ss));
}


/**
设置日期格式为yymmdd。如2015-10-30写为151030
*/
static ssize_t ds3231_setdate(struct device *pdev,
	struct device_attribute *attr,
	const char *buf,size_t count){

	struct ds3231_drv_data *drvdata=(struct ds3231_drv_data *)dev_get_drvdata(pdev);
	u8 yy,mm,dd;
	u8 data[4];
	int result;
	long x;
	
	sscanf(buf,"%ld",&x);
	yy = x / 10000;
	mm = x % 10000  / 100;
	dd = x % 100;

	//设置寄存器地址0	
	data[0]=4;//寄存器地址4
	data[1]=bcd(dd);//日
	data[2]=bcd(mm);//月
	data[3]=bcd(yy);//年
	result = i2c_master_send( drvdata->i2client,(const char *)data,sizeof(data));
	if(!result){
		printk("<4>set ds3231 register addr failured\n");
		return -1;
	}

	return count;
}

/**
读取时间并设置到ds3231。 格式为hhmmss。如14:32:15写为143215
*/
static ssize_t ds3231_settime(struct device *pdev,
	struct device_attribute *attr,
	const char *buf,size_t count){
	struct ds3231_drv_data *drvdata=(struct ds3231_drv_data *)dev_get_drvdata(pdev);
	u8 hh,mm,ss;
	u8 data[4];
	int result;
	long x;
	
	sscanf(buf,"%ld",&x);
	hh = x / 10000;
	mm = x % 10000  / 100;
	ss = x % 100;

	//设置寄存器地址0	
	data[0]=0;//寄存器地址0
	data[1]=bcd(ss);//秒
	data[2]=bcd(mm);//分
	data[3]=bcd(hh);//时
	result = i2c_master_send( drvdata->i2client,(const char *)data,sizeof(data));
	if(!result){
		printk("<4>set ds3231 register addr failured\n");
		return -1;
	}

	return count;
}


/**
打印所有寄存器
*/
static ssize_t ds3231_recv(struct device *pdev,
	struct device_attribute *attr,
	char *buf)
{
	struct ds3231_drv_data *drvdata=(struct ds3231_drv_data *)dev_get_drvdata(pdev);
	u8 regaddr,data[19];
	int result;
	char msgbuf[40];
	int count=0,i;


	printk("ds3231_recv start\n");
	//设置寄存器地址0	
	regaddr = 0;
	result = i2c_master_send( drvdata->i2client,(const char *)&regaddr,1);
	if(!result){
		printk("<4>set ds3231 register addr failured\n");
		return -1;
	}

	//读19
	memset(&data,0,sizeof(data));
	result = i2c_master_recv(drvdata->i2client,data,sizeof(data));
	if(!result){
		printk("<4>read ds3231 failured\n");
		return -1;
	}

	*buf=0;
	count=0;
	for(i=0;i<sizeof(data);i++){
		count += sprintf(msgbuf,"[%02x]=0x%02x\n",i,data[i]);
		strcat(buf,msgbuf);
	}
	
	count += sprintf(msgbuf,"\n");
	strcat(buf,msgbuf);

	return count;
}


static DEVICE_ATTR(register, 0444, ds3231_recv, NULL);
static DEVICE_ATTR(date, 0666, ds3231_getdate, ds3231_setdate);
static DEVICE_ATTR(time, 0666, ds3231_gettime, ds3231_settime);

static struct attribute *ds3231_attrs[] = {
      &dev_attr_register.attr,
	  &dev_attr_date.attr,
	  &dev_attr_time.attr,
      NULL,  /* need to NULL terminate the list of attributes */
};


static const struct attribute_group ds3231_attr_group = {
	.attrs = ds3231_attrs,
};


static int __devinit ds3231_probe(struct i2c_client *client,const struct i2c_device_id *id)
{
	struct i2c_adapter *padapter=client->adapter;
	struct device *pdev=padapter->dev.parent;
	struct ds3231_drv_data *ds3231_drv_data;
	//int nr=padapter->nr;
	int result;
	u8 data[6];
	u8 regaddr=0;
	struct class* class;
	dev_t devno;


	//打开第四复用gpio50 51，必须做
	__raw_writel(readl(LS1X_CBUS_FOURTHT1) |( 1 << (50 -  32)) ,LS1X_CBUS_FOURTHT1);
	__raw_writel(readl(LS1X_CBUS_FOURTHT1) |( 1 << (51 -  32)) ,LS1X_CBUS_FOURTHT1);


	//写地址0。是指ds3231中寄存器地址
	regaddr = 0;
	result = i2c_master_send( client,(const char *)&regaddr,1);
	if(!result){
		printk("<4>set ds3231 register addr failured\n");
		return -1;
	}

	//读数据
	memset(&data,0,sizeof(data));
	result = i2c_master_recv(client,data,sizeof(data));
	if(!result){
		printk("<4>read ds3231 failured\n");
		return -1;
	}

	printk("<4>%x %x %x %x %x %x \n",data[0],data[1],data[2],data[3],data[4],data[5]);

	//创建class和设备
	class = class_create(THIS_MODULE,"ds3231class");  
	devno=MKDEV(150,1);
	newds3231dev = device_create(class,pdev,devno,NULL,"ds3231"); 

	result = sysfs_create_group(&newds3231dev->kobj, &ds3231_attr_group);
	if (result) {
		printk("<4>sysfs_create_group result=%d\n",result);
		return -1;
	}


	////////分配driver data
	ds3231_drv_data = kzalloc(sizeof(struct ds3231_drv_data),GFP_KERNEL);
	if(!ds3231_drv_data){
		printk("kzalloc failured\n");
		return -1;
	}
	ds3231_drv_data->i2client=client;
	ds3231_drv_data->pdev = newds3231dev;
	dev_set_drvdata(newds3231dev,ds3231_drv_data);

	return 0;

}


static int __devexit ds3231_remove(struct i2c_client *client)
{
	struct ds3231_drv_data *drvdata;
	if(newds3231dev){
		sysfs_remove_group(&newds3231dev->kobj, &ds3231_attr_group);
		drvdata=(struct ds3231_drv_data *)dev_get_drvdata(newds3231dev);
		device_destroy(newds3231dev->class,newds3231dev->devt);
		class_destroy(newds3231dev->class);
		kfree(drvdata);
	}
	printk("device removed\n");
	
	return 0;
}



//1.????i2c driver
static const struct i2c_device_id ds3231_id_table[]={ 
	{"ds3231",0x68},
	{}
};




static struct i2c_driver ds3231_driver ={
        .driver={      .name="ds3231",        .owner=THIS_MODULE,         },
    .probe=ds3231_probe,
    .remove=__devexit_p(ds3231_remove),
    .id_table = ds3231_id_table,
   };



static int __init ds3231_drv_init(void)
{

    struct i2c_adapter *i2c_adap;
	printk("<4.1>ds3231_dev_client_init start \n");
    i2c_adap = i2c_get_adapter(2);
    ds3231_client = i2c_new_device(i2c_adap, &ds3231_info);
    i2c_put_adapter(i2c_adap);	
	
	printk("<4.2>ds3231_drv_init start \n");
	i2c_add_driver(&ds3231_driver); 
	return 0;
}

static void __exit ds3231_drv_exit(void)
{
	i2c_del_driver(&ds3231_driver);
	i2c_unregister_device(ds3231_client);
}
module_init(ds3231_drv_init);
module_exit(ds3231_drv_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("CCJY benb@qq.com");
MODULE_DESCRIPTION("ds3231 i2c driver");



