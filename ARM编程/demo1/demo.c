//#include <stm32f4xx.h>
//引入STM32F4标准外设库头文件，包含芯片所有
//1.外设的寄存器地址定义
//2.硬件抽象层的函数声明

#define GPIOEOUT    (*(volatile unsigned int*)0xC001E000)
#define GPIOEOUTENB (*(volatile unsigned int*)0xC001E004)
#define GPIOEALTFN0 (*(volatile unsigned int*)0xC001E020)
//硬件寄存器映射宏定义，主要用于通过软件操作硬件外设
//1.0xC001E000：硬件寄存器的物理地址
//2.(volatile unsigned int *)：将地址转换为指针
//  将物理地址（0xC001E000）强制转换为一个无符号整型指针
//  例如：(int *)0x1000 表示 “地址 0x1000 处的一个整型指针”。
//  volatile:该内存地址的值可能被硬件或其他线程修改，不要优化对它的读写操作
//3.*：解引用指针，访问寄存器值
//  通过指针解引用操作（*），可以直接读写该物理地址的内容

//GPIOEOUT​​：   对应GPIOE_ODR（输出数据寄存器）
//​​GPIOEOUTENB​​：对应GPIOE_OTYPER（输出类型寄存器）
//​​GPIOEALTFN0​​：对应GPIOE_MODER（模式寄存器）

void delay(void);
void _start(void){
	GPIOEALTFN0 &= ~(3<<26);
	//GPIO功能
	//​​&= ~(...)​​：清空第26-27位（设置00），即通用输出模式。
	//寄存器背景​​：每组GPIO的MODER寄存器控制16个引脚，每引脚占2位（00=输入，01=输出，10=复用，11=模拟）。
	GPIOEOUTENB |= 1<<13;
	
	while(1){
		GPIOEOUT &= ~(1<<13);
		
		delay();
		
		GPIOEOUT |= (1<<13);
		
		delay();		
	}
}

void delay(void){
	volatile unsigned int i = 0x2000000;
	while(i--);
}