#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

static struct cdev led_cdev;

static dev_t dev_num = 0;

static int led_chrdev_open(struct inode *inode, struct file *file){
	printk(KERN_INFO"led_chrdev_open");
	return 0;
}

int led_chrdev_close(struct inode *inode, struct file *file){
	printk(KERN_INFO"led_chrdev_close");
	return 0;
}

static const struct file_operations led_fops = {
	.owner		= THIS_MODULE,
	.open		= led_chrdev_open,
	.release	= led_chrdev_close,
};

static int __init myled_init(void){
	
	int rt;
	//制作一个设备号
	//需要查看\Documentation\debice.txt
	dev_num=MKDEV(240,0);
	//注册设备号
	rt = register_chrdev_region(dev_num,1,"myled");
	if(rt<0){
		printk(KERN_ERR"register_chrdev_region_err\n");
		goto err_register_chrdev_region;
	}
	
	cdev_init(&led_cdev, &led_fops);
	
	rt = cdev_add(&led_cdev, dev_num, 1);
	if(rt<0){
		printk(KERN_ERR"cdev_add_err\n");
		goto err_cdev_add;
	}
	
	printk(KERN_INFO"myled_init\n");
	
	return 0;
	
err_cdev_add:
	unregister_chrdev_region(dev_num,1);
	
err_register_chrdev_region:
	return rt;
}

static void __exit myled_exit(void){
	
	cdev_del(&led_cdev);
	//注销设备号
	unregister_chrdev_region(dev_num,1);
	printk(KERN_INFO"myled_exit\n");
	
}

/*1.err_cdev_add:
unregister_chrdev_region(dev_num,1);
和
err_register_chrdev_region:
return rt;
	
先err_cdev_add再err_register_chrdev_region的原因是
再注册到内核时需要涉及到设备号，如果先err_register_chrdev_region
会导致失败。符合先释放后注册的资源
	
2.cdev_del(&led_cdev);
和
unregister_chrdev_region(dev_num,1);
先释放后注册的资源
*/



module_init(myled_init);
module_exit(myled_exit);

MODULE_AUTHOR("LIXIAOFAN");
MODULE_DESCRIPTION("this is a led driver");
MODULE_LICENSE("GPL");


