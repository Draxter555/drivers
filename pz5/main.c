#include <linux/module.h>
#include <linux/pci.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

#define DRV_NAME "lab5_pci_net"

struct lab5_priv {
    void *hw_addr;  // заглушка, не используем
};

static int my_open(struct net_device *dev) {
    printk(KERN_EMERG "LAB5: netdev open\n");
    netif_start_queue(dev);
    return 0;
}

static int my_stop(struct net_device *dev) {
    printk(KERN_EMERG "LAB5: netdev stop\n");
    netif_stop_queue(dev);
    return 0;
}

static netdev_tx_t my_xmit(struct sk_buff *skb, struct net_device *dev) {
    dev_kfree_skb(skb);
    return NETDEV_TX_OK;
}

static const struct net_device_ops my_ops = {
    .ndo_open = my_open,
    .ndo_stop = my_stop,
    .ndo_start_xmit = my_xmit,
};

static int my_probe(struct pci_dev *pdev, const struct pci_device_id *id) {
    struct net_device *ndev;
    struct lab5_priv *priv;

    printk(KERN_EMERG "LAB5: probe entered\n");

    ndev = alloc_etherdev(sizeof(struct lab5_priv));
    priv = netdev_priv(ndev);
    priv->hw_addr = NULL;

    ndev->netdev_ops = &my_ops;
    SET_NETDEV_DEV(ndev, &pdev->dev);

    eth_random_addr(ndev->dev_addr);
    printk(KERN_EMERG "LAB5: MAC %pM\n", ndev->dev_addr);

    pci_set_drvdata(pdev, ndev);
    register_netdev(ndev);

    printk(KERN_EMERG "LAB5: probe success\n");
    return 0;
}

static void my_remove(struct pci_dev *pdev) {
    struct net_device *ndev = pci_get_drvdata(pdev);
    printk(KERN_EMERG "LAB5: remove entered\n");
    if (!ndev) return;
    unregister_netdev(ndev);
    free_netdev(ndev);
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
MODULE_DESCRIPTION("Lab5 PCI network driver (simplified)");
