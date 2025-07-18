from PyQt6.QtWidgets import QApplication
from PyQt6.QtWidgets import QLabel
from PyQt6.QtGui import QShortcut
from PyQt6.QtGui import QKeySequence
from PyQt6.QtGui import QPalette
from PyQt6.QtGui import QPixmap
from PyQt6.QtCore import Qt

from PyQt6.QtGui import QDragEnterEvent
from PyQt6.QtGui import QDropEvent
from PyQt6.QtGui import QPaintEvent
from PyQt6.QtGui import QMouseEvent
from PyQt6.QtGui import QWheelEvent
from PyQt6.QtGui import QResizeEvent

from project.view import View
from project.draw import Draw

class Canvas(QLabel):
    #region Constructor

    def __init__(self):
        super().__init__()

        self.view = View(self)
        self.draw = Draw(self, self.view)

        self.setFocusPolicy(Qt.FocusPolicy.ClickFocus)
        self.setWindowFlags(Qt.WindowType.Window)
        self.setAcceptDrops(True)
        self.setWindowTitle('2View')
        self.setStyleSheet(f'background-color: {QApplication.instance().palette().color(QPalette.ColorRole.Base).name()}')

        QShortcut(QKeySequence('Space'), self).activated.connect(self.view.resetView)

    #endregion Constructor
    #region Input

    def loadImage(self, filePath: str):
        loadedImage = QPixmap(filePath)
        if loadedImage.isNull():
            return
        
        self.setPixmap(loadedImage)
        self.view.resetView()

    def dragEnterEvent(self, event=QDragEnterEvent):
        if event.mimeData().hasUrls() or event.mimeData().hasImage():
            event.acceptProposedAction()

    def dropEvent(self, event=QDropEvent):
        mimeData = event.mimeData()
        if not mimeData.hasUrls():
            return
        
        urls = mimeData.urls()
        if not urls:
            return
        
        loadedImage = QPixmap(urls[0].toLocalFile())
        if loadedImage.isNull():
            return
        
        self.setPixmap(loadedImage)
        self.view.resetView()

    #endregion Input
    #region Qt Events

    def paintEvent(self, event: QPaintEvent):
        self.draw.paintEvent()

    def mousePressEvent(self, event: QMouseEvent):
        self.view.mousePressEvent(event)

    def mouseReleaseEvent(self, event: QMouseEvent):
        self.view.mouseReleaseEvent(event)
    
    def mouseMoveEvent(self, event: QMouseEvent):
        self.view.mouseMoveEvent(event)
    
    def wheelEvent(self, event: QWheelEvent):
        self.view.wheelEvent(event)

    def resizeEvent(self, event: QResizeEvent):
        self.view.resetView()

    #endregion Qt Events
    #region Getters

    def getImageWidth(self) -> int:
        if self.pixmap().isNull():
            return 1.0
        
        return self.pixmap().width()

    def getImageHeight(self) -> int:
        if self.pixmap().isNull():
            return 1.0
        
        return self.pixmap().height()

    def getSizePrimary(self) -> int:
        if self.pixmap().isNull():
            return 1.0
        
        if self.pixmap().width() > self.pixmap().height():
            return self.pixmap().width()
        
        return self.pixmap().height()


    def getScale(self) -> float:
        if self.pixmap().isNull():
            return 1.0
        
        sx = float(self.width()) / self.pixmap().width()
        sy = float(self.height()) / self.pixmap().height()

        return min(sx, sy)
    
    def getImage(self) -> QPixmap:
        return self.pixmap()
    
    #endregion Getters

