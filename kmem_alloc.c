#include <linux/kthread.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
unsigned int objnum = 4000;
module_param(objnum, int, 0);
struct KMM
{
	unsigned int arr[8];
};
static struct timeval begin;
static struct timeval finish;
static int simple_init (void) {
	int i;
	struct KMM * mem;
	do_gettimeofday(&begin);
	i=0; 
	while(i < objnum)
	{
		mem = (struct KMM*)kmalloc( sizeof(struct KMM), GFP_KERNEL);
		kfree(mem);
		i++;
	}
	do_gettimeofday(&finish);
	printk(KERN_INFO "Timing Differences: %lu", (finish.tv_usec - begin.tv_usec));
    	return 0;
}
	static void simple_exit (void) {
	printk(KERN_INFO "Unloading of kmem_alloc");
}
module_init (simple_init);
module_exit (simple_exit);
