#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "ModelPart.h"      
#include "ModelPartList.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkNew.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <QVTKOpenGLNativeWidget.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //RENDERING LOGIC---------
    /* 1. Link a render window with the Qt widget */
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->vtkWidget->setRenderWindow(renderWindow);

    /* 2. Add a renderer */
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    /* 3. Create the Cylinder object */
    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(8);

    /* 4. Setup Mapper and Actor */
    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(1.0, 0.0, 0.35); // Set color to pink/red
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(-45.0);

    renderer->AddActor(cylinderActor);

    /* 5. Reset and position Camera */
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();

    //END OF RENDERING LOGIC --------
    //connect(ui->actionOpen_File, &QAction::triggered, this, &MainWindow::on_actionOpen_File_triggered);
    ui->treeView->addAction(ui->actionItem_Options);

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

        ModelPart* childItem = new ModelPart({ name, visible, "0", "0", "0"});
        rootItem->appendChild(childItem);

        for (int j = 0; j < 5; j++) {
            QString name = QString("Item %1,%2").arg(i).arg(j);
            QString visible("true");
            ModelPart* childChildItem = new ModelPart({ name, visible, "0", "0", "0"});
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
    QString currentName = selectedPart->data(0).toString();
    bool currentVisible = (selectedPart->data(1).toString() == "true");

    int currentR = selectedPart->data(2).toInt();
    int currentG = selectedPart->data(3).toInt();
    int currentB = selectedPart->data(4).toInt();

    dialog.setFileData(currentName, currentVisible, currentR, currentG, currentB);

    if (dialog.exec() == QDialog::Accepted) {

        // 3. PUSH: Save the NEW values from the dialog back into the ModelPart
        selectedPart->set(0, dialog.getName());
        selectedPart->set(1, dialog.getIsVisible() ? "true" : "false");

        // Convert the integers from spinboxes back to strings for storage
        selectedPart->set(2, QString::number(dialog.getR()));
        selectedPart->set(3, QString::number(dialog.getG()));
        selectedPart->set(4, QString::number(dialog.getB()));

        emit statusUpdateMessage(QString("Updated part: %1").arg(dialog.getName()), 0);

        qDebug() << "Saved R value is:" << selectedPart->data(2).toString();

        // 4. REFRESH: Tell the tree to update the whole row (Columns 0 through 4)
        this->partList->dataChanged(index.siblingAtColumn(0), index.siblingAtColumn(4));
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

/*OLD OPEN FILE ACTION FUNCTION FOR PERVIOUS WKSHTS 
void MainWindow::on_actionOpen_File_triggered() {
    // 1. Get the full path from the user
    QString fullPath = QFileDialog::getOpenFileName(this, tr("Open File"), "/", tr("All Files (*.*)"));

    if (!fullPath.isEmpty()) {
        // 2. Extract the short name for the TreeView
        QFileInfo fileInfo(fullPath);
        QString shortName = fileInfo.fileName();

        QModelIndex index = ui->treeView->currentIndex();
        if (index.isValid()) {
            ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

            // 3. Update the tree item with the SHORT name
            selectedPart->set(0, shortName);

            // 4. Refresh the tree
            emit partList->dataChanged(index, index);

            // 5. Update the status bar with the FULL path
            emit statusUpdateMessage(QString("File Selected: %1").arg(fullPath), 0);
        }
        else {
            emit statusUpdateMessage(QString("No part selected to rename!"), 0);
        }
    }
}*/

//NEW OPEN FILE ACTION FUNCTION FOR WRKSHT 7
void MainWindow::on_actionOpen_File_triggered() {
    // 1. Get the filename selected
    QString fullPath = QFileDialog::getOpenFileName(this, tr("Open STL File"), "/", tr("STL Files (*.stl)"));

    if (!fullPath.isEmpty()) {
        QFileInfo fileInfo(fullPath);
        QString shortName = fileInfo.fileName();

        // Get the currently selected item to act as the parent
        QModelIndex index = ui->treeView->currentIndex();

        // 2. Add a new item to the tree view
        // We add it as a child of the currently selected index
        partList->appendChild(index, {shortName, true});

        // 3. Get the pointer to that newly created child part
        // We look at the last row of the current index
        int newRow = partList->rowCount(index) - 1;
        QModelIndex newPartIndex = partList->index(newRow, 0, index);
        ModelPart* newPart = static_cast<ModelPart*>(newPartIndex.internalPointer());

        // 4. Call the loadSTL() function of the newly created item
        if (newPart) {
            newPart->loadSTL(fullPath);

            // 5. Update the 3D renderer
            updateRender();

            emit statusUpdateMessage(QString("Loaded CAD Part: %1").arg(fullPath), 0);
        }
    }
}

void MainWindow::on_actionItem_Options_triggered() {
    handleButton2(); 
}

void MainWindow::updateRender() {
    renderer->RemoveAllViewProps(); // Clear the scene
    // Start recursion from the root of the tree
    updateRenderFromTree(partList->index(0, 0, QModelIndex()));
    renderer->Render(); // Refresh the window
}

void MainWindow::updateRenderFromTree(const QModelIndex& index) {
    if (index.isValid()) {
        // Retrieve the ModelPart from the current index
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

        // Add the part's actor to the renderer if it exists
        if (selectedPart && selectedPart->getActor()) {
            renderer->AddActor(selectedPart->getActor());
        }
    }

    // Check for children and recurse
    if (!partList->hasChildren(index)) return;

    int rows = partList->rowCount(index);
    for (int i = 0; i < rows; i++) {
        updateRenderFromTree(partList->index(i, 0, index));
    }
}