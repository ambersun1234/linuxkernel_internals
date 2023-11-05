#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "echo"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ambersun1234");
MODULE_DESCRIPTION("Echo from kernel driver");
MODULE_VERSION("1.0");

static dev_t echo_dev = 0;
static struct cdev *echo_cdev;
static struct class *echo_class;

static size_t size_of_message = 0;
static size_t numberOpens = 0;
static char message[256] = {0};

static int echo_open(struct inode *inodep, struct file *filep) {
	numberOpens++;
	printk(KERN_INFO "Echo: Device has been opened %ld times\n", numberOpens);
	return 0;
}

static ssize_t echo_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
	int error_count = 0;

	error_count = copy_to_user(buffer, message, size_of_message);
	if (error_count == 0) {
		printk(KERN_INFO "Echo: Sent %ld characters to the user space\n", size_of_message);
		return size_of_message = 0;
	}
	else {
		printk(KERN_INFO "Echo: Failed to send %ld characters to the user space\n", size_of_message);
		return -EFAULT;
	}
}

static ssize_t echo_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
	// sprintf(message, "%s(%zu letters)", buffer, len); // sprintf available in linux/kernel.h
	copy_from_user(message, buffer, 256);
	size_of_message = strlen(message); // strlen available in linux/kernel.h
	printk(KERN_INFO "Echo: Received %zu characters from user space\n", len);
	return len;
}

static int echo_release(struct inode *inodep, struct file *filep) {
	printk(KERN_INFO "Echo: Device successfully closed\n");
	return 0;
}

static struct file_operations fops = {
	.open    = echo_open,
	.read    = echo_read,
	.write   = echo_write,
	.release = echo_release,
};

static int __init echo_init(void) {
	int rc = alloc_chrdev_region(&echo_dev, 0, 1, DEVICE_NAME);
	if (rc < 0) {
		printk(KERN_ALERT "Echo: Failed to register echo Character Device Driver, rc = %i\n", rc);
		return rc;
	}

	echo_cdev = cdev_alloc();
	if (echo_cdev == NULL) {
		printk(KERN_ALERT "Echo: Failed to alloc cdev\n");
		rc = -1;
		goto failed_cdev;
	}

	cdev_init(echo_cdev, &fops);
	rc = cdev_add(echo_cdev, echo_dev, 1);
	if (rc < 0) {
		printk(KERN_ALERT "Echo: Failed to add cdev\n");
		rc = -2;
		goto failed_cdev;
	}

	echo_class = class_create(THIS_MODULE, DEVICE_NAME);
	if (!echo_class) {
		printk(KERN_ALERT "Echo: Failed to create device class\n");
		rc = -3;
		goto failed_class;
	}

	if (!device_create(echo_class, NULL, echo_dev, NULL, DEVICE_NAME)) {
		printk(KERN_ALERT "Echo: Failed to create device");
		rc = -4;
		goto failed_device;
	}
	return rc;

	failed_class:
		class_destroy(echo_class);
	failed_device:
		cdev_del(echo_cdev);
	failed_cdev:
		unregister_chrdev_region(echo_dev, 1);
	return rc;
}

static void __exit echo_exit(void) {
	device_destroy(echo_class, echo_dev);
	class_destroy(echo_class);
	cdev_del(echo_cdev);
	printk(KERN_INFO "Echo: Goodbye from LKM\n");
}

module_init(echo_init);
module_exit(echo_exit);
