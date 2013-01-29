#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/ioport.h>
#include <linux/sysfs.h>
#include <linux/device.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include "bcm2835_gpioreg.h"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Freeburn Robotics Limited");
MODULE_DESCRIPTION("LedBorg control driver");
MODULE_VERSION("0.1");

#define NUM_PARAMS			(3)
#define STEP_INTERVAL_MS	(10)
#define FALSE				(0)
#define TRUE				(!FALSE)

#define GPIO_ADDR			(0x20200000)
#define GPIO_SIZE_BYTES		(180)

//#define DEBUG_ENTRY
//#define DEBUG_STEP
//#define DEBUG_MEM

#ifdef DEBUG_ENTRY
#define START				printk(KERN_ALERT "LedBorg: ST %s\n", __FUNCTION__)
#define END					printk(KERN_ALERT "LedBorg: EX %s\n", __FUNCTION__)
#define ERROR				printk(KERN_ALERT "LedBorg: FAIL %s\n", __FUNCTION__)
#else
#define START
#define END
#define ERROR
#endif
#define CLAMP(x, min, max)	((x) < (min) ? (min) : (x) > (max) ? (max) : (x))

inline void WriteWord(void *base, unsigned long addr, unsigned long shift, unsigned long val);
inline void ModifyWord(void *base, unsigned long addr, unsigned long shift, unsigned long mask, unsigned long val);
inline unsigned long ReadWord(void *base, unsigned long addr);
static int __init led_init(void);
int led_open(struct inode *inode, struct file *filp);
int led_release(struct inode *inode, struct file *filp);
ssize_t led_read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
ssize_t led_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);
inline void led_set_red(bool on);
inline void led_set_green(bool on);
inline void led_set_blue(bool on);
void led_perform_step(unsigned long arg);
static void __exit led_exit(void);

typedef struct global_data {
	// Resources
	dev_t				dev;
	struct cdev			cdev;
	struct class		*pclass;
	struct semaphore	mutex;
	struct timer_list	timer;
	// Internal
	int					max;	// Maximum value per channel
	int					step;	// Sequence step
	unsigned long		delay;	// Jiffy interval
	// Control
	int					red;
	int					green;
	int					blue;
	int					new_red;
	int					new_green;
	int					new_blue;
} global_data;

struct file_operations led_fops = {
	.owner		= THIS_MODULE,
	.read		= led_read,
	.write		= led_write,
	.open		= led_open,
	.release	= led_release,
};
global_data led = {
	// Internal
	.max		= 2,
	.step		= -1,
	// Control
	.red		= 0,
	.green		= 0,
	.blue		= 0,
	.new_red	= 0,
	.new_green	= 0,
	.new_blue	= 0,
};

struct resource *gpio_res;
void *gpio;

inline void WriteWord(void *base, unsigned long addr, unsigned long shift, unsigned long val) {
	addr += (unsigned long)base;
	val <<= shift;
#ifdef DEBUG_MEM
	printk(KERN_ALERT "LedBorg: 0x%08lX <- 0x%08lX\n", addr, val);
#endif
	iowrite32(val, addr);
}

inline void ModifyWord(void *base, unsigned long addr, unsigned long shift, unsigned long mask, unsigned long val) {
	unsigned long word;
	addr += (unsigned long)base;
	word = ioread32(addr);
#ifdef DEBUG_MEM
	printk(KERN_ALERT "LedBorg: 0x%08lX -> 0x%08lX\n", addr, word);
#endif
	mask <<= shift;
	word &= ~mask;
	word |= mask & (val << shift);
#ifdef DEBUG_MEM
	printk(KERN_ALERT "LedBorg: 0x%08lX <- 0x%08lX [0x%08lX]\n", addr, word, mask);
#endif
	iowrite32(word, addr);
}

inline unsigned long ReadWord(void *base, unsigned long addr) {
	unsigned long word;
	addr += (unsigned long)base;
	word = ioread32(addr);
#ifdef DEBUG_MEM
	printk(KERN_ALERT "LedBorg: 0x%08lX -> 0x%08lX\n", addr, word);
#endif
	return word;
}

