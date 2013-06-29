
import unittest

import sys
sys.path.append('../python')
import output_presenter
from class_factory import Factory
import test_factory

class TestOutputPresenter(unittest.TestCase):

    def setUp(self):
        Factory.default_factory = test_factory.TestWindowFactory()
        self._output_presenter = output_presenter.OutputPresenter()

    def test_constructor(self):
        self.assertEqual(1, self._output_presenter.zoomFactor)
        assert self._output_presenter.outputWindow       
        self.assertEqual(1, Factory.default_factory.output_window.show.call_count)