#include "gui.h"
#include "changeslog.h"

Gui::Gui(QMainWindow *MainWindow)
{
    // Sets size of the main window
    MainWindow->resize(500, 500);
    MainWindow->setMinimumSize(QSize(500, 500));

    // Sets title of the main window
    MainWindow->setWindowTitle("PGR Projekt SketchUp");

    // Create toolbar
    this->createToolBar(MainWindow);

    // Creates cental widget
    this->createCentralWidget(MainWindow);

    // Creates OpenGL widget
    this->createOpenGLWidget();

    // Creates footer
    this->createFooter();

    // Adds widgets to central widget
    central_widget_vertical_layout->addWidget(opengl_widget);
    central_widget_vertical_layout->addWidget(footer_widget);

    // Creates menu
    this->createMenu(MainWindow);
}

/**
 * @brief Creates ToolBar's actions
 * @param MainWindow Reference to parent component
 */
void Gui::createActions(QMainWindow *MainWindow)
{
    // Undo action
    this->action_undo = new QAction(MainWindow);
    this->action_undo->setText("Undo");
    this->action_undo->setIcon(QIcon(":/icons/images/undo.png"));
    this->action_undo->setEnabled(false);
	this->action_undo->setShortcut(tr("Ctrl+Z"));

    // Redo action
    this->action_redo = new QAction(MainWindow);
    this->action_redo->setText("Redo");
    this->action_redo->setIcon(QIcon(":/icons/images/redo.png"));
    this->action_redo->setEnabled(false);
	this->action_redo->setShortcut(tr("Ctrl+Shift+Z"));

    // Select action
    this->action_select = new QAction(MainWindow);
    this->action_select->setText("Select");
    this->action_select->setIcon(QIcon(":/icons/images/left_ptr.png"));
	this->action_select->setShortcut(tr("Ctrl+Shift+S"));

//    // Rotate action
//    this->action_rotate = new QAction(MainWindow);
//    this->action_rotate->setText("Rotate");
//    this->action_rotate->setIcon(QIcon(":/icons/images/top_right_corner.png"));

    // Draw line action
    this->action_line = new QAction(MainWindow);
    this->action_line->setText("Line");
    this->action_line->setIcon(QIcon(":/icons/images/line.png"));
	this->action_line->setShortcut(tr("Ctrl+L"));

    // Delete action
    this->action_delete = new QAction(MainWindow);
    this->action_delete->setText("Delete");
    this->action_delete->setIcon(QIcon(":/icons/images/delete.png"));
    this->action_delete->setEnabled(false);
	this->action_delete->setShortcut(tr("Ctrl+D"));

    // Pan action
    this->action_pan = new QAction(MainWindow);
    this->action_pan->setText("Pan");
    this->action_pan->setIcon(QIcon(":/icons/images/hand1.png"));
}

/**
 * @brief Creates ToolBar and adds it to parent window
 * @param MainWindow Parent window of toolbar
 */
void Gui::createToolBar(QMainWindow *MainWindow)
{
    // Creates and adds toolbar to the mainwindow
    this->tool_bar = new QToolBar(MainWindow);
    MainWindow->addToolBar(Qt::TopToolBarArea, this->tool_bar);
    this->tool_bar->setAutoFillBackground(true);

    // Creates actions
    this->createActions(MainWindow);

    // Adds actions to the toolbar
    this->tool_bar->addAction(this->action_select);
//    this->tool_bar->addAction(this->action_rotate);
    this->tool_bar->addAction(this->action_pan);
    this->tool_bar->addAction(this->action_delete);
    this->tool_bar->addSeparator();
    this->tool_bar->addAction(this->action_undo);
    this->tool_bar->addAction(this->action_redo);
    this->tool_bar->addSeparator();
    this->tool_bar->addAction(this->action_line);

    // Connects actions' triggered signals to slots
    QObject::connect(this->action_undo,   SIGNAL(triggered()), this, SLOT(undo()));
    QObject::connect(this->action_redo,   SIGNAL(triggered()), this, SLOT(redo()));
    QObject::connect(this->action_select, SIGNAL(triggered()), this, SLOT(select()));
//    QObject::connect(this->action_rotate, SIGNAL(triggered()), this, SLOT(rotate()));
    QObject::connect(this->action_line,   SIGNAL(triggered()), this, SLOT(drawLine()));
    QObject::connect(this->action_delete, SIGNAL(triggered()), this, SLOT(dlt()));
    QObject::connect(this->action_pan,    SIGNAL(triggered()), this, SLOT(pan()));
}

/**
 * @brief Creates central widget used for adding other components
 * @param MainWindow Parent of the central widget
 */
void Gui::createCentralWidget(QMainWindow *MainWindow)
{
    // Creates central widget
    this->central_widget = new QWidget(MainWindow);

    // Creates vertical layout for the widget
    this->central_widget_vertical_layout = new QVBoxLayout(central_widget);
    this->central_widget_vertical_layout->setSpacing(0);
    this->central_widget_vertical_layout->setContentsMargins(0,0,0,0);

    // Sets the central widget for the parent
    MainWindow->setCentralWidget(this->central_widget);
}

