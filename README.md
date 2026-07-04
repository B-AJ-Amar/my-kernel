# Kernel

An educational x86 operating system written from scratch in **C** and **x86 Assembly**.

The goal of this project is to understand how operating systems work by implementing every major component manually, starting from the  boot  and gradually building toward a complete kernel. Rather than relying on existing bootloaders or operating system frameworks, this project uses a custom bootloader and focuses on learning the low-level details of computer architecture.


## Build

Requirements:

* NASM
* i686-elf GCC cross-compiler
* GNU Make
* QEMU
* Docker

Build the project:

```bash
make build
```

Run:

```bash
make run
```

Debug:

```bash
make debug
```
