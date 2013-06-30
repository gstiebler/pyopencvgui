
import unittest
import sys
import os
from ctypes import *

sys.path.append('../python')
import batch_presenter
from class_factory import Factory
import test_factory
import mock

class TestBatchPresenter(unittest.TestCase):

    def setUp(self):
        Factory.default_factory = test_factory.TestWindowFactory()
        self._presenter = batch_presenter.BatchPresenter()
        
    def test_constructor( self ):
        assert Factory.default_factory.batch_window
        self.assertEqual(5, Factory.default_factory.batch_window.add_custom_function.call_count)
        
    def test_ok_clicked( self ):
        patcher = mock.patch('func_presenter.FuncPresenter')
        MockClass = patcher.start()
        instance = MockClass.return_value
        TenIntegersType = c_int * 10
        TenDoublesType = c_double * 10
        int_stats = TenIntegersType(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)
        double_stats = TenDoublesType(0, 1, 2, 3, 4, 5, 6, 7, 8, 9)
        instance.execute.return_value = {'dest_image': 1, 'int_stats': int_stats, 'double_stats': double_stats}
    
        self._presenter.func_selected( None )
        patcher.stop()
        
        base_path = os.path.dirname(__file__) + "/data"
        stats_file_path = os.path.dirname(__file__) + "/data/stats.csv"
        
        self._presenter.ok_clicked( base_path, stats_file_path )
        
        stats_file = open(stats_file_path, "r")
        content_stats_file = stats_file.readlines()
        self.assertEqual( 4, len( content_stats_file ) )
        stats_file.close()
        os.remove( stats_file_path )
        
    