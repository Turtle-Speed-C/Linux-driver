#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <cfg_type.h>
#include <linux/delay.h>

#define LED_NUM 4

static unsigned int led_gpios[LED_NUM] = {
	PAD_GPIO_E + 13, 	//LED7
	PAD_GPIO_C + 17,	//LED8
	PAD_GPIO_C + 8,		//LED9
	PAD_GPIO_C + 7		//LED10
};

static struct cdev led_cdev;
static dev_t dev_num;
static struct class* led_class;
static struct device* led_device;

static int led_open(struct inode *inode, struct file *file){
	printk(KERN_INFO"led_open");
	return 0;
}

ssize_t led_write (struct file *file, const char __user *buf, size_t count, loff_t *pos){
	char kbuf[10] ={0}; 
	int num = 10;
	int i = 0;
	int j = 0;
	int ret = 0;
	printk(KERN_INFO "led_write\n");
	
	//unsigned long copy_from_user(void *to, const void __user *from, unsigned long n);
	ret = copy_from_user(kbuf,buf,count);
	if(ret > 0){
		printk(KERN_ERR "err_led_write\n");
		return -EFAULT;
	}
	
	while(num--){
		for(i=0;i<LED_NUM;i++){
			for(j=0;j<LED_NUM;j++){
				gpio_set_value(led_gpios[j],1);
			}
			gpio_set_value(led_gpios[i],0);
			
			msleep(300);
		}
	}
	
	return count;
}

int led_release (struct inode *inode, struct file *file){
	printk(KERN_INFO "led_release\n");
	return 0;
}

static const struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.open = led_open,
	.release = led_release,
	.write = led_write,
};

static int __init led_init(void){
	int ret,i;
	
	//dev_num = MKDEV(240,0);
	ret = alloc_chrdev_region(&dev_num, 0, 1, "flow_led");
	if(ret<0){
		printk(KERN_ERR"err_alloc_chrdev_region");
		goto err_register_chrdev_region;
	}
	
	cdev_init(&led_cdev,&led_fops);
	ret = cdev_add(&led_cdev,dev_num,1);
	if(ret<0){
		printk(KERN_ERR"err_cdev_add");
		goto err_cdev_add;
	}
	
	led_class = class_create(THIS_MODULE,"led_class");  //创建一个设备类 led_class，会在 /sys/class/led_class/ 生成对应目录。
	if(IS_ERR(led_class)){
		printk(KERN_ERR"err_class_create\n");
		goto err_class_create;
	}
	
	led_device = device_create(led_class,NULL,dev_num,NULL,"myled");  // /dev/myled
	if(IS_ERR(led_device)){
		printk(KERN_ERR"err_device_create\n");
		goto err_device_create;
	}
	
	for(i=0;i<LED_NUM;i++){
		gpio_free(led_gpios[i]);
		
		ret = gpio_request(led_gpios[i],"flow_led");
		if(ret<0){
			printk(KERN_ERR"gpio_request faile\n");
			goto err_gpio_request;
		}
		
		ret = gpio_direction_output(led_gpios[i],1);    // 默认全灭
		if (ret < 0) {
			printk(KERN_ERR"gpio_direction_output failed\n");
			gpio_free(led_gpios[i]);
			goto err_gpio_request;
		}
	}
	
    printk(KERN_INFO "led_init success\n");
    return 0;
	
	//为什么要这么写呢，因为这个是循环的，如果gpio_request失败了
	//需要把之前的都释放掉，他也要有一步gpio_free(led_gpios[i])；
	//所以应该把这些写在一起，
	//
	//有循环-->要考虑是不是改把这些合到一起
	//
err_gpio_request:
	while(--i>=0){
		gpio_free(led_gpios[i]);
	}
	device_destroy(led_class, dev_num); 
	
err_device_create:
	class_destroy(led_class);
	
err_class_create:
	cdev_del(&led_cdev);
	
err_cdev_add:
	unregister_chrdev_region(dev_num,1);
	
err_register_chrdev_region:
	return ret;
	
}

static void __exit led_exit(void){
	int i;
	for(i = 0;i<LED_NUM;i++){
		gpio_free(led_gpios[i]);
	}
	device_destroy(led_class, dev_num); 
	class_destroy(led_class);
	cdev_del(&led_cdev);
	unregister_chrdev_region(dev_num,1);
	printk(KERN_INFO"led_exit\n");
	return ;
}

module_init(led_init);
module_exit(led_exit);

MODULE_AUTHOR("LIXIAOFAN");
MODULE_DESCRIPTION("this is a led driver");
MODULE_LICENSE("GPL");


