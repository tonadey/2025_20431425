#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
}

OptionDialog::~OptionDialog()
{
    delete ui;
}


//Setter for filling the dialog with data 
void OptionDialog::setFileData(const QString& name, bool isVisible, int r, int g, int b) {
    ui->nameEdit->setText(name);           // Fills the LineEdit
    ui->visibleCheck->setChecked(isVisible); // Sets the CheckBox
    ui->spinRed->setValue(r);
    ui->spinGreen->setValue(g);
    ui->spinBlue->setValue(b);
}

//Getter for returning the name that the user types 
QString OptionDialog::getName() const {
    return ui->nameEdit->text(); // Returns the text from the UI
}

//Getter for returning the state of the check box (checked or unchecked)
bool OptionDialog::getIsVisible() const {
    return ui->visibleCheck->isChecked(); // Returns true/false
}

//Getters for RGB 
int OptionDialog::getR() const {
    return ui->spinRed->value();
}

int OptionDialog::getG() const {
    return ui->spinGreen->value();
}

int OptionDialog::getB() const {
    return ui->spinBlue->value();
}