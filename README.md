# raspberry-stm32
Develope stm32 on the raspberry pi 3

#＃　介绍
树莓派板子上在线编辑代码，编译，通过stm32flash下载到stm32　flash中，实现在线编程stm32,可以基于此开发树莓派＋stm32一些有趣的项目。

## 硬件
	1. 树莓派３ B+

	2. stm32开发板

	3. 杜邦线５根

## 树莓派上的软件
	1. 树莓派安装好系统并能ssh登录

	2. stm32flash

	3. arm-none-eabi-gcc

##　安装过程
在进行以下操作前先保证树莓派３工作正常，stm32开发板可以正常工作并且uart可用。

### 1. stm32flash安装
	1).下载源码

	https://sourceforge.net/projects/stm32flash/files/?source=navbar

	2). 进入目录，编译，安装，默认安装在/usr/local/bin目录下，最后有必要的话将此加入到PATH中即可
	
	$ make

	$ sudo make install

	3). 测试安装结果，执行stm32flash会打印帮助信息

	$ stm32flash

### 2. 树莓派３串口配置
树莓派３系统默认使用串口0作为蓝牙设备通信接口，因此需要重新配置，才能使用串口下载程序到stm32上。

	1). 关闭终端串口调试，释放串口0，执行raspi-config,高级设置里面关闭调试串口，可以看/boot/cmdline.txt，查看consle=是不是已经不是serial0了

	$ raspi-config

	2). 修改/boot/config.txt, 添加如下代码： 
	
	dtoverlay=pi3-miniuart-bt
	enable_uart=1

	3). 重启后即可使用串口0，操作节点是/dev/serial0, 这跟之前的树莓派版本不太一样，可不是/dev/ttyAMA0

### 3. 硬件连接
	

