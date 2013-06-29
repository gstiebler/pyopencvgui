
import pygtk
pygtk.require('2.0')
import gtk
import gtk.glade

class MainWindow:

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
            self.main_presenter.file_loaded( dialog.get_filename() )
        elif response == gtk.RESPONSE_CANCEL:
            print 'Closed, no files selected'
        dialog.destroy()

    def func1_callback(self, widget, function_xml):
        self.main_presenter.custom_func_button_clicked( function_xml )

    def add_custom_function(self, function_xml):
        func_button = gtk.Button( function_xml.getAttribute("name") )
        self.vbox1.pack_end( func_button )
        func_button.show()
        func_button.connect("clicked", self.func1_callback, function_xml)
        
    def captureCallback(self, widget, data=None):
        self.main_presenter.capture_window_button_clicked()
        
    def on_batch_click(self, widget, data=None):
        self.main_presenter.batch_window_button_clicked()
        
    def addFuncCallback(self, widget, data=None):
        buffer = self.newFuncTextView.get_buffer()
        (begin, end) = buffer.get_bounds()
        text = buffer.get_text(begin, end)
        
        self.main_presenter.add_func_clicked( text )
        
    def __init__(self, main_presenter):  
        self.main_presenter = main_presenter
    
        gladeBuilder = gtk.glade.XML( "../glade/MainWindow.glade", "mainWindow") 
        app_window = gladeBuilder.get_widget("mainWindow")
        openFileButton = gladeBuilder.get_widget("openFileButton")
        captureButton = gladeBuilder.get_widget("captureButton")
        batchButton = gladeBuilder.get_widget("batchButton")
        addFuncButton = gladeBuilder.get_widget("addFuncButton")

        app_window.connect("delete_event", lambda w,e: gtk.main_quit())
        openFileButton.connect("clicked", self.openFileCallback, None)
        captureButton.connect("clicked", self.captureCallback, None)
        addFuncButton.connect("clicked", self.addFuncCallback, None)
        batchButton.connect("clicked", self.on_batch_click, None)
        self.vbox1 = gladeBuilder.get_widget( "vbox1" )
        
        app_window.resize(200, 300);   
            
        app_window.set_title('Main Window')
        app_window.show()





