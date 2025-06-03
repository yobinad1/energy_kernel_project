#include <linux/kernel.h>
#include <linux/syscalls.h>

static int energy_mode = 0;  // 0 = normal, 1 = turbo

SYSCALL_DEFINE1(set_energy_mode, int, mode) {
    if (mode != 0 && mode != 1) {
        return -EINVAL;  // argumento inválido
    }
    energy_mode = mode;
    printk(KERN_INFO "Energy mode set to: %d\n", energy_mode);
    return 0;
}

SYSCALL_DEFINE0(get_energy_mode) {
    printk(KERN_INFO "Energy mode read: %d\n", energy_mode);
    return energy_mode;
}