static int __init led_init(void) {
	int errCode;
	START;

	led.delay = (STEP_INTERVAL_MS * HZ) / 1e3;
	printk(KERN_ALERT "LedBorg: Interval %dms, %lu jiff%s (%d jiffies/s)", STEP_INTERVAL_MS, led.delay, led.delay == 1 ? "y" : "ies", HZ);

	gpio_res = request_mem_region(GPIO_ADDR, GPIO_SIZE_BYTES, "ledborg");
	if (gpio_res == NULL) {
		printk(KERN_ALERT "LedBorg: Failed to request memory region\n");
		errCode = -EFAULT;
		goto err1;
	}

	gpio = ioremap(GPIO_ADDR, GPIO_SIZE_BYTES);
	if (gpio == NULL) {
		printk(KERN_ALERT "LedBorg: Failed to remap memory\n");
		errCode = -EFAULT;
		goto err2;
	}

	// Enable red LED
	ModifyWord(gpio, GPIO_FUNCTION_SELECT_1, GPIO_FUNCTION_SELECT_1_PIN_17_SHIFT, GPIO_FUNCTION_SELECT_1_PIN_17_MASK, GPIO_FUNCTION_SELECT_1_PIN_17_OUTPUT);
	WriteWord(gpio, GPIO_PIN_CLEAR_0, GPIO_PIN_CLEAR_0_PIN_17_SHIFT, GPIO_PIN_CLEAR_0_PIN_17_CLEAR);
	// Enable green LED
	ModifyWord(gpio, GPIO_FUNCTION_SELECT_2, GPIO_FUNCTION_SELECT_2_PIN_27_SHIFT, GPIO_FUNCTION_SELECT_2_PIN_27_MASK, GPIO_FUNCTION_SELECT_2_PIN_27_OUTPUT);
	WriteWord(gpio, GPIO_PIN_CLEAR_0, GPIO_PIN_CLEAR_0_PIN_27_SHIFT, GPIO_PIN_CLEAR_0_PIN_27_CLEAR);
	// Enable blue LED
	ModifyWord(gpio, GPIO_FUNCTION_SELECT_2, GPIO_FUNCTION_SELECT_2_PIN_22_SHIFT, GPIO_FUNCTION_SELECT_2_PIN_22_MASK, GPIO_FUNCTION_SELECT_2_PIN_22_OUTPUT);
	WriteWord(gpio, GPIO_PIN_CLEAR_0, GPIO_PIN_CLEAR_0_PIN_22_SHIFT, GPIO_PIN_CLEAR_0_PIN_22_CLEAR);

	errCode = alloc_chrdev_region(&led.dev, 0, 1, "ledborg");
	if (errCode) {
		printk(KERN_ALERT "LedBorg: Failed to allocate device\n");
		goto err3;
	}

	sema_init(&led.mutex, 1);

	cdev_init(&led.cdev, &led_fops);
	led.cdev.owner = THIS_MODULE;
	errCode = cdev_add(&led.cdev, led.dev, 1);
	if (errCode) {
		printk(KERN_ALERT "LedBorg: Failed to add character device\n");
		goto err4;
	}

	led.pclass = class_create(THIS_MODULE, "ledborg");
	if (IS_ERR(led.pclass)) {
		printk(KERN_ALERT "LedBorg: Failed to create class\n");
		errCode = PTR_ERR(led.pclass);
		goto err5;
	}

	device_create(led.pclass, NULL, led.dev, NULL, "ledborg");

	printk(KERN_INFO "LedBorg: Ready for use, max channel value of %d\n", led.max);

	init_timer(&led.timer);
	led.timer.data = 0;
	led.timer.function = led_perform_step;
	led.timer.expires = jiffies + led.delay;
	add_timer(&led.timer);

	END;
	return 0;

/*  ERROR HANDLER  */
err5:
	cdev_del(&led.cdev);
err4:
	unregister_chrdev_region(led.dev, 1);
err3:
	iounmap(gpio);
err2:
	release_mem_region(GPIO_ADDR, GPIO_SIZE_BYTES);
err1:
	ERROR;
	return errCode;
}

int led_open(struct inode *inode, struct file *filp) {
	START;

	END;
	return 0;
}

int led_release(struct inode *inode, struct file *filp) {
	START;

	END;
	return 0;
}

ssize_t led_read(struct file *filp, char __user *buff, size_t count, loff_t *offp) {
	ssize_t retval = 0;
	int i;
	char dat;
	START;

	if (down_interruptible(&led.mutex)) return -ERESTARTSYS;

	if (*offp >= NUM_PARAMS) goto out;
	if (*offp + count > NUM_PARAMS) count = NUM_PARAMS - *offp;

	for (i = 0; i < count; ++i) {
		switch (*offp) {
			case 0:		dat = '0' + (char)led.new_red;		break;
			case 1:		dat = '0' + (char)led.new_green;	break;
			case 2:		dat = '0' + (char)led.new_blue;		break;
			default:	dat = '\0';							break;
		}	
		if (copy_to_user(buff + i, &dat, 1)) {
			retval = -EFAULT;
			goto out;
		}
		++*offp;
		++retval;
	}

out:
	up(&led.mutex);
	END;
	return retval;
}

