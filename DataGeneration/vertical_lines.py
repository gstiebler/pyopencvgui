import math

def generate(src_img, num_lines=5, offset=0.023):
    height = src_img.shape[0]
    width = src_img.shape[1]
    
    line_height_factor = num_lines * 2 * math.pi / height
    
    for i in range(width):
        value = math.sin(i * line_height_factor + offset);
        for j in range(height):
            src_img[j][i] = value