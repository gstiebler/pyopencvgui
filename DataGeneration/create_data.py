import cv2
import numpy as np

import type_conversion
import horizontal_central_gradient
import image_operations

width = 50
height = 50

horizontal_gradient = np.zeros(shape=(height,width), dtype=np.float32)
sum_img = np.zeros(shape=(height,width), dtype=np.float32)

horizontal_central_gradient.generate(horizontal_gradient)

#image_operations.sum(vertical_img, horizontal_img, sum_img)
    
converted_img = type_conversion.float2uint8(horizontal_gradient, 0.0, 1.0)

final_img = cv2.cvtColor(converted_img, cv2.COLOR_GRAY2BGR)

cv2.imwrite("teste.bmp", final_img)