# Edge Impulse - OpenMV FOMO Object Detection Example with Fall LED indicator only
#
# This work is licensed under the MIT license.
# Copyright (c) 2013-2024 OpenMV LLC. All rights reserved.
# https://github.com/openmv/openmv/blob/master/LICENSE

import sensor, image, time, os, ml, math, uos, gc
from ulab import numpy as np
from machine import LED

# Initialize the red LED. (Adjust LED index if necessary for your board.)
led = LED("LED_BLUE")

sensor.reset()                         # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565)    # Set pixel format to RGB565
sensor.set_framesize(sensor.QVGA)      # Set frame size to QVGA (320x240)
sensor.set_windowing((240, 240))         # Set 240x240 window.
sensor.skip_frames(time=2000)          # Let the camera adjust.

net = None
labels = None
min_confidence = 0.5

try:
    net = ml.Model("trained.tflite", load_to_fb=uos.stat('trained.tflite')[6] > (gc.mem_free() - (64*1024)))
except Exception as e:
    raise Exception('Failed to load "trained.tflite". Did you copy the .tflite and labels.txt file onto the mass-storage device? (' + str(e) + ')')

try:
    labels = [line.rstrip('\n') for line in open("labels.txt")]
except Exception as e:
    raise Exception('Failed to load "labels.txt". Did you copy the .tflite and labels.txt file onto the mass-storage device? (' + str(e) + ')')

colors = [
    (255,   0,   0),
    (  0, 255,   0),
    (255, 255,   0),
    (  0,   0, 255),
    (255,   0, 255),
    (  0, 255, 255),
    (255, 255, 255),
]

threshold_list = [(math.ceil(min_confidence * 255), 255)]

def fomo_post_process(model, inputs, outputs):
    ob, oh, ow, oc = model.output_shape[0]

    x_scale = inputs[0].roi[2] / ow
    y_scale = inputs[0].roi[3] / oh
    scale = min(x_scale, y_scale)
    x_offset = ((inputs[0].roi[2] - (ow * scale)) / 2) + inputs[0].roi[0]
    y_offset = ((inputs[0].roi[3] - (ow * scale)) / 2) + inputs[0].roi[1]

    l = [[] for i in range(oc)]
    for i in range(oc):
        img = image.Image(outputs[0][0, :, :, i] * 255)
        blobs = img.find_blobs(
            threshold_list, x_stride=1, y_stride=1, area_threshold=1, pixels_threshold=1
        )
        for b in blobs:
            rect = b.rect()
            x, y, w, h = rect
            score = img.get_statistics(thresholds=threshold_list, roi=rect).l_mean() / 255.0
            x = int((x * scale) + x_offset)
            y = int((y * scale) + y_offset)
            w = int(w * scale)
            h = int(h * scale)
            l[i].append((x, y, w, h, score))
    return l

clock = time.clock()
while True:
    clock.tick()

    # Capture an image frame.
    img = sensor.snapshot()

    # Reset the fall detection flag for this frame.
    detected_fall = False

    # Process detections.
    for i, detection_list in enumerate(net.predict([img], callback=fomo_post_process)):
        if i == 0:
            continue  # Skip background class.
        if len(detection_list) == 0:
            continue  # No detections for this class.

        # Check for fall detection.
        if labels[i] == "Fall":
            detected_fall = True

        # Process each detection in the list.
        for x, y, w, h, score in detection_list:
            center_x = math.floor(x + (w / 2))
            center_y = math.floor(y + (h / 2))
            print("Detected %s at x %d, y %d with score %f" % (labels[i], center_x, center_y, score))
            img.draw_circle((center_x, center_y, 12), color=colors[i])

    # Turn on red LED if a fall is detected.
    if detected_fall:
        led.on()
    else:
        led.off()

    print(clock.fps(), "fps", end="\n\n")
