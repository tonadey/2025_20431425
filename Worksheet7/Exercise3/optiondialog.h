#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include <QString>


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
    void setFileData(const QString& name, bool isVisible, int r, int g, int b);
   
    //Getters
    QString getName() const;
    bool getIsVisible() const;
    int getR() const;
    int getG() const;
    int getB() const;

private:
    Ui::OptionDialog *ui;
};

#endif // OPTIONDIALOG_H



