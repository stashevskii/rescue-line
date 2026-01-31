import sensor, image, time

# --- Инициализация камеры ---
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)   # 160x120 (быстро и стабильно)
sensor.skip_frames(time=2000)

sensor.set_auto_gain(False)          # ВАЖНО: выключаем автоусиление
sensor.set_auto_whitebal(False)      # ВАЖНО: фиксируем баланс белого

clock = time.clock()

# --- Порог для ЧЁРНОЙ линии ---
# Подстрой при необходимости!
THRESHOLD = (0, 60)  # (min_L, max_L)

while True:
    clock.tick()
    img = sensor.snapshot()

    # Бинаризация изображения
    img.binary([THRESHOLD])

    # Поиск блобов (линии)
    blobs = img.find_blobs(
        [THRESHOLD],
        pixels_threshold=200,
        area_threshold=200,
        merge=True
    )

    if blobs:
        # Берём самый большой блоб (основная линия)
        blob = max(blobs, key=lambda b: b.pixels())

        # --- Рисуем прямоугольник вокруг линии ---
        img.draw_rectangle(blob.rect(), color=(0, 255, 255), thickness=2)

        # --- Центральная линия ---
        cx = blob.cx()
        img.draw_line(cx, 0, cx, img.height(), color=(255, 0, 0), thickness=1)

        # --- Левая и правая границы линии ---
        left_x = blob.x()
        right_x = blob.x() + blob.w()

        img.draw_line(left_x, 0, left_x, img.height(), color=(0, 0, 255), thickness=2)
        img.draw_line(right_x, 0, right_x, img.height(), color=(0, 0, 255), thickness=2)

        # --- Центр массы ---
        img.draw_cross(blob.cx(), blob.cy(), color=(0, 255, 0), size=10)

    print("FPS:", clock.fps())
