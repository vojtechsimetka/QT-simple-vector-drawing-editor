/**
 * PGR 2013 project
 * Application's main window header file
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/26
 * @version 1
 * @file    mainwindow.h
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "openglwidget.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void undo();
    void redo();
    void select();
    void rotate();
    void drawLine();
    void dlt();
    void pan();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static OpenGLWidget *opw;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
