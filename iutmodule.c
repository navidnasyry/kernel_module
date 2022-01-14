#include <linux/init.h> // For module init and exit
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h> // For fops
#include <linux/uaccess.h>
//#include <string.h> // Can't use it!

#define DEVICE_NAME "ARNA"
MODULE_LICENSE("GPL");

// FILE OPERATIONS
static int arna_open(struct inode*, struct file*);
static int arna_release(struct inode*, struct file*);
static ssize_t arna_read(struct file*, char*, size_t, loff_t*);
static ssize_t arna_write(struct file*, const char*,unsigned long,  loff_t*);

static struct file_operations fops = {
   .open = arna_open,
   .read = arna_read,
   .write = arna_write,
   .release = arna_release,
};

// Why "static"? --> To bound it to the current file.
static int major; // Device major number. Driver reacts to this major number.

// Event --> LOAD
static int __init arna_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops); // 0: dynamically assign a major number ||| name is displayed in /proc/devices ||| fops.
    if (major < 0) {
        printk(KERN_ALERT "arna_device load failed.\n");
        return major;
    }
    printk(KERN_INFO "arna_device module loaded: %d\n", major);
    return 0;
}

// Event --> UNLOAD
static void __exit arna_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "arna_device module unloaded.\n");
}

// Event --> OPEN
static int arna_open(struct inode *inodep, struct file *filep) {
   printk(KERN_INFO "arna_device opened.\n");
   return 0;
}

// Event --> CLOSE
static int arna_release(struct inode *inodep, struct file *filep) {
   printk(KERN_INFO "arna_device closed.\n");
   return 0;
}

// Event --> READ
static ssize_t arna_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    char *message = "ARNA OS project 2!";
    printk(KERN_INFO "ARNA:: IN READ FUNCTION\n");

    int errors = 0;
    errors = copy_to_user(buffer, message, strlen(message));
    return errors == 0 ? strlen(message) : -EFAULT;
}


/* Called when a process tries to write to our device */
static ssize_t arna_write(struct file *flip, const char *buffer,unsigned long len,  loff_t *offset) {
 /* This is a read-only device */
    printk(KERN_INFO "ARNA:: IN WRITE FUNCTION\n");
    char* buff;
    if ( copy_from_user(buff, buffer, strlen(buffer)) ) 
        return -EFAULT;

    return len;
}



// Registering load and unload functions.
module_init(arna_init);
module_exit(arna_exit);
