#include <linux/init.h> // For module init and exit
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h> // For fops
#include <linux/uaccess.h>
#include <linux/string.h>  // For strings
#include <linux/slab.h>    // For kmalloc
#include <linux/kthread.h> // For kthreads

#define DEVICE_NAME "ARNA"
MODULE_LICENSE("GPL");
spinlock_t lock;


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

void command(char *cmd)
{
    

    if (strcmp(cmd, "r") == 0)
    {
        initiation();
        return;
    }

    int sub[4];
    int i = 0;
    // sub[0] => command
    // sub[1] => first input
    //...
    char *token, *string ;// cmd = 0,1,2,3
    //string = strdup(cmd);

    while( (token = strsep(&cmd,",")) != NULL) {
        kstrtoint(token, 10, &sub[i]);

        if (strchr(token, '-') != NULL)
            sub[i] = -1;
        if(i == 0)
        {
            if (strcmp(token, "e") == 0) sub[i] = -2;
            
            else if (strcmp(token, "v") == 0) sub[i] = -3;
            
            else if (strcmp(token, "b") == 0) sub[i] = -4;
            
            else {
                printk(KERN_INFO "ARNA:: COMMAND ERROR :: Where you learn to type :))\n");
                return;
            }
        }
        i++;
    }

    if(sub[0] == -2)
    {
        if (acs[sub[1]].money < sub[3])
        {
            printk(KERN_INFO "ARNA:: ENTEGHAL ERROR :: You have no enough money :(\n");
            return;
        }
        acs[sub[1]].money -= sub[3];
        acs[sub[2]].money += sub[3];
    }
    else if (sub[0] == -3 && sub[1] == -1)//v
    {
        acs[sub[2]].money += sub[3];
    }
    else if (sub[0] == -4 && sub[2] == -1)//b
    {
        if (acs[sub[1]].money < sub[3])
        {
            printk(KERN_INFO "ARNA:: BARDASHT ERROR :: You have no enough money :(\n");
            return;
        }
        acs[sub[1]].money -= sub[3];
    }
    else
    {
        printk(KERN_INFO "ARNA:: COMMAND ERROR :: Your command format was incorrect :(\n");
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
        printk(KERN_ALERT "ARNA:: arna_device load failed.\n");
        return major;
    }
    printk(KERN_INFO "ARNA:: arna_device module loaded: %d\n", major);
    return 0;

    // hame hesaba 2000 beshan => initial account and lock
}

// Event --> UNLOAD
static void __exit arna_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "ARNA:: arna_device module unloaded.\n");
}

// Event --> OPEN
static int arna_open(struct inode *inodep, struct file *filep) {
   printk(KERN_INFO "ARNA:: arna_device opened.\n");
   return 0;
}

// Event --> CLOSE
static int arna_release(struct inode *inodep, struct file *filep) {
   printk(KERN_INFO "ARNA:: arna_device closed.\n");
   return 0;
}


static ssize_t arna_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
    char message[1024] = ""; // TODO : Get all balances

    int i;
    char bfr[10];
    //memset(message, 0, 1024);
    for (i = 0; i < NUM_OF_ACCOUNTS; i++)
    {
        sprintf(bfr, "%lld,", acs[i].money);
        strcat(message, bfr);

    }

    int errors = 0;
    errors = copy_to_user(buffer, message, strlen(message));
    return errors == 0 ? strlen(message) : -EFAULT;
}



/* Called when a process tries to write to our device */
static ssize_t arna_write(struct file *flip, const char *buffer,unsigned long len,  loff_t *offset) {
 /* This is a read-only device */
    printk(KERN_INFO "ARNA:: IN WRITE FUNCTION\n");
    char* cmd;


    // if (copy_from_user(cmd, buffer, strlen(buffer)) ) 
    //     return -EFAULT;
    int bl = copy_from_user(cmd, buffer, strlen(buffer));
    printk(KERN_INFO "ARNA:: IN WRITE FUNCTION {%d}\n", bl);


    //need lock

    //command(cmd);
    printk(KERN_INFO "ARNA:: IN WRITE FUNCTION 2\n");

    kmalloc(sizeof(cmd), GFP_KERNEL);
    printk(KERN_INFO "ARNA:: IN WRITE FUNCTION 3\n");

    spin_lock(&lock);
    if (!spin_is_locked(&lock)) 
    { 
        printk(KERN_INFO "ARNA :: UnLocked\n"); 
    } 
    command(cmd);
    spin_unlock(&lock);

    //need unlock

    //  size_t maxdatalen = 100, ncopied;
    // char command[maxdatalen];
    // if(count < maxdatalen)
    // {
    //     maxdatalen = count;
    // }
    // ncopied = copy_from_user(command, buf, maxdatalen);
    // command[maxdatalen + 1] = '0';
    // command[maxdatalen] = ',';
    // if(ncopied != 0)
    // {
    //     // return -EFAULT;
    // }
    // pointer_command = command;
    // kmalloc(sizeof(command), GFP_KERNEL);
    // spin_lock(&lock);
    // if (!spin_is_locked(&lock)) 
    // { 
    //     printk(KERN_INFO "UnLocked\n"); 
    // } 
    // commandChecker(command);
    // spin_unlock(&lock);
    // return count;



    return len;
}



// Registering load and unload functions.
module_init(arna_init);
module_exit(arna_exit);