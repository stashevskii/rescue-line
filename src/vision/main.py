import sensor
import time
import math
import pyb

# The hardware I2C bus for your OpenMV Cam is always I2C bus 2.
uart = pyb.UART(3, 19200, timeout_char=200)
# GRAYSCALE_THRESHOLD = [(0, 30)]

GRAYSCALE_THRESHOLD = [(0, 25)]
ROIS = [
    (0, 80, 160, 20, 0.7),
    (0, 40, 160, 20, 0.3),
    (0, 5, 160, 20, 0.3),
]


# Compute the weight divisor (we're computing this so you don't have to make weights add to 1).
weight_sum = 0
for r in ROIS:
    weight_sum += r[4]  # r[4] is the roi weight.

# Camera setup...
sensor.reset()  # Initialize the camera sensor.
sensor.set_pixformat(sensor.RGB565)  # use grayscale.
sensor.set_framesize(sensor.QQVGA)  # use QQVGA for speed.
sensor.skip_frames(time=2000)  # Let new settings take affect.
sensor.set_auto_gain(False)  # must be turned off for color tracking
sensor.set_auto_whitebal(False)  # must be turned off for color tracking
sensor.set_vflip(True)
sensor.set_windowing((25, 25, 150, 150))
clock = time.clock()  # Tracks FPS.

while True:
    pyb.LED(2).on()
    clock.tick()  # Track elapsed milliseconds between snapshots().
    img = sensor.snapshot()  # Take a picture and return the image.

    centroid_sum = 0

    for r in ROIS:
        blobs = img.find_blobs(
            GRAYSCALE_THRESHOLD, roi=r[0:4], merge=True
        )  # r[0:4] is roi tuple.

        if blobs:
            # Find the blob with the most pixels.
            largest_blob = max(blobs, key=lambda b: b.pixels())

            # Draw a rect around the blob.
            img.draw_rectangle(largest_blob.rect())
            img.draw_cross(largest_blob.cx(), largest_blob.cy())

            centroid_sum += largest_blob.cx() * r[4]

    center_pos = centroid_sum / weight_sum
    deflection_angle = 0
    deflection_angle = -math.atan((center_pos - 80) / 60)
    deflection_angle = math.degrees(deflection_angle)
    text = str(int(deflection_angle))

    uart.write("67")
