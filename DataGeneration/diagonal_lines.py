import math

def generate(src_img, num_lines=5):
    height = src_img.shape[0]
    width = src_img.shape[1]
    
    line_height_factor = num_lines * 2 * math.pi / height
    height_width_factor = height * 1.0 / width
    
    for j in range(height):
        for i in range(width):
            value = math.sin( (j + i * height_width_factor) * line_height_factor );
            src_img[j][i] = value