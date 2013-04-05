
import pygtk
pygtk.require('2.0')
import gtk
import gtk.glade
import cv2
import numpy

def getRGB(image, x, y):
    width = image.shape[0]
    baseIndex = int(y * width * 3 + x * 3)
    r = ord(image.data[baseIndex + 0])
    g = ord(image.data[baseIndex + 1])
    b = ord(image.data[baseIndex + 2])
    return (r, g, b)

class OutputWindow:

    def show_image(self, image):
        height = image.shape[0]
        width = image.shape[1]
        image2 = cv2.resize(image, (width * self.zoomFactor, height * self.zoomFactor), interpolation = cv2.INTER_NEAREST )
        self.img_pixbuf = gtk.gdk.pixbuf_new_from_array(image2, gtk.gdk.COLORSPACE_RGB, 8)
        self.image.set_from_pixbuf(self.img_pixbuf)

    def zoomChanged(self, adjustment, data=None): 
        self.zoomFactor = int(adjustment.get_value())
        self.show_image(self.currentImage)
        
    def mouse_move(self, widget, event, data2):
        x = int(event.x / self.zoomFactor)
        y = int(event.y / self.zoomFactor)
        (r, g, b) = getRGB(self.currentImage, x, y)
        
        str = '(%d, %d) -\t (%d, %d, %d)' % (x, y, r, g, b)
        self.infoLabel.set_label(str)
        
    def setCurrentImage(self, image):
        self.currentImage = image
        self.show_image(image)
    
    def getCurrentImage(self):
        return self.currentImage
        
    def set_src_image(self, image):
        self.setCurrentImage(image)
        self.src_image = image
    
    def get_src_image(self):
        return self.src_image
        
    def __init__(self):
        self.zoomFactor = 1
    
        self.gladeBuilder = gtk.glade.XML( "OutputWindow.glade", "mainWindow") 
        self.outputWindow = self.gladeBuilder.get_widget("mainWindow")
        self.scrolled_window = self.gladeBuilder.get_widget("scrWindow")
        self.zoomScale = self.gladeBuilder.get_widget("zoomScale")
        self.infoLabel = self.gladeBuilder.get_widget("infoLabel")
        
        self.image = gtk.Image()
        self.eventBox = gtk.EventBox()
        self.eventBox.add(self.image)
        self.eventBox.show()
        self.scrolled_window.add_with_viewport(self.eventBox)
        self.image.show()
        
        self.outputWindow.set_title('Ouput Window')
        
        self.outputWindow.connect("delete_event", lambda w,e: gtk.main_quit())
        self.zoomScale.connect("value-changed", self.zoomChanged, None)
        self.scrolled_window.connect("motion-notify-event", self.mouse_move, None)
        self.image.connect("motion-notify-event", self.mouse_move, None)
        
        self.outputWindow.show()
        