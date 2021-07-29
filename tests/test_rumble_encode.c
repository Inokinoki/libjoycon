#include <rumble.h>
#include <assert.h>

int main()
{
    // Test frequency
    struct RumbleEncodedData test_low_freq_data1
        = joycon_rumble_encode(40.875885, 0);
    assert(test_low_freq_data1.data[0] == 0x00);
    assert((test_low_freq_data1.data[1] & HBHF_INDICATOR_MASK) == 0x00);
    assert((test_low_freq_data1.data[2] & LBLF_FREQUENCY_MASK) == 0x01);

    struct RumbleEncodedData test_low_freq_data2
        = joycon_rumble_encode(80, 0);
    assert(test_low_freq_data2.data[0] == 0x00);
    assert((test_low_freq_data2.data[1] & HBHF_INDICATOR_MASK) == 0x00);
    assert((test_low_freq_data2.data[2] & LBLF_FREQUENCY_MASK) == 0x20);

    struct RumbleEncodedData test_middle_freq_data1
        = joycon_rumble_encode(81.75177, 0);
    assert(test_middle_freq_data1.data[0] == 0x04);
    assert((test_middle_freq_data1.data[1] & HBHF_INDICATOR_MASK) == 0x00);
    assert((test_middle_freq_data1.data[2] & LBLF_FREQUENCY_MASK) == 0x21);

    struct RumbleEncodedData test_middle_freq_data2
        = joycon_rumble_encode(313.143066, 0);
    assert(test_middle_freq_data2.data[0] == 0xFC);
    assert((test_middle_freq_data2.data[1] & HBHF_INDICATOR_MASK) == 0x00);
    assert((test_middle_freq_data2.data[2] & LBLF_FREQUENCY_MASK) == 0x5F);

    struct RumbleEncodedData test_high_freq_data1
        = joycon_rumble_encode(320, 0);
    assert(test_high_freq_data1.data[0] == 0x00);
    assert((test_high_freq_data1.data[1] & HBHF_INDICATOR_MASK) == 0x01);
    assert((test_high_freq_data1.data[2] & LBLF_FREQUENCY_MASK) == 0x60);

    struct RumbleEncodedData test_high_freq_data2
        = joycon_rumble_encode(626.286133, 0);
    assert(test_high_freq_data2.data[0] == 0x7C);
    assert((test_high_freq_data2.data[1] & HBHF_INDICATOR_MASK) == 0x01);
    assert((test_high_freq_data2.data[2] & LBLF_FREQUENCY_MASK) == 0x7F);

    struct RumbleEncodedData test_hh_freq_data1
        = joycon_rumble_encode(640, 0);
    assert(test_hh_freq_data1.data[0] == 0x80);
    assert((test_hh_freq_data1.data[1] & HBHF_INDICATOR_MASK) == 0x01);

    struct RumbleEncodedData test_hh_freq_data2
        = joycon_rumble_encode(1252.572266, 0);
    assert(test_hh_freq_data2.data[0] == 0xFC);
    assert((test_hh_freq_data2.data[1] & HBHF_INDICATOR_MASK) == 0x01);

    // Test amplititude
    struct RumbleEncodedData test_low_amp_data1
        = joycon_rumble_encode(80, 0);
    assert((test_low_amp_data1.data[1] & HBHF_AMPLITUDE_MASK) == 0x00);
    assert((test_low_amp_data1.data[2] & LBLF_INDICATOR_MASK) == 0x00);
    assert(test_low_amp_data1.data[3] == 0x40);

    // FIXME: This is not currently under processing
    // struct RumbleEncodedData test_low_amp_data2
    //     = joycon_rumble_encode(80, 0.007843);
    // assert((test_low_amp_data2.data[1] & HBHF_AMPLITUDE_MASK) == 0x02);
    // assert((test_low_amp_data2.data[2] & LBLF_INDICATOR_MASK) == 0x80);
    // assert(test_low_amp_data2.data[3] == 0x40);

    struct RumbleEncodedData test_mid_amp_data1
        = joycon_rumble_encode(80, 0.240087);
    assert((test_mid_amp_data1.data[1] & HBHF_AMPLITUDE_MASK) == 0x44);
    assert((test_mid_amp_data1.data[2] & LBLF_INDICATOR_MASK) == 0x00);
    assert(test_mid_amp_data1.data[3] == 0x51);

    struct RumbleEncodedData test_mid_amp_data2
        = joycon_rumble_encode(80, 0.245345);
    assert((test_mid_amp_data2.data[1] & HBHF_AMPLITUDE_MASK) == 0x46);
    assert((test_mid_amp_data2.data[2] & LBLF_INDICATOR_MASK) == 0x80);
    assert(test_mid_amp_data2.data[3] == 0x51);

    struct RumbleEncodedData test_high_amp_data1
        = joycon_rumble_encode(80, 0.960348);
    assert((test_high_amp_data1.data[1] & HBHF_AMPLITUDE_MASK) == 0xC4);
    assert((test_high_amp_data1.data[2] & LBLF_INDICATOR_MASK) == 0x00);
    assert(test_high_amp_data1.data[3] == 0x71);

    struct RumbleEncodedData test_high_amp_data2
        = joycon_rumble_encode(80, 0.981379);
    assert((test_high_amp_data2.data[1] & HBHF_AMPLITUDE_MASK) == 0xC6);
    assert((test_high_amp_data2.data[2] & LBLF_INDICATOR_MASK) == 0x80);
    assert(test_high_amp_data2.data[3] == 0x71);

    return 0;
}
