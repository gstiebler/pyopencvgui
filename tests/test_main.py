
import unittest

import sys
sys.path.append('../python')
import main_presenter
from class_factory import Factory
import test_factory

class TestMainPresenter(unittest.TestCase):

    def setUp(self):
        Factory.default_factory = test_factory.TestWindowFactory()
        self.presenter = main_presenter.MainPresenter()

    def test_constructor(self):
        assert Factory.default_factory.main_window 
        assert Factory.default_factory.output_window 
        
        self.assertEqual(5, Factory.default_factory.main_window.add_custom_function.call_count)
        
    def test_load_image(self):
        self.presenter.load_image( "test" )
        self.assertEqual(2, Factory.default_factory.output_window.set_src_image.call_count)
        
    def test_file_loaded(self):
        self.presenter.file_loaded( "test" )
        self.assertEqual(2, Factory.default_factory.output_window.set_src_image.call_count)
        
    def test_custom_func_button_clicked(self):
        self.presenter.custom_func_button_clicked( 2 )
        assert Factory.default_factory.func_window 
        
    def test_capture_window_button_clicked(self):
        self.presenter.capture_window_button_clicked()
        assert Factory.default_factory.capture_window 
        
    def test_batch_window_button_clicked(self):
        self.presenter.batch_window_button_clicked()
        assert Factory.default_factory.batch_window 