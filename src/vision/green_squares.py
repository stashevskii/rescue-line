import sensor
import image
import time

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time=2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)

# Широкий порог для зеленого цвета
green_threshold = (0, 100, -128, -10, -128, 127)

clock = time.clock()

while True:
    clock.tick()
    img = sensor.snapshot()

    # Находим ВСЕ зеленые пиксели
    green_blobs = img.find_blobs([green_threshold],
                               area_threshold=45,  # Минимальная площадь = 1 пиксель
                               pixels_threshold=1,
                               merge=False,       # Не сливать области
                               margin=5)          # Минимальный отступ

    total_green_area = 0
    green_pixels_count = 0
    print(len(green_blobs))
    # Считаем общую площадь зеленого
    for blob in green_blobs:
        total_green_area += blob.area()
        green_pixels_count += blob.pixels()

        img.draw_rectangle(blob.rect(), color=(255, 0, 0), thickness=1)
