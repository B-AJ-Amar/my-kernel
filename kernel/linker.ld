ENTRY(kernel_entry)


SECTIONS {
    . = 0x0100000;

    .text ALIGN(4096) :
    {
        *(.start)
        *(.text)
    }

    .rodata ALIGN(4096) :
    {
        *(.rodata)
    }

    .data ALIGN(4096) :
    {
        *(.data)
    }

    .bss ALIGN(4096) :
    {
        *(.bss)
    }

    kernel_end = .;

    .stack ALIGN(4096) (NOLOAD) :
    {
        __stack_bottom = .;
        . = . + 16*1024;   /* 16KB stack */
        __stack_top = .;
    }


}