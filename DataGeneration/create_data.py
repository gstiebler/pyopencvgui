import cv2
import numpy as np

import type_conversion
import horizontal_lines

width = 100
height = 50

initial_img = np.zeros(shape=(height,width), dtype=np.float32)

horizontal_lines.generate(initial_img)
    
converted_img = type_conversion.float2uint8(initial_img)

final_img = cv2.cvtColor(converted_img, cv2.COLOR_GRAY2BGR)

cv2.imwrite("teste.bmp", final_img)