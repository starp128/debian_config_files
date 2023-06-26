
/* kthread drain */

#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/kthread.h>
#include <linux/workqueue.h>

#include <linux/delay.h>	/* for msleep() */

//#include <linux/nbd-netlink.h>
#include <net/genetlink.h>


static unsigned int my_value = 16;
module_param(my_value, int, 0444);
MODULE_PARM_DESC(my_value, "number could be configured");


struct my_state {
	/* example for kthread (low level)*/
	struct task_struct *kthread;

	/* example for kthread_work */
	struct kthread_worker *worker;
	struct kthread_work work;
	struct kthread_delayed_work dwork;
	int kthread_work_count;

	/* example for workqueue.c */
	struct workqueue_struct *wq;
	struct work_struct wq_work;
	struct delayed_work wq_dwork;
	int wq_count;

	/* example wor system_wq */
	struct delayed_work system_wq_dwork;
	int system_wq_count;
} my_state;


int my_thread_fn(void *data)
{
	struct my_state *state = data;
	int i = 0;

	//we are in RUNNING state. set by kthread()

	for(;;) {
		//schedule_timeout_interruptible(3);
		msleep(1000);
		__set_current_state(TASK_RUNNING);
		pr_err("kthread count=%d\n", i++);
	}

	return 0;
}

void my_kthread_init(void)
{
	pr_err("called");

	//create kthread and wake up it.
	my_state.kthread = kthread_run(my_thread_fn, &my_state, "%s", "my_thread_name");

	if (IS_ERR(my_state.kthread))
		pr_err("create kthread failed\n");
}


/* example for kthread_worker and kthread_work */
void my_kthread_work_fn(struct kthread_work *work)
{
	struct my_state *s = container_of(work, struct my_state, work);

	pr_err("kthread_work_fn\n");
}

void my_kthread_dwork_fn(struct kthread_work *work)
{
	struct my_state *s = container_of(work, struct my_state, dwork.work);

	pr_err("kthread_work_count=%d\n", s->kthread_work_count++);

	kthread_init_delayed_work(&s->dwork, my_kthread_dwork_fn);
	if (!kthread_queue_delayed_work(s->worker, &s->dwork, 2000))
		pr_err("count not queue delayed work.\n");
}

void my_kthread_work_init(void)
{
	my_state.worker = kthread_create_worker(0, "kthreadwork_name");
	if (IS_ERR(my_state.worker))
		pr_err("create kthread worker failed\n");

	my_state.kthread_work_count = 0;
	kthread_init_work(&my_state.work, my_kthread_work_fn);

	if (!kthread_queue_work(my_state.worker, &my_state.work))
		pr_err("could not queue work. work already queued or is canceling\n");

	kthread_init_delayed_work(&my_state.dwork, my_kthread_dwork_fn);
	if (!kthread_queue_delayed_work(my_state.worker, &my_state.dwork, 2000))
		pr_err("count not queue delayed work.\n");

}


/* example for workqueue */
void my_workqueue_fn(struct work_struct *work)
{
	struct my_state *s = container_of(work, struct my_state, wq_work);

	pr_err("workqueue_fn\n");
}

void my_workqueue_d_fn(struct work_struct *work)
{
	struct my_state *s = container_of(work, struct my_state, wq_dwork.work);

	pr_err("wq_count %d\n", s->wq_count++);

	INIT_DELAYED_WORK(&s->wq_dwork, my_workqueue_d_fn);
	if (!queue_delayed_work(s->wq, &s->wq_dwork, 3000))
		pr_err("could not queue work. work already on a queue\n");
}

void my_workqueue_init(void)
{
	my_state.wq = alloc_workqueue("my_wq", WQ_UNBOUND | WQ_SYSFS, 1);
	if (my_state.wq == NULL)
		pr_err("alloc workqueue failed\n");

	my_state.wq_count = 0;
	INIT_WORK(&my_state.wq_work, my_workqueue_fn);

	if (!queue_work(my_state.wq, &my_state.wq_work))
		pr_err("queue work failed\n");

	INIT_DELAYED_WORK(&my_state.wq_dwork, my_workqueue_d_fn);
	if (!queue_delayed_work(my_state.wq, &my_state.wq_dwork, 3000))
		pr_err("could not queue work. work already on a queue\n");

}


