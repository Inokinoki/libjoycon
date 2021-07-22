#ifndef IR_H
#define IR_H

// 320 x 240
#define IR_RESOLUTION_FULL              0b00000000
#define IR_RESOLUTION_FULL_NUM_FRAG     0xFF

// 160 x 120
#define IR_RESOLUTION_HALF              0b01010000
#define IR_RESOLUTION_HALF_NUM_FRAG     0x3F

// 80 x 60
#define IR_RESOLUTION_QUAT              0b01100100
#define IR_RESOLUTION_QUAT_NUM_FRAG     0x0F

// 40 x 30
#define IR_RESOLUTION_OCTO              0b01101001;
#define IR_RESOLUTION_OCTO_NUM_FRAG     0x03;

#endif