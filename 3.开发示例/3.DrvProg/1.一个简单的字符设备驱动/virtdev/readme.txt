virtdev0.c
编写一个简单的字符设备驱动

（1）该字符设备为动态申请的设备号，加载进内核后需cat /proc/devices/virtdev查看设备号后，在使用mknod virtdev c major minor 创建设备节点，然后才可以读写
（2）设备并非真正的设备，而是一块内存区mem，我们可以对其进行读写操作；
（3）动态创建设备文件日后补充

http://blog.csdn.net/liuqinglong_along/article/details/51803138
virtdev.c
编写一个简单的字符设备驱动（自动创建设备文件）
说明：
（1）该篇在上篇的基础上实现了自动创建设备文件的功能；
（2）自动创建文件主要用到了class_create()、device_create()两个函数，声明在inclue/linux/device.h里；
（3）设备文件的创建和销毁放在模块加载和卸载函数中；

运行结果如下：
在CODE上查看代码片派生到我的代码片
allen-lql dev # echo "hello everyone!" > virtdev   
allen-lql dev # cat virtdev   
hello everyone! 

 代码：编写一个简单的字符设备驱动——创建多个同类型设备
 virtdev1.c
编写同类型多个设备字符驱动应注意一下几个问题：
1、申请设备号alloc_chrdev_region时须指定次设备号范围；
2、动态分配设备空间时同时分配NUM个设备的空间；
3、根据次设备号和统一的主设备号生成针对单个设备的devno，然后完成cdev_add注册；
4、设备文件的device_create中可以指定第五个参数来区别不同设备的设备文件（节点）名称；
5、卸载函数中注意循环卸载设备文件以及注销设备；
6、注销设备号；

遗留问题：通过cdev_add中指定count参数应该可以完成多个设备的同时注册，但cdev_del中并没有devno做入参，
                  暂未确认一次调用是否可以完成对多个设备的注销，留待日后解决。