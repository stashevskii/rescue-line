# Добавить игнорирование всего что находится после перекрестка, пофиксить ситуацию где два маркера

import sensor

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time=2000)

CROSS = False
rects_blobs = []

GRAYSCALE_THRESHOLD = [(0, 30)]

ROIS = [
    (0, 100, 160, 20, 0.7),
    (0, 50, 160, 20, 0.3),
    (0, 0, 160, 20, 0.1),
]

weight_sum = 0
for r in ROIS:
    weight_sum += r[4]


def find_green_threshold():
    img = sensor.snapshot()
    stats = img.get_statistics(roi=(img.width()//4, img.height()//4, img.width()//2, img.height()//2))

    return (stats.l_mean() - 20, stats.l_mean() + 20,
            stats.a_mean() - 20, stats.a_mean() + 20,
            stats.b_mean() - 20, stats.b_mean() + 20)


green_threshold = (30, 80, -70, -10, 10, 70)

while True:
    img = sensor.snapshot()

    # Обработка перекрестка
    if CROSS:
        rects_blobs = img.find_blobs([green_threshold],
                                     pixels_threshold=200,
                                     area_threshold=200,
                                     merge=True)

        rects_blobs.sort(key=lambda x: x.pixels(), reverse=True)

        # Нарисовать до 3-х зеленых blob'ов
        for i, blob in enumerate(rects_blobs):
            if i >= 3:
                break
            img.draw_rectangle(blob.rect(), color=(0, 255, 0))
            img.draw_cross(blob.cx(), blob.cy(), color=(0, 255, 0))

        # Определим позицию самого крупного зеленого blob'а (если есть)
        if rects_blobs:
            largest_green_blob = rects_blobs[0]
            center_x = img.width() // 2
            green_x = largest_green_blob.cx()

            # Определяем позицию относительно центра (слева, центр, справа)
            if len(rects_blobs) >= 2:
                position = "BOTH"
            elif green_x < center_x:
                position = "LEFT"
            elif green_x > center_x:
                position = "RIGHT"

            print("CROSS DETECTED")
            print("Green blob position relative to cross:", position)
        else:
            print("CROSS DETECTED, but no green blob found")

    # Обработка линии (чёрной)
    centroid_sum = 0

    for r in ROIS:
        blobs = img.find_blobs(
            GRAYSCALE_THRESHOLD, roi=r[0:4], merge=True
        )

        if blobs:
            largest_blob = max(blobs, key=lambda b: b.pixels())
            img.draw_rectangle(largest_blob.rect())
            img.draw_cross(largest_blob.cx(), largest_blob.cy())

            centroid_sum += largest_blob.cx()
            if largest_blob.pixels() >= 500:
                CROSS = True
            else:
                CROSS = False
        else:
            CROSS = False

    # Вывод состояния CROSS
    if not CROSS:
        print("NO CROSS")
