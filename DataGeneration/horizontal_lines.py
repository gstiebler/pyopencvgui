import math

def generate_horizontal_lines(src_img, num_lines=10):
    height = src_img.shape[0]
    width = src_img.shape[1]
    
    line_height_factor = num_lines * 2 * math.pi / height
    
    for i in range(height):
        value = math.sin(i * line_height_factor);
        for j in range(width):
            src_img[i][j] = value