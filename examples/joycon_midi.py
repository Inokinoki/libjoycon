from mido import MidiFile, tick2second, tempo2bpm
import time
import ctypes

import hid
import libjoycon

buffer_len = 64

def buf2list(buf, size):
    l = [0] * size
    for i in range(size):
        l[i] = libjoycon.byteArray_getitem(buf, i)
    return l

class JoyconNoteOutput:
    FREQ_OFFSET = 36
    freqs = [
        # C2 is 36
        65.41, 69.30, 73.42, 77.78, 82.41, 87.31, 92.50, 98.00, 103.83, 110.00, 116.54, 123.47,
        130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185.00, 196.00, 207.65, 220.00, 233.08, 246.94,
        261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.00, 415.30, 440.00, 466.16, 493.88,
        523.25, 554.37, 587.33, 622.25, 659.25, 698.46, 739.99, 783.99, 830.61, 880.00, 932.33, 987.77
        # C6 is 84
    ]

    def __init__(self, device_id):
        self._timer = 0
        self._is_busy = False
        self._device_id = device_id
        self._buf = None

        buf = libjoycon.joycon_allocate_buffer(buffer_len)

        # Init handler
        try:
            print(f"Opening the device {device_id}")

            h = hid.device()
            h.open(0x57e, device_id, None)
            print("Open device")

            libjoycon.joycon_packet_rumble_enable_only(buf, self._timer & 0xF)
            h.write(buf2list(buf, buffer_len))

            # enable non-blocking mode
            h.set_nonblocking(1)

            self._handler = h
            self._buf = buf
        except IOError as ex:
            self._handler = None
            libjoycon.joycon_free_buffer(buf)

    def is_busy(self):
        return self._is_busy
    
    def is_connected(self):
        return self._handler is not None

    def note_off(self, note):
        if self._buf:
            try:
                self._timer += 1
                libjoycon.joycon_packet_rumble_only(self._buf, self._timer & 0xF,\
                    JoyconNoteOutput.freqs[note - JoyconNoteOutput.FREQ_OFFSET], 0)
                self._handler.write(buf2list(self._buf, buffer_len))
            except IOError as ex:
                print(ex)
        self._is_busy = False
        print(f"\tNote off {note}")

    def note_on(self, note, amp=0.5):
        if self._buf and (note - JoyconNoteOutput.FREQ_OFFSET) < len(JoyconNoteOutput.freqs):
            try:
                self._timer += 1
                libjoycon.joycon_packet_rumble_only(self._buf, self._timer & 0xF,\
                    JoyconNoteOutput.freqs[note - JoyconNoteOutput.FREQ_OFFSET], amp)
                self._handler.write(buf2list(self._buf, buffer_len))
            except IOError as ex:
                print(ex)
        self._is_busy = True
        print(f"\tNote on {note}")
    
    def __del__(self):
        if self._handler is not None:
            try:
                self._timer += 1
                buf = self._buf
                libjoycon.joycon_packet_rumble_disable(buf, self._timer & 0xF, 0, 0)
                self._handler.write(buf2list(buf, buffer_len))

                print("Closing the device")
                self._handler.close()
                self._handler = None
            except IOError as ex:
                pass
            finally:
                libjoycon.joycon_free_buffer(self._buf)

# Add multiple joycon connections
controllers = []
for i in [0x2009, 0x2006, 0x2007]:
    c = JoyconNoteOutput(i)
    if c.is_connected():
        controllers.append(c)

mid = MidiFile('twinkle-twinkle-little-star.mid', clip=True)

print(len(mid.tracks), mid.length, mid.ticks_per_beat, mid.type)
max_passed_time = 0.0

notes_mapping = {}  # Map a note_on to a joycon

# Init tempo
tempo = 500000
for i, track in enumerate(mid.tracks):
    print('Track {}: {}'.format(i, track.name))
    print(len(track))

    begin_time = time.time() * 1000.0
    passed_time = 0.0
    is_main_track = False
    for msg in track:
        if msg.type == "end_of_track":
            break
        if msg.time != 0:
            sleep_time = tick2second(tick=msg.time, ticks_per_beat=mid.ticks_per_beat, tempo=tempo)
            print("Sleeping {} at {}".format(sleep_time, passed_time))
            time.sleep(sleep_time)
            passed_time += sleep_time
            # TODO: the time error can be fixed here
        if msg.is_meta and msg.type == "set_tempo":
            tempo = msg.tempo   # Set tempo
            print("BMP set: {}".format(tempo2bpm(msg.tempo)))
        elif msg.type == "note_on":
            # print(msg)
            # Dispatch notes to joycon(s)
            if msg.channel == 0 and msg.note not in notes_mapping.keys():
                # Find a usable joycon
                allocated_controller = None
                for controller in controllers:
                    if controller.is_busy():
                        continue
                    allocated_controller = controller
                    notes_mapping.update({msg.note: controller})
                    controller.note_on(msg.note)    # Send the note
                    break
                if not allocated_controller:
                    print(f"No usable controller to play note {msg.note}")
        elif msg.type == "note_off":
            if msg.channel == 0 and msg.note in notes_mapping.keys():
                # Send stop signal and remove
                controller = notes_mapping.pop(msg.note)
                controller.note_off(msg.note)
    max_passed_time = max(max_passed_time, passed_time)

# We finally wait for the end of file
time.sleep(max(0, mid.length - max_passed_time))
