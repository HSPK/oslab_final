#ifndef __PS2_MOUSE_
#define __PS2_MOUSE_

#define PS2_READ    0x01
#define PS2_WRITE   0x02
#define PS2_CTRL    0x64
#define PS2_STAT    PS2_CTRL
#define PS2_DATA    0x60
#define PS2_READ_BYTE_0_FROM_RAM    0x20
#define PS2_WRITE_BYTE_0_TO_RAM    0x60
#define PS2_READ_N_BYTE_FROM_RAM(n) (0x20 + n)
#define PS2_ENABLE_SECOND_PS2_PORT  0xa8
#define PS2_DISABLE_SECOND_PS2_PORT 0xa7
#define PS2_WRITE_NEXT_SECOND_PS2_IN_BUFFER 0xd4

#define PS2_MOUSE_SET_DEFAULT   0xf6
#define PS2_MOUSE_SET_SAMPLE_RATE   0xf3
#define PS2_MOUSE_GET_DEVICE_ID     0xf2
#define PS2_MOUSE_SET_STREAM_MODE   0xea
#define PS2_MOUSE_SET_RESOLUTION    0xe8
#define PS2_MOUSE_SET_SCALING       0xe6
#define PS2_ENABLE_DATA_REPORTING   0xf4


typedef struct {
    unsigned char data[3];
} mouse_packet_t;

typedef struct {
    int left_button;
    int right_button;
    int middle_button;
    int mouse_x;
    int mouse_y;
} mouse_stat_t;


void init_mouse();

#endif
