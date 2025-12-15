#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>
#include <linux/mutex.h>
#include <linux/slab.h> // kmalloc/kfree

#define DEVICE_NAME "pz4_dev"
#define BUF_SIZE 256

#define IOCTL_CLEAR _IO('q', 1)
#define IOCTL_HASDATA _IOR('q', 2, int)

static char *buffer;
static int buf_len = 0;
static struct cdev c_dev;
static dev_t dev;
static struct class *cls;
static DEFINE_MUTEX(buf_mutex);

static int pz4_open(struct inode *inode, struct file *file) { return 0; }
static int pz4_release(struct inode *inode, struct file *file) { return 0; }

static ssize_t pz4_read(struct file *file, char __user *user_buf, size_t len, loff_t *offset)
{
    ssize_t ret = 0;
    if (mutex_lock_interruptible(&buf_mutex)) return -ERESTARTSYS;
    if (*offset >= buf_len) goto out;
    if (len > buf_len - *offset) len = buf_len - *offset;
    if (copy_to_user(user_buf, buffer + *offset, len)) ret = -EFAULT;
    else { *offset += len; ret = len; }
out:
    mutex_unlock(&buf_mutex);
    return ret;
}

static ssize_t pz4_write(struct file *file, const char __user *user_buf, size_t len, loff_t *offset)
{
    ssize_t ret = 0;
    if (len > BUF_SIZE) len = BUF_SIZE;
    if (mutex_lock_interruptible(&buf_mutex)) return -ERESTARTSYS;
    if (copy_from_user(buffer, user_buf, len)) ret = -EFAULT;
    else { buf_len = len; ret = len; }
    mutex_unlock(&buf_mutex);
    return ret;
}

static long pz4_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int tmp;
    if (mutex_lock_interruptible(&buf_mutex)) return -ERESTARTSYS;
    switch(cmd) {
        case IOCTL_CLEAR:
            buf_len = 0;
            break;
        case IOCTL_HASDATA:
            tmp = (buf_len != 0);
            if (copy_to_user((int __user *)arg, &tmp, sizeof(int))) {
                mutex_unlock(&buf_mutex);
                return -EFAULT;
            }
            break;
        default:
            mutex_unlock(&buf_mutex);
            return -EINVAL;
    }
    mutex_unlock(&buf_mutex);
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = pz4_open,
    .release = pz4_release,
    .read = pz4_read,
    .write = pz4_write,
    .unlocked_ioctl = pz4_ioctl,
};

static int __init pz4_init(void)
{
    buffer = kmalloc(BUF_SIZE, GFP_KERNEL);
    if (!buffer) return -ENOMEM;

    if (alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME) < 0) {
        kfree(buffer);
        return -1;
    }

    cdev_init(&c_dev, &fops);
    if (cdev_add(&c_dev, dev, 1) < 0) {
        unregister_chrdev_region(dev, 1);
        kfree(buffer);
        return -1;
    }

    cls = class_create(THIS_MODULE, "pz4_class");
    if (IS_ERR(cls)) {
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, 1);
        kfree(buffer);
        return PTR_ERR(cls);
    }

    device_create(cls, NULL, dev, NULL, DEVICE_NAME);

    mutex_init(&buf_mutex);
    printk(KERN_INFO "PZ4 driver loaded\n");
    return 0;
}

static void __exit pz4_exit(void)
{
    device_destroy(cls, dev);
    class_destroy(cls);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, 1);
    kfree(buffer);
    printk(KERN_INFO "PZ4 driver removed\n");
}

module_init(pz4_init);
module_exit(pz4_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ваше Имя");
MODULE_DESCRIPTION("PZ4 character device driver");
