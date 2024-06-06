#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void loadFile(const QString &filePath);
    void saveFile(const QString &filePath);
    ~MainWindow();

private slots:
    void newAction();
    void openAction();
    void saveAction();
    void saveAsAction();
    void undoAction();
    void redoAction();
    void copyAction();
    void cutAction();
    void pasteAction();
    void runAction();

private:
    Ui::MainWindow *ui;
    QString currentFilePath;
};
#endif // MAINWINDOW_H
