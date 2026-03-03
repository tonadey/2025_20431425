#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "ModelPart.h"      
#include "ModelPartList.h"
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionOpen_File, &QAction::triggered, this, &MainWindow::on_actionOpen_File_triggered);

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
    // 1. Get the selected part from your TreeView (Logic from previous steps)
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) return;
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    // 2. Create the dialog and FILL it with current data
    OptionDialog dialog(this);
    dialog.setFileData(selectedPart->data(0).toString(), true);

    // 3. Show the dialog
    if (dialog.exec() == QDialog::Accepted) {
        
        // 4. Update the ModelPart with new data from the dialog
        QString newName = dialog.getName();
        bool newVisible = dialog.getIsVisible(); //check checkbox state

        //Change current name to new name 
        selectedPart->set(0, newName);

        //Display message confirming name has been updated 
        emit statusUpdateMessage(QString("Updated part to: %1").arg(newName), 0);

        // Refresh the tree to show the new name
        this->partList->dataChanged(index, index);
    }
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

void MainWindow::on_actionOpen_File_triggered() {
    // Call the static function with specific filters
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\", // Default directory
        tr("STL Files (*.stl);;Text Files (*.txt);;All Files (*)") // Combined filters
    );

    if (!fileName.isEmpty()) {
        // 1. Create the new part for your data structure
        ModelPart* newPart = new ModelPart({ fileName, "true" });

        // 2. Add it to the tree
        ModelPart* rootItem = this->partList->getRootItem();
        rootItem->appendChild(newPart);

        // 3. Update the Status Bar to prove it works
        emit statusUpdateMessage(QString("File Selected: %1").arg(fileName), 0);

        // 4. Refresh the view
        this->partList->dataChanged(QModelIndex(), QModelIndex());
    }
}