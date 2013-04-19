
#!/usr/bin/env python

import pygtk
pygtk.require('2.0')
import gtk
import gtk.glade
import cv2
import numpy

import output_window
import func_window

class MyProgram:

    def load_image(self, file_name):
        print file_name, 'selected'
        srcImage = cv2.imread(file_name, cv2.CV_LOAD_IMAGE_COLOR);
        self.outputWindow.set_src_image(srcImage)

    def openFileCallback(self, widget, data=None):
        dialog = gtk.FileChooserDialog("Open..",
                               None,
                               gtk.FILE_CHOOSER_ACTION_OPEN,
                               (gtk.STOCK_CANCEL, gtk.RESPONSE_CANCEL,
                                gtk.STOCK_OPEN, gtk.RESPONSE_OK))
        dialog.set_default_response(gtk.RESPONSE_OK)

        filter = gtk.FileFilter()
        filter.set_name("All files")
        filter.add_pattern("*")
        dialog.add_filter(filter)

        filter = gtk.FileFilter()
        filter.set_name("Images")
        filter.add_mime_type("image/png")
        filter.add_mime_type("image/jpeg")
        filter.add_mime_type("image/gif")
        filter.add_pattern("*.png")
        filter.add_pattern("*.jpg")
        filter.add_pattern("*.gif")
        filter.add_pattern("*.tif")
        filter.add_pattern("*.xpm")
        dialog.add_filter(filter)

        response = dialog.run()
        if response == gtk.RESPONSE_OK:
            self.load_image(dialog.get_filename())
        elif response == gtk.RESPONSE_CANCEL:
            print 'Closed, no files selected'
        dialog.destroy()

    def addFuncCallback(self, widget, data=None):
        buffer = self.newFuncTextView.get_buffer()
        (begin, end) = buffer.get_bounds()
        text = buffer.get_text(begin, end)
    
        func_window.FuncWindow(text, self.outputWindow)

    def func1_callback(self, widget, func_text):
        func_window.FuncWindow(func_text, self.outputWindow)

    def add_custom_function(self, func_text, func_name):
        func_button = gtk.Button( func_name )
        self.vbox1.pack_end( func_button )
        func_button.show()
        func_button.connect("clicked", self.func1_callback, func_text)
        
    def __init__(self):  
        self.gladeBuilder = gtk.glade.XML( "../glade/MainWindow.glade", "mainWindow") 
        self.app_window = self.gladeBuilder.get_widget("mainWindow")
        self.openFileButton = self.gladeBuilder.get_widget("openFileButton")
        self.addFuncButton = self.gladeBuilder.get_widget("addFuncButton")
        self.newFuncTextView = self.gladeBuilder.get_widget("newFuncTextView")

        self.app_window.connect("delete_event", lambda w,e: gtk.main_quit())
        self.openFileButton.connect("clicked", self.openFileCallback, None)
        self.addFuncButton.connect("clicked", self.addFuncCallback, None)
        self.vbox1 = self.gladeBuilder.get_widget( "vbox1" )

        #func_text = "mylib.seismicProcess\nnumPixelsString:int: value = 15, lower = 0, upper = 50\nxDebug:int: value = 15, lower = 0, upper = 600"
        func_text = "mylib.seismicProcess\nnumPixelsString:int: value = 15, lower = 0, upper = 50"
        self.add_custom_function( func_text, "custom_func" )

        self.app_window.set_title('Main Window')
        self.app_window.show()
        self.outputWindow = output_window.OutputWindow()

        self.load_image("dj.bmp")
        
        return

def main():
    gtk.main()
    return 0

if __name__ == "__main__":
    MyProgram()
    main()





