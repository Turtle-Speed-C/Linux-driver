#include <linux/init.h>
#include <linux/module.h>

static int baud=9600;
static int port[4]={1,2,3,4};
static int port_cnt=0;  //用来记录当前数组参数的个数
static char *name="vcom"; 

module_param(baud,int,0644);
module_param_array(port,int,&port_cnt,0644);
module_param(name,charp,0644);

static int __init myled_init(void){
	printk(KERN_INFO"myled_init\n");
	
	printk(KERN_INFO"baud=%d\n",baud);
	printk(KERN_INFO"port %d %d %d %d",port[0],port[1],port[2],port[3]);
	printk(KERN_INFO"name=%s\n",name);
	
	return 0;
}

static void __exit myled_exit(void){
	
	printk("myled_exit\n");
	
}

module_init(myled_init);
module_exit(myled_exit);

MODULE_AUTHOR("LIXIAOFAN");
MODULE_DESCRIPTION("this is a printk");
MODULE_LICENSE("GPL");