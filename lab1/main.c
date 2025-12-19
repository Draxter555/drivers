#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#include "ioctl.h"

#define DEV_NAME "my_char_dev"

static int major;
static struct class *my_class;
static struct device *my_device;

// Буфер и гистограмма (просто счётчик read)
static int dev_value = 0;
static int histo_count = 0; // сколько раз читали

static int dev_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "устройство открыто\n");
    return 0;
}

static int dev_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "устройство закрыто\n");
    return 0;
}

static ssize_t dev_read(struct file *file, char __user *buf, size_t count, loff_t *off)
{
    if (count != sizeof(int))
        return -EINVAL;

    if (copy_to_user(buf, &dev_value, sizeof(int)))
        return -EFAULT;

    histo_count++; // обновляем "гистограмму"
    return sizeof(int);
}

static ssize_t dev_write(struct file *file, const char __user *buf, size_t count, loff_t *off)
{
    if (count != sizeof(int))
        return -EINVAL;

    if (copy_from_user(&dev_value, buf, sizeof(int)))
        return -EFAULT;

    return sizeof(int);
}

static long dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    if (cmd == IOCTL_HISTO) {
        if (copy_to_user((int __user *)arg, &histo_count, sizeof(int)))
            return -EFAULT;
        return 0;
    }
    return -EINVAL;
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_release,
    .read = dev_read,
    .write = dev_write,
    .unlocked_ioctl = dev_ioctl,
};

static int __init my_init(void)
{
    // Регистрируем устройство
    major = register_chrdev(0, DEV_NAME, &fops);
    if (major < 0)
        return major;

    // Создаём class и device для /dev
    my_class = class_create(THIS_MODULE, "my_class");
    if (IS_ERR(my_class)) {
        unregister_chrdev(major, DEV_NAME);
        return PTR_ERR(my_class);
    }

    my_device = device_create(my_class, NULL, MKDEV(major, 0), NULL, DEV_NAME);
    if (IS_ERR(my_device)) {
        class_destroy(my_class);
        unregister_chrdev(major, DEV_NAME);
        return PTR_ERR(my_device);
    }

    printk(KERN_INFO "драйвер загружен, /dev/%s\n", DEV_NAME);
    return 0;
}

static void __exit my_exit(void)
{
    device_destroy(my_class, MKDEV(major, 0));
    class_destroy(my_class);
    unregister_chrdev(major, DEV_NAME);
    printk(KERN_INFO "драйвер выгружен\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("student");
MODULE_DESCRIPTION("простой символьный драйвер");
