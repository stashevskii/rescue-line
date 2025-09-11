
import sensor
import time

# Tracks a black line. Use [(128, 255)] for a tracking a white line.
GRAYSCALE_THRESHOLD = [(0, 30)]

# Each roi is (x, y, w, h). The line detection algorithm will try to find the
# centroid of the largest blob in each roi. The x position of the centroids
# will then be averaged with different weights where the most weight is assigned
# to the roi near the bottom of the image and less to the next roi and so on.
ROIS = [  # [ROI, weight]
    (0, 100, 160, 20, 0.7),  # You'll need to tweak the weights for your app
    (0, 50, 160, 20, 0.3),  # depending on how your robot is setup.
    (0, 0, 160, 20, 0.1),
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
clock = time.clock()  # Tracks FPS.

while True:
    clock.tick()  # Track elapsed milliseconds between snapshots().
    img = sensor.snapshot()  # Take a picture and return the image.

    centroid_sum = 0

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

            centroid_sum += largest_blob.cx() * r[4]  # r[4] is the roi weight.

    # connected to your computer. The FPS should increase once disconnected.
