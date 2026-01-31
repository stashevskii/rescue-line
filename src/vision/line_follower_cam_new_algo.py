import sensor
import time
import pyb

uart = pyb.UART(3, 19200)
uart.init(19200, bits=8, parity=None, stop=1)

GRAYSCALE_THRESHOLD = [(0, 70)]

WROIS = [
    (0, 90, 160, 30,    15),
    (20, 60, 120, 30,    10),
    (0, 35,     35, 60, 7),
    (80+45, 35, 35, 60, 7),
    (30, 25, 100, 30,   1),
]
N = len(WROIS)

sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QQVGA)

sensor.skip_frames(time=500)
clock = time.clock()

while True:
    clock.tick()
    img = sensor.snapshot()

    centroids = [0] * N
    pixels = [0] * N
    weights = [0] * N
    angles = [0] * N

    for i in range(N):
        blobs = img.find_blobs(
            GRAYSCALE_THRESHOLD, roi=WROIS[i][0:4], merge=True, pixels_threshold=45
        )
        if blobs:
            largest_blob = max(blobs, key=lambda b: b.pixels())
            img.draw_line(largest_blob.major_axis_line(), 60, 3)
            img.draw_cross(largest_blob.cx(), largest_blob.cy())

            centroids[i] = (largest_blob.cx() * WROIS[i][4])
            pixels[i] = largest_blob.pixels()
            weights[i] = WROIS[i][4]
            angles[i] = largest_blob.rotation_deg()-90

    if all(pixels[2:4]):
        if pixels[3] > pixels[2]:
            weights[2]=0
            centroids[2]=0
        else:
            weights[3]=0
            centroids[3]=0

    speed = int(250-abs(angles[1])*1.7)

    if any(pixels[2:4]):
        speed = 80

    if centroids:
        center_pos = sum(centroids)/(sum(weights)+0.001)
        turn_rate = round( (center_pos - 80))
    else:
        turn_rate = 0
        speed = 80

    img.draw_arrow(80, 5, 80+turn_rate, 5, 255, 10)
    img.draw_arrow(80, 6, 80+turn_rate, 6, 255, 10)

    for r in WROIS:
        img.draw_rectangle(r[:4], 0)
    print(turn_rate)
    uart.write(str(turn_rate) + "\n");
    time.sleep_ms(50)

