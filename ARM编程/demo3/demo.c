//D7:E13 D8:C17 D9:C8 D10:C7 D11:C12
//00 01 01 01 01(确定了GPIOxALTFN0/1 该怎么弄成GPIO模式)

#define GPIOEOUT    (*(volatile unsigned int*)0xC001E000)
#define GPIOEOUTENB (*(volatile unsigned int*)0xC001E004)
#define GPIOEALTFN0 (*(volatile unsigned int*)0xC001E020)

#define GPIOCOUT    (*(volatile unsigned int*)0xC001C000)
#define GPIOCOUTENB (*(volatile unsigned int*)0xC001C004)
#define GPIOCALTFN1 (*(volatile unsigned int*)0xC001C024)
#define GPIOCALTFN0 (*(volatile unsigned int*)0xC001C020)

void delay(void);

void _start(void){
	//D7变成GPIO模式
	GPIOEALTFN0 &= ~(3<<26);
	
	//D7变成输出模式
	GPIOEOUTENB |= (1<<13);
	
	//D8变成GPIO模式
	GPIOCALTFN1 &= ~(3<<2);
	GPIOCALTFN1 |= 1<<2;
	
	//D8变成输出模式
	GPIOCOUTENB |= (1<<17);
	
	//D9变成GPIO模式
	GPIOCALTFN0 &= ~(3<<16);
	GPIOCALTFN0 |= 1<<16;
	
	//D9变成输出模式
	GPIOCOUTENB |= (1<<8);
	
	//D10变成GPIO模式
	GPIOCALTFN0 &= ~(3<<14);
	GPIOCALTFN0 |= 1<<14;
	
	//D10变成输出模式
	GPIOCOUTENB |= (1<<7);
	
	//D11变成GPIO模式
	GPIOCALTFN0 &= ~(3<<24);
	GPIOCALTFN0 |= 1<<24;
	
	//D11变成输出模式
	GPIOCOUTENB |= (1<<12);
	
	
	while(1){
		GPIOEOUT &= ~(1<<13);
		delay();
		GPIOEOUT |= 1<<13;
		delay();
		GPIOCOUT &= ~(1<<17);
		delay();
		GPIOCOUT |= 1<<17;
		delay();
		GPIOCOUT &= ~(1<<8);
		delay();
		GPIOCOUT |= 1<<8;
		delay();
		GPIOCOUT &= ~(1<<7);
		delay();
		GPIOCOUT |= 1<<7;
		delay();
		GPIOCOUT &= ~(1<<12);
		delay();
		GPIOCOUT |= 1<<12;
		delay();
		
	}
	
}

void delay(void){
	volatile unsigned int i = 0x2000000;

	while(i--);
}