#include <linux/init.h>
#include <linux/module.h>

int getsum(int a,int b){
	return (a+b);
}

EXPORT_SYMBOL(getsum);

static int __init sum_init(void){
	//printk(KERN_INFO"sum\n");
	
	return 0;
}

static void __exit sum_exit(void){
	
	printk(KERN_EMERG"sum\n");
	
}

module_init(sum_init);
module_exit(sum_exit);

MODULE_AUTHOR("LIXIAOFAN");
MODULE_DESCRIPTION("this is a sum driver");
MODULE_LICENSE("GPL");