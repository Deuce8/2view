from PyQt6.QtWidgets import QLabel
from PyQt6.QtCore import QLine
from PyQt6.QtGui import QPainter
from PyQt6.QtGui import QColor
from PyQt6.QtGui import QPen

from project.view import View

THRESHOLD = 25

class Draw():
    def __init__(self, canvas: QLabel, view: View):
        self.canvas = canvas
        self.view = view

    def paintEvent(self):
        if self.canvas.getImage().isNull():
            return

        painter = QPainter(self.canvas)
        painter.translate(self.view.getOffset())
        painter.scale(self.view.getZoom() * self.canvas.getScale(), self.view.getZoom() * self.canvas.getScale())
        painter.drawPixmap(0, 0, self.canvas.pixmap())
        
        pen = QPen(QColor(255, 255, 255, 127))
        pen.setCosmetic(True)
        painter.setPen(pen)
        
        self.border(painter)
        if THRESHOLD > self.canvas.getSizePrimary() / self.view.getZoom():
            self.grid(painter)
    
    def grid(self, painter: QPainter):
        w, h = self.canvas.getImageWidth(), self.canvas.getImageHeight()

        lines = [QLine(x, 0, x, h) for x in range(1, w)] + \
                [QLine(0, y, w, y) for y in range(1, h)]

        painter.drawLines(lines)

    def border(self, painter: QPainter):
        w, h = self.canvas.getImageWidth(), self.canvas.getImageHeight()

        painter.drawLines([
            QLine(0, 0, 0, h),
            QLine(w, 0, w, h),
            QLine(0, 0, w, 0),
            QLine(0, h, w, h)
        ])