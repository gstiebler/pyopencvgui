
import pygtk
pygtk.require('2.0')
import gtk
import gtk.glade
import cv2
import numpy
import ctypes

from ctypes import *
# mylib = cdll.LoadLibrary("./libmyextension.so") 
mylib = windll.imageProcessing

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

    def process_line(self, line):  
        pieces = line.split(':')
        name = pieces.pop(0)
        type = pieces.pop(0)
        if type == 'int':
            str_exec = "adj = gtk.Adjustment(%s)" % pieces[0]
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

    def process_text(self, text):
        lines = text.splitlines()
        self.func_str = lines.pop(0)
        self.window.set_title(self.func_str)
        for line in lines:
            self.process_line(line)
            
    def execute(self):
        params_str = ''
        
        src_image = self.output_window.get_src_image()
        gray_image = cv2.cvtColor(src_image, cv2.COLOR_BGR2GRAY)
        
        func_module = self.func_str.split('.')[0]
        if func_module == "cv2":
            for widget in self.widget_params:
                if type(widget) is gtk.HScale:
                    params_str += '%d, ' % widget.get_value()
                elif type(widget) is gtk.ComboBox:
                    params_str += 'cv2.%s, ' % widget.get_active_text()
            params_str = params_str[:-2]
            
            func_str = 'result = %s(gray_image, %s)' % (self.func_str, params_str)
        elif func_module == "mylib":
            for widget in self.widget_params:
                if type(widget) is gtk.HScale:
                    params_str += '%d, ' % widget.get_value()
            params_str = params_str[:-2]
            
            tmp_image = cv2.cvtColor(src_image, cv2.COLOR_BGR2GRAY)
            arr1 = gray_image.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte))
            arr2 = tmp_image.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte))
            func_str = '%s(arr1, arr2, c_int(src_image.shape[0]), c_int(src_image.shape[1]), %s)' % (self.func_str, params_str)
            
        print func_str
        exec func_str
        
        colorImage = cv2.cvtColor(tmp_image, cv2.COLOR_GRAY2BGR)
        self.output_window.setCurrentImage(colorImage)
       
    def execute_button_callback(self, widget, data=None):
        self.execute()

    def __init__(self, text, output_window):
        self.widget_params = []
        self.output_window = output_window
    
        self.gladeBuilder = gtk.glade.XML( "FuncWindow.glade", "mainWindow") 
        self.window = self.gladeBuilder.get_widget("mainWindow")
        self.main_vbox = self.gladeBuilder.get_widget("mainVBox")
        self.execute_button = self.gladeBuilder.get_widget("executeButton")
        self.execute_button.connect("clicked", self.execute_button_callback, None)
        
        self.process_text(text)
        
        self.window.show()
        self.window.move( 600, 50 )

