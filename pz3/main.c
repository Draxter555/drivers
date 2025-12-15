#include <linux/module.h>
#include <linux/fs.h>

int major;

static int simple_open(struct inode *inode, struct file *file)
{
    printk("Device opened\n");
    return 0;
}

static int simple_release(struct inode *inode, struct file *file)
{
    printk("Device closed\n");
    return 0;
}

struct file_operations fops = {
    .open = simple_open,
    .release = simple_release,
};

int init_module(void)
{
    major = register_chrdev(0, "simple_char", &fops);
    printk("Driver loaded, major=%d\n", major);
    return 0;
}

void cleanup_module(void)
{
    unregister_chrdev(major, "simple_char");
    printk("Driver removed\n");
}

MODULE_LICENSE("GPL");
