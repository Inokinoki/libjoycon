import time
import ctypes

import hid
import libjoycon

# Init buffer and timer
buffer_len = 64
buf = libjoycon.joycon_allocate_buffer(buffer_len)
timer = 0

# Helper function to get all contents from C buffer
def buf2list(buf, size):
    l = [0] * size
    for i in range(size):
        l[i] = libjoycon.byteArray_getitem(buf, i)
    return l

try:
    print("Opening the device")

    h = hid.device()
    h.open(0x57e, 0x2007, None)
    print("Open device")
    # enable non-blocking mode
    h.set_nonblocking(1)

    # joycon_packet_player_light_enable(buf, timer & 0xF, PLAYER_LIGHT_KEEP_0);
    libjoycon.joycon_packet_player_light_enable(buf, timer & 0xF, 0x1)
    h.write(buf2list(buf, buffer_len))
    timer += 1
    time.sleep(1)
    libjoycon.joycon_packet_player_light_enable(buf, timer & 0xF, 0x2)
    h.write(buf2list(buf, buffer_len))
    timer += 1
    time.sleep(1)
    libjoycon.joycon_packet_player_light_enable(buf, timer & 0xF, 0x4)
    h.write(buf2list(buf, buffer_len))
    timer += 1
    time.sleep(1)
    libjoycon.joycon_packet_player_light_enable(buf, timer & 0xF, 0x8)
    h.write(buf2list(buf, buffer_len))
    timer += 1
    time.sleep(1)
    # joycon_packet_player_light_disable(buf, timer & 0xF);
    libjoycon.joycon_packet_player_light_disable(buf, timer & 0xF)
    h.write(buf2list(buf, buffer_len))
    timer += 1
    time.sleep(1)

    print("Closing the device")
    h.close()

except IOError as ex:
    print(ex)
finally:
    libjoycon.joycon_free_buffer(buf)

print("Done")
