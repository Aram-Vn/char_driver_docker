#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "simple_char"
#define BUF_LEN 80

static int major;
static char msg[BUF_LEN];
static int open_count = 0;

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

static int __init simple_char_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        pr_alert("Registering char device failed with %d\n", major);
        return major;
    }

    pr_info("Simple char driver loaded: major=%d\n", major);
    return 0;
}

static void __exit simple_char_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    pr_info("Simple char driver unloaded\n");
}

static int device_open(struct inode *inode, struct file *file)
{
    if (open_count)
        return -EBUSY;
    open_count++;
    try_module_get(THIS_MODULE);
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    open_count--;
    module_put(THIS_MODULE);
    return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
    return simple_read_from_buffer(buffer, length, offset, msg, strlen(msg));
}

static ssize_t device_write(struct file *filp, const char *buffer, size_t length, loff_t *offset)
{
    memset(msg, 0, BUF_LEN);
    if (length > BUF_LEN - 1)
        length = BUF_LEN - 1;
    if (copy_from_user(msg, buffer, length))
        return -EFAULT;
    msg[length] = '\0';
    return length;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aram_vn");
MODULE_DESCRIPTION("A Simple Char Driver");
module_init(simple_char_init);
module_exit(simple_char_exit);

