
class PrintStats:

    def printStats( self, func_name, int_stats, double_stats ):
        if func_name == "hEdgesMiddle":
            zero_width = 303.8228493
            factor = 0.222179218

            width = double_stats[6]
            dif_width = (width - zero_width)
            calculed_len = dif_width * factor
        
            print "A and B: {0} {1}".format( double_stats[0], double_stats[1] )
            print "A and B: {0} {1}".format( double_stats[2], double_stats[3] )
            print "Std Devs: {0}, {1}".format( double_stats[4], double_stats[5] )
            print "Width: {0}".format( width )
            print "Num points: {0}, {1}".format( int_stats[0], int_stats[1] )
            print "Calculed len: {0}".format( calculed_len )
            
            