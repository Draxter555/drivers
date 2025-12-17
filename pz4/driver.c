#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/mutex.h>
#include <linux/wait.h>

#define DEVICE_NAME "pz4_dev"
#define BUF_SIZE 256
#define IOCTL_CLEAR _IO('q',1)
#define IOCTL_HASDATA _IOR('q',2,int)

static char buf[BUF_SIZE];
static int buf_len = 0;

static dev_t dev;
static struct cdev c_dev;
static struct class *cls;
static DEFINE_MUTEX(buf_mutex);
static DECLARE_WAIT_QUEUE_HEAD(read_queue);

static int open_f(struct inode *i, struct file *f) { return 0; }
static int release_f(struct inode *i, struct file *f) { return 0; }

static ssize_t read_f(struct file *f, char __user *u, size_t l, loff_t *off)
{
    printk(KERN_INFO "read() ждёт данные\n");
    if (wait_event_interruptible(read_queue, buf_len>0)) return -ERESTARTSYS;
    mutex_lock(&buf_mutex);
    if (l>buf_len) l=buf_len;
    if (copy_to_user(u, buf, l)) l=-EFAULT;
    else buf_len=0;
    mutex_unlock(&buf_mutex);
    printk(KERN_INFO "данные появились, read() продолжает\n");
    return l;
}

static ssize_t write_f(struct file *f, const char __user *u, size_t l, loff_t *off)
{
    if (l>BUF_SIZE) l=BUF_SIZE;
    mutex_lock(&buf_mutex);
    if (copy_from_user(buf, u, l)) l=-EFAULT;
    else buf_len=l;
    mutex_unlock(&buf_mutex);
    wake_up_interruptible(&read_queue);
    return l;
}

static long ioctl_f(struct file *f, unsigned int cmd, unsigned long arg)
{
    int tmp;
    mutex_lock(&buf_mutex);
    if (cmd==IOCTL_CLEAR) buf_len=0;
    else if (cmd==IOCTL_HASDATA){
        tmp=(buf_len!=0);
        if (copy_to_user((int __user*)arg,&tmp,sizeof(int))) {
            mutex_unlock(&buf_mutex);
            return -EFAULT;
        }
    }
    else { mutex_unlock(&buf_mutex); return -EINVAL; }
    mutex_unlock(&buf_mutex);
    return 0;
}

static struct file_operations fops={
    .owner=THIS_MODULE,
    .open=open_f,
    .release=release_f,
    .read=read_f,
    .write=write_f,
    .unlocked_ioctl=ioctl_f,
};

static int __init init_f(void)
{
    alloc_chrdev_region(&dev,0,1,DEVICE_NAME);
    cdev_init(&c_dev,&fops);
    cdev_add(&c_dev,dev,1);
    cls=class_create(THIS_MODULE,"pz4_class");
    device_create(cls,NULL,dev,NULL,DEVICE_NAME);
    mutex_init(&buf_mutex);
    return 0;
}

static void __exit exit_f(void)
{
    device_destroy(cls,dev);
    class_destroy(cls);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev,1);
}

module_init(init_f);
module_exit(exit_f);
MODULE_LICENSE("GPL");
