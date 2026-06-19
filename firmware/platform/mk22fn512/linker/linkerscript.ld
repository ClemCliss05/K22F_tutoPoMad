ENTRY(Reset_Handler)

STACK_SIZE = 0x1000;
HEAP_SIZE  = 0x2000;

/*--------------------------------------------------------------------
 * MK22FN512
 *-------------------------------------------------------------------*/

MEMORY
{
    FLASH_VECTOR (rx) : ORIGIN = 0x00000000, LENGTH = 0x00000400
    FLASH_CONFIG (rx) : ORIGIN = 0x00000400, LENGTH = 0x00000010
    FLASH        (rx) : ORIGIN = 0x00000410, LENGTH = 0x0007FBF0

    SRAM_L       (rwx): ORIGIN = 0x1FFF0000, LENGTH = 64K
    SRAM_U       (rwx): ORIGIN = 0x20000000, LENGTH = 64K
}

_estack = ORIGIN(SRAM_U) + LENGTH(SRAM_U);

SECTIONS
{
    .isr_vector :
    {
        . = ALIGN(256);
        KEEP(*(.isr_vector))
        . = ALIGN(256);
    } > FLASH_VECTOR

    .FlashConfig :
    {
        KEEP(*(.FlashConfig))
    } > FLASH_CONFIG

    .text :
    {
        . = ALIGN(4);

        *(.text)
        *(.text*)

        *(.rodata)
        *(.rodata*)

        KEEP(*(.init))
        KEEP(*(.fini))

        . = ALIGN(4);
    } > FLASH

    .ARM.extab :
    {
        *(.ARM.extab*)
    } > FLASH

    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } > FLASH

    .preinit_array :
    {
        __preinit_array_start = .;
        KEEP(*(.preinit_array))
        __preinit_array_end = .;
    } > FLASH

    .init_array :
    {
        __init_array_start = .;
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        __init_array_end = .;
    } > FLASH

    .fini_array :
    {
        __fini_array_start = .;
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        __fini_array_end = .;
    } > FLASH

    _sidata = LOADADDR(.data);

    .data :
    {
        . = ALIGN(4);

        _sdata = .;

        *(.data)
        *(.data*)

        . = ALIGN(4);

        _edata = .;

    } > SRAM_L AT > FLASH

    .bss :
    {
        . = ALIGN(4);

        _sbss = .;

        *(.bss)
        *(.bss*)
        *(COMMON)

        . = ALIGN(4);

        _ebss = .;

    } > SRAM_L

    .heap (NOLOAD) :
    {
        . = ALIGN(8);

        __HeapBase = .;

        . += HEAP_SIZE;

        __HeapLimit = .;

    } > SRAM_U

    .stack (NOLOAD) :
    {
        . = ALIGN(8);

        __StackLimit = .;

        . += STACK_SIZE;

        __StackTop = .;

    } > SRAM_U

    _estack = __StackTop;

    PROVIDE(end = __HeapBase);
    PROVIDE(_end = __HeapBase);

    ASSERT(__HeapLimit <= __StackLimit,
       "SRAM_U overflow (heap/stack collision)")

    ASSERT(_sidata <= ORIGIN(FLASH) + LENGTH(FLASH),
       "FLASH overflow")
}