// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("vladyslav Poluianov <vladpoluianov@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

struct my_list_item {
	struct list_head list;
	ktime_t timestamp;
};

static LIST_HEAD(my_list);

static unsigned int hello_count = 1;
module_param(hello_count, uint, S_IRUGO);
MODULE_PARM_DESC(hello_count, "Number of times to print 'Hello, world!'");



static int __init hello_init(void)
{
	struct my_list_item *item;
	int i;

	if (hello_count == 0 || (hello_count >= 5 && hello_count <= 10)) {
		pr_warn("Hello,	world!	Warning: Count is 0 or between 5 and 10.\n");
	}
	
	BUG_ON(hello_count > 10);


	for (i = 0; i < hello_count; i++) {

		if (i == hello_count - 2) {
			item = NULL;
		}
		else{
			item = kmalloc(sizeof(struct my_list_item), GFP_KERNEL);
		}
		BUG_ON(!item);

		item->timestamp = ktime_get();
		INIT_LIST_HEAD(&item->list);
		list_add(&item->list, &my_list);
		printk(KERN_EMERG "Hello, world!\n");
		}

	return 0;
}

static void print_list(void)
{
	struct my_list_item *item;
	struct list_head *pos;

	list_for_each(pos, &my_list) {
		item = list_entry(pos, struct my_list_item, list);
		pr_info("Timestamp: %lld\n", ktime_to_ns(item->timestamp));
	}
}


static void __exit hello_exit(void)
{
	struct my_list_item *item;
	struct list_head *pos, *n;

	print_list();

	list_for_each_safe(pos, n, &my_list) {
		item = list_entry(pos, struct my_list_item, list);
		list_del(pos);
		kfree(item);
		}

	pr_info("Module unloaded and list cleared\n");

}

module_init(hello_init);
module_exit(hello_exit);
