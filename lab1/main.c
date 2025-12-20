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

// Данные устройства
static int buffer_value = 0;
static s64 write_timestamp_ns = 0;  // время окончания write

// Гистограмма: 500 бинов по 50 мкс → покрывает до 25 000 мкс
static size_t histogram[HISTO_MAX] = {0};
static size_t histo_max_bin = 0;    // длина = макс. использованный бин + 1

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

    if (copy_from_user(&buffer_value, buf, sizeof(int)))
        return -EFAULT;

    // Сохраняем точное время завершения write
    write_timestamp_ns = ktime_get_ns();

    return sizeof(int);
}

static ssize_t dev_read(struct file *file, char __user *buf, size_t count, loff_t *off)
{
    s64 read_time_ns;
    s64 delta_ns;
    long delta_us;
    size_t bin;

    if (count != sizeof(int))
        return -EINVAL;

    read_time_ns = ktime_get_ns();
    delta_ns = read_time_ns - write_timestamp_ns;
    delta_us = delta_ns / 1000;
    bin = (delta_us >= 0) ? (size_t)(delta_us / 50) : 0;
    if (bin >= HISTO_MAX)
        bin = HISTO_MAX - 1;

    histogram[bin]++;
    if (bin >= histo_max_bin)
        histo_max_bin = bin + 1;

    if (copy_to_user(buf, &buffer_value, sizeof(int)))
        return -EFAULT;

    return sizeof(int);
}



static long dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch (cmd) {
    case IOCTL_HISTO_LEN:
        if (copy_to_user((size_t __user *)arg, &histo_max_bin, sizeof(histo_max_bin)))
            return -EFAULT;
        break;

    case IOCTL_HISTO_BUF:
        if (copy_to_user((size_t __user *)arg, histogram, sizeof(histogram)))
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
MODULE_AUTHOR("student");
MODULE_DESCRIPTION("Lab 1: histogram of write-read latency (50 us per bin)");
