import math

def generate(src_img, num_lines=5, offset=0.059):
    height = src_img.shape[0]
    width = src_img.shape[1]
    
    line_height_factor = num_lines * 2 * math.pi / height
    
    for j in range(height):
        value = math.sin(j * line_height_factor + offset);
        for i in range(width):
            src_img[j][i] = value