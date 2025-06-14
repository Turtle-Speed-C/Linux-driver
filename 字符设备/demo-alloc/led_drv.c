#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

static dev_t dev_num = 0;

static int __init myled_init(void){
	
	int rt;
	//动态注册设备号
	/**
	 *int alloc_chrdev_region(dev_t *dev, unsigned baseminor, unsigned count,
				const char *name)
	 * alloc_chrdev_region() - register a range of char device numbers
	 * @dev: output parameter for first assigned number
	 * @baseminor: first of the requested range of minor numbers
	 * @count: the number of minor numbers required
	 * @name: the name of the associated device or driver
	 * Allocates a range of char device numbers.  The major number will be
	 * chosen dynamically, and returned (along with the first minor number)
	 * in @dev.  Returns zero or a negative error code.
	 */
	rt = alloc_chrdev_region(&dev_num,0,1,"myled_alloc");
	if(rt<0){
		printk(KERN_ERR"myled_init_err\n");
		goto err_register_chrdev_region;
	}
	
	printk(KERN_INFO"myled_init\n");
	printk(KERN_INFO"dev_num:major=%d minor=%d\n",MAJOR(dev_num),MINOR(dev_num));
	
	return 0;
	
err_register_chrdev_region:
	return rt;
}

static void __exit myled_exit(void){
	
	//注销设备号
	unregister_chrdev_region(dev_num,1);
	printk(KERN_INFO"myled_exit\n");
	
}




module_init(myled_init);
module_exit(myled_exit);

MODULE_AUTHOR("LIXIAOFAN");
MODULE_DESCRIPTION("this is a led driver");
MODULE_LICENSE("GPL");



//要静态注册一个设备register_chardev_region()->需要(dev_t from, unsigned count, const char *name)
//->dev_t from是一个设备号，所以需要用到static dev_t dev_num = 0 ->需要使用到MADEV来规定一个设备号
//