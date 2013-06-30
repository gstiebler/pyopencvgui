
import pygtk
pygtk.require('2.0')
import gtk
import gtk.glade
        
class FuncWindow:

    def add_param(self, widget, name):
        label = gtk.Label()
        label.set_label(name + ':')
        hbox = gtk.HBox()
        hbox.pack_start(label, False)
        hbox.pack_start(widget, True)
        hbox.set_spacing(10)
        self.main_vbox.pack_start(hbox, False)
        hbox.show()
        label.show()
        widget.show()
        
    def add_int_param( self, name, value, lower, upper ):
        adj_params = "value = %s, " % value
        adj_params += "lower = %s, " % lower
        adj_params += "upper = %s" % upper
        str_exec = "adj = gtk.Adjustment(%s)" % adj_params
        exec str_exec
        hscale = gtk.HScale(adj)
        adj.set_step_increment(1)
        adj.set_page_increment(1)
        hscale.set_digits(0)
        self.add_param(hscale, name)
        hscale.connect("value-changed", self.hscale_callback, None)
        self.int_widget_params.append(hscale)
        
    def add_choice_param( self, name, str_options ):
        combo = gtk.combo_box_new_text()
        for option in str_options:
            combo.append_text(option)
            
        combo.set_active(0)
        combo.connect("changed", self.combobox_callback, None)
            
        self.add_param(combo, name)
        self.choice_widget_params.append(combo)
        
    def get_int_params( self ):
        params = []
        for widget in self.int_widget_params:
            params.append(widget.get_value())   
        return params
        
    def get_choice_params( self ):
        params = []
        for widget in self.choice_widget_params:
            params.append(widget.get_active_text())  
        return params  

    def hscale_callback(self, adjustment, data=None):
        self._func_presenter.execution_needed()
        
    def combobox_callback(self, combobox, user_data):
        self._func_presenter.execution_needed()
       
    def execute_button_callback(self, widget, data=None):
        self._func_presenter.execution_needed()
        
    def set_title( self, title ):
        self.window.set_title( title )

    def __init__(self, func_presenter):
        self.int_widget_params = []
        self.choice_widget_params = []
        self._func_presenter = func_presenter
    
        self.gladeBuilder = gtk.glade.XML( "../glade/FuncWindow.glade", "mainWindow") 
        self.window = self.gladeBuilder.get_widget("mainWindow")
        self.main_vbox = self.gladeBuilder.get_widget("mainVBox")
        self.execute_button = self.gladeBuilder.get_widget("executeButton")
        self.execute_button.connect("clicked", self.execute_button_callback, None)
        
        self.window.show()
        self.window.move( 600, 50 )

