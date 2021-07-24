
#include <stdio.h>
#include <hidapi.h>
#include <string.h>
#include <stdint.h>
#include "../include/libjoycon.h"
#include "../include/ir.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#include <unistd.h>
#define Sleep(x) usleep(x * 1000)

typedef enum {
    NO_ERR,
    ERR_NUL,
    ERR_CON
} joycon_status;

static int handle_ok;
static int check_connection_ok;

static uint8_t timming_byte;

hid_device *handle;
hid_device *handle_l;

typedef enum {
    JOYCON_L,
    JOYCON_R,
    PROCON,
    OTHER
} joycon_type;

typedef struct
{
    hid_device *handle;
    joycon_type type;
} joycon;

joycon_status init_joycon(joycon *con, joycon_type type);

joycon_status release_joycon(joycon *con);

joycon_status get_joycon_info(joycon *con, u8 *buffer);

joycon_status get_joycon_spi_data(joycon *con, u32 offset, const u16 read_len, u8 *buffer);


int device_connection(){
    joycon con;
    if (check_connection_ok) {
        handle_ok = 0;
        // Try Joy-Con (L)
        if (init_joycon(&con, JOYCON_L) == NO_ERR) {
            handle_ok = 1;
            handle = con.handle;
            return handle_ok;
        }
        // Joy-Con (R)
        if (init_joycon(&con, JOYCON_R) == NO_ERR) {
            handle_ok = 2;
            handle = con.handle;
            return handle_ok;
        }
        // Pro Controller
        if (init_joycon(&con, PROCON) == NO_ERR) {
            handle_ok = 3;
            handle = con.handle;
            return handle_ok;
        }
        // Nothing found
        else {
            return 0;
        }
    }
    return handle_ok;
}

joycon_status init_joycon(joycon *con, joycon_type type)
{
    if (!con) {
        return ERR_NUL;
    }

    // Init controller
    switch (type)
    {
    case JOYCON_L:
        con->handle = hid_open(0x57e, 0x2006, NULL);
        break;
    case JOYCON_R:
        con->handle = hid_open(0x57e, 0x2007, NULL);
        break;
    case PROCON:
        con->handle = hid_open(0x57e, 0x2009, NULL);
        break;
    default:
        break;
    }

    // Set controller type
    if (con->handle) {
        con->type = type;
        return NO_ERR;
    }

    return ERR_CON;
}

joycon_status release_joycon(joycon *con)
{
    if (con && con->handle)
    {
        hid_close(con->handle);
        con->handle = NULL;
    }
    return NO_ERR;
}

