
# Kernel

An educational x86 operating system written from scratch in **C** and **x86 Assembly**.

The goal of this project is to understand how operating systems work by implementing every major component manually, starting from the  boot  and gradually building toward a complete kernel. Rather than relying on existing bootloaders or operating system frameworks, this project uses a custom bootloader and focuses on learning the low-level details of computer architecture.

## Build

### Requirements

* Make
* QEMU
* Docker
* GDB (i prefer pwndbg) for debuging

### Build

```bash
make build
```

### Run

```bash
make run
```

Or build and run in one command:

```bash
make rb
```

### Debug

```bash
make debug

make gdb
```

## Project Status

The project is being developed incrementally. Each subsystem is implemented from the ground up and integrated into the kernel as the project progresses.

### Completed

* [x] Custom bootloader
* [x] Protected mode initialization
* [x] GDT
* [x] Interrupt handling
* [x] IDT
* [x] PIC
* [x] VGA driver
* [x] PS/2 keyboard driver
* [x] Console
* [x] TTY
* [x] Timer driver
* [x] Sleep functionality
* [x] Common kernel libraries

  * [x] `stdio`
  * [x] `printf` and related output functions
  * [x] `string`
  * [x] `memcpy`, `memset`, `memcmp`, and related functions
* [x] Physical memory management (PMM)
* [x] Virtual memory management (VMM)
* [x] Kernel heap

### TODO

#### Scheduling

* [ ] Implement the kernel scheduler
* [ ] Implement task/thread management
* [ ] Implement context switching
* [ ] Support multiple kernel threads
* [ ] Integrate scheduling with the timer

#### Shell & Terminal

* [ ] Implement the shell
* [ ] Implement command parsing and execution
* [ ] Support multiple terminals
* [ ] Support multiple independent shell sessions

#### Filesystem

* [ ] Design the filesystem interface
* [ ] Implement a filesystem
* [ ] Implement file and directory operations
* [ ] Implement a file manager

#### Userspace

* [ ] Define the kernel/userspace boundary
* [ ] Implement userspace processes
* [ ] Implement system calls
* [ ] Implement userspace memory management
* [ ] Run programs in userspace
* [ ] Implement basic userspace utilities

#### Future Kernel Components

* [ ] Process management
* [ ] IPC
* [ ] Synchronization primitives
* [ ] More hardware drivers
* [ ] Networking
* [ ] Security and permissions
