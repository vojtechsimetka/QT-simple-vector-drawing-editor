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


void MainWindow::textChanged(QString string)
{
    //this->ui->widget->changeLength(string.toFloat());
}

/**
 * @brief Keyboard pressed event handler
 * @param keyEvent Reference to event descriptor
 */
void MainWindow::keyPressEvent(QKeyEvent *keyEvent)
{
    if (this->ui->lineEdit->text() == "0")
        this->ui->lineEdit->setText("");

    // Key was pressed
    switch(keyEvent->key())
    {
    case Qt::Key_0:
    case Qt::Key_Launch0:
        this->ui->lineEdit->setText(this->ui->lineEdit->text() + "0");
        break;

    case Qt::Key_1:
    case Qt::Key_Launch1:
        this->ui->lineEdit->setText(this->ui->lineEdit->text() + "1");
        break;

    case Qt::Key_2:
    case Qt::Key_Launch2:
        this->ui->lineEdit->setText(this->ui->lineEdit->text() + "2");
        break;

    case Qt::Key_3:
    case Qt::Key_Launch3:
        this->ui->lineEdit->setText(this->ui->lineEdit->text() + "3");
        break;

    case Qt::Key_4:
    case Qt::Key_Launch4:
        this->ui->lineEdit->setText(this->ui->lineEdit->text() + "4");
        break;

    case Qt::Key_5:
    case Qt::Key_Launch5:
        this->ui->lineEdit->setText(this->ui->lineEdit->text() + "5");
        break;

    case Qt::Key_6:
    case Qt::Key_Launch6:
        this->ui->lineEdit->setText(this->ui->lineEdit->text() + "6");
        break;

    case Qt::Key_7:
    case Qt::Key_Launch7:
        this->ui->lineEdit->setText(this->ui->lineEdit->text() + "7");
        break;

    case Qt::Key_8:
    case Qt::Key_Launch8:
        this->ui->lineEdit->setText(this->ui->lineEdit->text() + "8");
        break;

    case Qt::Key_9:
    case Qt::Key_Launch9:
        this->ui->lineEdit->setText(this->ui->lineEdit->text() + "9");
        break;

    case Qt::Key_Backspace:
    case Qt::Key_Delete:
    case Qt::Key_Back:
    case Qt::Key_B:
        this->ui->lineEdit->setText(this->ui->lineEdit->text().remove(this->ui->lineEdit->text().count()-1, 1));
        if (this->ui->lineEdit->text().count() == 0)
            this->ui->lineEdit->setText("0");
        break;

    case Qt::Key_Period:
    case Qt::Key_Comma:
        if (MainWindow::lineEdit->text().contains("."))
            break;

        this->ui->lineEdit->setText(this->ui->lineEdit->text() + ".");
        break;
    }
}
