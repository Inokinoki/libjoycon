import hid
import libjoycon
import taichi as ti

# Use another thread for painting
import _thread

ti.init(arch=ti.cpu)

n = 320
pixels = ti.field(dtype=float, shape=(n * 2, n))

def joycon_gyro_decode(data, sensitivity):
    return data * sensitivity

@ti.func
def complex_sqr(z):
    return ti.Vector([z[0]**2 - z[1]**2, z[1] * z[0] * 2])


@ti.kernel
def paint(t: float):
    for i, j in pixels:  # Parallized over all pixels
        c = ti.Vector([-0.8, ti.cos(t) * 0.2])
        z = ti.Vector([i / n - 1, j / n - 0.5]) * 2
        iterations = 0
        while z.norm() < 20 and iterations < 50:
            z = complex_sqr(z) + c
            iterations += 1
        pixels[i, j] = 1 - iterations * 0.02


gui = ti.GUI("Julia Set", res=(n * 2, n))

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

angle_x = 0.0
def thread_read(count):
    global angle_x
    counter = 0
    while counter < count:
        # Read
        resp = h.read(64)
        if resp[0] == 48:
            # Decode the frame
            for i in range(3):
                # print(int.from_bytes(resp[13 + 12 * i + 6: 13 + 12 * i + 6 + 2], byteorder='little', signed=True))
                decoded_gyro_x = 0.005 * joycon_gyro_decode(
                    int.from_bytes(resp[13 + 12 * i + 6: 13 + 12 * i + 6 + 2], byteorder='little', signed=True), 0.07)
                angle_x = angle_x + decoded_gyro_x

            # print(angle_x)
            counter += 1
        else:
            print("Received another frame")

try:
    print("Opening the device")

    h = hid.device()
    h.open(0x57e, 0x2007)

    # Enable IMU
    libjoycon.joycon_packet_imu_enable(buf, timer & 0xF)
    h.write(buf2list(buf, buffer_len))
    timer += 1

    # Use SIR Full Speed
    libjoycon.joycon_packet_input_report_mode(buf, timer & 0xF, 48)
    h.write(buf2list(buf, buffer_len))
    timer += 1

    # Mainloop
    counter = 0
    count = 600 * 10
    thread = _thread.start_new_thread(thread_read, (count, ))
    while counter < count:
        paint(angle_x * 0.03)
        gui.set_image(pixels)
        gui.show()
        counter += 1

    # Disable IMU
    libjoycon.joycon_packet_imu_disable(buf, timer & 0xF)
    h.write(buf2list(buf, buffer_len))
    timer += 1

    # Recover basic input
    libjoycon.joycon_packet_input_report_mode(buf, timer & 0xF, 63)
    h.write(buf2list(buf, buffer_len))
    timer += 1

    print("Closing the device")
    h.close()

except IOError as ex:
    print(ex)

print("Done")
