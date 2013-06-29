
import pygtk
pygtk.require('2.0')
import gtk
import cv2
import os
import time
from xml.dom.minidom import parse, parseString

import func_window

def stats_to_line( double_stats, int_stats, num_double, num_int, file, sub_dir ):
    line = "{0}; {1};".format(file, sub_dir)
    for i in range(num_double):
        line = line + "{0}; ".format(double_stats[i])
        
    for i in range(num_int):
        line = line + "{0}; ".format(int_stats[i])
        
        
    return line + "\n"

class BatchWindow:

    def func_callback(self, widget, function_xml):
        self.func_window = func_window.FuncWindow(function_xml, None)

    def add_custom_function(self, function_xml):
        self.last_radio = gtk.RadioButton(self.last_radio, function_xml.getAttribute("name"))
        self.vbox.pack_start( self.last_radio, False, False, 5 )
        self.last_radio.connect("clicked", self.func_callback, function_xml)
        
    def on_ok_click(self, widget):
        self.stats_file = open(self.statsPathEntry.get_text(), "w")
    
        start = time.clock()
        base_path = self.imagesPathEntry.get_text()
        for sub_dir in os.listdir(base_path):
            complete_path = os.path.join(base_path, sub_dir)
            if os.path.isdir(complete_path):
                self.process_dir( complete_path, sub_dir )
                  
        elapsed = (time.clock() - start)
        print " total time: {0}".format(elapsed)
        
        self.stats_file.close()

    def process_dir( self, directory, sub_dir ):
        print directory
        for file in os.listdir(directory):
            self.process_file( directory, file, sub_dir )
            
    def process_file(self, directory, file, sub_dir):
        file_path = os.path.join(directory, file)
        print file_path, sub_dir
        src_image = cv2.imread(file_path, cv2.CV_LOAD_IMAGE_COLOR)
        
        start = time.clock()
        func_return = self.func_window.execute(src_image)
        elapsed = (time.clock() - start)
        print elapsed
        
        dest_img = func_return['dest_image']
        int_stats = func_return['int_stats']
        double_stats = func_return['double_stats']
        
        line = stats_to_line( double_stats, int_stats, 7, 2, file, sub_dir )
        self.stats_file.write( line )
        

    def __init__(self):
    
        self.last_radio = None
        
        self.window = gtk.Window()
        self.vbox = gtk.VBox( spacing = 5 )
        self.imagesPathEntry = gtk.Entry()
        self.statsPathEntry = gtk.Entry()
        executeButton = gtk.Button("Execute")
        self.imagesPathEntry.set_text("C:/Projetos/LensometroSVN2/imagens")
        self.statsPathEntry.set_text("C:/Projetos/LensometroSVN2/stats.csv")
        
        executeButton.connect("clicked", self.on_ok_click)
        
        dom1 = parse('functions.xml')
        functions = dom1.getElementsByTagName("function")
        for function in functions:
            self.add_custom_function( function ) 
        
        self.vbox.pack_start(self.imagesPathEntry, False, False, 5)
        self.vbox.pack_start(self.statsPathEntry, False, False, 5)
        self.vbox.pack_start(executeButton, False, False, 5)
        
        self.window.add(self.vbox)

        self.window.show_all()            
        
