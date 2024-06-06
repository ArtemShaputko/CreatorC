#include "mainwindow.h"
#include <QApplication>
#include <QtCore5Compat/QTextCodec>

int main(int argc, char *argv[])
{
    setlocale(LC_CTYPE, "ru_RU.UTF-8");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
