
import unittest
import os
import cv2
import sys
from xml.dom.minidom import parse, parseString

sys.path.append('../python')
import func_presenter
from class_factory import Factory
import test_factory
from mock import MagicMock

class TestFuncPresenter(unittest.TestCase):

    def setUp(self):
        Factory.default_factory = test_factory.TestWindowFactory()
        
        dom1 = parse(os.path.dirname(__file__) + "/../python/functions.xml")
        self.functions = dom1.getElementsByTagName("function")
        
        self._output_presenter = MagicMock()
        
    def loadImage(self):
        file_path = os.path.dirname(__file__) + "/data/dj.bmp"
        return cv2.imread(file_path, cv2.CV_LOAD_IMAGE_COLOR)

    def test_constructor_seismic(self):
        selected_func = self.functions[1]
        self._func_presenter = func_presenter.FuncPresenter( selected_func, self._output_presenter )
        self.assertEqual(1, Factory.default_factory.func_window.add_int_param.call_count)

    def test_constructor_edge(self):
        selected_func = self.functions[4]
        self._func_presenter = func_presenter.FuncPresenter( selected_func, self._output_presenter )
        self.assertEqual(5, Factory.default_factory.func_window.add_int_param.call_count)
      
    def test_execution_needed(self):
        selected_func = self.functions[4]
        self._func_presenter = func_presenter.FuncPresenter( selected_func, self._output_presenter )
        
        src_img = self.loadImage()
        self._output_presenter.get_src_image.return_value = src_img;
        Factory.default_factory.func_window.get_choice_params.return_value = []
        int_params = [10, 8, 7, 5, 5]
        Factory.default_factory.func_window.get_int_params.return_value = int_params
        
        self._func_presenter.execution_needed()
        
        self.assertEqual(1, self._output_presenter.setCurrentImage.call_count)
    
        