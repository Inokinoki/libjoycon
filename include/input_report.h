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

#pragma pack(push, 1)
struct InputReportHeader {
    uint8_t id;
    uint8_t timer;
    uint8_t battery_level: 4;
    uint8_t conn_info: 4;
    uint8_t btn_status[3];  // TODO: Replace with a struct
    uint8_t left_stick_status[3];   // TODO: Replace with a struct
    uint8_t right_stick_status[3];  // TODO: Replace with a struct
    uint8_t vibrator_input_report;
};  // 13 bytes

struct InputReportSubcommandReply {
    uint8_t ack;
    uint8_t replied_subcommmand_id;
    uint8_t data[35];
};

struct InputReportNFCIRFWUpdate {
    uint8_t data[37];
};

struct InputReportIMU {
    // TODO
    uint8_t data[36];
};

struct InputReportIMUNFCIR {
    // TODO
    uint8_t data[36];
    uint8_t nfc_ir_data[313];
};

// IMU data
#include "imu.h"

#endif