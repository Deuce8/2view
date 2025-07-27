#ifndef CANVAS_H
#define CANVAS_H

#include <QLabel>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDragEnterEvent>

class Canvas : public QLabel {
    Q_OBJECT

public:
    Canvas(QWidget *parent = nullptr);

    void loadImage(const QString &filepath);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    float zoom;
    bool panning;
    QPointF offset;
    QPointF last_point;

    int sizePrimary() const;
    float scale() const;

    static constexpr float ZOOM_FACTOR = 1.25f;
    static constexpr float ZOOM_MAX = 10;
    static constexpr float ZOOM_MIN = 0.5f;
    static constexpr int MARGINS = 16;
    static constexpr int GRID_THRESHOLD = 25;

private slots:
    void resetView();
    void clampOffset();
};

#endif