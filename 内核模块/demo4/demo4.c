#include <linux/init.h>
#include <linux/module.h>

extern int getsum(int a,int b);

static int __init demo4_init(void){
	printk(KERN_INFO "myled_init\n");
	printk(KERN_INFO "%d\n",getsum(100,200));
	return 0;
}

static void __exit demo4_exit(void){
	
	printk("myled_exit\n");
	
}

module_init(demo4_init);
module_exit(demo4_exit);

MODULE_AUTHOR("LIXIAOFAN");
MODULE_DESCRIPTION("this is a led driver");
MODULE_LICENSE("GPL");