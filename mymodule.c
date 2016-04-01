#include <linux/init.h>
#include <linux/module.h>
#include <linux/kprobes.h>
MODULE_LICENSE("Dual BSD/GPL");

int my_callback(int pid, int policy, struct jprobe *param)
{
    printk(KERN_ALERT "Here I am\n");
    jprobe_return();
    return 0;
}


static struct jprobe my_jprobe = {
    .kp = {
        .symbol_name = "do_sched_setscheduler",
    },
    .entry = (kprobe_opcode_t *) my_callback
};


static int my_init(void) {
    register_jprobe(&my_jprobe);
    return 0;
}

static void my_exit(void) {
    printk(KERN_ALERT "ByBye !");
}
    

module_init(my_init);
module_exit(my_exit);
	    
