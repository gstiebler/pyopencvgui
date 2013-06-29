
import unittest

import sys
sys.path.append('../python')
import output_presenter
from class_factory import Factory
import test_factory
import os
import cv2

class TestOutputPresenter(unittest.TestCase):

    def setUp(self):
        Factory.default_factory = test_factory.TestWindowFactory()
        self._output_presenter = output_presenter.OutputPresenter()
        
    def loadImage(self):
        file_path = os.path.dirname(__file__) + "/data/dj.bmp"
        return cv2.imread(file_path, cv2.CV_LOAD_IMAGE_COLOR)

    def test_constructor(self):
        self.assertEqual(1, self._output_presenter.zoomFactor)
        assert self._output_presenter.outputWindow       

    def test_setCurrentImage(self):
        srcImage = self.loadImage()
        self._output_presenter.setCurrentImage( srcImage )
        self.assertEqual(1, Factory.default_factory.output_window.show_image.call_count)
        
    def test_set_src_image(self):
        srcImage = self.loadImage()
        self._output_presenter.set_src_image( srcImage )
        self.assertEqual(1, Factory.default_factory.output_window.show_image.call_count)

    def test_zoom_changed(self):
        self._output_presenter.setCurrentImage( self.loadImage() )
        self._output_presenter.zoom_changed( 4 )
        self.assertEqual(2, Factory.default_factory.output_window.show_image.call_count)

    def test_spin_changed(self):
        self._output_presenter.setCurrentImage( self.loadImage() )
        self._output_presenter.spin_changed( 4, 5 )
        self.assertEqual(2, Factory.default_factory.output_window.show_image.call_count)

    def test_mouse_moved(self):
        self._output_presenter.setCurrentImage( self.loadImage() )
        self._output_presenter.mouse_moved( 4, 5 )
        self.assertEqual(1, Factory.default_factory.output_window.set_info_label.call_count)
        