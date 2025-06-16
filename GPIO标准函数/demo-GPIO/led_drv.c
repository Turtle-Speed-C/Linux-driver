#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <cfg_type.h>


#define GPIOD7 (PAD_GPIO_E + 13)

static struct cdev led_cdev;

static dev_t dev_num = 0;

static struct class *led_class = NULL;

static struct device *led_device = NULL;

static int led_chrdev_open(struct inode *inode, struct file *file){
	printk(KERN_INFO"led_chrdev_open");
	return 0;
}

static int led_chrdev_close(struct inode *inode, struct file *file){
	printk(KERN_INFO"led_chrdev_close");
	return 0;
}

//pos	loff_t *	文件偏移量指针，可根据写入后更新
static ssize_t led_chrdev_read (struct file *file, char __user *buf, size_t count, loff_t * pos){
	
	int rt = 0;
	char kbuf[1] = {'5'};
	if(count>1){
		count= 1;
	}
	
	//copy_to_user() 是把内核空间 → 拷贝到 → 用户空间
	//long copy_to_user(void __user *to, const void *from, unsigned long n);
	rt = copy_to_user(buf,kbuf,1);
	if(rt>0){
		printk(KERN_ERR"cope_to_user_err\n");
		return -EFAULT;  // 表示用户空间地址非法或访问失败
	}
	
	rt = count - rt;
	return rt;
}

static ssize_t led_chrdev_write (struct file *file, const char __user *buf, size_t count, loff_t *pos){
	int i;
	int rt = 0;
	char data[2] = {0};
	if(count>2){
		count = 2;
	}
	
	//copy_from_user() 是把用户空间 → 拷贝到 → 内核空间
	//返回值是 未被复制的字节数（如果成功全部复制，返回 0）
	rt = copy_from_user(data,buf,count);
	if(rt>0){
		printk(KERN_ERR"cope_from_user_err\n");
		return -EFAULT;  // 表示用户空间地址非法或访问失败
	}

	for(i=0;i<2;i++){
		if(data[i]=='1'){
			gpio_set_value(GPIOD7, 0);  // 灭LED
		}
		if(data[i]=='0'){
			gpio_set_value(GPIOD7, 1);  // 点亮LED
		}
	}
	
	rt = count - rt;
	return rt;
	
}

static const struct file_operations led_fops = {
	.owner		= THIS_MODULE,
	.open		= led_chrdev_open,
	.release	= led_chrdev_close,
	.read		= led_chrdev_read,
	.write		= led_chrdev_write,	
};

static int __init myled_init(void){
	
	int rt;
	
	//需要查看\Documentation\debice.txt
	dev_num=MKDEV(240,0);                  //创建一个设备号
	//注册设备号
	rt = register_chrdev_region(dev_num,1,"myled");   // 向内核注册设备号 
	if(rt<0){
		printk(KERN_ERR"register_chrdev_region_err\n");
		goto err_register_chrdev_region;
	}
	
	cdev_init(&led_cdev, &led_fops);    // 初始化 cdev，绑定文件操作函数
	
	rt = cdev_add(&led_cdev, dev_num, 1);   // 向内核注册 cdev 和设备号的对应关系
	if(rt<0){
		printk(KERN_ERR"cdev_add_err\n");
		goto err_cdev_add;
	}
	
	led_class = class_create(THIS_MODULE,"led_class");  //创建一个设备类 led_class，会在 /sys/class/led_class/ 生成对应目录。
	if(IS_ERR(led_class)){
		printk(KERN_ERR"class_create faile\n");
		goto err_class_create;
	}
	led_device = device_create(led_class,NULL,dev_num,NULL,"myled");  // /dev/myled
	if(IS_ERR(led_device)){
		printk(KERN_ERR"device_create faile\n");
		goto err_device_create;
	}
	
	gpio_free(GPIOD7);    // 释放 GPIO（防止之前别的驱动占用）
	
	rt = gpio_request(GPIOD7,"led_D7");   // 申请 GPIO
	if(rt<0){
		printk(KERN_ERR"gpio_request faile\n");
		goto err_gpio_request;
	}
	//配置GPIOE13为输出模式，初始电平为高电平
	rt = gpio_direction_output(GPIOD7,1);    // 设置为输出模式，高电平（LED 亮）
	if (rt < 0) {
		printk(KERN_ERR"gpio_direction_output failed\n");
		goto err_gpio_direction;
	}
	
	printk(KERN_INFO"myled_init\n");
	
	return 0;
	
//register_chrdev_region
//cdev_init
//cdev_add
//class_create
//device_create
//gpio_request
//gpio_direction_output
	
err_gpio_direction:
	gpio_free(GPIOD7);
	
err_gpio_request:
	device_destroy(led_class, dev_num); 
	
err_device_create:
	class_destroy(led_class);
	
err_class_create:
	cdev_del(&led_cdev);
	
err_cdev_add:
	unregister_chrdev_region(dev_num,1);
	
err_register_chrdev_region:
	return rt;
}

static void __exit myled_exit(void){
	
	gpio_free(GPIOD7);
	device_destroy(led_class, dev_num); 
	class_destroy(led_class); 
	unregister_chrdev_region(dev_num,1);
	printk(KERN_INFO"myled_exit\n");
	
}


module_init(myled_init);
module_exit(myled_exit);

MODULE_AUTHOR("LIXIAOFAN");
MODULE_DESCRIPTION("this is a led driver");
MODULE_LICENSE("GPL");


