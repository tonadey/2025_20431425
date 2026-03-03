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
void OptionDialog::setFileData(const QString& name, bool isVisible) {
    ui->nameEdit->setText(name);           // Fills the LineEdit
    ui->visibleCheck->setChecked(isVisible); // Sets the CheckBox
}

//Getter for returning the name that the user types 
QString OptionDialog::getName() const {
    return ui->nameEdit->text(); // Returns the text from the UI
}

//Getter for returning the state of the check box (checked or unchecked)
bool OptionDialog::getIsVisible() const {
    return ui->visibleCheck->isChecked(); // Returns true/false
}