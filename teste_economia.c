#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>

#define SYS_set_energy_mode 468
#define SYS_get_energy_mode 469

int set_energy_mode(int mode) {
    return syscall(SYS_set_energy_mode, mode);
}

int get_energy_mode() {
    return syscall(SYS_get_energy_mode);
}

int main() {
    int old_mode, new_mode, current_mode;

    printf("Lendo modo de energia atual...\n");
    old_mode = get_energy_mode();
    if (old_mode < 0) {
        perror("get_energy_mode falhou");
        return 1;
    }
    printf("Modo de energia atual: %d\n", old_mode);

    new_mode = (old_mode == 0) ? 1 : 0;
    printf("Alterando modo de energia para: %d\n", new_mode);
    if (set_energy_mode(new_mode) < 0) {
        perror("set_energy_mode falhou");
        return 1;
    }

    current_mode = get_energy_mode();
    if (current_mode < 0) {
        perror("get_energy_mode falhou");
        return 1;
    }
    printf("Novo modo de energia: %d\n", current_mode);

    printf("Restaurando modo de energia para: %d\n", old_mode);
    if (set_energy_mode(old_mode) < 0) {
        perror("set_energy_mode falhou");
        return 1;
    }

    printf("Modo de energia restaurado com sucesso.\n");
    return 0;
}
