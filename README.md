# AxoCore
Synthcore - это микроядро для архитектуры x86.  
Имеет [лицензию GPL 3.0](LICENSE).

## Сборка

Установить пакеты:
```bash
sudo pacman -S clang grub lld
```

В директории [scripts](scripts/) имеется файл [build.sh](scripts/build.sh).  
```bash
./build.sh
```

Или же для самостоятельной сборки, находясь в корне проекта, пропишите в терминале следующее:

```bash
cmake . -B build
cmake --build build
```

и запустите:

```bash
make run
```

или

```bash
qemu-system-x86_64 -cdrom build/SynthCore.iso
```