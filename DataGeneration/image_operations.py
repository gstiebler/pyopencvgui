
def max(img1, img2, img_dst):
    
    height = img1.shape[0]
    width = img1.shape[1]
    
    for j in range(height):
        for i in range(width):
            if img1[j][i] > img2[j][i]:
                img_dst[j][i] = img1[j][i]
            else:
                img_dst[j][i] = img2[j][i]
                
def sum(img1, img2, img_dst):
    
    height = img1.shape[0]
    width = img1.shape[1]
    
    for j in range(height):
        for i in range(width):
            img_dst[j][i] = img1[j][i] + img2[j][i]
                
def sub(img1, img2, img_dst):
    
    height = img1.shape[0]
    width = img1.shape[1]
    
    for j in range(height):
        for i in range(width):
            img_dst[j][i] = img1[j][i] - img2[j][i]