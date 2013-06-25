
import pygtk
pygtk.require('2.0')
import gtk
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

    def __init__(self):
    
        self.last_radio = None
        
        self.window = gtk.Window()
        self.vbox = gtk.VBox( spacing = 5 )
        entry = gtk.Entry()
        executeButton = gtk.Button("Execute")
        entry.set_text(os.getcwd())
        
        dom1 = parse('functions.xml')
        functions = dom1.getElementsByTagName("function")
        for function in functions:
            self.add_custom_function( function ) 
        
        self.vbox.pack_start(entry, False, False, 5)
        self.vbox.pack_start(executeButton, False, False, 5)
        
        self.window.add(self.vbox)

        self.window.show_all()            
        
