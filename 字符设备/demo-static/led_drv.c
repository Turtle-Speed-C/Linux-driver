#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

static dev_t dev_num = 0;

static int __init myled_init(void){
	
	int rt;
	//制作一个设备号
	//需要查看\Documentation\debice.txt
	dev_num=MKDEV(240,0);
	//注册设备号
	rt = register_chrdev_region(dev_num,1,"myled");
	if(rt<0){
		printk("KERN_ERR""myled_init_err\n");
		goto err_register_chrdev_region;
	}
	printk("KERN_INFO","myled_init\n");
	
	return 0;
	
err_register_chrdev_region:
	return rt;
}

static void __exit myled_exit(void){
	
	//注销设备号
	unregister_chrdev_region(dev_num,1);
	printk("KERN_INFO","myled_exit\n");
	
}




module_init(myled_init);
module_exit(myled_exit);

MODULE_AUTHOR("LIXIAOFAN");
MODULE_DESCRIPTION("this is a led driver");
MODULE_LICENSE("GPL");



//要静态注册一个设备register_chardev_region()->需要(dev_t from, unsigned count, const char *name)
//->dev_t from是一个设备号，所以需要用到static dev_t dev_num = 0 ->需要使用到MADEV来规定一个设备号
//