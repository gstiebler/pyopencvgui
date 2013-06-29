
import pygtk
pygtk.require('2.0')
import gtk

import main_presenter

def main():
    gtk.main()
    return 0

if __name__ == "__main__":
    main_presenter.MainPresenter()
    main()
