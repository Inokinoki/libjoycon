/* libjoycon.i */
%module libjoycon
%{
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
