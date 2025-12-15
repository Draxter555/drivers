#include <linux/module.h>
#include <linux/fs.h>

int major;

int simple_open(struct inode *inode, struct file *file)
{
    printk("Device opened\n");
    return 0;
}

int simple_release(struct inode *inode, struct file *file)
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
    major = register_chrdev(0, "main", &fops);
    printk("Driver loaded, major=%d\n", major);
    return 0;
}

void cleanup_module(void)
{
    unregister_chrdev(major, "main");
    printk("Driver removed\n");
}

MODULE_LICENSE("GPL");
