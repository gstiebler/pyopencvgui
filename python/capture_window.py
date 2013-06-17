import cv2
import pygtk
pygtk.require('2.0')
import gtk
import glib
import time

class CaptureWindow:

    def idle_func(self):
        rval, frame = self.vc.read()
        
        self.img_pixbuf = gtk.gdk.pixbuf_new_from_array(frame, gtk.gdk.COLORSPACE_RGB, 8)
        self.image.set_from_pixbuf(self.img_pixbuf)
        
        if self.last_key == 65307: # exit on ESC
            self.vc.release()
            self.window.destroy()
            return False
        elif self.last_key == 115:
            self.output_window.setCurrentImage(frame)
            
        self.last_key = 0
        
        time.sleep(0.05)
            
        return True
        
    def on_key_press_event(self, widget, event):
        keyname = gtk.gdk.keyval_name(event.keyval)
        self.last_key = event.keyval
        print "Key %s (%d) was pressed" % (keyname, event.keyval)
            
    def __init__(self, output_window):
        self.output_window = output_window
        self.last_key = 0
        self.vc = cv2.VideoCapture(1)
        #vc.set(3, 1920)
        #vc.set(4, 1080)
        
        self.window = gtk.Window()
        self.image = gtk.Image()
        self.window.add(self.image)
        self.window.show_all()

        self.window.connect('key_press_event', self.on_key_press_event)

        if self.vc.isOpened(): # try to get the first frame
            rval, self.frame = self.vc.read()
        else:
            rval = False

        glib.idle_add(self.idle_func)
    