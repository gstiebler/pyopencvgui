
import cv2
import numpy
import ctypes
import copy

from class_factory import Factory

from ctypes import *
# mylib = cdll.LoadLibrary("./libmyextension.so") 
mylib = ctypes.WinDLL('../bin/imageProcessing.dll')

def get_xml_text( node ):
    if node.length > 0:
        return node[0].firstChild.data
    else:
        return None

class FuncPresenter:

    def process_text(self, function_xml):
        self.func_str = function_xml.getAttribute("name")
        self.func_module = get_xml_text(function_xml.getElementsByTagName("srcLib"))
        self.srcDataType = get_xml_text(function_xml.getElementsByTagName("sourceDataType"))
        self.destDataType = get_xml_text(function_xml.getElementsByTagName("destDataType"))
        self.hasStats = get_xml_text(function_xml.getElementsByTagName("hasStats")) 
        self.numIntStats = get_xml_text(function_xml.getElementsByTagName("numIntStats")) 
        self.numDoubleStats = get_xml_text(function_xml.getElementsByTagName("numDoubleStats")) 
        self._func_window.set_title(self.func_str)
        
        params = function_xml.getElementsByTagName("params")[0].getElementsByTagName("param")
        for param in params:
            self.process_param(param)
            
    def execution_needed( self ):
        dest_image = self.execute( self._output_presenter.get_src_image() )['dest_image']
        self._output_presenter.setCurrentImage(dest_image)
        
    def process_param(self, param):  
        name = param.getAttribute("name")
        type = param.getAttribute("type")
        if type == 'int':
            value = get_xml_text(param.getElementsByTagName("value"))
            lower = get_xml_text(param.getElementsByTagName("lower"))
            upper = get_xml_text(param.getElementsByTagName("upper"))
            self._func_window.add_int_param( name, value, lower, upper )
        elif type == 'choice':
            options = pieces[0].split(',')
            str_options = []
            for option in options:
                str_options.append(option.strip())
                
            self._func_window.add_choice_param( name, str_options )
            
    def execute(self, src_image):
        int_params = self._func_window.get_int_params()
        choice_params = self._func_window.get_choice_params()
        
        params_str = ''
        for param in int_params:
            params_str += '%d, ' % param
            
        for param in choice_params:
            params_str += '%s.%s, ' % (self.func_module, widget.get_active_text())
            
        params_str = params_str[:-2]
        
        if self.destDataType == "8bits":
            dest_image = cv2.cvtColor(src_image, cv2.COLOR_BGR2GRAY)
        else:
            dest_image = copy.deepcopy(src_image)
        
        if self.srcDataType == "8bits":
            src_image = cv2.cvtColor(src_image, cv2.COLOR_BGR2GRAY)
        
        TenIntegersType = c_int * 10
        TenDoublesType = c_double * 10
        int_stats = TenIntegersType(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)
        double_stats = TenDoublesType(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)
        func_str = ""
        if self.func_module == "cv2":
            func_str = 'result = %s(src_image, %s)' % (self.func_str, params_str)
        elif self.func_module == "mylib":
            arr1 = src_image.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte))
            arr2 = dest_image.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte))
            func_str = '%s.%s(arr1, arr2, c_int(src_image.shape[0]), c_int(src_image.shape[1]), '
            if self.hasStats == "yes":
                func_str = func_str + 'int_stats, double_stats, '
            func_str = func_str + ' %s)'  
            func_str = func_str % (self.func_module, self.func_str, params_str)
            
        exec func_str
        
        if self.destDataType == "8bits":
            dest_image = cv2.cvtColor(dest_image, cv2.COLOR_GRAY2BGR)
            
        return {'dest_image': dest_image, 'int_stats': int_stats, 'double_stats': double_stats}
        
    def __init__(self, function_xml, output_presenter):
        self._output_presenter = output_presenter
        
        self._func_window = Factory.default_factory.new_func_window( self )
        self.process_text( function_xml )
        