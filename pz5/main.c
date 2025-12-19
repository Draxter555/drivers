#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

#define DRV_NAME "lab5_net"

static int lab5_open(struct net_device *dev)
{
    pr_info("lab5: device opened\n");
    netif_start_queue(dev);
    return 0;
}

static int lab5_stop(struct net_device *dev)
{
    pr_info("lab5: device stopped\n");
    netif_stop_queue(dev);
    return 0;
}

static netdev_tx_t lab5_xmit(struct sk_buff *skb, struct net_device *dev)
{
    pr_info("lab5: packet received, len=%u\n", skb->len);
    dev_kfree_skb(skb);   // имитация отправки
    return NETDEV_TX_OK;
}

static const struct net_device_ops lab5_ops = {
    .ndo_open       = lab5_open,
    .ndo_stop       = lab5_stop,
    .ndo_start_xmit = lab5_xmit,
};

static void lab5_setup(struct net_device *dev)
{
    ether_setup(dev);
    dev->netdev_ops = &lab5_ops;
    eth_random_addr(dev->dev_addr);
}

static struct net_device *lab5_dev;

static int __init lab5_init(void)
{
    pr_info("lab5: init\n");

    lab5_dev = alloc_netdev(0, "lab5%d", NET_NAME_UNKNOWN, lab5_setup);
    if (!lab5_dev)
        return -ENOMEM;

    return register_netdev(lab5_dev);
}

static void __exit lab5_exit(void)
{
    pr_info("lab5: exit\n");
    unregister_netdev(lab5_dev);
    free_netdev(lab5_dev);
}

module_init(lab5_init);
module_exit(lab5_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("student");
MODULE_DESCRIPTION("Lab5 virtual network driver");
