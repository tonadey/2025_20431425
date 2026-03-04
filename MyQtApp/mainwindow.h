#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ModelPart.h"
#include "ModelPartList.h"
#include "optiondialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

signals:
    // Define the signal that carries a message and a timeout value
    void statusUpdateMessage(const QString& message, int timeout);
//Declare slots
private slots:
    void handleButton1(); 
    void handleButton2(); 
    void handleTreeClicked(); 
    void on_actionOpen_File_triggered();
    void on_actionItem_Options_triggered(); 

private:
    Ui::MainWindow* ui;      
    ModelPartList* partList; // pointer to the tree data
};

#endif // MAINWINDOW_H