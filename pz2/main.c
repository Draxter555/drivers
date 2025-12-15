#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void)
{
    printk("Module loaded\n");
    return 0;
}

void cleanup_module(void)
{
    printk("Module removed\n");
}

MODULE_LICENSE("GPL");
