
import pygtk
pygtk.require('2.0')
import gtk

class BatchWindow:

    def func_callback(self, widget, function_xml):
        self._presenter.func_selected( function_xml )

    def add_custom_function(self, function_xml):
        self.last_radio = gtk.RadioButton(self.last_radio, function_xml.getAttribute("name"))
        self.vbox.pack_start( self.last_radio, False, False, 5 )
        self.last_radio.show()
        self.last_radio.connect("clicked", self.func_callback, function_xml)
        
    def on_ok_click(self, widget):
        stats_file = self.statsPathEntry.get_text()
        base_path = self.imagesPathEntry.get_text()
        self._presenter.ok_clicked( base_path, stats_file )

    def __init__(self, presenter):
        self._presenter = presenter
    
        self.last_radio = None
        
        self.window = gtk.Window()
        self.vbox = gtk.VBox( spacing = 5 )
        self.imagesPathEntry = gtk.Entry()
        self.statsPathEntry = gtk.Entry()
        executeButton = gtk.Button("Execute")
        self.imagesPathEntry.set_text("C:/Projetos/LensometroSVN2/imagens")
        self.statsPathEntry.set_text("C:/Projetos/LensometroSVN2/stats.csv")
        
        executeButton.connect("clicked", self.on_ok_click)
        
        self.vbox.pack_start(self.imagesPathEntry, False, False, 5)
        self.vbox.pack_start(self.statsPathEntry, False, False, 5)
        self.vbox.pack_start(executeButton, False, False, 5)
        
        self.window.add(self.vbox)

        self.window.show_all()            
        
