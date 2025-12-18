#include <linux/module.h>
#include <linux/pci.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/io.h>

#define DRV_NAME "lab5_pci_net"

/* ЗАГЛУШКА ID (подставишь свои из lspci -nn) */
#define VENDOR_ID 0x8086
#define DEVICE_ID 0x100e

#define REG_RAL0 0x5400
#define REG_RAH0 0x5404

struct lab5_priv {
    void __iomem *hw_addr;
};

static int lab5_open(struct net_device *dev)
{
    netif_start_queue(dev);
    pr_info("lab5: netdev opened\n");
    return 0;
}

static int lab5_stop(struct net_device *dev)
{
    netif_stop_queue(dev);
    pr_info("lab5: netdev stopped\n");
    return 0;
}

static netdev_tx_t lab5_xmit(struct sk_buff *skb, struct net_device *dev)
{
    dev_kfree_skb(skb);
    return NETDEV_TX_OK;
}

static const struct net_device_ops lab5_netdev_ops = {
    .ndo_open = lab5_open,
    .ndo_stop = lab5_stop,
    .ndo_start_xmit = lab5_xmit,
};

static void lab5_read_mac(void __iomem *hw, unsigned char *mac)
{
    u32 ral = readl(hw + REG_RAL0);
    u32 rah = readl(hw + REG_RAH0);

    mac[0] = ral & 0xff;
    mac[1] = (ral >> 8) & 0xff;
    mac[2] = (ral >> 16) & 0xff;
    mac[3] = (ral >> 24) & 0xff;
    mac[4] = rah & 0xff;
    mac[5] = (rah >> 8) & 0xff;
}

static int lab5_probe(struct pci_dev *pdev, const struct pci_device_id *id)
{
    struct net_device *ndev;
    struct lab5_priv *priv;
    int err;

    pr_info("lab5: probe called\n");

    err = pci_enable_device(pdev);
    if (err)
        return err;

    err = pci_request_regions(pdev, DRV_NAME);
    if (err)
        goto err_disable;

    ndev = alloc_etherdev(sizeof(struct lab5_priv));
    if (!ndev) {
        err = -ENOMEM;
        goto err_regions;
    }

    priv = netdev_priv(ndev);

    priv->hw_addr = pci_iomap(pdev, 0, 0);
    if (!priv->hw_addr) {
        err = -EIO;
        goto err_free;
    }

    ndev->netdev_ops = &lab5_netdev_ops;
    SET_NETDEV_DEV(ndev, &pdev->dev);

    lab5_read_mac(priv->hw_addr, ndev->dev_addr);
    pr_info("lab5: MAC %pM\n", ndev->dev_addr);

    pci_set_drvdata(pdev, ndev);

    err = register_netdev(ndev);
    if (err)
        goto err_iounmap;

    return 0;

err_iounmap:
    pci_iounmap(pdev, priv->hw_addr);
err_free:
    free_netdev(ndev);
err_regions:
    pci_release_regions(pdev);
err_disable:
    pci_disable_device(pdev);
    return err;
}

static void lab5_remove(struct pci_dev *pdev)
{
    struct net_device *ndev = pci_get_drvdata(pdev);
    struct lab5_priv *priv = netdev_priv(ndev);

    pr_info("lab5: remove called\n");

    unregister_netdev(ndev);
    pci_iounmap(pdev, priv->hw_addr);
    free_netdev(ndev);
    pci_release_regions(pdev);
    pci_disable_device(pdev);
}

static const struct pci_device_id lab5_ids[] = {
    { PCI_ANY_ID, PCI_ANY_ID },
    { 0, }
};


MODULE_DEVICE_TABLE(pci, lab5_ids);

static struct pci_driver lab5_driver = {
    .name = DRV_NAME,
    .id_table = lab5_ids,
    .probe = lab5_probe,
    .remove = lab5_remove,
};

module_pci_driver(lab5_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("student");
MODULE_DESCRIPTION("Lab5 PCI network driver");

