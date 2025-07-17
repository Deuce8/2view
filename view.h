#ifndef VIEW_H
#define VIEW_H

#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>

class Canvas;

class View {
public:
    View(Canvas &canvas);

    void mousePressEvent(QMouseEvent &event);
    void mouseReleaseEvent(QMouseEvent &event);
    void mouseMoveEvent(QMouseEvent &event);
    void wheelEvent(QWheelEvent &event);
    void resetView();
    void clampOffset();

    float getZoom() const;
    QPointF getOffset() const;

private:
    Canvas &canvas;

    float zoom;
    bool panning;
    QPointF offset;
    QPointF last_point;

    static constexpr float ZOOM_FACTOR = 1.25f;
    static constexpr float ZOOM_MAX = 10;
    static constexpr float ZOOM_MIN = 0.5f;
    static constexpr int MARGINS = 16;
};

#endif