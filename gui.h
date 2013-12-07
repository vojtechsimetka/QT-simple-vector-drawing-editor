#ifndef GUI_H
#define GUI_H

#include <QObject>
#include <QtCore>
#include <QtWidgets>
#include "openglwidget.h"

QT_BEGIN_NAMESPACE

class OpenGLWidget;

class Gui: public QObject
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
    Gui(QMainWindow *MainWindow);
    OpenGLWidget * getOpenGLWidget() const;
    void setLineEditText(const QString &);
    QString getLineEditText() const;
    void appendTextToLineEdit(const QString &);
    void deleteLastCharOfLineEdit();
    void setLineLabelText(const QString &);
    void setUndo(bool);
    void setRedo(bool);
    void setDelete(bool);

private:
    QAction *action_undo;
    QAction *action_redo;
    QAction *action_select;
    QAction *action_rotate;
    QAction *action_line;
    QAction *action_delete;
    QAction *action_pan;

    QWidget *central_widget;
    QWidget *footer_widget;
    OpenGLWidget *opengl_widget;

    QVBoxLayout *central_widget_vertical_layout;
    QLabel *line_label;
    QLineEdit *line_edit;
    QMenuBar *menu_bar;
    QToolBar *tool_bar;
    void createActions(QMainWindow *MainWindow);
    void createToolBar(QMainWindow *MainWindow);
    void createCentralWidget(QMainWindow *MainWindow);
    void createOpenGLWidget();
    void createFooter();
    void createMenu(QMainWindow *MainWindow);
};

#endif // GUI_H
