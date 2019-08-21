# 1. 虚拟机64位分析
由于电脑是64位系统, 但是linux下很多软件还是32的, 所以需要32位运行库,
如果没装的话,**编译时出错**。

作为Linux系统最出名的发行版之一，Ubuntu这几年来一直在讨论放弃32位X86支持的问题，最终Ubuntu社区决定从今年下半年的Ubuntu 19.10中正式移除对32位i386（Ubuntu社区里将32位x86等系列称为i386）的支持。

最后支持32位x86的系统将是Ubuntu 18.04，这是一个LTS长期支持版本，会持续支持到2023年，总计5年时间，所以为了防止出错，Ubuntu也禁止了Ubuntu 18.04发行版用户升级到Ubuntu 18.10版本。

不再支持32位x86对某些用户来说会遇到一些麻烦，特别是一些Linux上的Steam平台及部分游戏只能用32位模式运行，不过Ubuntu社区表示还有一些方法可以让这些程序及游戏运行在32位模式下。

下面讨论 **怎样打开64位 Ubuntu 的32位支持功能**。

***网盘上存放的镜像是 32 位系统。***

# 2. 解决方法总结
## 1) 通用方法

	12.04下面：sudo apt-get install ia32-libs

14.04下面：
	sudo apt-get install libglib2.0-0:i386

16.04下面:

	sudo apt-get install lib32ncurses5 ​
	sudo apt-get install lib32z1

## 2) Linux 公社方法
[怎样打开64位 Ubuntu 的32位支持功能](https://www.linuxidc.com/Linux/2014-04/100848.htm)
首先要打开64位系统对32位的支持
### 第一步: 确认有一个64位架构的内核
输入：
		
	dpkg --print-architecture

输出：

	adm64
说明已拥有64位架构内核。

输出：

	i386
说明这是32位架构内核。


### 第二步：确认打开了多架构支持功能

输入：

	dpkg --print-foreign-architectures


输出：

	i386
	
说明已打开，如果没有需要手动打开

### 另一种第二步 打开多架构支持
输入：

	sudo dpkg --add-architecture i386
	sudo apt-get update

再执行

	sudo apt-get dist-upgrade  

这一步是更新所有的软件，如果你对新版本软件的需求不是那么迫切，可以不执行。
它会检查你已经拥有的库文件是否有更新的版本。
若果然有的话，它会问你是否需要更新(y/n)。
如此这般，你就已经成功地为你的64位系统安装了32位支持了。

### 3）或者只安装相关库

	sudo apt-get install lib32z1 lib32ncurses5 lib32bz2-1.0  
	（有的还需要32位stdc++库 lib32stdc++6-4.8-dbg）
	
如此这般你就拥有了64位系统对32位的支持。

其次是安装gcc multilab
 
	sudo apt-get install gcc-multilib g++-multilib  

### 4）单独安装 

	apt-get install libc6:i386 libx11-6:i386 libasound2:i386 libatk1.0-0:i386 libcairo2:i386 libcups2:i386 libdbus-glib-1-2:i386 libgconf-2-4:i386 libgdk-pixbuf2.0-0:i386 libgtk-3-0:i386 libice6:i386 libncurses5:i386 libsm6:i386 liborbit2:i386 libudev1:i386 libusb-0.1-4:i386 libstdc++6:i386 libxt6:i386 libxtst6:i386 libgnomeui-0:i386 libusb-1.0-0-dev:i386 libcanberra-gtk-module:i386 gtk2-engines-murrine:i386

### 5）有关其它方案

[Ubuntu遇到的问题记录](https://blog.csdn.net/xiang_freedom/article/details/78533520)

版本：16.04  x86_64
1.安装Android Studio报错
按照官方教程执行

	sudo apt-get install lib32z1 lib32ncurses5 lib32bz2-1.0 lib32stdc++6
提示
E: 无法定位软件包 lib32bz2-1.0
E: 无法按照 glob ‘lib32bz2-1.0’ 找到任何软件包
E: 无法按照正则表达式 lib32bz2-1.0 找到任何软件包

解决：

	sudo apt-get install libz1 libncurses5 libbz2-1.0:i386 libstdc++6 libbz2-1.0


# 3. 查看当前 Ubuntu 的版本：
- sudo uname -m 如果显示i686,安装了32位操作系统；如果显示 x86_64，安装了64位操作系统。

- sudo uname -s 显示内核名字s

- sudo uname -r 显示内核版本

- sudo uname -n 显示网络主机名

- sudo uname -p 显示cpu 

- cat /proc/version  显示内核版本信息  gcc版本 ubuntu 版本

```
root@ubuntu:~# cat /proc/version
Linux version 4.15.0-32-generic (buildd@lgw01-amd64-035) (gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.10)) #35~16.04.1-Ubuntu SMP Fri Aug 10 21:52:48 UTC 2018
```

- uname -a 显示内核名称、版本，主机名称、版本

```
root@ubuntu:~# uname -a
Linux ubuntu 4.15.0-32-generic #35~16.04.1-Ubuntu SMP Fri Aug 10 21:52:48 UTC 2018 i686 i686 i686 GNU/Linux
```
- lsb_release -a 显示 Ubuntu 版本，主机名称、版本

```
root@ubuntu:~# lsb_release -a
No LSB modules are available.
Distributor ID:	Ubuntu
Description:	Ubuntu 16.04.5 LTS
Release:	16.04
Codename:	xenial
```







