OUTPUT_FORMAT("binary");

SECTIONS {
    .magic : {SHORT(0x4B45)}
    .bss_start : {QUAD(__bss_start)}
    .bss_size : {QUAD(__bss_end - __bss_start)}
    .text : {*(.text)}
    __bss_start = .;
    .bss : {*(.bss)}
    __bss_end = .;
    .data : {*(.data)}
    .rodata : {*(.rodata)}
}
