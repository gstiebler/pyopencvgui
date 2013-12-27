import cv2
import numpy as np

import type_conversion

width = 100
height = 100
initial_img = np.zeros(shape=(width,height), dtype=np.float32)

for i in range(width):
    initial_img[i][i] = 1.0;
    
converted_img = type_conversion.float2uint8(initial_img)

final_img = cv2.cvtColor(converted_img, cv2.COLOR_GRAY2BGR)

cv2.imwrite("teste.bmp", final_img)