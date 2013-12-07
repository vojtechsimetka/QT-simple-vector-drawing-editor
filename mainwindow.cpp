/**
 * PGR 2013 project
 * Application's main window
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/26
 * @version 1
 * @file    mainwindow.cpp
 */
#include "mainwindow.h"

/**
 * @brief MainWindow constructor
 * @param parent Reference to parent component
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Creates GUI
    this->ui = new Gui(this);
}

/**
 * @brief MainWindow destructor
 */
MainWindow::~MainWindow()
{
    // Deletes user interface
    delete ui;
}




