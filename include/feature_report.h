#ifndef FEATURES_H
#define FEATURES_H

enum Features {
    GetLastSubcmdRep    = 0x02,
    EnableOTAFWUpgrade  = 0x70,
    SetupMemoryRead,   // 0x71
    MemoryRead,        // 0x72
    MemorySectorErase, // 0x73
    MemoryWrite,       // 0x74
    Launch,            // 0x75
    UnknownSend         = 0xCC,
    UnknownRead         = 0xFE
};

#endif