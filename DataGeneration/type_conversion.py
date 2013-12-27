import numpy as np

# recebe uma matriz de float entre -1 e 1
def float2uint8(src_img):
    dest_img = np.zeros(shape=(src_img.shape[0], src_img.shape[1]), dtype=np.uint8)
    for j in range(src_img.shape[0]):
        for i in range(src_img.shape[1]):
            dest_img[j][i] = (src_img[j][i] + 1.0) * 127.5
    
    return dest_img