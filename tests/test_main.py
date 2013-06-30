
import unittest

import sys
sys.path.append('../python')
import main_presenter
from class_factory import Factory
import test_factory
import mock


class TestMainPresenter(unittest.TestCase):

    def setUp(self):
        Factory.default_factory = test_factory.TestWindowFactory()
        self.output_presenter_patcher = mock.patch('output_presenter.OutputPresenter')
        self.func_presenter_patcher = mock.patch('func_presenter.FuncPresenter')
        self.output_presenter_patcher.start()
        self.func_presenter_patcher.start()
        self._presenter = main_presenter.MainPresenter()
        
    def tearDown(self):
        self.output_presenter_patcher.stop()
        self.func_presenter_patcher.stop()

    def test_constructor(self):
        assert Factory.default_factory.main_window 
        assert main_presenter.output_presenter
        
        self.assertEqual(5, Factory.default_factory.main_window.add_custom_function.call_count)
        
    def test_load_image(self):
        self._presenter.load_image( "test" )
        self.assertEqual(2, self._presenter._output_presenter.set_src_image.call_count)
        
    def test_file_loaded(self):
        self._presenter.file_loaded( "test" )
        self.assertEqual(2, self._presenter._output_presenter.set_src_image.call_count)
        
    def test_custom_func_button_clicked(self):
        self._presenter.custom_func_button_clicked( 2 )
        
    def test_capture_window_button_clicked(self):
        self._presenter.capture_window_button_clicked()
        assert Factory.default_factory.capture_window 
        
    def test_batch_window_button_clicked(self):
        self._presenter.batch_window_button_clicked()
        assert Factory.default_factory.batch_window 