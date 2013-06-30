
import cv2
import os
import time
from xml.dom.minidom import parse, parseString

import func_presenter

def stats_to_line( double_stats, int_stats, num_double, num_int, file, sub_dir ):
    line = "{0}; {1};".format(file, sub_dir)
    for i in range(num_double):
        line = line + "{0}; ".format(double_stats[i])
        
    for i in range(num_int):
        line = line + "{0}; ".format(int_stats[i])
        
    return line + "\n"

from class_factory import Factory
    
class BatchPresenter:

    def process_dir( self, directory, sub_dir ):
        print directory
        for file in os.listdir(directory):
            self.process_file( directory, file, sub_dir )
            
    def process_file(self, directory, file, sub_dir):
        file_path = os.path.join(directory, file)
        print file_path, sub_dir
        src_image = cv2.imread(file_path, cv2.CV_LOAD_IMAGE_COLOR)
        
        start = time.clock()
        func_return = self.func_presenter.execute(src_image)
        elapsed = (time.clock() - start)
        print elapsed
        
        dest_img = func_return['dest_image']
        int_stats = func_return['int_stats']
        double_stats = func_return['double_stats']
        
        line = stats_to_line( double_stats, int_stats, 7, 2, file, sub_dir )
        self.stats_file.write( line )
        
    def ok_clicked( self, base_path, stats_file_path ):
        self.stats_file = open(stats_file_path, "w")
    
        start = time.clock()
        for sub_dir in os.listdir(base_path):
            complete_path = os.path.join(base_path, sub_dir)
            if os.path.isdir(complete_path):
                self.process_dir( complete_path, sub_dir )
                  
        elapsed = (time.clock() - start)
        print " total time: {0}".format(elapsed)
        
        self.stats_file.close()
        
    def func_selected( self, function_xml ):
        self.func_presenter = func_presenter.FuncPresenter( function_xml, None)

    def __init__(self):
        window = Factory.default_factory.new_batch_window( self )
        dom1 = parse(os.path.dirname(__file__) + "/functions.xml")
        functions = dom1.getElementsByTagName("function")
        for function in functions:
            window.add_custom_function( function ) 
            