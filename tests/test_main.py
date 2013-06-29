
import unittest

import sys
sys.path.append('../python')
import main_presenter

class TestMainPresenter(unittest.TestCase):

    def setUp(self):
        presenter = main_presenter.MainPresenter()

    def test_basic(self):
        presenter = main_presenter.MainPresenter()
        
