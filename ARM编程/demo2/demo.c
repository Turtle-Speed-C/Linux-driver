#define GPIOEOUT    (*(volatile unsigned int*)0xC001E000)
#define GPIOEOUTENB (*(volatile unsigned int*)0xC001E004)
//控制功能使能：
//用来启用或禁用某个 GPIO 引脚的输出功能
#define GPIOEALTFN0 (*(volatile unsigned int*)0xC001E024)
//控制工作模式：
//用来配置 GPIO 引脚的备用功能，决定引脚是作为普通的 GPIO 引脚，
//还是作为其他特定功能（如 UART、SPI、I2C 等）。

#define GPIOCOUT    (*(volatile unsigned int*)0xC001C000)
#define GPIOCOUTENB (*(volatile unsigned int*)0xC001C004)
#define GPIOCALTFN1 (*(volatile unsigned int*)0xC001C024)

void delay(void);
void _start(void){
	//GPIOE13引脚改为GPIO模式
	GPIOEALTFN0 &= ~(3<<26);
	
	//E13引脚改为输出模式
	GPIOEOUTENB |= 1<<13;
	
	//GPIOC17引脚改为GPIO模式
	GPIOCALTFN1 &= ~(3<<2);
	//GPIOCALTFN1|=(1<<2);
	
	//C17引脚改为输出模式
	GPIOCOUTENB |= 1<<17;
	
	while(1){
		GPIOEOUT &= ~(1<<13);
		
		delay();
		
		GPIOEOUT |= 1<<13;
		
		delay();
		
		GPIOCOUT &= ~(1<<17);
		
		delay();
		
		GPIOCOUT |= 1<<17;
		
		delay();
		
	}
}

void delay(void){
	volatile unsigned int i = 0x2000000;
	
	while(i--);
}
