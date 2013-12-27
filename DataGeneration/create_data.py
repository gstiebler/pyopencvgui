import cv2
import numpy as np

import type_conversion
import crossing_lines

width = 150
height = 50

initial_img = crossing_lines.generate(width, height)
    
converted_img = type_conversion.float2uint8(initial_img)

final_img = cv2.cvtColor(converted_img, cv2.COLOR_GRAY2BGR)

cv2.imwrite("teste.bmp", final_img)