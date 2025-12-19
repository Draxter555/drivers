#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

#define DRV_NAME "lab6_net"

static int lab6_open(struct net_device *dev)
{
    pr_info("lab6: device opened\n");
    netif_start_queue(dev);
    return 0;
}

static int lab6_stop(struct net_device *dev)
{
    pr_info("lab6: device stopped\n");
    netif_stop_queue(dev);
    return 0;
}

static netdev_tx_t lab6_xmit(struct sk_buff *skb, struct net_device *dev)
{
    pr_info("lab6: packet received, len=%u\n", skb->len);
    dev_kfree_skb(skb);   // имитация отправки
    return NETDEV_TX_OK;
}

static const struct net_device_ops lab6_ops = {
    .ndo_open       = lab6_open,
    .ndo_stop       = lab6_stop,
    .ndo_start_xmit = lab6_xmit,
};

static void lab6_setup(struct net_device *dev)
{
    ether_setup(dev);
    dev->netdev_ops = &lab6_ops;
    eth_random_addr(dev->dev_addr);
}

static struct net_device *lab6_dev;

static int __init lab6_init(void)
{
    pr_info("lab6: init\n");

    lab6_dev = alloc_netdev(0, "lab6%d", NET_NAME_UNKNOWN, lab6_setup);
    if (!lab6_dev)
        return -ENOMEM;

    return register_netdev(lab6_dev);
}

static void __exit lab6_exit(void)
{
    pr_info("lab6: exit\n");
    unregister_netdev(lab6_dev);
    free_netdev(lab6_dev);
}

module_init(lab6_init);
module_exit(lab6_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("student");
MODULE_DESCRIPTION("Lab6 virtual network driver");