/**
 * @brief Creates OpenGL Widget
 */
void Gui::createOpenGLWidget()
{
    // Creates widget
    this->opengl_widget = new OpenGLWidget(this, this->central_widget);
    this->opengl_widget->setObjectName(QStringLiteral("widget"));
}

/**
 * @brief Creates footer
 */
void Gui::createFooter()
{
    // Creates footer widget
    this->footer_widget = new QWidget(this->central_widget);
    this->footer_widget->setFixedHeight(30);

    // Creates horizontal layout in footer
    QHBoxLayout *layout = new QHBoxLayout(this->footer_widget);
    layout->setSpacing(6);
    layout->setContentsMargins(11, 11, 11, 11);
    layout->setContentsMargins(5, 5, 5, 5);

    // Creates label for the line edit
    this->line_label = new QLabel(this->footer_widget);
    this->line_label->setText("Length:");
    this->line_label->setEnabled(false);

    // Creates line edit
    this->line_edit = new QLineEdit(this->footer_widget);
    this->line_edit->setEnabled(true);
    this->line_edit->setFixedSize(QSize(100, 20));
    this->line_edit->setInputMethodHints(Qt::ImhDigitsOnly);
    this->line_edit->setReadOnly(true);

    // Adds spacer, label and line edit to the footer
    layout->addItem(new QSpacerItem(321, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    layout->addWidget(this->line_label);
    layout->addWidget(this->line_edit);
}

/**
 * @brief Creates menu for the window
 * @param MainWindow Window reference
 */
void Gui::createMenu(QMainWindow *MainWindow)
{
    this->menu_bar = new QMenuBar(MainWindow);
    MainWindow->setMenuBar(this->menu_bar);
}

/**
 * @brief Undo clicked
 */
void Gui::undo()
{
    // Undo action
    ChangesLog::sharedInstance()->undoStep();

    // Tests if there is something that can be redone or undone
    this->setUndo(ChangesLog::sharedInstance()->canUndo());
    this->setRedo(ChangesLog::sharedInstance()->canRedo());

    // Repaints OpenGL widget to display changes
    this->opengl_widget->repaint();
}

/**
 * @brief Redo clicked
 */
void Gui::redo()
{
    // Redo action
    ChangesLog::sharedInstance()->redoStep();

    // Tests if there is something that can be redone or undone
    this->setUndo(ChangesLog::sharedInstance()->canUndo());
    this->setRedo(ChangesLog::sharedInstance()->canRedo());

    // Repaints OpenGL widget to display changes
    this->opengl_widget->repaint();
}

/**
 * @brief Select tool selected
 */
void Gui::select()
{
    this->opengl_widget->setAction(SELECT_E);
}

/**
 * @brief Rotate tool selected
 */
void Gui::rotate()
{
    this->opengl_widget->setAction(ROTATE);
}

/**
 * @brief Drawline tool selected
 */
void Gui::drawLine()
{
    this->opengl_widget->setAction(ElementType::LINE);
}

/**
 * @brief Delete tool selected
 */
void Gui::dlt()
{
    this->opengl_widget->deleteSelection();
}


/**
 * @brief Pan tool selected
 */
void Gui::pan()
{
    this->opengl_widget->setAction(PAN);
}

/**
 * @brief Sets new test to be displayed in line edit
 * @param text
 */
void Gui::setLineEditText(const QString &text)
{
    this->line_edit->setText(text);
}

/**
 * @brief Returns text of line edit
 * @return Text of line edit
 */
QString Gui::getLineEditText() const
{
    return this->line_edit->text();
}

/**
 * @brief Appends text to line edit
 * @param text Text to be appended
 */
void Gui::appendTextToLineEdit(const QString &text)
{
    this->line_edit->setText(this->line_edit->text() + text);
}

/**
 * @brief Deletes last character of line edit
 */
void Gui::deleteLastCharOfLineEdit()
{
    this->line_edit->setText(this->line_edit->text().remove(this->line_edit->text().count()-1, 1));

    if (this->line_edit->text().count() == 0)
        this->line_edit->setText("0");
}

/**
 * @brief Sets line label text
 * @param text Text to be set
 */
void Gui::setLineLabelText(const QString &text)
{
    this->line_label->setText(text);
}

/**
 * @brief Disables or enables undo button
 * @param value True if undo button should be enabled
 */
void Gui::setUndo(bool value)
{
    this->action_undo->setEnabled(value);
}

/**
 * @brief Disables or enables redo button
 * @param value True if redo button should be enabled
 */
void Gui::setRedo(bool value)
{
    this->action_redo->setEnabled(value);
}

/**
 * @brief Disables or enables delete button
 * @param value True if delete button should be enabled
 */
void Gui::setDelete(bool value)
{
    this->action_delete->setEnabled(value);
}
