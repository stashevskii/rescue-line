import sensor
import time
import math
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
    lineBlobX = 0

    green_blobs = img.find_blobs([green_threshold],
        area_threshold=60,
        pixels_threshold=40,
        merge=False,
        margin=1
    )

    total_green_area = 0
    green_pixels_count = 0
    good_blobs = 0
    greenBlobsPos = 0

    for blob in green_blobs:
        if blob.y() > 40:
            good_blobs += 1
            if blob.x() < lineBlobX:
                greenBlobsPos += 1
            if blob.x() > lineBlobX:
                greenBlobsPos += 2
        total_green_area += blob.area()
        green_pixels_count += blob.pixels()
        #img.draw_rectangle(blob.rect(), color=(255, 0, 0), thickness=1)

    centroid_sum = 0
    cross = False
    a = 0
    for r in ROIS:
        blobs = img.find_blobs(GRAYSCALE_THRESHOLD, roi=r[0:4], merge=True)
        if blobs:
            largest_blob = max(blobs, key=lambda b: b.pixels())
            #img.draw_rectangle(largest_blob.rect())
            #img.draw_cross(largest_blob.cx(), largest_blob.cy())
            if (a == 1 and largest_blob.pixels() >= 400):
                cross = True
            lineBlobX = largest_blob.cx()
            centroid_sum += largest_blob.cx() * r[4]
        a += 1

    center_pos = centroid_sum / weight_sum
    deflection_angle = -math.atan((center_pos - 80) / 60)
    deflection_angle = math.degrees(deflection_angle)

    ROI_top = ROIS[2]
    ROI_bottom = ROIS[0]

    centerY_top = ROI_top[1] + ROI_top[3]//2
    centerY_bottom = ROI_bottom[1] + ROI_bottom[3]//2

    centerX_top = 80
    centerX_bottom = 80

    green_positions = []

    l, r = False, False

    for blob in green_blobs:
        relY = blob.cy() - centerY_top
        relX = blob.cx() - centerX_bottom
        green_positions.append((relX, relY))
        if (relX < 0):
            if (relY > 30): l = True
            else: r = True
    resp = 0
    if cross:
        if (l and r): resp = 1
        elif (l): resp = 2
        else: resp = 3
    uart.write(str(resp) + '\n')

    """img.draw_string(5, 25, f"Blobs: {len(green_blobs)}", color=(0,255,0))
    img.draw_string(5, 5, f"good_blobs: {good_blobs}", color=(255,0,0))
    img.draw_string(5, 50, f"greenBlobsPos:{greenBlobsPos}", color=(255,255,0))
    img.draw_string(lineBlobX, 100, f"X:{lineBlobX}", color=(0, 0, 255))"""
