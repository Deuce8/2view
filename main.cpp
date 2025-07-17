#include <QApplication>
#include <Qt>
#include <QFileInfo>

#include "canvas.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Canvas canvas;

    if (argc > 1) {
        QString filePath = QString::fromLocal8Bit(argv[1]);
        if (QFileInfo::exists(filePath))
            canvas.loadImage(filePath);
    }

    canvas.show();
    return app.exec();
}