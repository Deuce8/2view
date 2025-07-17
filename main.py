import sys

from PyQt6.QtWidgets import QApplication
from PyQt6.QtCore import QFileInfo

from project.canvas import Canvas

if __name__ == "__main__":
    app = QApplication(sys.argv)
    canvas = Canvas()

    if len(sys.argv) > 1:
        filePath = sys.argv[1]
        if QFileInfo.exists(filePath):
            canvas.loadImage(filePath)
    
    canvas.show()
    app.exec()
