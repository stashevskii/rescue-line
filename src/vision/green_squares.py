import sensor
import time

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time=2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)

green_threshold = (0, 100, -128, -10, -128, 127)

clock = time.clock()

while True:
    clock.tick()
    img = sensor.snapshot()

    green_blobs = img.find_blobs([green_threshold],
                               area_threshold=45,
                               pixels_threshold=1,
                               merge=False,
                               margin=5)

    print(len(green_blobs))
    for blob in green_blobs:
        img.draw_rectangle(blob.rect(), color=(255, 0, 0), thickness=2)
