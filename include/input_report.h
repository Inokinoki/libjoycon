#ifndef INPUT_REPORT_H
#define INPUT_REPORT_H

enum InputCommand {
    ButtonTrigged       = 0x3F,
    
    SIRWithSubcommand   = 0x21,
    NFCIRFWUpdateIR     = 0x23,
    
    SIRFullSpeed        = 0x30,
    NFCIRIR,           // 0x31
    SIRUnknown1,       // 0x32
    SIRUnknown2        // 0x33
};

// IMU data
#include "imu.h"

#endif