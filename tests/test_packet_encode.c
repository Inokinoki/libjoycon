#include <mcu.h>
#include <rumble.h>
#include <packet.h>

#include <stdio.h>
#include <assert.h>

int main()
{
    uint8_t buffer[64];

    assert(36 == sizeof(struct MCUConfiguration));
    assert(49 == sizeof(struct PacketMCUReadComm));

    // Test ACK encoding
    uint8_t ack_index = 1;
    joycon_packet_mcu_read_ack_encode(buffer, 1, ack_index);
    assert(buffer[0] == 0x11);
    assert(buffer[10] == 0x03);
    assert(buffer[14] == ack_index);
    assert(buffer[47] == mcu_crc8(buffer + 11, 36));
    assert(buffer[48] == 0xFF);

    // Test REQ encoding
    uint8_t req_index = 2;
    joycon_packet_mcu_read_req_encode(buffer, 2, req_index);
    assert(buffer[0] == 0x11);
    assert(buffer[10] == 0x03);
    assert(buffer[13] == req_index);
    assert(buffer[14] == 0);
    assert(buffer[47] == mcu_crc8(buffer + 11, 36));
    assert(buffer[48] == 0xFF);

    return 0;
}