
import main_window
import output_window
import capture_window
import batch_window
import func_window

class GtkWindowFactory:

    def new_main_window( self, param ):
        return main_window.MainWindow( param )
        
    def new_func_window( self, func_presenter ): 
        return func_window.FuncWindow( func_presenter )
    
    def new_output_window( self, presenter ):
        return output_window.OutputWindow( presenter )
        
    def new_capture_window( self, outputWindow ):
        return capture_window.CaptureWindow( outputWindow )
    
    def new_batch_window( self, presenter ):
        return batch_window.BatchWindow( presenter )
    
        
class Factory:

    default_factory = GtkWindowFactory()
 