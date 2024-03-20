#include <linux/fs.h>					// struct file, struct file_operations
#include <linux/module.h>				// for module init and exit macros
#include <linux/miscdevice.h>			// for misc_device_register and struct miscdev
#include <asm/io.h>
#define WRREG_BASE 0x50
#define WRREG_SPAN 16
/* Kernel functions device driver /dev/wrReg. */
static int device_open(struct inode *, struct file *);
static int device_release (struct inode *, struct file *);
static void device_write(int data);

void * LW_virtual;			// Lightweight bridge base address
volatile int *wrReg_ptr;	// Virtual address	
static int wrReg_registered = 0;
int err;

static struct file_operations wrReg_fops = {
	.owner = THIS_MODULE,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

#define SUCCESS 0
#define DEV_NAME "wrReg"

static struct miscdevice wrReg = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEV_NAME,
	.fops = &wrReg_fops,
	.mode = 0666
};

/* This function is executed when the device driver is inserted into the Linux kernel */
static int __init start_driver(void){
	// generate a virtual address for the FPGA lightweight bridge
	LW_virtual = ioremap_nocache(WRREG_BASE, WRREG_SPAN);
	wrReg_ptr  = LW_virtual; //+ wrReg_BASE;	// virtual address for wrReg Bus

	err = misc_register (&wrReg);
	if (err < 0) { // Checks whether the device driver will have been successfully created by the Linux Kernel.
		printk (KERN_ERR "/dev/%s: misc_register() failed\n", DEV_NAME);
	}
	else {
		printk (KERN_INFO "/dev/%s driver registered\n", DEV_NAME);
		wrReg_registered = 1;
	}
	return err;
}

/* This function is executed when this driver is removed from the kernel */
static void __exit stop_driver(void){
	if(wrReg_registered){
		//iounmap (LW_virtual); // erase the memory mapping
		misc_deregister(&wrReg);
		printk (KERN_INFO "/dev/%s driver de-registered\n", DEV_NAME);
	}
}

/* Called when a process opens wrReg */
static int device_open(struct inode *inode, struct file *file) {
	return SUCCESS;
}

/* Called when a process closes wrReg */
static int device_release(struct inode *inode, struct file *file) {
	return 0;
}

/* Sends data to wrReg bus */
static void device_write(int data){
	*wrReg_ptr = data;
}

MODULE_LICENSE("GPL");
module_init(start_driver);
module_exit(stop_driver);