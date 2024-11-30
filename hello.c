#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/ktime.h>

MODULE_AUTHOR("Makovetskyi Dmytro <dima.makovetskuy@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint count = 1;
module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "The number of “Hello world!”");

struct hello_time {
	struct list_head list;
	ktime_t time;
};

static LIST_HEAD(hello_list);

static int __init hello_init(void)
{
	struct hello_time *ht;
	unsigned int i;

	if (count == 0 || (count >= 5 && count <= 10))
	pr_warn("Warning: Parameter count is 0 or between 5 and 10\n");

	if (count > 10) {
	pr_err("Error: Parameter count is greater than 10\n");
	return -EINVAL;
	}

	for (i = 0; i < count; i++) {
	ht = kmalloc(sizeof(*ht), GFP_KERNEL);
	if (!ht) {
		pr_err("Failed to allocate memory\n");
		return -ENOMEM;
	}

	ht->time = ktime_get();
	list_add_tail(&ht->list, &hello_list);
	pr_info("Hello, world!\n");
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct hello_time *ht, *tmp;

	list_for_each_entry_safe(ht, tmp, &hello_list, list) {
	pr_info("Time: %lld ns\n", ktime_to_ns(ht->time));
	list_del(&ht->list);
	kfree(ht);
	}
}

module_init(hello_init);
module_exit(hello_exit);
