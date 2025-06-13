//k2:GPIOA28 K3:GPIOB30 K4:GPIOB31 K6:GPIOB9
//00 01 01 00
//D7:E13 D8:C17 D9:C8 D10:C7 
//00 01 01 01 (确定了GPIOxALTFN0/1 该怎么弄成GPIO模式)


//K6:GPIOB9
#define GPIOBALTFN0 (*(volatile unsigned int *)0xC001B020)
#define GPIOBOUTENB (*(volatile unsigned int *)0xC001B004)
#define GPIOBPAD    (*(volatile unsigned int *)0xC001B018)
//k2:GPIOA28
#define GPIOAALTFN1 (*(volatile unsigned int *)0xC001A024)
#define GPIOAOUTENB (*(volatile unsigned int *)0xC001A004)
#define GPIOAPAD    (*(volatile unsigned int *)0xC001A018)
//K3:GPIOB30
#define GPIOBALTFN1 (*(volatile unsigned int *)0xC001B024)
//#define GPIOBOUTENB (*(volatile unsigned int *)0xC001A004)
//#define GPIOBPAD    (*(volatile unsigned int *)0xC001A018)
//K4:GPIOB31
//#define GPIOBALTFN1 (*(volatile unsigned int *)0xC001B024)
//#define GPIOBOUTENB (*(volatile unsigned int *)0xC001A004)
//#define GPIOBPAD    (*(volatile unsigned int *)0xC001A018)

#define GPIOEOUT    (*(volatile unsigned int*)0xC001E000)
#define GPIOEOUTENB (*(volatile unsigned int*)0xC001E004)
#define GPIOEALTFN0 (*(volatile unsigned int*)0xC001E020)

#define GPIOCOUT    (*(volatile unsigned int*)0xC001C000)
#define GPIOCOUTENB (*(volatile unsigned int*)0xC001C004)
#define GPIOCALTFN1 (*(volatile unsigned int*)0xC001C024)
#define GPIOCALTFN0 (*(volatile unsigned int*)0xC001C020)

void _start(void){
	//按键改为GPIO模式
	GPIOBALTFN0 &= ~(3<<18); //K6:GPIOB9 00
	
	GPIOAALTFN1 &= ~(3<<24); //k2:GPIOA28 00
	
	GPIOBALTFN1 &= ~(3<<28); //K3:GPIOB30 01
	GPIOBALTFN1 |= 1<<28;
	
	GPIOBALTFN1 &= ~(3<<30);//K4:GPIOB31 01
	GPIOBALTFN1 |= 1<<30;
	
	//按键改为输入模式
	GPIOBOUTENB &= ~(1<<9);
	GPIOAOUTENB &= ~(1<<28);
	GPIOBOUTENB &= ~(1<<30);
	GPIOBOUTENB &= ~(1<<31);
	
	//灯改为GPIO模式
	GPIOEALTFN0 &= ~(3<<26); //D7变成GPIO模式
	
	GPIOEOUTENB |= (1<<13); //D7变成输出模式
	
	GPIOCALTFN1 &= ~(3<<2); //D8变成GPIO模式
	GPIOCALTFN1 |= 1<<2;
	
	GPIOCOUTENB |= (1<<17); //D8变成输出模式
	
	GPIOCALTFN0 &= ~(3<<16); //D9变成GPIO模式
	GPIOCALTFN0 |= 1<<16;
	
	GPIOCOUTENB |= (1<<8); //D9变成输出模式
	
	GPIOCALTFN0 &= ~(3<<14); //D10变成GPIO模式
	GPIOCALTFN0 |= 1<<14;
	
	GPIOCOUTENB |= (1<<7); //D10变成输出模式
	
	while(1){
		if((GPIOAPAD & (1<<28)) == 0){
			GPIOEOUT &= ~(1<<13);
		}
		else{
			GPIOEOUT |= (1<<13);
		}
		
		if((GPIOBPAD & (1<<9)) == 0){
			GPIOCOUT &= ~(1<<17);
		}
		else{
			GPIOCOUT |= (1<<17);
		}
		
		if((GPIOBPAD & (1<<30)) == 0){
			GPIOCOUT &= ~(1<<8);
		}
		else{
			GPIOCOUT |= (1<<8);
		}
		
		if((GPIOBPAD & (1<<31)) == 0){
			GPIOCOUT &= ~(1<<7);
		}
		else{
			GPIOCOUT |= (1<<7);
		}
	}
	
}
