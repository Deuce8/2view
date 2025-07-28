#include "canvas.h"

#include <QMimeData>
#include <QPalette>
#include <QShortcut>
#include <QKeySequence>
#include <QPainter>
#include <Qt>
#include <QSettings>

#pragma region Constructor

Canvas::Canvas(QWidget *parent) : QLabel(parent) {
    setFocusPolicy(Qt::ClickFocus);
    setWindowFlags(Qt::Window);
    setAcceptDrops(true);
    setWindowTitle(tr("2View"));
    setStyleSheet(QString("background-color: %1").arg(qApp->palette().color(QPalette::Base).name()));

    connect(new QShortcut(QKeySequence(Qt::Key_Space), this), &QShortcut::activated, this, &Canvas::resetView);

    QSettings settings("Deuce8", "2view");
    drawGrid = settings.value("drawGird", true).toBool();
    drawBorder = settings.value("drawBorder", true).toBool();
    gridColor = settings.value("gridColor", "#ffffff").value<QColor>();
    zoomFactor = settings.value("zoomFactor", 1.25).toInt();
    zoomMaximum = settings.value("zoomMaximum", 10).toFloat();
    zoomMinimum = settings.value("zoomMinimum", 0.5f).toFloat();
    margins = settings.value("margins", 32).toInt();
    gridThreshold = settings.value("gridThreshold", 32).toInt();

    //Write default settings
    if (settings.contains("drawGird"))
        return;

    settings.setValue("drawGird", true);
    settings.setValue("drawBorder", true);
    settings.setValue("gridColor", "#ffffff");
    settings.setValue("zoomFactor", 1.25);
    settings.setValue("zoomMaximum", 10);
    settings.setValue("zoomMinimum", 0.5f);
    settings.setValue("margins", 32);
    settings.setValue("gridThreshold", 32);
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

    QPen pen(gridColor);
    pen.setCosmetic(true);
    painter.setPen(pen);

    //Border and Grid
    const int w = pixmap().width();
    const int h = pixmap().height();

    //Border
    if(drawBorder)
        painter.drawLines(QVector<QLine>{
            QLine(0, 0, 0, h),
            QLine(w, 0, w, h),
            QLine(0, 0, w, 0),
            QLine(0, h, w, h)
        });

    //Grid
    if(gridThreshold <= sizePrimary() / zoom || !drawGrid)
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

    zoom *= event->angleDelta().y() > 0 ? zoomFactor : 1 / zoomFactor;
    zoom = std::max(zoomMinimum, std::min(zoom, float(sizePrimary()) / zoomMaximum));

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
    const float scaled_margins = margins * zoomed_scale;

    offset = QPointF((view_w < w) ? (w - view_w) / 2 : std::max(std::min(float(offset.x()), scaled_margins), w - view_w - scaled_margins), 
                     (view_h < h) ? (h - view_h) / 2 : std::max(std::min(float(offset.y()), scaled_margins), h - view_h - scaled_margins));
}

#pragma endregion Private Slots