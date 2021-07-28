#include <input_report.h>

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
