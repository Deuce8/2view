#include "draw.h"

#include "canvas.h"
#include "view.h"

Draw::Draw(Canvas &canvas, View &view) : canvas(canvas), view(view) {}

void Draw::paintEvent(){
    if(canvas.getImage().isNull())
        return;

    QPainter painter(&canvas);
    painter.translate(view.getOffset());
    painter.scale(view.getZoom() * canvas.getScale(), view.getZoom() * canvas.getScale());
    painter.drawPixmap(0, 0, canvas.getImage());

    QPen pen(QColor(255, 255, 255, 127));
    pen.setCosmetic(true);
    painter.setPen(pen);

    border(painter);
    if(THRESHOLD > canvas.getSizePrimary() / view.getZoom())
        grid(painter);
}

void Draw::grid(QPainter &painter){
    const int w = canvas.getImageWidth();
    const int h = canvas.getImageHeight();

    QVector<QLine> lines;

    for (int x = 1; x < w; x++)
        lines.append(QLine(x, 0, x, h));
    for (int y = 1; y < h; y++)
        lines.append(QLine(0, y, w, y));

    painter.drawLines(lines);
}

void Draw::border(QPainter &painter){
    const int w = canvas.getImageWidth();
    const int h = canvas.getImageHeight();

    painter.drawLines(QVector<QLine>{
        QLine(0, 0, 0, h),
        QLine(w, 0, w, h),
        QLine(0, 0, w, 0),
        QLine(0, h, w, h)
    });
}