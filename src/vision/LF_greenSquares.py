import sensor
import time
import math
import pyb
import struct

# The hardware I2C bus for your OpenMV Cam is always I2C bus 2.
# i2c = pyb.I2C(4, pyb.I2C.SLAVE, addr=0x12)
# i2c.deinit()  # Fully reset I2C device...
# i2c = pyb.I2C(4, pyb.I2C.SLAVE, addr=0x12)
# print("Waiting for Arduino...")

GRAYSCALE_THRESHOLD = [(0, 30)]

ROIS = [
    (0, 100, 160, 20, 0.7),
    (0, 50, 160, 20, 0.3),
    (0, 10, 160, 20, 0.3),
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
sensor.set_hmirror(True)  # Горизонтальное зеркальное отображение
sensor.set_vflip(True)    # Вертикальное отображение (переворот)

green_threshold = (0, 100, -128, -10, -128, 127)
clock = time.clock()  # Tracks FPS.

while True:
    clock.tick()  # Track elapsed milliseconds between snapshots().
    img = sensor.snapshot()  # Take a picture and return the image.

    centroid_sum = 0
    lineBlobX = 0
    i = 0
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
            if i == 0:
                lineBlobX = largest_blob.cx()
            i+=1
            if (largest_blob.pixels() >= 400):
                print("Cross")
            else:
                print("No cross")

            centroid_sum += largest_blob.cx() * r[4]

    center_pos = centroid_sum / weight_sum

    deflection_angle = 0
    deflection_angle = -math.atan((center_pos - 80) / 60)

    deflection_angle = math.degrees(deflection_angle)

    # Находим ВСЕ зеленые пиксели
    green_blobs = img.find_blobs([green_threshold],
    area_threshold=60,  # Минимальная площадь = 1 пиксель
    pixels_threshold=40,
    merge=True,       # Не сливать области
    margin=2)          # Минимальный отступ

    total_green_area = 0
    green_pixels_count = 0
    good_blobs = 0
    greenBlobsPos = 0
    # Считаем общую площадь зеленого
    for blob in green_blobs:
        if blob.y()>40: # Пороговое значение y от верха экрана для отсеивания ненужных меток
            good_blobs += 1
            if blob.x()<lineBlobX: # Пороговое значение y от верха экрана для отсеивания ненужных меток
                greenBlobsPos += 1
            if blob.x()>lineBlobX: # Пороговое значение y от верха экрана для отсеивания ненужных меток
                greenBlobsPos += 2
        total_green_area += blob.area()
        green_pixels_count += blob.pixels()
        img.draw_rectangle(blob.rect(), color=(255, 0, 0), thickness=1)
    img.draw_string(5, 25, f"Blobs: {len(green_blobs)}", color=(0,255,0))
    img.draw_string(5, 5, f"good_blobs: {good_blobs}", color=(255,0,0))
    img.draw_string(5, 50, f"greenBlobsPos:{greenBlobsPos}", color=(255,255,0))
    img.draw_string(lineBlobX, 100, f"X:{lineBlobX}", color=(0, 0, 255))

    # text = str(int(deflection_angle))
    # data = struct.pack("<%ds" % len(text), text)

    # try:
    #     i2c.send(
    #         struct.pack("<h", len(data)), timeout=10000
    #     )
    #     try:
    #         i2c.send(data, timeout=10000)
    #         print("Sent Data!")
    #     except OSError:
    #         pass
    # except OSError:
    #    pass
