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

    bool drawGrid;
    bool drawBorder;
    QColor gridColor;
    float zoomFactor;
    float zoomMaximum;
    float zoomMinimum;
    int margins;
    int gridThreshold;

private slots:
    void resetView();
    void clampOffset();
};

#endif