ssize_t led_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp) {
	ssize_t retval = 0;
	int i;
	char dat;
	START;

	if (down_interruptible(&led.mutex)) return -ERESTARTSYS;

	for (i = 0; i < count; ++i) {
		if (__copy_from_user(&dat, buff + i, 1)) {
			retval = -EFAULT;
			goto out;
		}
		switch (*offp) {
			case 0:		led.new_red		= CLAMP(dat - '0', 0, led.max);		break;
			case 1:		led.new_green	= CLAMP(dat - '0', 0, led.max);		break;
			case 2:		led.new_blue	= CLAMP(dat - '0', 0, led.max);		break;
			default:														break;
		}	
		++*offp;
		++retval;
	}

out:
	up(&led.mutex);
	END;
	return retval;
}

static void __exit led_exit(void) {
	START;

	del_timer(&led.timer);

	led_set_red(FALSE);
	led_set_green(FALSE);
	led_set_blue(FALSE);

	device_destroy(led.pclass, led.dev);
	class_destroy(led.pclass);
	cdev_del(&led.cdev);
	unregister_chrdev_region(led.dev, 1);
	iounmap(gpio);
	release_mem_region(GPIO_ADDR, GPIO_SIZE_BYTES);

	END;
}

inline void led_set_red(bool on) {
	if (on) {
#ifdef DEBUG_STEP
		printk("R");
#endif
		WriteWord(gpio, GPIO_PIN_SET_0, GPIO_PIN_SET_0_PIN_17_SHIFT, GPIO_PIN_SET_0_PIN_17_SET);
	} else {
#ifdef DEBUG_STEP
		printk("r");
#endif
		WriteWord(gpio, GPIO_PIN_CLEAR_0, GPIO_PIN_CLEAR_0_PIN_17_SHIFT, GPIO_PIN_CLEAR_0_PIN_17_CLEAR);
	}
}

inline void led_set_green(bool on) {
	if (on) {
#ifdef DEBUG_STEP
		printk("G");
#endif
		WriteWord(gpio, GPIO_PIN_SET_0, GPIO_PIN_SET_0_PIN_27_SHIFT, GPIO_PIN_SET_0_PIN_27_SET);
	} else {
#ifdef DEBUG_STEP
		printk("g");
#endif
		WriteWord(gpio, GPIO_PIN_CLEAR_0, GPIO_PIN_CLEAR_0_PIN_27_SHIFT, GPIO_PIN_CLEAR_0_PIN_27_CLEAR);
	}
}

inline void led_set_blue(bool on) {
	if (on) {
#ifdef DEBUG_STEP
		printk("B");
#endif
		WriteWord(gpio, GPIO_PIN_SET_0, GPIO_PIN_SET_0_PIN_22_SHIFT, GPIO_PIN_SET_0_PIN_22_SET);
	} else {
#ifdef DEBUG_STEP
		printk("b");
#endif
		WriteWord(gpio, GPIO_PIN_CLEAR_0, GPIO_PIN_CLEAR_0_PIN_22_SHIFT, GPIO_PIN_CLEAR_0_PIN_22_CLEAR);
	}
}

void led_perform_step(unsigned long arg) {
	START;

	++led.step;
	if (led.step >= led.max) led.step = 0;

#ifdef DEBUG_STEP
	printk(KERN_INFO "LedBorg: Step %d ", led.step);
#endif

	if (led.step == 0) {
		led.red = led.new_red;
		led.green = led.new_green;
		led.blue = led.new_blue;
		led_set_red(led.red > 0);
		led_set_green(led.green > 0);
		led_set_blue(led.blue > 0);
	} else {
		if (led.step == led.red) led_set_red(FALSE);
		if (led.step == led.green) led_set_green(FALSE);
		if (led.step == led.blue) led_set_blue(FALSE);
	}
	
	led.timer.expires += led.delay;
	add_timer(&led.timer);

#ifdef DEBUG_STEP
	printk("\n");
#endif

	END;
}

module_init(led_init);
module_exit(led_exit);

