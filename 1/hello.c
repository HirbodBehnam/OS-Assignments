#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/ktime.h>
#include <linux/rtc.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hirbod Behnam");
MODULE_DESCRIPTION("OS assignment kernel module");

static int __init hello_init(void)
{
	time64_t current_time = ktime_get_real();
	struct rtc_time t = rtc_ktime_to_tm(current_time);
	printk(KERN_INFO "Hirbod Behnam 99171333 at %lld of epoch (nano)! This is %d-%d-%d %d:%d:%d\n", current_time, t.tm_year+ 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
	return 0;
}

static void __exit hello_cleanup(void)
{
	time64_t current_time = ktime_get_real_seconds();
	printk(KERN_INFO "Cleaning up module at %lld of epoch.\n", current_time);
}

module_init(hello_init);
module_exit(hello_cleanup);
