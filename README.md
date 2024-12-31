# ICS2021 Programming Assignment

This project is the programming assignment of the class ICS(Introduction to Computer System)
in Department of Computer Science and Technology, Nanjing University.

For the guide of this programming assignment,
refer to https://jyywiki.cn/ICS/2021

To initialize, run
```bash
bash init.sh subproject-name
```
See `init.sh` for more details.

The following subprojects/components are included. Some of them are not fully implemented.
* [NEMU](https://github.com/NJU-ProjectN/nemu)
* [Abstract-Machine](https://github.com/NJU-ProjectN/abstract-machine)
* [Nanos-lite](https://github.com/NJU-ProjectN/nanos-lite)
* [Navy-apps](https://github.com/NJU-ProjectN/navy-apps)


#### 配置这个项目
##### 1.安装以下工具
```shell
sudo apt-get install build-essential    # build-essential packages, include binary utilities, gcc, make, and so on
sudo apt-get install man                # on-line reference manual
sudo apt-get install gcc-doc            # on-line reference manual for gcc
sudo apt-get install gdb                # GNU debugger
sudo apt-get install git                # revision control system
sudo apt-get install libreadline-dev    # a library used later
sudo apt-get install libsdl2-dev        # a library used later
sudo apt-get install llvm               # llvm project, which contains libraries used later
sudo apt-get install bison              # bison
sudo apt-get install riscv64               # flex
sudo apt-get install flex               # flex
sudo apt-get install ccache
sudo apt install gcc-riscv64-linux-gnu
sudo apt install g++-riscv64-linux-gnu

```


##### 2.设置环境变量
在~/.zshrc中加入下面两行
```shell
export NEMU_HOME=/home/bhx/work/ics2021/nemu
export AM_HOME=/home/bhx/work/ics2021/abstract-machine
export NAVY_HOME=/home/bhx/work/ics2021/navy-apps
```

##### 3.配置NEMU
在nemu目录下执行
```shell
make menuconfig
```
##### 5.配置仙剑奇侠传
从网盘上下载文件，放入pal/repo/data目录下

写配置文件config.cfg放入pal/repo/data主要将材质和视图大小设置为320*200

##### 4.编译运行
在nanos-lite目录下执行
```shell
make ARCH=riscv32-nemu update
```
会出现一个缺少头文件的报错，进入引用的头文件中注释掉即可。


##### 6.自由选择可运行文件
在navy-apps/Makefile中设置

#### NOTE
```
目前已完成全部必做内容
实现多道批处理系统。
支持运行仙剑、flappy bird、nslider、nterm等应用程序
基于虚存机制，实现分时系统。支持用户进程、操作系统、内核线程之间的并发执行
实现内核栈与用户栈的切换存储，支持用户进程之间的并发运行
```
#### TODO
```
PA2开始，由于版本问题，diffrential test无法支持
PA3的一些很有价值的选做：系统声卡、am环境迁移、多道程序ftracer
仙剑有两个已知的BUG，一个是战斗时卡死、一个是存档机制异常
busybox有BUG，会循环执行命令
```