#include <linux/init.h>
#include <linux/module.h>

static int __init myled_init(void){
	printk("sum\n");
	
	return 0;
}

static void __exit myled_exit(void){
	
	printk("sum\n");
	
}

module_init(myled_init);
module_exit(myled_exit);

MODULE_AUTHOR("LIXIAOFAN");
MODULE_DESCRIPTION("this is a sum driver");
MODULE_LICENSE("GPL");