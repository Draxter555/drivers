#include <linux/module.h>
#include <linux/pci.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/io.h>

#define DRV_NAME "lab5_pci_net"

/* приватные данные драйвера */
struct lab5_priv {
    void __iomem *hw_addr;
};

/* ================= net_device ops ================= */

static int lab5_open(struct net_device *dev)
{
    printk(KERN_EMERG "LAB5: netdev open\n");
    netif_start_queue(dev);
    return 0;
}

static int lab5_stop(struct net_device *dev)
{
    printk(KERN_EMERG "LAB5: netdev stop\n");
    netif_stop_queue(dev);
    return 0;
}

static netdev_tx_t lab5_xmit(struct sk_buff *skb, struct net_device *dev)
{
    dev_kfree_skb(skb);
    return NETDEV_TX_OK;
}

static const struct net_device_ops lab5_netdev_ops = {
    .ndo_open       = lab5_open,
    .ndo_stop       = lab5_stop,
    .ndo_start_xmit = lab5_xmit,
};

/* ================= PCI callbacks ================= */

static int lab5_probe(struct pci_dev *pdev,
                      const struct pci_device_id *id)
{
    struct net_device *ndev;
    struct lab5_priv *priv;
    int err;

    printk(KERN_EMERG "LAB5: PROBE ENTERED\n");

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

    /* для лабы — генерируем MAC */
    eth_random_addr(ndev->dev_addr);
    printk(KERN_EMERG "LAB5: MAC %pM\n", ndev->dev_addr);

    pci_set_drvdata(pdev, ndev);

    err = register_netdev(ndev);
    if (err)
        goto err_iounmap;

    printk(KERN_EMERG "LAB5: PROBE SUCCESS\n");
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
    struct lab5_priv *priv;

    printk(KERN_EMERG "LAB5: REMOVE ENTERED\n");

    if (!ndev)
        return;

    priv = netdev_priv(ndev);

    unregister_netdev(ndev);
    pci_iounmap(pdev, priv->hw_addr);
    free_netdev(ndev);
    pci_release_regions(pdev);
    pci_disable_device(pdev);
}

/* ================= PCI ID table ================= */

/* специально PCI_ANY_ID — чтобы probe гарантированно вызывался в VM */
static const struct pci_device_id lab5_ids[] = {
    { PCI_ANY_ID, PCI_ANY_ID },
    { 0, }
};
MODULE_DEVICE_TABLE(pci, lab5_ids);

/* ================= driver struct ================= */

static struct pci_driver lab5_driver = {
    .name     = DRV_NAME,
    .id_table = lab5_ids,
    .probe    = lab5_probe,
    .remove   = lab5_remove,
};

module_pci_driver(lab5_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("student");
MODULE_DESCRIPTION("Lab 5 PCI network driver");
