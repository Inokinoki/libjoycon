#include <input_report.h>
#include <libjoycon.h>

struct StickStatus joycon_input_report_stick_decode(struct SIRStickStatus status)
{
    struct StickStatus result = {
        .horizontal = joycon_input_report_left_stick_decode(status),
        .vertical = joycon_input_report_right_stick_decode(status)
    };
    return result;
}

uint16_t joycon_input_report_left_stick_decode(struct SIRStickStatus status)
{
    return status.horizontal_low | ((status.share & 0x0F) << 8);
}

uint16_t joycon_input_report_right_stick_decode(struct SIRStickStatus status)
{
    return status.vertical_high | (status.share >> 4);
}

void joycon_packet_input_report_mode(uint8_t *buf, uint8_t timer, uint8_t mode)
{
    memset(buf, 0, 64);
    struct Header *hdr = (struct Header *)buf;
    struct SubcommandBody *pkt = (struct SubcommandBody *)(hdr + 1);
    hdr->command = Subcommand;
    hdr->counter = timer;
    pkt->subcommand = SetInputReportMode;
    pkt->args.arg1 = mode;
}
