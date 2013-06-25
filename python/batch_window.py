
import pygtk
pygtk.require('2.0')
import gtk

class BatchWindow:

    def __init__(self):
    
        self.window = gtk.Window()
        vbox = gtk.VBox( spacing = 10 )
        self.window.add(vbox)
        self.window.show_all()
