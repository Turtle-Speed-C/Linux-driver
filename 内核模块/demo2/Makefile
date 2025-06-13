obj-m +=jcprintk.o
#obj-m 表示编译为可加载模块（*.ko），区别于 obj-y（直接编入内核）。
#led_drv.o 是目标文件名，实际会生成 led_drv.ko。
#obj-m 是 Linux 内核构建系统（Kbuild）中定义的一个特殊变量，​​
#不是内核文件​​，而是一个 ​​Makefile 变量​​，
#用于指定需要编译为可加载内核模块（.ko 文件）的目标文件列表。

#obj-m 的值为 led_drv.o
KERNEL_DIR :=/home/gec/6818GEC/kernel
#指定内核源代码的绝对路径

CROSS_COMPILE :=/home/gec/6818GEC/prebuilts/gcc/linux-x86/arm/arm-eabi-4.8/bin/arm-eabi-
#交叉编译工具链前缀。arm-eabi- 是工具链前缀（如 arm-eabi-gcc 为 C 编译器）
#路径需确保可执行文件存在（如 /bin/arm-eabi-gcc）

PWD:=$(shell pwd)
#作用​​：动态获取当前 Makefile 所在目录路径。
#用途​​：传递给内核构建系统，定位模块源码位置。
#当前模块所在的目录


default:
	$(MAKE) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	rm *.o *.order .*.cmd *.mod.c *.symvers .tmp_versions -rf