#include <linux/init.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/time.h>
#include <linux/blkdev.h>
#include <linux/miscdevice.h>
#include <linux/cdev.h>
#include <linux/device.h>

MODULE_LICENSE("Dual BSD/GPL");


struct kprobe kp;

/* pre_handler: this is called just before the probed instruction is
 *executed.
 */
int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
	printk("pre_handler: p->addr=0x%p, eflags=0x%lx\n",p->addr,
	       regs->eflags);
	return 0;
}

/* post_handler: this is called after the probed instruction is executed
 * (provided no exception is generated).
 */
void handler_post(struct kprobe *p, struct pt_regs *regs, unsigned long flags)
{
	printk("post_handler: p->addr=0x%p, eflags=0x%lx \n", p->addr,
	       regs->eflags);
}

/* fault_handler: this is called if an exception is generated for any
 *instruction within the fault-handler, or when Kprobes
 *single-steps the probed instruction.
 */
int handler_fault(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
	printk("fault_handler:p->addr=0x%p, eflags=0x%lx\n", p->addr,
	       regs->eflags);
	return 0;
}



static int k_looper_init(void)
{

	/* specify pre_handler address
	 */
	kp.pre_handler=handler_pre;
	/* specify post_handler address
	 */
	kp.post_handler=handler_post;
	/* specify fault_handler address
	 */
	kp.fault_handler=handler_fault;
	/* specify the address/offset where you want to insert probe.
	 * You can get the address using one of the methods described above.
	 */
	kp.addr = (kprobe_opcode_t *) kallsyms_lookup_name("do_fork");
	/* check if the kallsyms_lookup_name() returned the correct value.
	 */
	if (kp.add == NULL) {
		printk("kallsyms_lookup_name could not find address
		for the specified symbol name\n");
		return 1;
	}
	/*or specify address directly.
	 * $grep "do_fork" /usr/src/linux/System.map
	 * or
	 * $cat /proc/kallsyms |grep do_fork
	 * or
	 * $nm vmlinuz |grep do_fork
	 */
	kp.addr = (kprobe_opcode_t *) 0xc01441d0;
	/* All set to register with Kprobes
	 */
	register_kprobe(&kp);

	printk(KERN_ALERT "k_looper init\n");

	return 0;
}

static void k_looper_exit(void)
{
	printk(KERN_ALERT "k_looper exti\n");
}

module_init(k_looper_init);
module_exit(k_looper_exit);
