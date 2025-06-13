#define CPIOEOUT    (*(volatile unsigned int *)0xC001E000)
#define CPIOEOUTENB (*(volatile unsigned int *)0xC001E004)
#define CPIOEALTFN0 (*(volatile unsigned int *)0xC001E020)


void delay(void);
void _start(void){
	CPIOEALTFN0&=~(3<<26);
	CPIOEOUTENB|=1<<13;
	
	while(1){
		CPIOEOUT&=~(1<<13);
		delay();
		CPIOEOUT|=(1<<13);
		delay();
	}
}

void delay(void){
	volatile unsigned int i = 0x2000000;
	
	while(i--);
}