int get_raw_ir_image(u8 show_status) {
/*
    std::ostream ir_status = std::cerr;

    int elapsed_time = 0;
    int elapsed_time2 = 0;

    u8 buf[49];
    u8 buf_reply[0x170];
    u8 *buf_image = new u8[19 * 4096]; // 8bpp greyscale image.
	uint16_t bad_signal = 0;
    int error_reading = 0;
    float noise_level = 0.0f;
    int avg_intensity_percent = 0.0f;
    int previous_frag_no = 0;
    int got_frag_no = 0;
    int missed_packet_no = 0;
    bool missed_packet = false;
    int initialization = 2;
    int max_pixels = ((ir_max_frag_no < 218 ? ir_max_frag_no : 217) + 1) * 300;
    int white_pixels_percent = 0;

    memset(buf_image, 0, sizeof(buf_image));

    memset(buf, 0, sizeof(buf));
    memset(buf_reply, 0, sizeof(buf_reply));
    auto hdr = (brcm_hdr *)buf;
    auto pkt = (brcm_cmd_01 *)(hdr + 1);
    hdr->cmd = 0x11;
    pkt->subcmd = 0x03;
    buf[48] = 0xFF;

    // First ack
    hdr->timer = timming_byte & 0xF;
    timming_byte++;
    buf[14] = 0x0;
    buf[47] = mcu_crc8(buf + 11, 36);
    hid_write(handle, buf, sizeof(buf));

    // IR Read/ACK loop for fragmented data packets. 
    // It also avoids requesting missed data fragments, we just skip it to not complicate things.
    while (enable_IRVideoPhoto || initialization) {
        memset(buf_reply, 0, sizeof(buf_reply));
        hid_read_timeout(handle, buf_reply, sizeof(buf_reply), 200);

        //Check if new packet
        if (buf_reply[0] == 0x31 && buf_reply[49] == 0x03) {
            got_frag_no = buf_reply[52];
            if (got_frag_no == (previous_frag_no + 1) % (ir_max_frag_no + 1)) {
                
                previous_frag_no = got_frag_no;

                // ACK for fragment
                hdr->timer = timming_byte & 0xF;
                timming_byte++;
                buf[14] = previous_frag_no;
                buf[47] = mcu_crc8(buf + 11, 36);
                hid_write(handle, buf, sizeof(buf));

                memcpy(buf_image + (300 * got_frag_no), buf_reply + 59, 300);

                // Auto exposure.
                // TODO: Fix placement, so it doesn't drop next fragment.
                if (enable_IRAutoExposure && initialization < 2 && got_frag_no == 0){
                    white_pixels_percent = (int)((*(u16*)&buf_reply[55] * 100) / max_pixels);
                    ir_sensor_auto_exposure(white_pixels_percent);
                }

                // Status percentage
                ir_status.str("");
                ir_status.clear();
                if (initialization < 2) {
                    if (show_status == 2)
                        ir_status << "Status: Streaming.. ";
                    else
                        ir_status << "Status: Receiving.. ";
                }
                else
                    ir_status << "Status: Initializing.. ";
                ir_status << std::setfill(' ') << std::setw(3);
                ir_status << std::fixed << std::setprecision(0) << (float)got_frag_no / (float)(ir_max_frag_no + 1) * 100.0f;
                ir_status << "% - ";

                //debug
               // printf("%02X Frag: Copy\n", got_frag_no);

                // Check if final fragment. Draw the frame.
                if (got_frag_no == ir_max_frag_no) {
                    // TODO: Update

                    //debug
                    //printf("%02X Frag: Draw -------\n", got_frag_no);

                    // Stats/IR header parsing
                    // buf_reply[53]: Average Intensity. 0-255 scale.
                    // buf_reply[54]: Unknown. Shows up only when EXFilter is enabled.
                    // *(u16*)&buf_reply[55]: White pixels (pixels with 255 value). Max 65535. Uint16 constraints, even though max is 76800.
                    // *(u16*)&buf_reply[57]: Pixels with ambient noise from external light sources (sun, lighter, IR remotes, etc). Cleaned by External Light Filter.
                    noise_level = (float)(*(u16*)&buf_reply[57]) / ((float)(*(u16*)&buf_reply[55]) + 1.0f);
                    white_pixels_percent = (int)((*(u16*)&buf_reply[55] * 100) / max_pixels);
                    avg_intensity_percent = (int)((buf_reply[53] * 100) / 255);

                    if (initialization)
                        initialization--;
                }
            }
            // Repeat/Missed fragment
            else if (got_frag_no  || previous_frag_no) {
                // Check if repeat ACK should be send. Avoid writing to image buffer.
                if (got_frag_no == previous_frag_no) {
                    //debug
                    //printf("%02X Frag: Repeat\n", got_frag_no);

                    // ACK for fragment
                    hdr->timer = timming_byte & 0xF;
                    timming_byte++;
                    buf[14] = got_frag_no;
                    buf[47] = mcu_crc8(buf + 11, 36);
                    hid_write(handle, buf, sizeof(buf));

                    missed_packet = false;
                }
                // Check if missed fragment and request it.
                else if(missed_packet_no != got_frag_no && !missed_packet) {
                    if (ir_max_frag_no != 0x03) {
                        //debug
                        //printf("%02X Frag: Missed %02X, Prev: %02X, PrevM: %02X\n", got_frag_no, previous_frag_no + 1, previous_frag_no, missed_packet_no);

                        // Missed packet
                        hdr->timer = timming_byte & 0xF;
                        timming_byte++;
                        //Request for missed packet. You send what the next fragment number will be, instead of the actual missed packet.
                        buf[12] = 0x1;
                        buf[13] = previous_frag_no + 1;
                        buf[14] = 0;
                        buf[47] = mcu_crc8(buf + 11, 36);
                        hid_write(handle, buf, sizeof(buf));

                        buf[12] = 0x00;
                        buf[13] = 0x00;

                        memcpy(buf_image + (300 * got_frag_no), buf_reply + 59, 300);

                        previous_frag_no = got_frag_no;
                        missed_packet_no = got_frag_no - 1;
                        missed_packet = true;
                    }
                    // Check if missed fragment and res is 30x40. Don't request it.
                    else {
                        //debug
                        //printf("%02X Frag: Missed but res is 30x40\n", got_frag_no);

                        // ACK for fragment
                        hdr->timer = timming_byte & 0xF;
                        timming_byte++;
                        buf[14] = got_frag_no;
                        buf[47] = mcu_crc8(buf + 11, 36);
                        hid_write(handle, buf, sizeof(buf));

                        memcpy(buf_image + (300 * got_frag_no), buf_reply + 59, 300);

                        previous_frag_no = got_frag_no;
                    }
                }
                // Got the requested missed fragments.
                else if (missed_packet_no == got_frag_no){
                    //debug
                    //printf("%02X Frag: Got missed %02X\n", got_frag_no, missed_packet_no);

                    // ACK for fragment
                    hdr->timer = timming_byte & 0xF;
                    timming_byte++;
                    buf[14] = got_frag_no;
                    buf[47] = mcu_crc8(buf + 11, 36);
                    hid_write(handle, buf, sizeof(buf));

                    memcpy(buf_image + (300 * got_frag_no), buf_reply + 59, 300);

                    previous_frag_no = got_frag_no;
                    missed_packet = false;
                }
                // Repeat of fragment that is not max fragment.
                else {
                    //debug
                    //printf("%02X Frag: RepeatWoot M:%02X\n", got_frag_no, missed_packet_no);

                    // ACK for fragment
                    hdr->timer = timming_byte & 0xF;
                    timming_byte++;
                    buf[14] = got_frag_no;
                    buf[47] = mcu_crc8(buf + 11, 36);
                    hid_write(handle, buf, sizeof(buf));
                }
                
                // Status percentage
                ir_status.str("");
                ir_status.clear();
                if (initialization < 2) {
                    if (show_status == 2)
                        ir_status << "Status: Streaming.. ";
                    else
                        ir_status << "Status: Receiving.. ";
                }
                else
                    ir_status << "Status: Initializing.. ";
                ir_status << std::setfill(' ') << std::setw(3);
                ir_status << std::fixed << std::setprecision(0) << (float)got_frag_no / (float)(ir_max_frag_no + 1) * 100.0f;
                ir_status << "% - ";

            }
            
            // Streaming start
            else {
                // ACK for fragment
                hdr->timer = timming_byte & 0xF;
                timming_byte++;
                buf[14] = got_frag_no;
                buf[47] = mcu_crc8(buf + 11, 36);
                hid_write(handle, buf, sizeof(buf));

                memcpy(buf_image + (300 * got_frag_no), buf_reply + 59, 300);

                //debug
                //printf("%02X Frag: 0 %02X\n", buf_reply[52], previous_frag_no);

                previous_frag_no = 0;
            }

        }
        // Empty IR report. Send Ack again. Otherwise, it fallbacks to high latency mode (30ms per data fragment)
        else if (buf_reply[0] == 0x31) {
            // ACK for fragment
            hdr->timer = timming_byte & 0xF;
            timming_byte++;

            // Send ACK again or request missed frag
            if (buf_reply[49] == 0xFF) {
                buf[14] = previous_frag_no;
            }
            else if (buf_reply[49] == 0x00) {
                buf[12] = 0x1;
                buf[13] = previous_frag_no + 1;
                buf[14] = 0;
               // printf("%02X Mode: Missed next packet %02X\n", buf_reply[49], previous_frag_no + 1);
            }

            buf[47] = mcu_crc8(buf + 11, 36);
            hid_write(handle, buf, sizeof(buf));

            buf[12] = 0x00;
            buf[13] = 0x00;
        }
    }
    
    delete[] buf_image;
*/
    return 0;
}


