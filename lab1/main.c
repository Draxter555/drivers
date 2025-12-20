#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/timekeeping.h>

#include "ioctl.h"

#define DEV_NAME "lab1_dev"

static dev_t dev_num;
static struct cdev cdev;
static struct class *cls;
static struct device *dev;

static int buffer_val = 0;
static s64 write_time_ns = 0;

static size_t histo[HISTO_MAX] = {0};

static int dev_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int dev_release(struct inode *inode, struct file *file)
{
    return 0;
}

static ssize_t dev_write(struct file *file, const char __user *buf, size_t count, loff_t *off)
{
    if (count != sizeof(int))
        return -EINVAL;
    if (copy_from_user(&buffer_val, buf, sizeof(int)))
        return -EFAULT;
    write_time_ns = ktime_get_ns();
    return sizeof(int);
}

static ssize_t dev_read(struct file *file, char __user *buf, size_t count, loff_t *off)
{
    s64 rtime, delta_ns;
    long delta_us;
    size_t bin;

    if (count != sizeof(int))
        return -EINVAL;

    rtime = ktime_get_ns();
    delta_ns = rtime - write_time_ns;
    delta_us = delta_ns / 1000;
    bin = (delta_us >= 0) ? (size_t)(delta_us / 50) : 0;
    if (bin >= HISTO_MAX)
        bin = HISTO_MAX - 1;

    histo[bin]++;

    if (copy_to_user(buf, &buffer_val, sizeof(int)))
        return -EFAULT;

    return sizeof(int);
}

static long dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch (cmd) {
    case IOCTL_HISTO_LEN:
        return -EINVAL; // НЕ НУЖЕН — мы всегда возвращаем 500
    case IOCTL_HISTO_BUF:
        if (copy_to_user((size_t __user *)arg, histo, sizeof(histo)))
            return -EFAULT;
        break;
    default:
        return -EINVAL;
    }
    return 0;
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_release,
    .write = dev_write,
    .read = dev_read,
    .unlocked_ioctl = dev_ioctl,
};

static int __init lab1_init(void)
{
    if (alloc_chrdev_region(&dev_num, 0, 1, DEV_NAME) < 0)
        return -1;

    cdev_init(&cdev, &fops);
    if (cdev_add(&cdev, dev_num, 1) < 0) {
        unregister_chrdev_region(dev_num, 1);
        return -1;
    }

    cls = class_create(THIS_MODULE, DEV_NAME);
    if (IS_ERR(cls)) {
        cdev_del(&cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(cls);
    }

    dev = device_create(cls, NULL, dev_num, NULL, DEV_NAME);
    if (IS_ERR(dev)) {
        class_destroy(cls);
        cdev_del(&cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(dev);
    }

    return 0;
}

static void __exit lab1_exit(void)
{
    device_destroy(cls, dev_num);
    class_destroy(cls);
    cdev_del(&cdev);
    unregister_chrdev_region(dev_num, 1);
}

module_init(lab1_init);
module_exit(lab1_exit);
MODULE_LICENSE("GPL");
