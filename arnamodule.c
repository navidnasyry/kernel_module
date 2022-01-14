#include <linux/init.h> // For module init and exit
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h> // For fops
#include <linux/uaccess.h>
#include <linux/string.h>


#define DEVICE_NAME "ARNA"
MODULE_LICENSE("GPL");

#define NUM_OF_ACCOUNTS 100

struct accounts
{
    int id;
    unsigned long long int money;    
}acs[NUM_OF_ACCOUNTS];

void initiation(void)
{
    int i = 0;
    for (i = 0; i < NUM_OF_ACCOUNTS; i++)
    {
        acs[i].id = i;
        acs[i].money = 2000000;
    }
    //return;
}
char *get_moneys(int i)
{
    char *tmp;
    sprintf(tmp, "%lld,", acs[i].money);
    return tmp;
}       

void command(const char *cmd)
{
    if (strchr(cmd, 'r') != NULL)
    {
        initiation();
    }
    else if(strchr(cmd, 'e') != NULL)
    {
        /* code */
    }
    else if (strchr(cmd, 'v') != NULL)
    {
        /* code */
    }
    else if (strchr(cmd, 'b') != NULL)
    {
        /* code */
    }
    else
    {
        /* code */
    }
    return;
}

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

    initiation();
    major = register_chrdev(0, DEVICE_NAME, &fops); // 0: dynamically assign a major number ||| name is displayed in /proc/devices ||| fops.
    if (major < 0) {
        printk(KERN_ALERT "arna_device load failed.\n");
        return major;
    }
    printk(KERN_INFO "arna_device module loaded: %d\n", major);
    return 0;

    // hame hesaba 2000 beshan => initial account and lock
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
    
    char message[1024] = "";
    
    //etelaat hesaba
    int i = 0;
    for (i = 0; i < NUM_OF_ACCOUNTS; i++)
    {
        strcat(message,get_moneys(i));
        //if (i != NUM_OF_ACCOUNTS - 1)
        strcat(message,",");
        printk(KERN_INFO "ARNA:: IN READ LOOP FUNCTION\n");
    }
    //char *message = "ARNA OS project 2!";
    //printk(KERN_INFO "ARNA:: IN READ FUNCTION\n");

    int errors = 0;
    errors = copy_to_user(buffer, message, strlen(message));
    return errors == 0 ? strlen(message) : -EFAULT;
}


/* Called when a process tries to write to our device */
static ssize_t arna_write(struct file *flip, const char *buffer,unsigned long len,  loff_t *offset) {
 /* This is a read-only device */
    printk(KERN_INFO "ARNA:: IN WRITE FUNCTION\n");
    char* buff;

    if (copy_from_user(buff, buffer, strlen(buffer)) ) 
        return -EFAULT;


    //need lock

    command(buff);

    //need unlock



    return len;
}



// Registering load and unload functions.
module_init(arna_init);
module_exit(arna_exit);