int ir_config_sensor(struct IRImageConfig ir_cfg) {
    uint8_t buf[64];
    uint8_t buf_read[370];

    memset(buf, 0, sizeof(buf));
    struct Header *hdr = (struct Header *)buf;
    struct SubcommandBody *pkt = (struct SubcommandBody *)(hdr + 1);
    hdr->command = Subcommand;
    hdr->counter = timming_byte & 0xF;
    timming_byte++;
    pkt->subcommand = SetMCUConf;
    struct MCUConfiguration *conf = (struct MCUConfiguration *)&pkt->args;
    conf->command.command = 0x23;
    conf->command.subcommand = 0x04;
    conf->registers.number = 0x09;
    conf->registers.regs[0] = joycon_ir_resolution_encode(ir_cfg.ir_res_reg);
    conf->registers.regs[1] = joycon_ir_exposure_time_high_encode(ir_cfg.ir_exposure);
    conf->registers.regs[2] = joycon_ir_exposure_time_low_encode(ir_cfg.ir_exposure);
    conf->registers.regs[3] = joycon_ir_enable_max_exposure_time_encode(0);
    conf->registers.regs[4] = joycon_ir_led_group_state_encode(ir_cfg.ir_leds);
    conf->registers.regs[5] = joycon_ir_digital_gain_high_encode(ir_cfg.ir_digital_gain);
    conf->registers.regs[6] = joycon_ir_digital_gain_low_encode(ir_cfg.ir_digital_gain);
    conf->registers.regs[7] = joycon_ir_external_light_filter_encode(ir_cfg.ir_ex_light_filter);
    conf->registers.regs[8] = joycon_ir_white_pixel_threshold_encode(0xc8);

    buf[48] = mcu_crc8(buf + 12, 36);
    hid_write(handle, buf, sizeof(buf));

    hdr->command = Subcommand;
    hdr->counter = timming_byte & 0xF;
    timming_byte++;
    pkt->subcommand = SetMCUConf;
    conf->command.command = 0x23;
    conf->command.subcommand = 0x04;
    conf->registers.number = 0x08;
    conf->registers.regs[0] = joycon_ir_led_intensity_high_encode(ir_cfg.ir_leds_intensity);
    conf->registers.regs[1] = joycon_ir_led_intensity_low_encode(ir_cfg.ir_leds_intensity);
    conf->registers.regs[2] = joycon_ir_flip_encode(ir_cfg.ir_flip);
    conf->registers.regs[3] = joycon_ir_enable_denoise_smooth_encode(ir_cfg.ir_denoise_smooth_enabled);
    conf->registers.regs[4] = joycon_ir_denoise_smooth_threshold_encode(ir_cfg.ir_denoise_threshold);
    conf->registers.regs[5] = joycon_ir_color_interpol_threshold_encode(ir_cfg.ir_color_interpol_threshold);
    conf->registers.regs[6] = joycon_ir_buffer_update_time_low_encode(0xFF & ir_cfg.ir_buffer_update_time);
    conf->registers.regs[7] = joycon_ir_finalize_config_encode();

    buf[48] = mcu_crc8(buf + 12, 36);
    hid_write(handle, buf, sizeof(buf));

    return 0;
}

