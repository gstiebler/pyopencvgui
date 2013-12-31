import numpy as np

def float2uint8(src_img, min_value, max_value):
    inv_range = 1.0 / (max_value - min_value)
    dest_img = np.zeros(shape=(src_img.shape[0], src_img.shape[1]), dtype=np.uint8)
    for j in range(src_img.shape[0]):
        for i in range(src_img.shape[1]):
            dest_img[j][i] = (src_img[j][i] * 1.0 - min_value) * inv_range * 255
    
    return dest_img