/* netlink */


/* Netlink interface. */
enum {
	MY_CMD_UNSPEC,
	MY_CMD_1,
	__MY_CMD_MAX,
};
#define MY_CMD_MAX     (__MY_CMD_MAX - 1)


static int my_genl_cmd_1(struct sk_buff *skb, struct genl_info *info)
{
	int a = 0, b = 0;

	if (info->attrs[1])
		a = nla_get_u32(info->attrs[1]);

	if (info->attrs[2])
		b = nla_get_u32(info->attrs[2]);

	pr_err("a=%d b=%d\n", a, b);
	return 0;
}

static const struct genl_small_ops my_connect_genl_ops[] = {
        {
                .cmd    = MY_CMD_1,
                .validate = GENL_DONT_VALIDATE_STRICT | GENL_DONT_VALIDATE_DUMP,
                .doit   = my_genl_cmd_1,
        },
};

static const struct nla_policy my_attr_policy[3] = {
	[0]                =       { .type = NLA_U32 },
	[1]                =       { .type = NLA_U32 }, /* start here */
	[2]                =       { .type = NLA_U32 },
};

static const struct genl_multicast_group my_mcast_grps[] = {
        { .name = "my_mcast", },
};

static struct genl_family my_genl_family = {
	/* .id  will be set when register by netlink core*/
        .hdrsize        = 0,
        .name           = "my_netlink",
        .version        = 0x1,
        .module         = THIS_MODULE,

	.small_ops      = my_connect_genl_ops,
        .n_small_ops    = ARRAY_SIZE(my_connect_genl_ops),


        .maxattr        = 3,
        .policy		= my_attr_policy,

        .mcgrps         = my_mcast_grps,
        .n_mcgrps       = ARRAY_SIZE(my_mcast_grps),
};


static void my_mcast(int a, int b)
{
	struct sk_buff *skb;
	void *msg_head;
	int ret;

	skb = genlmsg_new(nla_total_size(sizeof(u32)), GFP_KERNEL);
	if (!skb)
		return;

	/* 6th arg is command. user space will got this command type */
	msg_head = genlmsg_put(skb, 0, 0, &my_genl_family, 0, 0xcc);
	if (!msg_head) {
		nlmsg_free(skb);
		return;
	}

	/*
	 * 2rd is attr type . seem should start from 1
	 */
	ret = nla_put_u32(skb, 1, a);
	if (ret) {
		nlmsg_free(skb);
		return;
	}

	ret = nla_put_u32(skb, 2, b);
	if (ret) {
		nlmsg_free(skb);
		return;
	}

	genlmsg_end(skb, msg_head);
	genlmsg_multicast(&my_genl_family, skb, 0, 0, GFP_KERNEL);
}

static void my_mcast_trigger(struct work_struct *work)
{
	struct my_state *s = container_of(work, struct my_state, system_wq_dwork.work);

	pr_err("trigger %d\n", s->system_wq_count);
	my_mcast(s->system_wq_count, s->system_wq_count+1);
	s->system_wq_count++;

	INIT_DELAYED_WORK(&s->system_wq_dwork, my_mcast_trigger);
	schedule_delayed_work(&s->system_wq_dwork, 10000);

}

void my_netlink_init(void)
{
	if (genl_register_family(&my_genl_family)) {
		pr_err("register genetlink failed\n");
        }

	my_state.system_wq_count = 0;

	INIT_DELAYED_WORK(&my_state.system_wq_dwork, my_mcast_trigger);
	schedule_delayed_work(&my_state.system_wq_dwork, 10000);
}


static int my_init(void)
{
	 my_kthread_init();
	 my_kthread_work_init();
	 my_workqueue_init();
	 my_netlink_init();

	 return 0;
}

void my_cleanup(void)
{

}


module_init(my_init);
module_exit(my_cleanup);

MODULE_DESCRIPTION("kernel module example");
MODULE_LICENSE("GPL");




