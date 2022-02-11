import time
import ctypes

import hid
import libjoycon

# Init buffer and timer
buffer_len = 64
buf = libjoycon.joycon_allocate_buffer(buffer_len)
timer = 0

def buf2list(buf, size):
    l = [0] * size
    for i in range(size):
        l[i] = libjoycon.byteArray_getitem(buf, i)
    return l

rhythm = 65

freqs = [
    123.471,
    # C
    130.813, 146.832, 164.814, 174.614, 195.998, 220.000, 246.942,
    261.626, 293.665, 329.629, 349.228, 391.995, 440.000, 493.883,
    523.251, 587.330, 659.255, 698.456, 783.991, 880.000, 987.767
]
print(freqs)

notes     = [9,   7,   8,   6,   9,   7,   8,   6] * 5
durations = [1/2, 1/2, 1/2, 1/2, 1/2, 1/2, 1/2, 1/2] * 5

notes.extend([10, 0, 8, 9, 8,                   10, 0, 8, 9, 8, 9, 10])
durations.extend([2, 1.25, 0.25, 0.25, 0.25,    2, 0.75, 0.25, 0.25, 0.25, 0.25, 0.25])
# 都是勇敢的 你额头的伤
notes.extend([6, 8, 6, 8, 6, 8, 9, 8,                               7, 0])
durations.extend([0.75, 0.25, 0.75, 0.25, 0.75, 0.25, 0.5, 0.5,     2, 2])
# 口你的不同你犯的错

notes.extend([10, 0, 8, 9, 8,                   10, 0, 8, 9, 8, 9, 10])
durations.extend([2, 1.25, 0.25, 0.25, 0.25,    2, 0.75, 0.25, 0.25, 0.25, 0.25, 0.25])
# 都不必隐藏 你破旧的玩
notes.extend([6, 8, 6, 8, 6, 8, 9, 8,                               7, 0])
durations.extend([0.75, 0.25, 0.75, 0.25, 0.75, 0.25, 0.5, 0.5,     2, 2])
# 偶你的面具你的自我

notes.extend([       6,    8,   13,   13,   13,   12,  13,   13,   12,   13,   12,   13,   12,   10,   10,   0,    6,    8])
durations.extend([0.25, 0.25, 0.75, 0.25, 0.25, 0.25, 0.5, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 1.75, 1.5, 0.25, 0.25])
# 他们说要带着光驯服每一头怪兽 他们

notes.extend([13, 13, 13, 12, 13, 12, 14, 14, 14, 13, 14])
durations.extend([0.75, 0.25, 0.25, 0.25, 0.25, 0.25, 0.75, 0.25, 0.25, 0.25, 0.5])
# 说要缝好你的伤没有人爱

notes.extend([14, 13, 10, 0, 10, 12, 10])
durations.extend([0.25, 0.5, 2.25, 0.25, 0.25, 0.25, 0.25])
# 小丑 为何孤

notes.extend([       9,   10,    9,   10,    9,   10,   12,   10,    12,   10])
durations.extend([0.75, 0.25, 0.75, 0.25, 0.75, 0.25, 0.25,  0.25, 0.25, 0.25])
# 独不可光荣人只有不完

notes.extend([       9,   10,    9,   10, 9,   0,    8,    9])
durations.extend([0.75, 0.25, 0.75, 0.25, 1, 0.5, 0.25, 0.25])
# 美值得歌颂 谁说

notes.extend([     10,   6,   8,  10,    9,   10,    9,    8,   8])
durations.extend([0.5, 0.5, 0.5, 0.5, 0.75, 0.25, 0.25, 0.25, 0.5])
# 污泥满身的不算英

notes.extend([6, 0, 13, 14])
durations.extend([2, 1.5, 0.25, 0.25])
# 雄 爱你

notes.extend([15, 16, 14, 15, 15, 15, 14, 15, 16, 14, 15, 15, 15, 16])
durations.extend([0.25, 0.25, 0.25, 0.25, 0.5, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.5, 0.25, 0.25])
# 孤身走暗巷 爱你不跪得模样 爱你

notes.extend([      17,   16,   17,   16,  17,   17,   16,  17,  19,   17,  13,   14])
durations.extend([0.25, 0.25, 0.25, 0.25, 0.5, 0.25, 0.25, 0.5, 0.5, 0.5, 0.25, 0.25])
# 对峙过绝望不肯哭一场 爱你

notes.extend([15, 16, 14, 15, 15, 15, 14, 15, 16, 14, 15, 15, 15, 16])
durations.extend([0.25, 0.25, 0.25, 0.25, 0.5, 0.25, 0.25, 0.25, 0.25, 0.25, 0.25, 0.5, 0.25, 0.25])
# 破烂的衣裳却敢堵命运的枪 爱你

notes.extend([      17,   16,   17,   16,  17,   17,   16,  17,  19,  17, 19])
durations.extend([0.25, 0.25, 0.25, 0.25, 0.5, 0.25, 0.25, 0.5, 0.5, 0.5, 0.5])
# 和我那么像缺口都一样 去

notes.extend([17, 19, 17, 19, 17, 19, 20, 17, 19, 19])
durations.extend([0.75, 0.25, 0.75, 0.25, 0.25, 0.25, 0.25, 0.25, 0.5, 0.5])
# 吗 配吗 这褴褛的披风 战

notes.extend([      17,   19,   17,   19,   17,   19,   20,   17,  19,   19,   19])
durations.extend([0.75, 0.25, 0.75, 0.25, 0.25, 0.25, 0.25, 0.25, 0.5, 0.25, 0.25])
# 吗 战啊 以最卑微的梦致那

notes.extend([      17,   16,  16,  16,  15,   17,  16,  16,   15,   15])
durations.extend([0.25, 0.25, 0.5, 0.5, 0.5, 0.25, 0.5, 0.5, 0.25, 0.25])
# 黑夜中的呜鸣与怒吼

notes.extend([    13,   0,   19,   19,   17,   16,  16,  16,   15,  17,   16,  16,  16,   15,   15, 13])
durations.extend([2, 1.25, 0.25, 0.25, 0.25, 0.25, 0.5, 0.5, 0.25, 0.5, 0.25, 0.5, 0.5, 0.25, 0.25, 2])
# 谁说站在光里的才算英雄

if len(notes) != len(durations):
    print("Error length", len(notes), len(durations))
    exit(1)

try:
    print("Opening the device")

    h = hid.device()
    h.open(0x57e, 0x2007, None)
    print("Open device")
    # enable non-blocking mode
    h.set_nonblocking(1)

    libjoycon.joycon_packet_rumble_enable_only(buf, timer & 0xF)
    h.write(buf2list(buf, buffer_len))
    for note, duration in zip(notes, durations):
        timer += 1
        if note == 0:
            libjoycon.joycon_packet_rumble_only(buf, timer & 0xF, 0, 0)
        else:
            libjoycon.joycon_packet_rumble_only(buf, timer & 0xF, freqs[note], 1 if note > 14 else 0.5)
        h.write(buf2list(buf, buffer_len))
        time.sleep(60.0 / rhythm * duration)
    timer += 1
    libjoycon.joycon_packet_rumble_disable(buf, timer & 0xF, 0, 0)
    h.write(buf2list(buf, buffer_len))

    print("Closing the device")
    h.close()

except IOError as ex:
    print(ex)
finally:
    libjoycon.joycon_free_buffer(buf)

print("Done")
