#ifndef RUMBLE_H
#define RUMBLE_H

#include "types.h"

/**
 * Rumble encoded data:
 *  ___________________________________________________________
 * | 0bFFFF FFFF | 0bAAAA AAAH |   0bIFFF FFFF   | 0bAAAA AAAA |
 * |    HB LF    |  HBHF, Amp  | LB LF, Int. Amp |    LB Amp   |
 * |_____________|_____________|_________________|_____________|
 */

#define HBHF_INDICATOR_MASK 0x01
#define HBHF_AMPLITUDE_MASK 0xFE
#define LBLF_FREQUENCY_MASK 0x7F
#define LBLF_INDICATOR_MASK 0x80

struct RumbleEncodedData joycon_rumble_encode(float freq, float amp);

#endif