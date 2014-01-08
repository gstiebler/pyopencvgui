import math

def generate(src_img, brightest=0.9, darkest=0.5, center=0.5, offset=0.0035):
    height = src_img.shape[0]
    width = src_img.shape[1]
    
    center_width = int(width * center)
    lum_dif = brightest - darkest
    
    for i in range(center_width):
        value = darkest + lum_dif * i / center_width - offset;
        for j in range(height):
            src_img[j][i] = value
        
    for i in range(center_width, width):
        value = darkest + lum_dif * (1.0 - (i - center_width) * 1.0 / center_width)
        for j in range(height):
            src_img[j][i] = value