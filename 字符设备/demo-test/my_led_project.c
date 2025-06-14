#include <linux/moudle.h>
#include <linux/init.h>
//定义了初始化与清理函数的标记宏 __init、__exit，用于将这些函数放到特定的节段，以便在初始化后回收内存。
#include <linux/cdev.h>
//提供字符设备（struct cdev）相关的接口和声明，包括 cdev_init()、cdev_add()、cdev_del() 等。
#include <linux/fs.h>
//定义了文件系统核心的数据结构与接口，最重要的是 struct file_operations（文件操作接口）和各种设备号管理函数。

/*struct cdev {
	struct kobject kobj;
	struct module *owner;
	const struct file_operations *ops;
	struct list_head list;
	dev_t dev;
	unsigned int count;
};*/
static struct cdev led_cdev;

//typedef __u32 __kernel_dev_t;
//typedef __kernel_dev_t  dev_t;
//它用来报错一个设备号（主次设备号打包后的类型），初始化为0
static dev_t led_dev_num=0;

//应用程序执行open，就会执行此函数
static int led_open(struct inode *inode,struct file *file){
/* struct inode:
 * Keep mostly read-only and often accessed (especially for
 * the RCU path lookup and 'stat' data) fields at the beginning
 * of the 'struct inode'
 * 索引节点结构体
 * 把大多数是只读的，并且经常被访问的字段（特别是在 RCU 路径查找和 `stat` 系统调用中使用的数据）
 * 放在结构体开头
 */
 
 /*
 * fu_list becomes invalid after file_free is called and queued via
 * fu_rcuhead for RCU freeing
 */
 /*
 * fu_list变得无效，在file_free被调用后并且通过fu_rcuhead排队等待RCU释放
 */
	printk("<4>""led_open");

	return 0;

}

int led_close(struct inode *inode,struct file *file){
	printk("<4>""led_close");

	return 0;
}

/* file_operations
 * These macros are for out of kernel modules to test that
 * the kernel supports the unlocked_ioctl and compat_ioctl
 * fields in struct file_operations. */

static struct file_operations led_fops={
	//#define THIS_MODULE ((struct module *)0)
	.owner = THIS_MODULE,
	.open = led_open,
	.release = led_close,
}

//函数入口
static int __init led_init(void){
	//__init标记告诉编译器将函数放入.init.text，初始化完毕后可回收
	int rt = 0;

	//生成设备号
	//#define MKDEV(ma,mi)	(((ma) << MINORBITS) | (mi))
	//#define MINORBITS	20
	led_dev_num=MKDEV(239,0);
	
	//静态注册设备
	//若设备号有效，则在/proc/devices文件中找到"myled"，只是简单描述有哪些设备
	/**
	 * int register_chrdev_region(dev_t from, unsigned count, const char *name)
	 * register_chrdev_region() - register a range of device numbers
	 * @from: the first in the desired range of device numbers; must include
	 *        the major number.
	 * @count: the number of consecutive device numbers required
	 * @name: the name of the device or driver.
	 *
	 * Return value is zero on success, a negative error code on failure.
	 */
	rt = register_chardev_region(led_dev_num,1,"myled");
	if(rt<0){
		printk("<3>""err_register_chardev_region \n");

		goto err_register_chardev_region;
	}
	/**
	 * void cdev_init(struct cdev *cdev, const struct file_operations *fops)
	 * cdev_init() - initialize a cdev structure
	 * @cdev: the structure to initialize
	 * @fops: the file_operations for this device
	 *
	 * Initializes @cdev, remembering @fops, making it ready to add to the
	 * system with cdev_add().
	 */
	cdev_init(&led_cdev,&led_fops);

	/**
	 * int cdev_add(struct cdev *p, dev_t dev, unsigned count)
	 * cdev_add() - add a char device to the system
	 * @p: the cdev structure for the device
	 * @dev: the first device number for which this device is responsible
	 * @count: the number of consecutive minor numbers corresponding to this
	 *         device
	 *
	 * cdev_add() adds the device represented by @p to the system, making it
	 * live immediately.  A negative error code is returned on failure.
	 */
    //将字符设备加入内核
	rt=cdev_add(led_cdev,led_cdev_num,1);
	if(rt<0){
		printk("<3>""err_cdev_add \n");

		goto err_cdev_add;
	}

	printk("Hello lxf\n");

	return 0;


	err_register_chardev_region:
		return rt;
	err_cdev_add:
	/**
	 * void unregister_chrdev_region(dev_t from, unsigned count)
	 * unregister_chrdev_region() - return a range of device numbers
	 * @from: the first in the range of numbers to unregister
	 * @count: the number of device numbers to unregister
	 *
	 * This function will unregister a range of @count device numbers,
	 * starting with @from.  The caller should normally be the one who
	 * allocated those numbers in the first place...
	 */
		unregister_chardev_region(led_dev_num,1);

}

static void __exit led_exit(void){
/**
 * 从内核中移除
 * void cdev_del(struct cdev *p)
 * cdev_del() - remove a cdev from the system
 * @p: the cdev structure to be removed
 *
 * cdev_del() removes @p from the system, possibly freeing the structure
 * itself.
 */

	cdev_del(&led_cdev);
	unregister_chardev_region(led_dev_num,1);
	printk("GoodBye lxf");
}


module_init(led_init);

module_exit(led_exit);


MODULE_AUTHOR("lixiaofan");
MODULE_DESCRIPTION("Led Device Driver");
MODULE_LICENSE("GPL");	



























