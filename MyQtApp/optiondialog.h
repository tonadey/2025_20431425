#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog();

    //Setter  
    void setFileData(const QString& name, bool isVisible);
   
    //Getters
    QString getName() const;
    bool getIsVisible() const;

private:
    Ui::OptionDialog *ui;
};

#endif // OPTIONDIALOG_H