void ir_reset() {
    uint8_t buf[64];
    memset(buf, 0, sizeof(buf));
    struct Header *hdr = (struct Header *)buf;
    struct SubcommandBody *pkt = (struct SubcommandBody *)(hdr + 1);
    hdr->command = Subcommand;
    hdr->counter = timming_byte & 0xF;
    timming_byte++;
    pkt->subcommand = SetInputReportMode;
    pkt->args.arg1 = ButtonTrigged;
    hid_write(handle, buf, sizeof(buf));
}

int main()
{
    int res;
    int retry;
    check_connection_ok = 1;
    if (!device_connection()) {
        fprintf(stderr, "No connected device\n");
        return 1;
    }
    fprintf(stderr, "Connected device\n");

    uint8_t buf[64];
    uint8_t buf_read[370];
    memset(buf, 0, sizeof(buf));
    struct Header *hdr = (struct Header *)buf;
    struct SubcommandBody *pkt = (struct SubcommandBody *)(hdr + 1);
    hdr->command = Subcommand;
    hdr->counter = timming_byte & 0xF;
    timming_byte++;
    pkt->subcommand = SetInputReportMode;
    pkt->args.arg1 = MCUIR;
    hid_write(handle, buf, sizeof(buf));

    retry = 0;
    while (retry < 8)
    {
        res = hid_read_timeout(handle, buf_read, sizeof(buf_read), 64);
        struct InputReportHeader *header = (struct InputReportHeader *)buf_read;
        if (header->id == SIRWithSubcommand)
        {
            struct InputReportSubcommandReply *reply = (struct InputReportSubcommandReply *)(header + 1);
            fprintf(stderr, "Ack:\t0x%02X\n", reply->ack);
            fprintf(stderr, "SCID:\t0x%02X\n", reply->replied_subcommmand_id);
            if (reply->replied_subcommmand_id == SetInputReportMode)
            {
                // It's OK :)
                break;
            }
        }
        retry++;
        if (retry >= 8)
        {
            ir_reset();
            return 1;
        }
    }

    // Enable MCU
    hdr->command = Subcommand;
    hdr->counter = timming_byte & 0xF;
    timming_byte++;
    pkt->subcommand = SetMCUState;
    pkt->args.arg1 = MCU_RESUME;
    hid_write(handle, buf, sizeof(buf));

    retry = 0;
    while (retry < 8)
    {
        res = hid_read_timeout(handle, buf_read, sizeof(buf_read), 64);
        struct InputReportHeader *header = (struct InputReportHeader *)buf_read;
        if (header->id == SIRWithSubcommand)
        {
            struct InputReportSubcommandReply *reply = (struct InputReportSubcommandReply *)(header + 1);
            fprintf(stderr, "Ack:\t0x%02X\n", reply->ack);
            fprintf(stderr, "SCID:\t0x%02X\n", reply->replied_subcommmand_id);
            if (reply->replied_subcommmand_id == SetMCUState)
            {
                // It's OK :)
                break;
            }
        }
        retry++;
        if (retry >= 8)
        {
            ir_reset();
            return 1;
        }
    }

    // Set MCU mode
    hdr->command = Subcommand;
    hdr->counter = timming_byte & 0xF;
    timming_byte++;
    pkt->subcommand = SetMCUConf;
    struct MCUConfiguration *conf = (struct MCUConfiguration *)&pkt->args;
    conf->command.command = 0x21;
    conf->command.subcommand = 0x00;
    conf->mode = MCUMode_IR;
    buf[48] = mcu_crc8(buf + 12, 36);
    hid_write(handle, buf, sizeof(buf));
    retry = 0;
    while (retry < 8)
    {
        res = hid_read_timeout(handle, buf_read, sizeof(buf_read), 64);
        struct InputReportHeader *header = (struct InputReportHeader *)buf_read;
        if (header->id == SIRWithSubcommand)
        {
            struct InputReportSubcommandReply *reply = (struct InputReportSubcommandReply *)(header + 1);
            fprintf(stderr, "Ack:\t0x%02X\n", reply->ack);
            fprintf(stderr, "SCID:\t0x%02X\n", reply->replied_subcommmand_id);
            if (reply->replied_subcommmand_id == SetMCUConf)
            {
                // It's OK :)
                break;
            }
        }
        retry++;
        if (retry >= 8)
        {
            ir_reset();
            return 1;
        }
    }

    // Set IR mode
    hdr->command = Subcommand;
    hdr->counter = timming_byte & 0xF;
    timming_byte++;
    pkt->subcommand = SetMCUConf;
    struct MCUConfiguration *conf = (struct MCUConfiguration *)&pkt->args;
    conf->command.command = 0x23;
    conf->command.subcommand = 0x01;
    conf->ir_mode.mode 0x07;
    conf->ir_mode.number = IR_RESOLUTION_FULL_NUM_FRAG;
    conf->ir_mode.major_version.high = 0x00;
    conf->ir_mode.major_version.low = 0x05;
    conf->ir_mode.minor_version.high = 0x00;
    conf->ir_mode.minor_version.low = 0x18;
    buf[48] = mcu_crc8(buf + 12, 36);
    hid_write(handle, buf, sizeof(buf));
    retry = 0;
    while (retry < 8)
    {
        res = hid_read_timeout(handle, buf_read, sizeof(buf_read), 64);
        struct InputReportHeader *header = (struct InputReportHeader *)buf_read;
        if (header->id == SIRWithSubcommand)
        {
            struct InputReportSubcommandReply *reply = (struct InputReportSubcommandReply *)(header + 1);
            fprintf(stderr, "Ack:\t0x%02X\n", reply->ack);
            fprintf(stderr, "SCID:\t0x%02X\n", reply->replied_subcommmand_id);
            if (reply->replied_subcommmand_id == SetMCUConf)
            {
                // It's OK :)
                break;
            }
        }
        retry++;
        if (retry >= 8)
        {
            ir_reset();
            return 1;
        }
    }

    struct IRImageConfig config;
    config.ir_res_reg = FULL;
    config.ir_flip = 0;
    config.ir_leds_intensity = 0x0000;
    config.ir_denoise_smooth_enabled = 0x01;
    config.ir_denoise_threshold = 0x23;
    config.ir_color_interpol_threshold = 0x44;
    config.ir_buffer_update_time = 0x32;
    config.ir_exposure = 0x4920;
    config.ir_leds = 0x00;
    config.ir_ex_light_filter = 0x00;
    config.ir_digital_gain = 0x0000;

    res = ir_config_sensor(config);
    if (res == 0) {
        get_raw_ir_image(1);
    }

    ir_reset();

    return 0;
}
