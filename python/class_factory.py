
import main_window
import output_window
import capture_window
import batch_window
import func_window

class GtkWindowFactory:

    def new_main_window( self, param ):
        return main_window.MainWindow( param )
        
    def new_func_window( self, function_xml, outputWindow ):
        return func_window.FuncWindow(function_xml, outputWindow)
    
    def new_output_window( self ):
        return output_window.OutputWindow()
        
    def new_capture_window( self, outputWindow ):
        return capture_window.CaptureWindow( outputWindow )
    
    def new_batch_window( self ):
        return batch_window.BatchWindow()
    
        
class Factory:

    default_factory = GtkWindowFactory()
 