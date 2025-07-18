#ifndef CANVAS_H
#define CANVAS_H

#include <QLabel>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDragEnterEvent>

class Draw;
class View;

class Canvas : public QLabel {
    Q_OBJECT

public:
    Canvas(QWidget *parent = nullptr);

    void loadImage(QString &filepath);

    int getImageWidth() const;
    int getImageHeight() const;
    int getSizePrimary() const;
    float getScale() const;
    QPixmap getImage() const;

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
    Draw* draw;
    View* view;
};

#endif