import math

def generate(src_img, brightest=0.9, darkest=0.5, offset=0.0035):
    height = src_img.shape[0]
    width = src_img.shape[1]
    
    half_width = width / 2
    lum_dif = brightest - darkest
    
    for i in range(half_width):
        value = 1.0 - i * 1.0 / half_width - offset;
        for j in range(height):
            src_img[j][i] = value
        
    for i in range(half_width, width):
        value = (i - half_width) * 1.0 / half_width;
        for j in range(height):
            src_img[j][i] = value