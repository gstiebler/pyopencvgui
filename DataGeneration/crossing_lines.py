import numpy as np

import diagonal_lines
import horizontal_lines
import image_operations

def generate(width, height, hor_lines=5, diag_lines=2):

    horizontal_lines_img = np.zeros(shape=(height,width), dtype=np.float32)  
    diagonal_lines_img = np.zeros(shape=(height,width), dtype=np.float32)
    dest_img = np.zeros(shape=(height,width), dtype=np.float32)
    
    horizontal_lines.generate(horizontal_lines_img, hor_lines)
    diagonal_lines.generate(diagonal_lines_img, diag_lines)
    image_operations.max(horizontal_lines_img, diagonal_lines_img, dest_img)
    
    return dest_img