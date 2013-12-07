/**
 * PGR 2013 project
 * Application's main class
 *
 * @author  xskota05 Klara Vankova
 *          xsimet00 Vojtech Simetka
 * @date    2013/11/26
 * @version 1
 * @file    main.cpp
 */
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Creates application
    QApplication a(argc, argv);

    // Show icons in menu
    a.setAttribute(Qt::AA_DontShowIconsInMenus, false);

    // Creates window
    MainWindow w;

    // Displays window
    w.show();

    // Starts application and returns
    return a.exec();
}
