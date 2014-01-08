import math

def generate(src_img):
    height = src_img.shape[0]
    width = src_img.shape[1]
    
    startY = 3
    
    #horizontal bar 1 line
    for i in range(5, 15):
        src_img[startY][i] = 0.5
        
    startY += 2    
    
    #horizontal bar 2 lines
    for i in range(5, 15):
        src_img[startY][i] = 0.5
        src_img[startY + 1][i] = 0.5
        
    startY += 4
    
    #horizontal bar 2 lines offset of 1
    for i in range(5, 15):
        src_img[startY][i] = 0.5
        src_img[startY + 1][i + 1] = 0.5