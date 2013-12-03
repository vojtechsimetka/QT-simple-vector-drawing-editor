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

OpenGLWidget * MainWindow::opw = NULL;
QLineEdit * MainWindow::lineEdit = NULL;

/**
 * @brief MainWindow constructor
 * @param parent Reference to parent component
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Set's up user interface from an xml file
    ui->setupUi(this);

    // Sets static reference to opengl widget
    MainWindow::opw = ui->widget;

    MainWindow::lineEdit = ui->lineEdit;
    MainWindow::lineEdit->setText("0");
}

/**
 * @brief MainWindow destructor
 */
MainWindow::~MainWindow()
{
    // Deletes user interface
    delete ui;
}

/**
 * @brief Undo clicked
 */
void MainWindow::undo()
{
    // Undo
    ChangesLog::sharedInstance()->undoStep();
}

/**
 * @brief Redo clicked
 */
void MainWindow::redo()
{
    // Redo
    ChangesLog::sharedInstance()->redoStep();
}

/**
 * @brief Select tool selected
 */
void MainWindow::select()
{
    this->ui->widget->setAction(SELECT_E);
}

/**
 * @brief Rotate tool selected
 */
void MainWindow::rotate()
{
    this->ui->widget->setAction(ROTATE);
}

/**
 * @brief Drawline tool selected
 */
void MainWindow::drawLine()
{
    this->ui->widget->setAction(DRAWLINE);
}

/**
 * @brief Delete tool selected
 */
void MainWindow::dlt()
{
    this->ui->widget->setAction(DLT);
    this->ui->widget->deleteSelection();
}


void MainWindow::pan()
{
    this->ui->widget->setAction(PAN);
}

// TODO: REMOVE
void MainWindow::textChanged(QString string)
{
    //this->ui->widget->changeLength(string.toFloat());
}


