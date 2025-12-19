#include <linux/module.h>
#include <linux/pci.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>

#define DRV_NAME "lab5_pci_net"
#define DEV_NAME "rawdemo"

static char demo_data[24] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x12,0x34,0x68,0x65,
    0x6c,0x6c,0x6f,0x20,0x77,0x6f,0x72,0x6c
};

/* ---------- char device ---------- */
static int my_char_open(struct inode *inode, struct file *file) {
    printk(KERN_EMERG "LAB5: char device opened\n");
    return 0;
}

static int my_char_release(struct inode *inode, struct file *file) {
    printk(KERN_EMERG "LAB5: char device closed\n");
    return 0;
}

static ssize_t my_char_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
    if(*ppos >= sizeof(demo_data)) return 0;
    if(count > sizeof(demo_data) - *ppos)
        count = sizeof(demo_data) - *ppos;
    if(copy_to_user(buf, demo_data + *ppos, count))
        return -EFAULT;
    *ppos += count;
    return count;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_char_open,
    .release = my_char_release,
    .read = my_char_read,
};

/* ---------- PCI driver ---------- */
struct lab5_priv { void *hw_addr; };
static int major;
static struct class *my_class;

static int my_probe(struct pci_dev *pdev, const struct pci_device_id *id) {
    printk(KERN_EMERG "LAB5: probe entered\n");

    eth_random_addr(demo_data + 12);
    printk(KERN_EMERG "LAB5: MAC %pM\n", demo_data + 12);
    printk(KERN_EMERG "LAB5: probe success\n");

    /* регистрируем char device */
    major = register_chrdev(0, DEV_NAME, &fops);
    my_class = class_create(THIS_MODULE, DEV_NAME);
    device_create(my_class, NULL, MKDEV(major,0), NULL, DEV_NAME);

    return 0;
}

static void my_remove(struct pci_dev *pdev) {
    device_destroy(my_class, MKDEV(major,0));
    class_destroy(my_class);
    unregister_chrdev(major, DEV_NAME);
    printk(KERN_EMERG "LAB5: remove entered\n");
}

static const struct pci_device_id my_ids[] = {
    { PCI_ANY_ID, PCI_ANY_ID },
    { 0, }
};
MODULE_DEVICE_TABLE(pci, my_ids);

static struct pci_driver my_driver = {
    .name = DRV_NAME,
    .id_table = my_ids,
    .probe = my_probe,
    .remove = my_remove,
};

module_pci_driver(my_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("student");
MODULE_DESCRIPTION("Lab5 PCI + char demo driver with /dev/rawdemo");
