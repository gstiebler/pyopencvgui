
import pygtk
pygtk.require('2.0')
import gtk
import gtk.glade

class OutputWindow:

    def show_image(self, image):
        self.img_pixbuf = gtk.gdk.pixbuf_new_from_array(image, gtk.gdk.COLORSPACE_RGB, 8)
        self.image.set_from_pixbuf(self.img_pixbuf)

    def on_zoom_changed(self, adjustment, data=None): 
        self.output_presenter.zoom_changed( int(adjustment.get_value()) )
        
    def on_mouse_move(self, widget, event, data2):
        self.output_presenter.mouse_moved( event.x, event.y )
        
    def on_spin_changed(self, widget, data):
        x = self.xSpin.get_value()
        y = self.ySpin.get_value()
        self.output_presenter.mouse_moved( x, y )
        
    def set_info_label( self, label ):
        self.infoLabel.set_label( label )
        
    def __init__(self, output_presenter):
        self.output_presenter = output_presenter
        self.gladeBuilder = gtk.glade.XML( "../glade/OutputWindow.glade", "mainWindow") 
        self.outputWindow = self.gladeBuilder.get_widget("mainWindow")
        self.scrolled_window = self.gladeBuilder.get_widget("scrWindow")
        self.zoomScale = self.gladeBuilder.get_widget("zoomScale")
        self.infoLabel = self.gladeBuilder.get_widget("infoLabel")
        self.xSpin = self.gladeBuilder.get_widget("xSpin")
        self.ySpin = self.gladeBuilder.get_widget("ySpin")
        
        self.image = gtk.Image()
        self.eventBox = gtk.EventBox()
        self.eventBox.add(self.image)
        self.eventBox.show()
        self.scrolled_window.add_with_viewport(self.eventBox)
        self.image.show()
        
        self.outputWindow.set_title('Ouput Window')
        
        self.outputWindow.connect("delete_event", lambda w,e: gtk.main_quit())
        self.zoomScale.connect("value-changed", self.on_zoom_changed, None)
        self.scrolled_window.connect("motion-notify-event", self.on_mouse_move, None)
        self.image.connect("motion-notify-event", self.on_mouse_move, None)
        self.xSpin.connect("value_changed", self.on_spin_changed, None )
        self.ySpin.connect("value_changed", self.on_spin_changed, None )
        
        self.outputWindow.move( 50, 300 )
        