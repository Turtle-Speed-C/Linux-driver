#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <cfg_type.h>
#include <linux/delay.h>

//超声波工作原理
//✅ 一、超声波模块（如 HC-SR04）测距原理图解
// Trig（触发）引脚
//驱动程序给 Trig 引脚发送至少 10 微秒的高电平；
//这会触发模块发射 一个 40kHz 的超声波脉冲（持续 8 个周期）；
//发射完成后，模块会自动拉高 Echo 引脚。

//️Echo（回响）引脚
//Echo 引脚在 Trig 触发后，会：
//立即从低电平变为高电平；
//然后模块开始监听回波；
//当回波返回时，Echo 引脚由高变回低电平；
//Echo 为高电平的这段时间，表示超声波从发射到返回所用的时间。


static dev_t sr04_num=0;
static struct cdev sr04_dev;

static struct class *sr04_class;
static struct device *sr04_device;

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
static struct gpio sr04_gpios[2]={
	//引脚编号，引脚输出模式-初始电平状态为高电平，自定义引脚的名字
	{PAD_GPIO_D+19,GPIOF_OUT_INIT_LOW,"SR04 Trip"}, //输出模式
	{PAD_GPIO_D+15,GPIOF_IN          ,"SR04 Echo"}, //输入模式
};


int sr04_open(struct inode *inode, struct file *file){
	printk(KERN_INFO"sr04_open\n");
	return 0;
}

int sr04_release(struct inode *inode, struct file *file){
	printk(KERN_INFO"sr04_release\n");
	return 0;
}

unsigned int sr04_get_distance(void){
	//int gpio_get_value(unsigned int gpio);
	//gpio：GPIO 引脚编号（比如 sr04_gpios[1].gpio，即 Echo 引脚）。
	//从指定 GPIO 引脚读取当前的电平状态
	
	//gpio：GPIO 引脚编号（比如 sr04_gpios[0].gpio，即 Trig 引脚）；
	//value：设置的电平值（0 = 低电平，非 0 = 高电平）。
	//向指定的 GPIO 引脚设置输出电平
	
	//unsigned int
	uint32_t t=0;
	int timeout=10000;
	float distance = 0;
	
	//TRIG引脚输出高电平
	gpio_set_value(sr04_gpios[0].gpio,1);
	
	udelay(20);
	
	//TRIG引脚输出低电平
	gpio_set_value(sr04_gpios[0].gpio,0);
	
	//等待测量结果，等待ECHO引脚由低变高
	while(gpio_get_value(sr04_gpios[1].gpio)!=1){
		udelay(1);
		timeout--;
	}
	if (timeout == 0) {
        printk("Timeout waiting for Echo HIGH\n");
        return 0;
    }
	
	//当 Echo 高电平时进入循环；
	//每次 udelay(9) 是 延时 9 微秒；
	//每次延时都 t++，所以最终 t 表示 Echo 高电平持续的时间，单位是：9 微秒。
	//测量ECHO引脚高电平的持续时间
	while(gpio_get_value(sr04_gpios[1].gpio)){
		
		udelay(9);
		
		t++;
		
	}
	
	if (t > 100000) {
        printk(KERN_WARNING "Echo pulse is too long\n");
        return 0;  // 返回一个合理值或错误码
    }
	
	distance=t*0.153f;	
	
	return distance;
}

ssize_t sr04_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
    unsigned int distance_cm = sr04_get_distance();
    
    if (copy_to_user(buf, &distance_cm, sizeof(distance_cm)))
        return -EFAULT;

    return sizeof(distance_cm);
}

static const struct file_operations sr04_fops={
	.owner = THIS_MODULE,
	.open = sr04_open,
	.release = sr04_release,  
	.read = sr04_read,
};

static int __init sr04_init(void){
	int rt=0;
	
	rt=alloc_chrdev_region(&sr04_num,0,1,"mysr04");
	if(rt<0){
		printk(KERN_ERR"err_alloc_chrdev_region\n");
		goto err_alloc_chrdev_region;
	}
	
	printk("major=%d\n",MAJOR(sr04_num));
	printk("minor=%d\n",MINOR(sr04_num));
	
	cdev_init(&sr04_dev,&sr04_fops);
	rt=cdev_add(&sr04_dev,sr04_num,1);
	if(rt<0){
		printk(KERN_ERR"err_cdev_add\n");
		goto err_cdev_add;
	}
	
	sr04_class = class_create(THIS_MODULE,"sr04_class");  //创建一个设备类 led_class，会在 /sys/class/sr04_class/ 生成对应目录。
	if(IS_ERR(sr04_class)){
		printk(KERN_ERR"err_class_create\n");
		goto err_class_create;
	}
	
	sr04_device = device_create(sr04_class,NULL,sr04_num,NULL,"mysr04");  // /dev/mysr04
	if(IS_ERR(sr04_device)){
		printk(KERN_ERR"err_device_create\n");
		goto err_device_create;
	}
	
	gpio_free_array(sr04_gpios, ARRAY_SIZE(sr04_gpios));
	
	rt=gpio_request_array(sr04_gpios, ARRAY_SIZE(sr04_gpios));
	if(rt<0){
		printk(KERN_ERR"err_gpio_request_array\n");
		goto err_gpio_request_array;
	}
	printk(KERN_INFO"lxf is success\n");
	
	return 0;
	
err_gpio_request_array:
	device_destroy(sr04_class, sr04_num); 
	
err_device_create:
	class_destroy(sr04_class); 
	
err_class_create:
	cdev_del(&sr04_dev);
	
err_cdev_add:
	unregister_chrdev_region(sr04_num,1);
		
err_alloc_chrdev_region:
	return rt; 
	
}

static void __exit sr04_exit(void){
	gpio_free_array(sr04_gpios, ARRAY_SIZE(sr04_gpios));
	device_destroy(sr04_class, sr04_num); 
	class_destroy(sr04_class); 
	cdev_del(&sr04_dev);
	unregister_chrdev_region(sr04_num,1);
	return ; 
}

module_init(sr04_init);
module_exit(sr04_exit);

MODULE_AUTHOR("LIXIAOFAN");
MODULE_DESCRIPTION("this is a sr04 driver");
MODULE_LICENSE("GPL");