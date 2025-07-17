#ifndef DRAW_H
#define DRAW_H

#include <QPainter>

class Canvas;
class View;

class Draw {
public:
    Draw(Canvas &canvas, View &view);
    void paintEvent();

private:
    void grid(QPainter &painter);
    void border(QPainter &painter);

    View &view;
    Canvas &canvas;

    static constexpr int THRESHOLD = 25;
};

#endif