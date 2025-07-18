#include "canvas.h"

#include <QMimeData>
#include <QPalette>
#include <QString>
#include <QShortcut>
#include <QKeySequence>
#include <Qt>

#include "draw.h"
#include "view.h"

#pragma region Constructor

Canvas::Canvas(QWidget *parent) : QLabel(parent) {
    view = new View(*this);
    draw = new Draw(*this, *view);

    setFocusPolicy(Qt::ClickFocus);
    setWindowFlags(Qt::Window);
    setAcceptDrops(true);
    setWindowTitle(tr("2View"));
    setStyleSheet(QString("background-color: %1").arg(qApp->palette().color(QPalette::Base).name()));

    connect(new QShortcut(QKeySequence(Qt::Key_Space), this), &QShortcut::activated, this, [this]() { this->view->resetView(); });
}

#pragma endregion Constructor
#pragma region Input

void Canvas::loadImage(QString &filepath){
    const QPixmap loadedImage(filepath);
    if (loadedImage.isNull())
        return;

    setPixmap(loadedImage);
    view->resetView();
}

void Canvas::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls() || event->mimeData()->hasImage())
        event->acceptProposedAction();
}

void Canvas::dropEvent(QDropEvent *event) {
    const QMimeData *mimeData = event->mimeData();
    if (!mimeData->hasUrls()) 
        return;

    const QList<QUrl> urls = mimeData->urls();
    if(urls.isEmpty())
        return;
    
    const QPixmap loadedImage = QPixmap(urls.first().toLocalFile());
    if(loadedImage.isNull())
        return;

    setPixmap(loadedImage);
    view->resetView();
}

#pragma endregion Input
#pragma region Qt Events

void Canvas::paintEvent(QPaintEvent *) {
    draw->paintEvent();
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    view->mousePressEvent(*event);
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    view->mouseReleaseEvent(*event);
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    view->mouseMoveEvent(*event);
}

void Canvas::wheelEvent(QWheelEvent *event) {
    view->wheelEvent(*event);
}

void Canvas::resizeEvent(QResizeEvent *){
    view->resetView();
}

#pragma endregion Qt Events
#pragma region Getters

int Canvas::getImageWidth() const {
    if(pixmap().isNull())
        return 1.0;
        
    return pixmap().width();
}

int Canvas::getImageHeight() const {
    if(pixmap().isNull())
        return 1.0;

    return pixmap().height();
}

int Canvas::getSizePrimary() const {
    if(pixmap().isNull())
        return 1.0;

    if(pixmap().width() > pixmap().height())
        return pixmap().width();
        
    return pixmap().height();
}

float Canvas::getScale() const {
    if(pixmap().isNull())
        return 1.0;

    float sx = static_cast<float>(width()) / pixmap().width();
    float sy = static_cast<float>(height()) / pixmap().height();

    return std::min(sx, sy);
}

QPixmap Canvas::getImage() const {
    return pixmap();
}

#pragma endregion Getters