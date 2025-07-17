#include "view.h"

#include <algorithm>
#include <Qt>

#include "canvas.h"

#pragma region Constructor

View::View(Canvas &canvas) : canvas(canvas) {
    zoom = 1;
    panning = false;
    offset = QPointF(0, 0);
    last_point = QPointF(0, 0);
}

#pragma endregion Constructor
#pragma region Qt Events

void View::mousePressEvent(QMouseEvent &event){
    if (event.button() != Qt::MiddleButton)
        return;
     
    panning = true;
    last_point = event.pos();
}

void View::mouseReleaseEvent(QMouseEvent &event){
    if (event.button() != Qt::MiddleButton)
        return;
     
    panning = false;
}

void View::mouseMoveEvent(QMouseEvent &event){
    if(!panning)
        return;

    offset += event.pos() - last_point;
    last_point = event.pos();

    clampOffset();
    canvas.update();
}

void View::wheelEvent(QWheelEvent &event){
    const float old_zoom = zoom;

    zoom *= event.angleDelta().y() > 0 ? ZOOM_FACTOR : 1 / ZOOM_FACTOR;
    zoom = std::max(ZOOM_MIN, std::min(zoom, float(canvas.getSizePrimary()) / ZOOM_MAX));

    offset = (offset - event.position()) * (zoom / old_zoom) + event.position();

    clampOffset();
    canvas.update();
}

#pragma endregion Qt Events
#pragma region Helper Functions

void View::resetView(){
    zoom = 1;
    panning = false;
    offset = QPointF(0, 0);
    last_point = QPointF(0, 0);

    clampOffset();
    canvas.update();
}

void View::clampOffset(){
    const float w = canvas.width();
    const float h = canvas.height();
    const float zoomed_scale = zoom * canvas.getScale();
    const float view_w = canvas.getImageWidth() * zoomed_scale;
    const float view_h = canvas.getImageHeight() * zoomed_scale;
    const float margins = MARGINS * zoomed_scale;

    offset = QPointF((view_w < w) ? (w - view_w) / 2 : std::max(std::min(float(offset.x()), margins), w - view_w - margins), 
                     (view_h < h) ? (h - view_h) / 2 : std::max(std::min(float(offset.y()), margins), h - view_h - margins));
}

#pragma endregion Helper Functions
#pragma region Getters

float View::getZoom() const {
    return zoom;
}

QPointF View::getOffset() const {
    return offset;
}

#pragma endregion Getters