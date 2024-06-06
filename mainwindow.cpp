#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <fstream>
#include <QProcess>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->plainTextEdit);
    this->setWindowIcon(QIcon(":/imgs/logo.png"));
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openAction);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveAction);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::saveAsAction);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newAction);
    connect(ui->actionUndo, &QAction::triggered, this, &MainWindow::undoAction);
    connect(ui->actionRedo, &QAction::triggered, this, &MainWindow::redoAction);
    connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::copyAction);
    connect(ui->actionCut, &QAction::triggered, this, &MainWindow::cutAction);
    connect(ui->actionPaste, &QAction::triggered, this, &MainWindow::pasteAction);
    connect(ui->actionRun, &QAction::triggered, this, &MainWindow::runAction);
    const int tabStop = 4;  // 4 characters
}

void MainWindow::loadFile(const QString &filePath)
{
//    ui->plainTextEdit->document()->blockSignals(true);
    ifstream file(filePath.toStdString());

    if (!file.is_open()) {
        QMessageBox::warning(this, tr("Recent Files Example"),
                             tr("This file could not be found:\n%1.")
                                 .arg(filePath));
        return;
    }
    currentFilePath = filePath;
    this->setWindowTitle(currentFilePath);
    stringstream buffer;
    buffer << file.rdbuf();
    ui->plainTextEdit->document()->setPlainText(QString::fromStdString(buffer.str()));
//    ui->plainTextEdit->document()->blockSignals(false);
}

void MainWindow::saveFile(const QString &filePath)
{
    ofstream file(filePath.toStdString());

    if (!file.is_open()) {
        QMessageBox::warning(this, tr("Recent Files Example"),
                             tr("This file could not be found:\n%1.")
                                 .arg(filePath));
        return;
    }

    file << ui->plainTextEdit->toPlainText().toStdString();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newAction()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "New file", "Do you want to open file this window?",
                                            QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        reply = QMessageBox::question(this, "Save", "Do you want to save file?",
                                      QMessageBox::Yes|QMessageBox::No);

        if(reply == QMessageBox::Yes) saveAction();

        currentFilePath.clear();
        this->setWindowTitle("CreatorC");
        ui->plainTextEdit->document()->blockSignals(true);
        ui->plainTextEdit->clear();
        ui->plainTextEdit->document()->blockSignals(false);
    }
    else
    {
        MainWindow *other = new MainWindow;
        other->show();
    }
}

void MainWindow::openAction()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Open file", "Do you want to open file this window?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        reply = QMessageBox::question(this, "Save", "Do you want to save file?",
                                      QMessageBox::Yes|QMessageBox::No);

        if(reply == QMessageBox::Yes) saveAction();

        QString filePath = QFileDialog::getOpenFileName(
            this, tr("Open File"), "",
            tr("All Files (*.*)"));
        if(!filePath.isEmpty())
        {
            loadFile(filePath);
        }
    }
    else
    {
    MainWindow *other = new MainWindow;
    other->show();
    }
}

void MainWindow::saveAction()
{
    if(currentFilePath.isEmpty())
        saveAsAction();
    else
        saveFile(currentFilePath);
}

void MainWindow::saveAsAction()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return;
    if(!fileName.endsWith(".c") && !fileName.endsWith(".h"))
        fileName.push_back(".c");

    currentFilePath = fileName;
    this->setWindowTitle(currentFilePath);
    saveFile(fileName);
}

void MainWindow::undoAction()
{
    try{
        if(ui->plainTextEdit->undoPos != ui->plainTextEdit->undoRedo.end())
        {
            ui->plainTextEdit->undoPos->undo();
            ui->plainTextEdit->undoPos++;
        }
    }
    catch(StackException)
    {
        QMessageBox::warning(this, "Oops","Something went wrong, check file for errors");
    }
}

void MainWindow::redoAction()
{
    try {
        if(ui->plainTextEdit->undoPos != ui->plainTextEdit->undoRedo.begin())
        {
            ui->plainTextEdit->undoPos--;
            ui->plainTextEdit->undoPos->redo();
        }
    }
    catch (StackException)
    {
        QMessageBox::warning(this, "Oops","Something went wrong, check file for errors");
    }
}

void MainWindow::copyAction()
{
    ui->plainTextEdit->copy();
}

void MainWindow::cutAction()
{
    ui->plainTextEdit->cut();
}

void MainWindow::pasteAction()
{
    ui->plainTextEdit->paste();
}

void MainWindow::runAction()
{
    saveAction();
    QString outputFile;
    if(!currentFilePath.isEmpty() && (currentFilePath.endsWith(".c") ||
                                       currentFilePath.endsWith(".h")))
    {
        outputFile = currentFilePath.mid(0, currentFilePath.size()-2);
        outputFile+=".exe";
        // Компиляция
        if(system(string{"gcc -o " + outputFile.toStdString() + " " + currentFilePath.toStdString()}.c_str()) != 0)
        {
            system(string{"start cmd /K gcc -o " + outputFile.toStdString() + " " + currentFilePath.toStdString()}.c_str());
            QMessageBox::warning(this, "Oops","Something went wrong, check file for errors");
            system("exit");
        }
        else system(outputFile.toStdString().c_str());
    }
    else
    {
            QMessageBox::warning(this, "Wrong file name","Can`t compile this file,"
                                                      " check if it has .h/.c extension");
    }
}
