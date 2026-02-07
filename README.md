# Rescue line (powered by [pinmode.by](pinmode.by))

## Team CyberCode:
[@Leshechkagit](https://github.com/Leshechkagit)<br>
[@stashevskii](https://github.com/stashevskii)

## План:

* Raspberry Pi:
  * Только обработка изображения с камеры
  * ЯП: Python
  * Либа для камеры: OpenCV
  * Камера - Raspberry Pi Camera Module **N**
* Ардуинка:
  * Нано
  * Кнопки
  * Светодиоды
  * Драйвера и моторы
  * Дисплей OLED
  * Общение с Raspberry по SPI
* Питание:
  * АКБ (**4.2Вx4** - 2 последовательно, 2 параллельно для поднятие емкости)
  * Стабилизатор **5Vx2**
* PCB:
  * 1 плата и для питания (стабилизатор и тд), и для ардуинки (кнопки, светодиоды и тд)