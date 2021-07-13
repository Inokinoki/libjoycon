#ifndef OUTPUT_REPORT_H
#define OUTPUT_REPORT_H

enum OutputCommand {
    Subcommand          = 0x01,
    NFCIRFirmwareUpdate = 0x03,
    Rumble              = 0x10,
    NFCIRRead           = 0x11,
};

enum OutputSubCommand {
    GetControllerState  = 0x00,
    BluetoothManPair,   // 0x01
    GetDeviceInfo,      // 0x02
    SetInputReportMode, // 0x03
    ButtonElapseTime,   // 0x04
    GetPageListState,   // 0x05
    SetHCIState,        // 0x06
    ResetPairInfo,      // 0x07
    SetShipmentLowPo,   // 0x08

    SPIFlashRead        = 0x10,
    SPIFlashWrite,      // 0x11,
    SPISectorErase,     // 0x12,

    ResetNFCIR          = 0x20,
    SetNFCIRConf,       // 0x21
    SetNFCIRState,      // 0x22

    SetNFCIRXXXState    = 0x28,
    GetNFCIRX28State    = 0x29,
    SetGPIOPinOut2      = 0x2A,
    GetNFCIRX29State    = 0x2B,

    SetPlayerLight      = 0x30,
    GetPlayerLight,    // 0x31
    SetHomeLight        = 0x38,

    EnableIMU           = 0x40,
    SetIMUSensitivity, // 0x41
    WriteIMUReg,       // 0x42
    ReadIMUReg,        // 0x43

    EnableVibration     = 0x48,

    GetRegulatedVoltage = 0x50,

    SetGPIOPinOut1      = 0x51,
    GetGPIOPinInOut     = 0x52
};

#endif