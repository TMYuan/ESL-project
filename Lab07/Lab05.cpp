#include "widget.h"
#include <QApplication>
#include <QPushButton>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PainterWidget *widget=new PainterWidget();
    widget->setGeometry(0,0,800,400);

    widget->setWindowTitle("Demo");


    widget->show();

    return a.exec();
}
