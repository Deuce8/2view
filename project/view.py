from PyQt6.QtWidgets import QLabel
from PyQt6.QtCore import QPointF
from PyQt6.QtCore import Qt

from PyQt6.QtGui import QMouseEvent
from PyQt6.QtGui import QWheelEvent

ZOOM_FACTOR = 1.25
ZOOM_MIN = 0.5
ZOOM_MAX = 10
MARGINS = 16

class View():
    #region Constructor

    def __init__(self, canvas: QLabel):
        self.canvas = canvas

        self.zoom = 1
        self.panning = False
        self.offset = QPointF(0, 0)
        self.last_point = QPointF(0, 0)

    #endregion Constructor
    #region Qt Events

    def mousePressEvent(self, event: QMouseEvent):
        if event.button() != Qt.MouseButton.MiddleButton:
            return
        
        self.panning = True
        self.last_point = event.pos()

    def mouseReleaseEvent(self, event: QMouseEvent):
        if event.button() != Qt.MouseButton.MiddleButton:
            return
        
        self.panning = False

    def mouseMoveEvent(self, event: QMouseEvent):
        if self.panning is False:
            return
        
        self.offset += QPointF(event.pos()) - QPointF(self.last_point)
        self.last_point = event.pos()

        self.clampOffset()
        self.canvas.update()

    def wheelEvent(self, event: QWheelEvent):
        old_zoom = self.zoom

        self.zoom *= ZOOM_FACTOR if event.angleDelta().y() > 0 else 1 / ZOOM_FACTOR
        self.zoom = max(ZOOM_MIN, min(self.zoom, self.canvas.getSizePrimary() / ZOOM_MAX))

        self.offset = (self.offset - event.position()) * (self.zoom / old_zoom) + event.position()

        self.clampOffset()
        self.canvas.update()

    #endregion Qt Events
    #region Helper Functions

    def resetView(self):
        self.zoom = 1
        self.panning = False
        self.offset = QPointF(0, 0)
        self.last_point = QPointF(0, 0)
        self.clampOffset()
        self.canvas.update()

    def clampOffset(self):
        w, h = self.canvas.width(), self.canvas.height()
        zoomed_scale = self.zoom * self.canvas.getScale()
        view_w = self.canvas.getImageWidth() * zoomed_scale
        view_h = self.canvas.getImageHeight() * zoomed_scale
        margins = MARGINS * zoomed_scale

        self.offset = QPointF((w - view_w) / 2 if view_w < w else max(min(self.offset.x(), margins), w - view_w - margins), 
                              (h - view_h) / 2 if view_h < h else max(min(self.offset.y(), margins), h - view_h - margins))

    #endregion Helper Functions
    #region Getters

    def getZoom(self) -> float:
        return self.zoom

    def getOffset(self) -> QPointF:
        return self.offset
    
    #endregion Getters