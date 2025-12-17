import sensor
import time
import pyb

uart = pyb.UART(3, 115200)
uart.init(115200, bits=8, parity=None, stop=1)

GRAYSCALE_THRESHOLD = [(0, 25)]

ROIS = [
    (0, 80, 160, 20, 0.7),
    (0, 40, 160, 20, 0.3),
    (0, 5, 160, 20, 0.3),
]

weight_sum = 10
for r in ROIS:
    weight_sum += r[4]

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time=2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
sensor.set_hmirror(True)
sensor.set_vflip(True)
sensor.set_windowing((25, 25, 150, 150))

green_threshold = (0, 100, -128, -10, -128, 127)
clock = time.clock()

while True:
    clock.tick()
    img = sensor.snapshot()

    green_blobs = img.find_blobs([green_threshold],
        area_threshold=60,
        pixels_threshold=40,
        merge=False,
        margin=1
    )

    for blob in green_blobs:
        img.draw_rectangle(blob.rect(), color=(255, 0, 0), thickness=1)

    cross = False

    low_blob_x, mid_blob_y = 0, 0
    for i in range(len(ROIS)):
        r = ROIS[i]
        blobs = img.find_blobs(GRAYSCALE_THRESHOLD, roi=r[0:4], merge=True)
        if blobs:
            largest = max(blobs, key=lambda b: b.pixels())
            img.draw_rectangle(largest.rect())
            img.draw_cross(largest.cx(), largest.cy())
            if i == 0:
                low_blob_x = largest.cx()
            elif i == 1 and largest.pixels() >= 400:
                mid_blob_y = largest.cy()
                cross = True

    l, r = False, False
    for blob in green_blobs:
        relY = blob.cy() - mid_blob_y
        relX = blob.cx() - low_blob_x
        if (relY > 0):
            if (relX < 0): l = True
            else: r = True

    resp = 0
    if cross:
        if (l and r): resp = 1
        elif (l): resp = 2
        else: resp = 3
    uart.write(str(resp))
