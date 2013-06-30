
import sys
sys.path.append('../python')

from mock import MagicMock

class DummyTestClass:
    pass

class TestWindowFactory:

    def new_main_window( self, param ):
        self.main_window = MagicMock() 
        return self.main_window
        
    def new_func_window( self, presenter ):
        self.func_window = MagicMock() 
        return self.func_window
    
    def new_output_window( self, presenter ):
        self.output_window = MagicMock() 
        return self.output_window
        
    def new_capture_window( self, outputWindow ):
        self.capture_window = MagicMock() 
        return self.capture_window
    
    def new_batch_window( self ):
        self.batch_window = MagicMock() 
        return self.batch_window
