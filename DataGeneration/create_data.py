import cv2
import numpy as np

import type_conversion
import vertical_lines
import horizontal_lines
import image_operations

width = 50
height = 50

vertical_img = np.zeros(shape=(height,width), dtype=np.float32)
horizontal_img = np.zeros(shape=(height,width), dtype=np.float32)
max_img = np.zeros(shape=(height,width), dtype=np.float32)

vertical_lines.generate(vertical_img)
horizontal_lines.generate(horizontal_img)

image_operations.sum(vertical_img, horizontal_img, max_img)
    
converted_img = type_conversion.float2uint8(max_img, -2.0, 2.0)

final_img = cv2.cvtColor(converted_img, cv2.COLOR_GRAY2BGR)

cv2.imwrite("teste.bmp", final_img)