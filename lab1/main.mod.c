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
	{ 0x633f53bb, "device_destroy" },
	{ 0xc4d37928, "class_destroy" },
	{ 0xdfff5b72, "device_create" },
	{ 0xb35bd512, "cdev_del" },
	{ 0xecd155b, "__class_create" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xc75d4a28, "cdev_add" },
	{ 0x88afa5a9, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0xb43f9365, "ktime_get" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "D960AB5BC7ADDC1C4DFA666");
