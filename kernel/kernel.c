void kernel_main(void) {
    // x86 maps the VGA textmode framebuffer to this address
    volatile unsigned short *video = (unsigned short *)0xB8000;

    video[0] = 0x0F41;
    video[1] = 0x0E4E;
    video[2] = 0x0F49;
    video[3] = 0x0F4F;
    video[4] = 0x0F53;
}
