#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <cfg_type.h>
#include <linux/delay.h>

static dev_t led_dev_num=0;
static struct cdev led_dev;

static struct class *led_class;
static struct device *led_device;

/**
 * struct gpio - a structure describing a GPIO with configuration
 * @gpio:	the GPIO number
 * @flags:	GPIO configuration as specified by GPIOF_*
 * @label:	a literal description string of this GPIO
 *
struct gpio {
	unsigned	gpio;      //引脚编号
	unsigned long	flags; //引脚模式
	const char	*label;    //自动逸引脚名称
};
*/
static struct gpio led_gpios[4]={
	//引脚编号，引脚输出模式-初始电平状态为高电平，自定义引脚的名字
	{PAD_GPIO_E+13,GPIOF_OUT_INIT_HIGH,"LED_D7"},
	{PAD_GPIO_C+17,GPIOF_OUT_INIT_HIGH,"LED_D8"},
	{PAD_GPIO_C+8,GPIOF_OUT_INIT_HIGH,"LED_D9"},
	{PAD_GPIO_C+7,GPIOF_OUT_INIT_HIGH,"LED_D10"},
};

ssize_t led_write(struct file *file, const char __user *buf, size_t len, loff_t *pos){
	char kbuf[2]={0};
	
	int rt = 0;
	
	if(buf==NULL){
		return -EINVAL;
	}
	
	if(len>sizeof(kbuf)){
		return -EINVAL;
	}
	
	//unsigned long copy_from_user(void *to, const void __user *from, unsigned long n);
	rt=copy_from_user(kbuf,buf,len);
	switch(kbuf[0]){
		case 7:{
			gpio_set_value(PAD_GPIO_E+13,kbuf[1]?1:0);
		}break;
		case 8:{
			gpio_set_value(PAD_GPIO_C+17,kbuf[1]?1:0);
		}break;
		case 9:{
			gpio_set_value(PAD_GPIO_C+8,kbuf[1]?1:0);
		}break;
		case 10:{
			gpio_set_value(PAD_GPIO_C+7,kbuf[1]?1:0);
		}break;
		
		default:break;
	}
	
	len=len-rt;
	
	return len;
}

int led_open(struct inode *inode, struct file *file){
	printk(KERN_INFO"led_open\n");
	return 0;
}

int led_release(struct inode *inode, struct file *file){
	printk(KERN_INFO"led_release\n");
	return 0;
}

static const struct file_operations led_fops={
	.owner = THIS_MODULE,
	.open = led_open,
	.release = led_release,
	.write = led_write,
};

static int __init led_init(void){
	int rt=0;
	
	rt=alloc_chrdev_region(&led_dev_num,0,1,"myled");
	if(rt<0){
		printk(KERN_ERR"err_alloc_chrdev_region\n");
		goto err_alloc_chrdev_region;
	}
	
	printk("major=%d\n",MAJOR(led_dev_num));
	printk("minor=%d\n",MINOR(led_dev_num));
	
	cdev_init(&led_dev,&led_fops);
	rt=cdev_add(&led_dev,led_dev_num,1);
	if(rt<0){
		printk(KERN_ERR"err_cdev_add\n");
		goto err_cdev_add;
	}
	
	led_class = class_create(THIS_MODULE,"led_class");  //创建一个设备类 led_class，会在 /sys/class/led_class/ 生成对应目录。
	if(IS_ERR(led_class)){
		printk(KERN_ERR"err_class_create\n");
		goto err_class_create;
	}
	
	led_device = device_create(led_class,NULL,led_dev_num,NULL,"myled");  // /dev/myled
	if(IS_ERR(led_device)){
		printk(KERN_ERR"err_device_create\n");
		goto err_device_create;
	}
	
	gpio_free_array(led_gpios, ARRAY_SIZE(led_gpios));
	
	rt=gpio_request_array(led_gpios, ARRAY_SIZE(led_gpios));
	if(rt<0){
		printk(KERN_ERR"err_gpio_request_array\n");
		goto err_gpio_request_array;
	}
	printk(KERN_INFO"lxf is success\n");
	
	return 0;
	
err_gpio_request_array:
	device_destroy(led_class, led_dev_num); 
	
err_device_create:
	class_destroy(led_class); 
	
err_class_create:
	cdev_del(&led_dev);
	
err_cdev_add:
	unregister_chrdev_region(led_dev_num,1);
		
err_alloc_chrdev_region:
	return rt; 
	
}

static void __exit led_exit(void){
	gpio_free_array(led_gpios, ARRAY_SIZE(led_gpios));
	device_destroy(led_class, led_dev_num); 
	class_destroy(led_class); 
	cdev_del(&led_dev);
	unregister_chrdev_region(led_dev_num,1);
	return ; 
}

module_init(led_init);
module_exit(led_exit);

MODULE_AUTHOR("LIXIAOFAN");
MODULE_DESCRIPTION("this is a led driver");
MODULE_LICENSE("GPL");