#include <QApplication>
#include "mainwidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    app.setStyle("Fusion");
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(230, 230, 230));
    palette.setColor(QPalette::WindowText, Qt::black);
    palette.setColor(QPalette::Base, QColor(255, 255, 255));
    palette.setColor(QPalette::Text, Qt::black);
    palette.setColor(QPalette::Button, QColor(240, 240, 240));
    palette.setColor(QPalette::ButtonText, Qt::black);
    app.setPalette(palette);

    MainWidget widget;
    widget.setWindowTitle("MSDscript");
    widget.resize(700, 500);
    widget.show();

    return app.exec();
}
