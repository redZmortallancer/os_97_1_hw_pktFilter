#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x110d4c8c, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xd76101cd, __VMLINUX_SYMBOL_STR(nf_unregister_net_hook) },
	{ 0xc053f0d0, __VMLINUX_SYMBOL_STR(class_unregister) },
	{ 0x664598a, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x92c8d733, __VMLINUX_SYMBOL_STR(nf_register_net_hook) },
	{ 0xb942fee1, __VMLINUX_SYMBOL_STR(init_net) },
	{ 0x859afd4a, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x96d80bf7, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x6bc3fbc0, __VMLINUX_SYMBOL_STR(__unregister_chrdev) },
	{ 0x6acac11f, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0xdca6c951, __VMLINUX_SYMBOL_STR(__register_chrdev) },
	{ 0xb44ad4b3, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0x88db9f48, __VMLINUX_SYMBOL_STR(__check_object_size) },
	{ 0xe914e41e, __VMLINUX_SYMBOL_STR(strcpy) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0xe2d5255a, __VMLINUX_SYMBOL_STR(strcmp) },
	{ 0x20000329, __VMLINUX_SYMBOL_STR(simple_strtoul) },
	{ 0x20c55ae0, __VMLINUX_SYMBOL_STR(sscanf) },
	{ 0x28318305, __VMLINUX_SYMBOL_STR(snprintf) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "83AA4D0CA87BA05C317D105");
