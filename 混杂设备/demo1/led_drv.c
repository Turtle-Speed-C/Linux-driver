#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <mach/platform.h>
#include <linux/miscdevice.h>

static struct gpio leds_gpios[] = {
	{ PAD_GPIO_E+13, GPIOF_OUT_INIT_HIGH, "D7 LED" }, /* default to OFF */
	{ PAD_GPIO_C+17, GPIOF_OUT_INIT_HIGH, "D8 LED" }, /* default to OFF */
	{ PAD_GPIO_C+8,  GPIOF_OUT_INIT_HIGH, "D9 LED" }, /* default to OFF */	
	{ PAD_GPIO_C+7,  GPIOF_OUT_INIT_HIGH, "D10 LED" }, /* default to OFF */	
};

int myled_open (struct inode * inode, struct file * file)
{
	printk(KERN_INFO"myled_open\n");
	return 0;
}

int myled_release (struct inode *inode, struct file *file)
{
	printk(KERN_INFO"myled_release\n");
	return 0;
}

ssize_t myled_write (struct file *file, const char __user *buf, size_t len, loff_t *loff)
{

	int rt;
	
	char kbuf[2]={0};
	int n=0;
	
	if(buf == NULL)
		return -EFAULT;

	if(len > sizeof kbuf)
		len = sizeof kbuf;

	rt = copy_from_user(kbuf,buf,len);

	rt = len - rt; //得到已经拷贝成功的字节数
	
	//kbuf[0]:7~10，用于指定控制哪个led灯，D7 ~ D10
	//kbuf[1]:1-点亮 0-熄灭
	
	n = kbuf[0] - 7;//0~3

	gpio_set_value(leds_gpios[n].gpio,!kbuf[1]);  

	printk(KERN_INFO"[myled_write]:kbuf[0]=%d kbuf[1]=%d\n",kbuf[0],kbuf[1]);

	return rt;
	
}

static struct file_operations myled_fops={
	.owner = THIS_MODULE,
	.open = myled_open,
	.release = myled_release,
	.write = &myled_write,
};

static struct miscdevice myled_misc={
	.minor=MISC_DYNAMIC_MINOR,
	.name="myled",
	.fops=&myled_fops,
};

static int __init myled_init(void)
{
	int rt;

	/*1.混杂设备的注册 */
	rt = misc_register(&myled_misc);
	if(rt < 0)
	{
		printk(KERN_ERR"misc_register fail\n");

		goto err_misc_register;			
		
	}	
	
	/*2.批量申请gpio引脚 */
	gpio_free_array(leds_gpios,ARRAY_SIZE(leds_gpios));
	//因为内核已经申请了这些引脚，所以释放之后，才能调用gpio_request_array函数
	rt = gpio_request_array(leds_gpios,ARRAY_SIZE(leds_gpios));
	if(rt < 0)
	{
		printk(KERN_ERR"gpio_request_array fail\n");

		goto err_gpio_request_array;			
		
	}

	printk(KERN_INFO"myled_init\n");
	return 0;

err_gpio_request_array:
	misc_deregister(&myled_misc);	
err_misc_register:
	return rt;
}

static void __exit myled_exit(void)
{
	/* 注销混杂设备 */
	misc_deregister(&myled_misc);
	
	/* 释放gpio引脚 */
	gpio_free_array(leds_gpios,ARRAY_SIZE(leds_gpios));
	
	
	printk(KERN_INFO"myled_exit\n");	
	
}

module_init(myled_init);//驱动的入口，使用insmod命令加载该驱动
module_exit(myled_exit);//驱动的出口，使用rmmod命令卸载该驱动

MODULE_AUTHOR("Teacher.Wen");
MODULE_DESCRIPTION("This is led driver");
MODULE_LICENSE("GPL");