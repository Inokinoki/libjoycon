/* libjoycon.i */
%module libjoycon
%{
uint8_t *joycon_allocate_buffer(size_t size);
void joycon_free_buffer(uint8_t *buf);

extern void joycon_packet_mcu_read_ack_encode(uint8_t *buffer, uint8_t timer, uint8_t index);
extern void joycon_packet_mcu_read_req_encode(uint8_t *buffer, uint8_t timer, uint8_t index);

extern void joycon_packet_mcu_read_status_encode(uint8_t *buf, uint8_t timer);

extern void joycon_packet_mcu_enable(uint8_t *buf, uint8_t timer);
extern void joycon_packet_mcu_disable(uint8_t *buf, uint8_t timer);

extern void joycon_packet_mcu_conf_mode(uint8_t *buffer, uint8_t timer, uint8_t mode);
extern void joycon_packet_mcu_conf_ir_mode(uint8_t *buffer, uint8_t timer, uint8_t mode, uint8_t number);
extern void joycon_packet_mcu_conf_registers(uint8_t *buffer, uint8_t timer, uint8_t number,
    uint16_t *addrs, uint8_t *vals);

// Encode rumble into packet
extern void joycon_packet_rumble_only(uint8_t *buffer, uint8_t timer, float freq, float amp);
extern void joycon_packet_append_rumble(uint8_t *buffer, float freq, float amp);

// Enable or disable rumble
extern void joycon_packet_rumble_enable(uint8_t *buffer, uint8_t timer, float freq, float amp);
extern void joycon_packet_rumble_disable(uint8_t *buffer, uint8_t timer, float freq, float amp);
extern void joycon_packet_rumble_enable_only(uint8_t *buffer, uint8_t timer);
extern void joycon_packet_rumble_disable_only(uint8_t *buffer, uint8_t timer);

// Encode SPI reading data
extern void joycon_packet_read_spi_sn(uint8_t *buffer, uint8_t timer);
extern void joycon_packet_read_spi_body_color(uint8_t *buffer, uint8_t timer);
extern void joycon_packet_read_spi_btn_color(uint8_t *buffer, uint8_t timer);
extern void joycon_packet_read_spi_color(uint8_t *buffer, uint8_t timer);

extern void joycon_packet_player_light_enable(uint8_t *buf, uint8_t timer, uint8_t light);
extern void joycon_packet_player_light_disable(uint8_t *buf, uint8_t timer);

extern void joycon_packet_imu_enable(uint8_t *buf, uint8_t timer);
extern void joycon_packet_imu_disable(uint8_t *buf, uint8_t timer);

extern void joycon_packet_input_report_mode(uint8_t *buf, uint8_t timer, uint8_t mode);

enum IMUSensitivity {
    Sensitivity0 = 0x00,
    Sensitivity1,
    Sensitivity2,
    Sensitivity3
};
extern float joycon_gyro_decode(int16_t data, enum IMUSensitivity sensitivity);
extern float joycon_accel_decode(int16_t data, enum IMUSensitivity sensitivity);
%}

%include "stdint.i"
%include "carrays.i"
%array_functions(uint8_t,byteArray)

%inline %{
    uint8_t *joycon_allocate_buffer(size_t size)
    {
        uint8_t *buf = malloc(size);
        memset(buf, 0, size);
        return buf;
    }
    void joycon_free_buffer(uint8_t *buf) { free(buf); }
%}

extern void joycon_packet_mcu_read_ack_encode(uint8_t *buffer, uint8_t timer, uint8_t index);
extern void joycon_packet_mcu_read_req_encode(uint8_t *buffer, uint8_t timer, uint8_t index);

extern void joycon_packet_mcu_read_status_encode(uint8_t *buf, uint8_t timer);

extern void joycon_packet_mcu_enable(uint8_t *buf, uint8_t timer);
extern void joycon_packet_mcu_disable(uint8_t *buf, uint8_t timer);

extern void joycon_packet_mcu_conf_mode(uint8_t *buffer, uint8_t timer, uint8_t mode);
extern void joycon_packet_mcu_conf_ir_mode(uint8_t *buffer, uint8_t timer, uint8_t mode, uint8_t number);
extern void joycon_packet_mcu_conf_registers(uint8_t *buffer, uint8_t timer, uint8_t number,
    uint16_t *addrs, uint8_t *vals);

// Encode rumble into packet
extern void joycon_packet_rumble_only(uint8_t *buffer, uint8_t timer, float freq, float amp);
extern void joycon_packet_append_rumble(uint8_t *buffer, float freq, float amp);

// Enable or disable rumble
extern void joycon_packet_rumble_enable(uint8_t *buffer, uint8_t timer, float freq, float amp);
extern void joycon_packet_rumble_disable(uint8_t *buffer, uint8_t timer, float freq, float amp);
extern void joycon_packet_rumble_enable_only(uint8_t *buffer, uint8_t timer);
extern void joycon_packet_rumble_disable_only(uint8_t *buffer, uint8_t timer);

// Encode SPI reading data
extern void joycon_packet_read_spi_sn(uint8_t *buffer, uint8_t timer);
extern void joycon_packet_read_spi_body_color(uint8_t *buffer, uint8_t timer);
extern void joycon_packet_read_spi_btn_color(uint8_t *buffer, uint8_t timer);
extern void joycon_packet_read_spi_color(uint8_t *buffer, uint8_t timer);

extern void joycon_packet_player_light_enable(uint8_t *buf, uint8_t timer, uint8_t light);
extern void joycon_packet_player_light_disable(uint8_t *buf, uint8_t timer);

extern void joycon_packet_imu_enable(uint8_t *buf, uint8_t timer);
extern void joycon_packet_imu_disable(uint8_t *buf, uint8_t timer);

extern void joycon_packet_input_report_mode(uint8_t *buf, uint8_t timer, uint8_t mode);

enum IMUSensitivity {
    Sensitivity0 = 0x00,
    Sensitivity1,
    Sensitivity2,
    Sensitivity3
};
extern float joycon_gyro_decode(int16_t data, enum IMUSensitivity sensitivity);
extern float joycon_accel_decode(int16_t data, enum IMUSensitivity sensitivity);
