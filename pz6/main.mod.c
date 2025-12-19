#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xd3166ddb, "module_layout" },
	{ 0xd4dc0aa, "pci_unregister_driver" },
	{ 0xebc5f684, "__pci_register_driver" },
	{ 0x4d7f10c1, "register_netdev" },
	{ 0x651e6bf, "alloc_netdev_mqs" },
	{ 0xa0440539, "kmem_cache_alloc_trace" },
	{ 0x2c330630, "kmalloc_caches" },
	{ 0x45b55ead, "consume_skb" },
	{ 0xf625b3a7, "ether_setup" },
	{ 0x37a0cba, "kfree" },
	{ 0x720aa0d, "free_netdev" },
	{ 0x54c38114, "unregister_netdev" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x92997ed8, "_printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("pci:v00008086d0000100Esv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "0CABDCDAD897D0FAA28BD00");
