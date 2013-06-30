
import cv2
import numpy
import os
from xml.dom.minidom import parse, parseString

from class_factory import Factory
import output_presenter
import func_presenter

class MainPresenter:
    
    def load_image(self, file_name):
        srcImage = cv2.imread(file_name, cv2.CV_LOAD_IMAGE_COLOR)
        self._output_presenter.set_src_image(srcImage)
        
    def file_loaded(self, file_name):
        self.load_image(file_name)
        
    def custom_func_button_clicked(self, function_xml):
        func_presenter.FuncPresenter( function_xml, self._output_presenter )
        
    def capture_window_button_clicked(self):
        Factory.default_factory.new_capture_window( self._output_presenter )
        
    def batch_window_button_clicked(self):
        Factory.default_factory.new_batch_window()
        
    def add_func_clicked( self, text ):
       Factory.default_factory.new_func_window(text, self._output_presenter )
    
    def __init__(self): 
        self.main_window = Factory.default_factory.new_main_window( self )
        self._output_presenter = output_presenter.OutputPresenter()
               
        dom1 = parse(os.path.dirname(__file__) + "/functions.xml")
        functions = dom1.getElementsByTagName("function")
        for function in functions:
            self.main_window.add_custom_function( function ) 
            
        self.load_image(os.path.dirname(__file__) + "/../bin/dj.bmp")
