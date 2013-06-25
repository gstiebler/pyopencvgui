
import pygtk
pygtk.require('2.0')
import gtk
import gtk.glade
import cv2
import numpy
import ctypes
import copy

from ctypes import *
# mylib = cdll.LoadLibrary("./libmyextension.so") 
mylib = ctypes.WinDLL('../bin/imageProcessing.dll')

def get_xml_text( node ):
    return node[0].firstChild.data

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
        self.widget_params.append(widget)

    def hscale_callback(self, adjustment, data=None):
        self.execute()
        
    def combobox_callback(self, combobox, user_data):
        self.execute()
        
    def get_xml_text( node ):
        return node[0].firstChild.data

    def process_param(self, param):  
        name = param.getAttribute("name")
        type = param.getAttribute("type")
        if type == 'int':
            adj_params = "value = %s, " % get_xml_text(param.getElementsByTagName("value"))
            adj_params += "lower = %s, " % get_xml_text(param.getElementsByTagName("lower"))
            adj_params += "upper = %s" % get_xml_text(param.getElementsByTagName("upper"))
            str_exec = "adj = gtk.Adjustment(%s)" % adj_params
            exec str_exec
            hscale = gtk.HScale(adj)
            adj.set_step_increment(1)
            adj.set_page_increment(1)
            hscale.set_digits(0)
            self.add_param(hscale, name)
            hscale.connect("value-changed", self.hscale_callback, None)
        elif type == 'choice':
            combo = gtk.combo_box_new_text()
            options = pieces[0].split(',')
            for option in options:
                combo.append_text(option.strip())
                
            combo.set_active(0)
            combo.connect("changed", self.combobox_callback, None)
                
            self.add_param(combo, name)

    def process_text(self, function_xml):
        self.func_str = function_xml.getAttribute("name")
        self.func_module = get_xml_text(function_xml.getElementsByTagName("srcLib"))
        self.srcDataType = get_xml_text(function_xml.getElementsByTagName("sourceDataType"))
        self.destDataType = get_xml_text(function_xml.getElementsByTagName("destDataType"))
        self.window.set_title(self.func_str)
        
        params = function_xml.getElementsByTagName("params")[0].getElementsByTagName("param")
        for param in params:
            self.process_param(param)
            
    def execute(self, src_image):
        params_str = ''
        
        for widget in self.widget_params:
            if type(widget) is gtk.HScale:
                params_str += '%d, ' % widget.get_value()
            elif type(widget) is gtk.ComboBox:
                params_str += '%s.%s, ' % (func_module, widget.get_active_text())
        params_str = params_str[:-2]
        
        if self.destDataType == "8bits":
            dest_image = cv2.cvtColor(src_image, cv2.COLOR_BGR2GRAY)
        else:
            dest_image = copy.deepcopy(src_image)
        
        if self.srcDataType == "8bits":
            src_image = cv2.cvtColor(src_image, cv2.COLOR_BGR2GRAY)
        
        func_str = ""
        if self.func_module == "cv2":
            func_str = 'result = %s(src_image, %s)' % (self.func_str, params_str)
        elif self.func_module == "mylib":
            arr1 = src_image.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte))
            arr2 = dest_image.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte))
            func_str = '%s.%s(arr1, arr2, c_int(src_image.shape[0]), c_int(src_image.shape[1]), %s)' % (self.func_module, self.func_str, params_str)
            
        print func_str
        exec func_str
        
        if self.destDataType == "8bits":
            dest_image = cv2.cvtColor(dest_image, cv2.COLOR_GRAY2BGR)
        return dest_image
       
    def execute_button_callback(self, widget, data=None):
        dest_image = self.execute( self.output_window.get_src_image() )
        self.output_window.setCurrentImage(dest_image)

    def __init__(self, text, output_window):
        self.widget_params = []
        self.output_window = output_window
    
        self.gladeBuilder = gtk.glade.XML( "../glade/FuncWindow.glade", "mainWindow") 
        self.window = self.gladeBuilder.get_widget("mainWindow")
        self.main_vbox = self.gladeBuilder.get_widget("mainVBox")
        self.execute_button = self.gladeBuilder.get_widget("executeButton")
        self.execute_button.connect("clicked", self.execute_button_callback, None)
        
        self.process_text(text)
        
        self.window.show()
        self.window.move( 600, 50 )

