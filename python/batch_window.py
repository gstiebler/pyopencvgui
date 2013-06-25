
import pygtk
pygtk.require('2.0')
import gtk
import cv2
import os
from xml.dom.minidom import parse, parseString

import func_window

class BatchWindow:

    def func_callback(self, widget, function_xml):
        self.func_window = func_window.FuncWindow(function_xml, None)

    def add_custom_function(self, function_xml):
        self.last_radio = gtk.RadioButton(self.last_radio, function_xml.getAttribute("name"))
        self.vbox.pack_start( self.last_radio, False, False, 5 )
        self.last_radio.connect("clicked", self.func_callback, function_xml)
        
    def on_ok_click(self, widget):
        base_path = self.entry.get_text()
        for sub_dir in os.listdir(base_path):
            complete_path = os.path.join(base_path, sub_dir)
            if os.path.isdir(complete_path):
                self.process_dir( complete_path, sub_dir )
        
    def process_dir( self, directory, sub_dir ):
        print directory
        for file in os.listdir(directory):
            file_path = os.path.join(directory, file)
            self.process_file( file_path, sub_dir )
            
    def process_file(self, file_path, sub_dir):
        print file_path, sub_dir
        src_image = cv2.imread(file_path, cv2.CV_LOAD_IMAGE_COLOR)
        dest_img = self.func_window.execute(src_image)

    def __init__(self):
    
        self.last_radio = None
        
        self.window = gtk.Window()
        self.vbox = gtk.VBox( spacing = 5 )
        self.entry = gtk.Entry()
        executeButton = gtk.Button("Execute")
        self.entry.set_text(os.getcwd())
        
        executeButton.connect("clicked", self.on_ok_click)
        
        dom1 = parse('functions.xml')
        functions = dom1.getElementsByTagName("function")
        for function in functions:
            self.add_custom_function( function ) 
        
        self.vbox.pack_start(self.entry, False, False, 5)
        self.vbox.pack_start(executeButton, False, False, 5)
        
        self.window.add(self.vbox)

        self.window.show_all()            
        
