
import cv2
import numpy

from class_factory import Factory

class OutputPresenter:

    def getRGB(image, x, y):
        width = image.shape[1]
        baseIndex = int(y * width * 3 + x * 3)
        r = ord(image.data[baseIndex + 0])
        g = ord(image.data[baseIndex + 1])
        b = ord(image.data[baseIndex + 2])
        return (r, g, b)
        
    def setRGB(image, x, y, r, g, b):
        width = image.shape[1]
        baseIndex = int(y * width * 3 + x * 3)
        image.data[baseIndex + 0] = chr(r)
        image.data[baseIndex + 1] = chr(g)
        image.data[baseIndex + 2] = chr(b)
    
    def setCurrentImage(self, image):
        self.currentImage = image
        self.show_image(image)
    
    def getCurrentImage(self):
        return self.currentImage
        
    def set_src_image(self, image):
        self.setCurrentImage(image)
        self.src_image = image
        
    def show_image(self, image):
        height = image.shape[0]
        width = image.shape[1]
        image2 = cv2.resize(image, (width * self.zoomFactor, height * self.zoomFactor), interpolation = cv2.INTER_NEAREST )
        self.outputWindow.show_image( image2 )
        
    def zoom_changed( self, new_zoom ):
        self.zoomFactor = new_zoom
        self.show_image( self.currentImage )
        
    def spin_changed( self, x, y ):
        setRGB(self.currentImage, x, y, 0, 255, 0)
        self.show_image(self.currentImage)
        
    def mouse_moved(self, x, y):
        zoomed_x = int(x / self.zoomFactor)
        zoomed_y = int(y / self.zoomFactor)
        (r, g, b) = getRGB(self.currentImage, zoomed_x, zoomed_y)
        
        str = '(%d, %d) -\t (%d, %d, %d)' % (zoomed_x, zoomed_y, r, g, b)
        self.output_window.set_info_label(str)

    def __init__(self):
        self.zoomFactor = 1
    
        self.outputWindow = Factory.default_factory.new_output_window( self )
        self.outputWindow.show()