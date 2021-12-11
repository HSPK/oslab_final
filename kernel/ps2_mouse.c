#include "ps2_mouse.h"
#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"

mouse_packet_t mouse_data;
mouse_stat_t mouse_stat;
u8 mouse_data_idx = 0;
void ps2_mouse_handler();
u8 ps2_read();

/* 等待ps2控制器可读/写 rw = 1 读，rw = 2 写 */
void ps2_wait(int rw) {
    u8 data, time_out = 100;
    do {
        data = in_byte(PS2_STAT);
    } while (((data & rw) + rw - 2) && --time_out);
}

void ps2_write_sencond(u8 data) {
    ps2_wait(PS2_WRITE);
    out_byte(PS2_CTRL, PS2_WRITE_NEXT_SECOND_PS2_IN_BUFFER);
    ps2_wait(PS2_WRITE);
    out_byte(PS2_DATA, data);
    ps2_read();
}

u8 ps2_read() {
    ps2_wait(PS2_READ);
    return in_byte(PS2_DATA);
}

void set_sample_rate(u8 rate) {
    ps2_write_sencond(PS2_MOUSE_SET_SAMPLE_RATE);
    ps2_write_sencond(rate);
}

void set_resolution(u8 resolution) {
    ps2_write_sencond(PS2_MOUSE_SET_RESOLUTION);
    ps2_write_sencond(resolution);
}

void set_scaling(u8 scale) {
    ps2_write_sencond(PS2_MOUSE_SET_SCALING);
    ps2_write_sencond(scale);
}

void init_mouse() {
    u8 ack;
    // enable second ps/2 port
    ps2_wait(PS2_WRITE);
    out_byte(PS2_CTRL, PS2_ENABLE_SECOND_PS2_PORT);
    ack = ps2_read();
    // 使能 irq 12
    ps2_wait(PS2_WRITE);
    out_byte(PS2_CTRL, PS2_READ_BYTE_0_FROM_RAM);
    ps2_wait(PS2_READ);
    u8 configure = in_byte(PS2_DATA);
    configure |= 0x02;
    configure &= 0xdf;
    ps2_wait(PS2_WRITE);
    out_byte(PS2_CTRL, PS2_WRITE_BYTE_0_TO_RAM);
    ps2_wait(PS2_WRITE);
    out_byte(PS2_DATA, configure);

    ps2_write_sencond(PS2_MOUSE_SET_DEFAULT);
    set_sample_rate(100);
    set_resolution(3);
    set_scaling(1);
    ps2_write_sencond(PS2_ENABLE_DATA_REPORTING);
    printl("enable mouse irq...\n");

    put_irq_handler(PS2_MOUSE_IRQ, ps2_mouse_handler);
    enable_irq(PS2_MOUSE_IRQ);
}

void compute_mouse_stat() {
    u8 state = mouse_data.data[0];
    mouse_stat.left_button = state & 0x1;
    mouse_stat.right_button = state & 0x2;
    mouse_stat.middle_button = state & 0x4;
    mouse_stat.mouse_x = mouse_stat.mouse_x + 
                mouse_data.data[1] - ((state << 4) & 0x100);
    mouse_stat.mouse_y = mouse_stat.mouse_y + 
                mouse_data.data[2] - ((state << 3) & 0x100);
}

void ps2_mouse_handler() {
    u8 data = in_byte(PS2_DATA);
    mouse_data.data[mouse_data_idx++] = data;
    if (mouse_data_idx == 3) {
        mouse_data_idx = 0;
        compute_mouse_stat();
        if (mouse_stat.left_button) {
            printl("mouse : left button clicked\n");
        } else if (mouse_stat.right_button) {
            printl("mouse : right button clicked\n");
        }
    }
}