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
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xb35bd512, "cdev_del" },
	{ 0xc4d37928, "class_destroy" },
	{ 0x633f53bb, "device_destroy" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0xdfff5b72, "device_create" },
	{ 0xecd155b, "__class_create" },
	{ 0xc75d4a28, "cdev_add" },
	{ 0x88afa5a9, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x92540fbf, "finish_wait" },
	{ 0x1000e51, "schedule" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0x56470118, "__warn_printk" },
	{ 0x800473f, "__cond_resched" },
	{ 0x92997ed8, "_printk" },
	{ 0x3eeb2322, "__wake_up" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "0E02F59CC887915E0FFD508");
