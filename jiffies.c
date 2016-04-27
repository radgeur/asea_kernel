#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/seq_file.h>
#include <linux/jiffies.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>

#define NBMAX_PID 512
#define JIFFIES_BUFFER_LEN 6
static char jiffies_buffer[JIFFIES_BUFFER_LEN];
static int  jiffies_flag = 0;
static const struct file_operations jiffies_proc_fops;

static struct proc_dir_entry  *ase;
static int pid_list[NBMAX_PID];
static int cptpid = 0;

static int
jiffies_proc_show(struct seq_file *m, void *v)
{
    if (jiffies_flag)
        seq_printf(m, "%llu\n",
                   (unsigned long long) get_jiffies_64());
    return 0;
}

//when a file opening
static int
jiffies_proc_open(struct inode *inode, struct file *file)
{

    return single_open(file, jiffies_proc_show, NULL);
}

//when write somethinf in a file
static ssize_t
jiffies_proc_write(struct file *filp, const char __user *buff,
                   size_t len, loff_t *data)
{
    long res;
    printk(KERN_INFO "JIFFIES: Write has been called");
    if (len > (JIFFIES_BUFFER_LEN - 1)) {
        printk(KERN_INFO "JIFFIES: error, input too long");
        return -EINVAL;
    }
    else if (copy_from_user(jiffies_buffer, buff, len)) {
        return -2;
    }
    jiffies_buffer[len] = 0;
    kstrtol(jiffies_buffer, 0, &res);
    //jiffies_flag content the echo
    jiffies_flag = res;

    //if the pid exist
    if(find_vpid(jiffies_flag)!=NULL) {
        //copy the content of jiffies_flag in jiffies_buffer
        snprintf(jiffies_buffer, 10, "%d", jiffies_flag);
        //create a file in proc/ase with the pid give in jiffies_buffer
        proc_create(jiffies_buffer, 0666, ase, &jiffies_proc_fops);
        pid_list[cptpid] = jiffies_flag;
        cptpid++;
    } else{
        printk(KERN_INFO "JIFFIES: error, this pid doesn't exist");
    }



    return len;
}

//struct for call the right function at the right moment with the corresponding function
static const struct file_operations jiffies_proc_fops = {
    .owner      = THIS_MODULE,
    .open       = jiffies_proc_open,
    .read       = seq_read,
    .write      = jiffies_proc_write,
    .llseek     = seq_lseek,
    .release    = single_release,
};

static int __init
jiffies_proc_init(void)
{
    proc_create("ase_cmd", 0666, NULL, &jiffies_proc_fops);
    ase = proc_mkdir("ase", NULL);
    //proc_create("crash_jiffies", 0666, NULL, &jiffies_proc_fops);

    return 0;

}

static void __exit
jiffies_proc_exit(void)
{
    remove_proc_entry("ase_cmd",NULL);
    remove_proc_entry("ase",NULL);
    //remove_proc_entry("crash_jiffies", NULL);
}

module_init(jiffies_proc_init);
module_exit(jiffies_proc_exit);

MODULE_AUTHOR("Modified by Giuseppe Lipari from Robert P. J. Day, http://crashcourse.ca");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A jiffies /proc file.");
