#include <rumble.h>
#include <packet.h>
#include <output_report.h>
#include <string.h>
#include <math.h>

struct RumbleEncodedData joycon_rumble_encode(float freq, float amp)
{
    // The encoded hex freq varies from 64 (0x40) to 
    uint8_t encoded_hex_freq = (uint8_t)round(log2f(freq / 10.0f) * 32.0f);
    uint16_t hf = (encoded_hex_freq > 0x60 ? (encoded_hex_freq - 0x60) << 2 : 0);
    uint8_t lf = encoded_hex_freq - 0x40;

    uint8_t encoded_hex_amp = 0;
    // Saturate
    if (amp > 1.0f) amp = 1.0f;
    if (amp < 0.0f) amp = 0.0f;

    if (amp > 0.23f) encoded_hex_amp = (uint8_t)round(log2f(amp * 8.7f) * 32.0f);
    else if (amp > 0.12f) encoded_hex_amp = (uint8_t)round(log2f(amp * 17.0f) * 16.0f);

    uint16_t hf_amp = encoded_hex_amp << 1;
    uint16_t lf_amp = (encoded_hex_amp >> 1) + 0x40;

    // Byte has been swapped
    struct RumbleEncodedData data;
    data.data[0] = (uint8_t)(hf & 0xFF);
    data.data[1] = (uint8_t)hf_amp + ((hf >> 8) & 0xFF);
    data.data[2] = (lf | ((encoded_hex_amp % 2) << 7));
    data.data[3] = (uint8_t)(lf_amp & 0xFF);
    return data;
}

void joycon_packet_rumble_only(uint8_t *buffer, uint8_t timer, float freq, float amp)
{
    // Reset packet to a rumble-only packet
    memset(buffer, 0, OUTPUT_REPORT_LEGNTH);
    struct Header *hdr = (struct Header *)buffer;
    hdr->command = Rumble;
    hdr->counter = timer;
    hdr->rumble_l = joycon_rumble_encode(freq, amp);
    memcpy(hdr->rumble_r.data, hdr->rumble_l.data, sizeof(hdr->rumble_l));
}

void joycon_packet_append_rumble(uint8_t *buffer, float freq, float amp)
{
    // Append rumble to a packet
    struct Header *hdr = (struct Header *)buffer;
    hdr->rumble_l = joycon_rumble_encode(freq, amp);
    memcpy(hdr->rumble_r.data, hdr->rumble_l.data, sizeof(hdr->rumble_l));
}

void joycon_packet_rumble_enable(uint8_t *buffer, uint8_t timer, float freq, float amp)
{
    joycon_packet_rumble_enable_only(buffer, timer);

    // Encode rumble frequency and amplitude
    joycon_packet_append_rumble(buffer, freq, amp);
}

void joycon_packet_rumble_disable(uint8_t *buffer, uint8_t timer, float freq, float amp)
{
    joycon_packet_rumble_disable_only(buffer, timer);

    // Encode rumble frequency and amplitude
    joycon_packet_append_rumble(buffer, freq, amp);
}

void joycon_packet_rumble_enable_only(uint8_t *buffer, uint8_t timer)
{
    memset(buffer, 0, OUTPUT_REPORT_LEGNTH);
    struct Header *hdr = (struct Header *)buffer;
    struct SubcommandBody *pkt = (struct SubcommandBody *)(hdr + 1);
    hdr->command = Subcommand;
    hdr->counter = timer;
    pkt->subcommand = EnableVibration;
    pkt->args.arg1 = VIBRATION_ENABLE;
}

void joycon_packet_rumble_disable_only(uint8_t *buffer, uint8_t timer)
{
    memset(buffer, 0, OUTPUT_REPORT_LEGNTH);
    struct Header *hdr = (struct Header *)buffer;
    struct SubcommandBody *pkt = (struct SubcommandBody *)(hdr + 1);
    hdr->command = Subcommand;
    hdr->counter = timer;
    pkt->subcommand = EnableVibration;
    pkt->args.arg1 = VIBRATION_DISABLE;
}
