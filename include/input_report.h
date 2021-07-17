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
struct SIRButtonStatus {
    uint8_t right;
    uint8_t share;
    uint8_t left;
};

struct InputReportHeader {
    uint8_t id;
    uint8_t timer;
    uint8_t battery_level: 4;
    uint8_t conn_info: 4;
    struct SIRButtonStatus btn_status;
    uint8_t left_stick_status[3];   // TODO: Replace with a struct
    uint8_t right_stick_status[3];  // TODO: Replace with a struct
    uint8_t vibrator_input_report;
};  // 13 bytes

// Right
#define SIR_BUTTON_STATUS_MASK_Y        0x01
#define SIR_BUTTON_STATUS_MASK_X        0x02
#define SIR_BUTTON_STATUS_MASK_B        0x04
#define SIR_BUTTON_STATUS_MASK_A        0x08
#define SIR_BUTTON_STATUS_MASK_R        0x40
#define SIR_BUTTON_STATUS_MASK_ZR       0x80
// Left
#define SIR_BUTTON_STATUS_MASK_DOWN     0x01
#define SIR_BUTTON_STATUS_MASK_UP       0x02
#define SIR_BUTTON_STATUS_MASK_RIGHT    0x04
#define SIR_BUTTON_STATUS_MASK_LEFT     0x08
#define SIR_BUTTON_STATUS_MASK_L        0x40
#define SIR_BUTTON_STATUS_MASK_ZL       0x80
// Common
#define SIR_BUTTON_STATUS_MASK_SR       0x10
#define SIR_BUTTON_STATUS_MASK_SL       0x20
// Share
#define SIR_BUTTON_STATUS_MASK_MINUS    0x01
#define SIR_BUTTON_STATUS_MASK_PLUS     0x02
#define SIR_BUTTON_STATUS_MASK_RSTICK   0x04
#define SIR_BUTTON_STATUS_MASK_LSTICK   0x08
#define SIR_BUTTON_STATUS_MASK_HOME     0x10
#define SIR_BUTTON_STATUS_MASK_CAPTURE  0x20
#define SIR_BUTTON_STATUS_MASK_GRIP     0x80

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

// Replies
struct GetRegulatedVoltageReply {
    uint8_t low;
    uint8_t high;
};

struct JoyconColorReply {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct SubcommandBodySPIColorData {
    struct SubcommandBodySPIData data;
    struct JoyconColorReply color;
};

struct SubcommandBodySPI2ColorsData {
    struct SubcommandBodySPIData data;
    struct JoyconColorReply body_color;
    struct JoyconColorReply button_color;
};

struct IMUPackedDataReply {
    struct IMUPackedData data[3];
};

#endif