# Projeto Kernel Linux I - Implementação de *syscall* : energy_mode

## Integrantes
- Daniel Henriques Pamplona - RA: 260401
- Gabriel Pavani Giro - RA: 247114
- Jonatas de Sousa Santos - RA: 225334

## Descrição geral

O trabalho consiste na implementação de duas novas chamadas de sistema (*syscalls*) no kernel Linux, com o objetivo de manipular um modo de energia global do sistema. A funcionalidade implementada permite:

Definir (*set*) o modo de energia:

- 0 → Modo normal
- 1 → Modo turbo

Consultar (*get*) o modo de energia atual.

As syscalls permitem que aplicações em espaço de usuário possam alterar ou consultar o estado de energia do sistema, simulando um controle simplificado de políticas energéticas.

## Arquivos modificados

### energy_mode.c
  
O arquivo contendo a implementação das duas *syscalls*:

- ```SYSCALL_DEFINE1(set_energy_mode, int mode) ```
- ```SYSCALL_DEFINE0(get_energy_mode)```

### kernel/syscalls/sycall_64.tbl

Adição das novas *syscalls* na tabela:

- ```468 common set_energy_mode sys_set_energy_mode```

- ```469 common get_energy_mode sys_get_energy_mode```

### include/linux/syscalls.h

Declaração das novas *syscalls*:

- ```asmlinkage long sys_set_energy_mode(int mode); ```

- ```asmlinkage long sys_get_energy_mode(void); ```

### include/uapi/asm-generic/unistd.h

Atualização do número total de *syscalls*

- ```#define __NR_syscalls 469```

### kernel/Makefile

Inclusão do energy_mode.o:

- ```obj-y += energy_mode.o```

### energy_test.c

O programa de teste foi criado na host e, para o devido teste, foi passada para a VM da seguinte forma:

- Compila o arquivo:
```bash
gcc energy_test.c -o energy_test
```

- Monta a imagem na VM:
```bash
sudo mount -o loop my_disk.raw /mnt
```

- Copia o arquivo já compilado
```bash
sudo cp ~/path/energy_test /mnt/root/
```

- Desmonta:
```bash
sudo umount /mnt
```

Além disso, o programa de teste verifica o funcionamento das novas *syscalls*:

- Consulta o modo atual (*get*)
- Alterna o modo (*set*)
- Verifica novamente e restaura ao estado atual

## Testando

Para testar, com o arquivo teste já compilado na VM, basta utilizar:

```bash
./economy_test
```
## Saída esperada

Ao executar o programa, a saída esperada deveria ser:
```
Lendo modo de energia atual...
Modo de energia atual: 0
Alterando modo de energia para: 1
Novo modo de energia: 1
Restaurando modo de energia para: 0
Modo de energia restaurado com sucesso.
```

### Explicação

Primeiramente, o teste utiliza o ```get_energy_mode()``` para verificar o valor inicial de ```energy_mode```. Como a variável é inicializada como ```0``` no kernel (```static int energy_mode = 0;```), ele retorna ```0```.

Depois disso, ele troca para o estado ```1``` caso o estado anterior fosse ```0```, utilizando o ```set_energy_mode()```.

Por fim, ele restaura o modo de energia anterior.

#### Obs:
Podemos ainda verificar o ```dmesg``` para confirmar que os ```printk``` foram chamados utilizando ```dmesg | grep "Energy mode"```.

O output esperado dessa verificação é:

```
[   29.231270] Energy mode read: 0
[   29.233682] Energy mode set to: 1
[   29.233688] Energy mode read: 1
[   29.235975] Energy mode set to: 0
```

Cada vez que o programa chama as funções ```get``` e ```set```, o kernel imprime uma mensagem no log. Isso nos confirma que as *syscalls* foram executadas no kernel.

