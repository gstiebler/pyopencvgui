import cv2
import numpy as np

import type_conversion
import horizontal_central_gradient
import shapes
import image_operations

width = 20
height = 15

src_img = np.ones(shape=(height,width), dtype=np.float32)
gradient_img = np.zeros(shape=(height,width), dtype=np.float32)
sum_img = np.zeros(shape=(height,width), dtype=np.float32)

shapes.generate(src_img)
horizontal_central_gradient.generate(gradient_img, brightest=0.0, darkest=0.45, center=0.5)

image_operations.sub(src_img, gradient_img, sum_img)
    
converted_img = type_conversion.float2uint8(sum_img, 0.0, 1.0)

final_img = cv2.cvtColor(converted_img, cv2.COLOR_GRAY2BGR)

cv2.imwrite("teste.bmp", final_img)