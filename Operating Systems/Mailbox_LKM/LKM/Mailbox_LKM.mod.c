#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x31a7e495, "module_layout" },
	{ 0x7e9ab7be, "kmem_cache_destroy" },
	{ 0xddded912, "kmem_cache_create" },
	{ 0x268cc6a2, "sys_close" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0x43079eba, "pid_task" },
	{ 0xfc90f63e, "find_vpid" },
	{ 0x75bb675a, "finish_wait" },
	{ 0x622fa02a, "prepare_to_wait" },
	{ 0x4292364c, "schedule" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0x48eb0c0d, "__init_waitqueue_head" },
	{ 0xfdde3d7a, "kmem_cache_alloc" },
	{ 0xe45f60d8, "__wake_up" },
	{ 0x2f287f0d, "copy_to_user" },
	{ 0xf1faac3a, "_raw_spin_lock_irq" },
	{ 0xde52487a, "current_task" },
	{ 0x2316c2a3, "kmem_cache_free" },
	{ 0x50eedeb8, "printk" },
	{ 0xf862395e, "pv_cpu_ops" },
	{ 0xb4390f9a, "mcount" },
	{ 0x8ff4079b, "pv_irq_ops" },
	{ 0x76ebea8, "pv_lock_ops" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "DFBC8BE6808D11DE3F11A93");
