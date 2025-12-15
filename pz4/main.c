#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h> // для copy_to_user / copy_from_user
#include <linux/cdev.h>
#include <linux/ioctl.h>
#include <linux/mutex.h>

#define DEVICE_NAME "simple_dev"
#define BUF_SIZE 100

#define IOCTL_RESET _IO('q', 1)
#define IOCTL_IS_EMPTY _IOR('q', 2, int)

static char buffer[BUF_SIZE];
static int buf_len = 0;
static struct cdev c_dev;
static dev_t dev;
static struct class *cls;
static DEFINE_MUTEX(buf_mutex);

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

static ssize_t simple_read(struct file *file, char __user *user_buf, size_t len, loff_t *offset)
{
    ssize_t ret = 0;
    if (mutex_lock_interruptible(&buf_mutex))
        return -ERESTARTSYS;
    if (*offset >= buf_len)
        goto out;
    if (len > buf_len - *offset)
        len = buf_len - *offset;
    if (copy_to_user(user_buf, buffer + *offset, len))
        ret = -EFAULT;
    else {
        *offset += len;
        ret = len;
    }
out:
    mutex_unlock(&buf_mutex);
    return ret;
}

static ssize_t simple_write(struct file *file, const char __user *user_buf, size_t len, loff_t *offset)
{
    ssize_t ret = 0;
    if (len > BUF_SIZE)
        len = BUF_SIZE;
    if (mutex_lock_interruptible(&buf_mutex))
        return -ERESTARTSYS;
    if (copy_from_user(buffer, user_buf, len))
        ret = -EFAULT;
    else {
        buf_len = len;
        ret = len;
    }
    mutex_unlock(&buf_mutex);
    return ret;
}

static long simple_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int tmp;
    if (mutex_lock_interruptible(&buf_mutex))
        return -ERESTARTSYS;
    switch(cmd) {
        case IOCTL_RESET:
            buf_len = 0;
            printk("Buffer reset\n");
            break;
        case IOCTL_IS_EMPTY:
            tmp = (buf_len == 0);
            if (copy_to_user((int __user *)arg, &tmp, sizeof(int)))
                return -EFAULT;
            break;
        default:
            mutex_unlock(&buf_mutex);
            return -EINVAL;
    }
    mutex_unlock(&buf_mutex);
    return 0;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = simple_open,
    .release = simple_release,
    .read = simple_read,
    .write = simple_write,
    .unlocked_ioctl = simple_ioctl,
};

int init_module(void)
{
    if (alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME) < 0)
        return -1;
    cdev_init(&c_dev, &fops);
    if (cdev_add(&c_dev, dev, 1) < 0)
        return -1;
    cls = class_create(THIS_MODULE, "simple_class");
    device_create(cls, NULL, dev, NULL, DEVICE_NAME);
    mutex_init(&buf_mutex);
    printk("Driver loaded\n");
    return 0;
}

void cleanup_module(void)
{
    device_destroy(cls, dev);
    class_destroy(cls);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, 1);
    printk("Driver removed\n");
}

MODULE_LICENSE("GPL");
