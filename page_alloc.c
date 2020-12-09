#include <linux/kthread.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/gfp.h>
MODULE_LICENSE("GPL");
unsigned int objnum = 4000;
module_param(objnum, int, 0);
struct KMM
{
	unsigned int arr[8];
};
static struct timeval begin;
static struct timeval finish;
int serial;
static struct page *page;
static int page_alloc (void* none)
{        
	int size;
	struct KMM p1;
 	size=sizeof(p1);

	printk(KERN_INFO "Objnum: %u",objnum);
        printk(KERN_INFO "Size of struct : %d bytes",size);
	do_gettimeofday(&begin);
	serial = (int)ilog2(1+(objnum/128));
	printk(KERN_INFO "Allocation of two to the power pages:  %d",serial);
	page = alloc_pages(GFP_KERNEL, serial);
	__free_pages(page, serial);
	do_gettimeofday(&finish);
	printk(KERN_INFO "Timing Difference: %lu", (finish.tv_usec - begin.tv_usec));
	return 0;
}
static struct task_struct *thread;
static int simple_init (void) {
	
	thread=kthread_create(page_alloc,NULL, "thread");
	kthread_bind(thread, 0);
	wake_up_process(thread);
 	return 0;
}

static void simple_exit (void) {
	printk(KERN_INFO "Loading of Page Module");
}
module_init (simple_init);
module_exit (simple_exit);
