#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/random.h>
MODULE_AUTHOR("Hiroki Shimada");
MODULE_DESCRIPTION("Rock-Paper-Scissors");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;
static volatile u32 *gpio_base = NULL;

static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos){
    char hand, cp_hand;
    char ex_hand[4][9] = {"Rock", "Scissors", "Paper", '\0'};
    if(copy_from_user(&hand,buf,sizeof(char))) return -EFAULT;
    if(hand != '\n'){
    	cp_hand = get_random_int();
	cp_hand = cp_hand % 3 + 1;
    	printk(KERN_INFO "cp_hand = %s\n",ex_hand[cp_hand-1]);

//cp_hand: 1 = Rock, 2 = Scissors, 3 = Paper
    	if((hand == 'r' && cp_hand == 2) || (hand == 's' && cp_hand == 3) || (hand == 'p' && cp_hand == 1)){
        	gpio_base[7] = 1<<25;
    	}
    	else{
        	gpio_base[10] = 1<<25;
    	}
    }
    return 1;
}

static ssize_t howTo_read(struct file* filp, char* buf, size_t count, loff_t* pos){
  int size = 0;
  printk(KERN_INFO "Rock  	->	r\n");
  printk(KERN_INFO "Scissors	->	s\n");
  printk(KERN_INFO "Paper	->	p\n");
  return 0;
}


static struct file_operations led_fops = {
        .owner = THIS_MODULE,
        .write = led_write,
        .read = howTo_read
};

static int __init init_mod(void){
    int retval;
    gpio_base = ioremap_nocache(0x3f200000, 0xA0);
    const u32 led = 25;
    const u32 index = led/10;
    const u32 shift = (led%10)*3;
    const u32 mask = ~(0x7 << shift);
    gpio_base[index] = (gpio_base[index] & mask) | (0x1 << shift);
    retval = alloc_chrdev_region(&dev, 0, 1, "myled");
    printk(KERN_INFO "%s is loaded. major:%d\n",__FILE__, MAJOR(dev));

    cdev_init(&cdv, &led_fops);
    retval = cdev_add(&cdv, dev, 1);
    if(retval < 0){
        printk(KERN_ERR "cdev_add failed. major:%d, minor:%d",MAJOR(dev),MINOR(dev));
        return retval;
        }
    cls = class_create(THIS_MODULE, "myled");
    if(IS_ERR(cls)){
        printk(KERN_ERR "class_create failed.");
        return PTR_ERR(cls);
    }
    device_create(cls, NULL, dev, NULL, "myled%d",MINOR(dev));
    return 0;
}

static void __exit cleanup_mod(void){
    cdev_del(&cdv);
    device_destroy(cls, dev);
    class_destroy(cls);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "%s is unloaded. major:%d\n",__FILE__, MAJOR(dev));
}

module_init(init_mod);
module_exit(cleanup_mod);
