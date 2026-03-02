#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "ModelPart.h"      
#include "ModelPartList.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Connect the wires to the push buttons
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton1);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton2);

    // Connect your custom signal to the statusbar's slot
    connect(this, &MainWindow::statusUpdateMessage,
        ui->statusbar, &QStatusBar::showMessage);

    //Partlist
    this->partList = new ModelPartList("PartsList");
    ui->treeView->setModel(this->partList); // Link the data to the UI widget

    ModelPart* rootItem = this->partList->getRootItem();

    // Create 3 top-level items with 5 sub-items each
    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        ModelPart* childItem = new ModelPart({ name, visible });
        rootItem->appendChild(childItem);

        for (int j = 0; j < 5; j++) {
            QString name = QString("Item %1,%2").arg(i).arg(j);
            QString visible("true");
            ModelPart* childChildItem = new ModelPart({ name, visible });
            childItem->appendChild(childChildItem);
        }
    }
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//Function for Button 1 Action
void MainWindow::handleButton1() {
    // This causes MainWindow to emit the signal
    // '0' means the message stays on the status bar until it is cleared or replaced
    emit statusUpdateMessage(QString("Add button was clicked"), 0);
}

//Function for Button 2 Action 
void MainWindow::handleButton2() {
    QMessageBox msgBox;
    msgBox.setText("Button 2 was clicked!");
    msgBox.exec();
}

//Function for Selection Logic 
void MainWindow::handleTreeClicked() {
    /* Get the index of the selected item */
    QModelIndex index = ui->treeView->currentIndex();

    /* Check if the index is valid (important to prevent crashes!) */
    if (!index.isValid()) return;

    /* Get a pointer to the item from the index */
    // Note: index.internalPointer() returns the ModelPart we stored earlier
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    /* Retrieve the name string from the internal QVariant data array */
    // data(0) refers to the first column (the Name)
    QString text = selectedPart->data(0).toString();

    /* Update the status bar */
    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);
}