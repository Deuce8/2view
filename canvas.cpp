#include "canvas.h"

#include <QMimeData>
#include <QPalette>
#include <QShortcut>
#include <QKeySequence>
#include <QPainter>
#include <Qt>

#pragma region Constructor

Canvas::Canvas(QWidget *parent) : QLabel(parent) {
    setFocusPolicy(Qt::ClickFocus);
    setWindowFlags(Qt::Window);
    setAcceptDrops(true);
    setWindowTitle(tr("2View"));
    setStyleSheet(QString("background-color: %1").arg(qApp->palette().color(QPalette::Base).name()));

    connect(new QShortcut(QKeySequence(Qt::Key_Space), this), &QShortcut::activated, this, &Canvas::resetView);
}

#pragma endregion Constructor
#pragma region Public Functions

void Canvas::loadImage(const QString &filepath){
    const QPixmap loadedImage(filepath);
    if (loadedImage.isNull())
        return;

    setPixmap(loadedImage);
    resetView();
}

#pragma endregion Public Functions
#pragma region Protected

void Canvas::paintEvent(QPaintEvent *) {
    if(pixmap().isNull())
        return;

    QPainter painter(this);
    painter.translate(offset);
    painter.scale(zoom * scale(), zoom * scale());
    painter.drawPixmap(0, 0, pixmap());

    QPen pen(QColor(255, 255, 255, 127));
    pen.setCosmetic(true);
    painter.setPen(pen);

    //Border and Grid
    const int w = pixmap().width();
    const int h = pixmap().height();

    //Border
    painter.drawLines(QVector<QLine>{
        QLine(0, 0, 0, h),
        QLine(w, 0, w, h),
        QLine(0, 0, w, 0),
        QLine(0, h, w, h)
    });

    //Grid
    if(GRID_THRESHOLD <= sizePrimary() / zoom)
        return;

    QVector<QLine> lines;

    for (int x = 1; x < w; x++)
        lines.append(QLine(x, 0, x, h));
    for (int y = 1; y < h; y++)
        lines.append(QLine(0, y, w, y));

    painter.drawLines(lines);
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    if (event->button() != Qt::MiddleButton)
        return;
     
    panning = true;
    last_point = event->position();
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() != Qt::MiddleButton)
        return;
     
    panning = false;
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    if(!panning)
        return;

    offset += event->position() - last_point;
    last_point = event->position();

    clampOffset();
    update();
}

void Canvas::wheelEvent(QWheelEvent *event) {
    const float old_zoom = zoom;

    zoom *= event->angleDelta().y() > 0 ? ZOOM_FACTOR : 1 / ZOOM_FACTOR;
    zoom = std::max(ZOOM_MIN, std::min(zoom, float(sizePrimary()) / ZOOM_MAX));

    offset = (offset - event->position()) * (zoom / old_zoom) + event->position();

    clampOffset();
    update();
}

void Canvas::resizeEvent(QResizeEvent *){
    resetView();
}

void Canvas::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void Canvas::dropEvent(QDropEvent *event) {
    const QMimeData *mimeData = event->mimeData();
    if (!mimeData->hasUrls()) 
        return;

    const QList<QUrl> urls = mimeData->urls();
    if(urls.isEmpty())
        return;
    
    loadImage(urls.first().toLocalFile());
}

#pragma endregion Protected
#pragma region Private Functions

int Canvas::sizePrimary() const {
    if(pixmap().isNull())
        return 0.0;

    return std::max(pixmap().width(), pixmap().height());
}

float Canvas::scale() const {
    if(pixmap().isNull())
        return 0.0;

    float sx = static_cast<float>(width()) / pixmap().width();
    float sy = static_cast<float>(height()) / pixmap().height();

    return std::min(sx, sy);
}

#pragma endregion Private Functions
#pragma region Private Slots

void Canvas::resetView(){
    zoom = 1;
    panning = false;
    offset = QPointF(0, 0);
    last_point = QPointF(0, 0);

    clampOffset();
    update();
}

void Canvas::clampOffset(){
    const float w = width();
    const float h = height();
    const float zoomed_scale = zoom * scale();
    const float view_w = pixmap().width() * zoomed_scale;
    const float view_h = pixmap().height() * zoomed_scale;
    const float margins = MARGINS * zoomed_scale;

    offset = QPointF((view_w < w) ? (w - view_w) / 2 : std::max(std::min(float(offset.x()), margins), w - view_w - margins), 
                     (view_h < h) ? (h - view_h) / 2 : std::max(std::min(float(offset.y()), margins), h - view_h - margins));
}

#pragma endregion Private Slots