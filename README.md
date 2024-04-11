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

#### 备注
```
目前已完成PA4.2,
实现多道批处理系统。
支持运行仙剑、flappy bird、nslider、nterm等应用程序
基于虚存机制，实现分时系统。支持用户进程、操作系统、内核线程的并发执行
```
#### TODO
```
PA2开始，由于版本问题，diffrential test无法支持
PA3的一些很有价值的选做：系统声卡、am环境迁移、多道程序ftracer
仙剑有两个已知的BUG，一个是战斗时卡死、一个是存档机制异常
busybox有BUG，会循环执行命令
需要实现页回收机制，并且有的时候检测不出page fault
```

