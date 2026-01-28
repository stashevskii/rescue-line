import sensor
import image
import time
import math
import pyb


# Настройка UART
uart = pyb.UART(3, 19200)
uart.init(19200, bits=8, parity=None, stop=1)

# Настройка камеры
sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time=2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
sensor.set_hmirror(True)
sensor.set_vflip(True)

# Порог для черной линии
BLACK_THRESHOLD = (0, 40)

# Зоны сканирования (многоуровневые)
SCAN_ZONES = [
    {"y": 100, "height": 20, "weight": 0.2},  # Нижняя зона
    {"y": 70, "height": 20, "weight": 0.3},   # Средняя зона
    {"y": 40, "height": 20, "weight": 0.5},   # Верхняя зона
]

# Центральное окно для фильтрации
CENTER_WINDOW_WIDTH = 80
CENTER_WINDOW_X = (160 - CENTER_WINDOW_WIDTH) // 2

clock = time.clock()

def find_line_in_zones(img):
    """Находит линию в нескольких зонах сканирования"""
    centroids = []
    total_weight = 0

    for zone in SCAN_ZONES:
        # Сканируем только центральную часть каждой зоны
        roi = (
            CENTER_WINDOW_X,
            zone["y"],
            CENTER_WINDOW_WIDTH,
            zone["height"]
        )

        # Ищем черные blob'ы в ROI
        blobs = img.find_blobs([BLACK_THRESHOLD],
                              roi=roi,
                              merge=True,
                              margin=10,
                              area_threshold=50,
                              pixels_threshold=50)

        if blobs:
            # Берем самый большой blob в этой зоне
            largest_blob = max(blobs, key=lambda b: b.area())

            # Визуализация
            img.draw_rectangle(largest_blob.rect(), color=150)
            img.draw_cross(largest_blob.cx(), largest_blob.cy(), color=200)

            # Добавляем центроид с весом
            centroids.append({
                "x": largest_blob.cx(),
                "y": largest_blob.cy(),
                "weight": zone["weight"]
            })
            total_weight += zone["weight"]

    return centroids, total_weight

def calculate_line_from_centroids(centroids, total_weight):
    """Вычисляет линию по центроидам из разных зон"""
    if not centroids or total_weight == 0:
        return None, 0

    # Вычисляем взвешенный центр линии
    weighted_x_sum = sum(c["x"] * c["weight"] for c in centroids)
    weighted_y_sum = sum(c["y"] * c["weight"] for c in centroids)

    avg_x = weighted_x_sum / total_weight
    avg_y = weighted_y_sum / total_weight

    # Для простоты считаем вертикальную линию через этот центр
    # В реальности можно делать линейную регрессию
    return (avg_x, avg_y), avg_x

def calculate_steering_angle(line_center_x, img_center_x=80):
    """Вычисляет угол поворота"""
    # Простое пропорциональное управление
    error = line_center_x - img_center_x

    # Коэффициент пропорциональности (настраивается)
    Kp = 0.8

    # Угол поворота
    angle = -Kp * error  # Минус для инвертирования

    # Ограничение угла
    MAX_ANGLE = 45
    if angle > MAX_ANGLE:
        angle = MAX_ANGLE
    elif angle < -MAX_ANGLE:
        angle = -MAX_ANGLE

    return angle, error

def main_blob_version():
    prev_angle = 0
    prev_center = 80
    lost_counter = 0
    MAX_LOST = 10  # Максимальное количество кадров без линии

    print("Starting blob-based line follower...")

    while True:
        clock.tick()
        img = sensor.snapshot()

        # Рисуем центральное окно
        img.draw_rectangle(CENTER_WINDOW_X, 0,
                          CENTER_WINDOW_WIDTH, sensor.height(),
                          color=100, thickness=1)

        # Ищем линию в зонах
        centroids, total_weight = find_line_in_zones(img)

        # Вычисляем центр линии
        line_info, line_center_x = calculate_line_from_centroids(centroids, total_weight)

        if line_info:
            lost_counter = 0  # Сброс счетчика потерь

            # Вычисляем угол поворота
            angle, error = calculate_steering_angle(line_center_x)

            # Фильтрация угла
            filtered_angle = angle * 0.7 + prev_angle * 0.3
            prev_angle = filtered_angle
            prev_center = line_center_x

            # Рисуем линию от центра к центроидам
            for centroid in centroids:
                img.draw_line(int(line_center_x), int(line_info[1]),
                            centroid["x"], centroid["y"],
                            color=200, thickness=1)

        # Отправка данных
        angle_int = int(filtered_angle)

        # Отправка по UART
        uart.write(str(angle_int) + "\n")

        # Отображение на изображении

        time.sleep_ms(20)
