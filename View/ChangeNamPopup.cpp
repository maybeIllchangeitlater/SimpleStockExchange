#include "ChangeNamPopup.hpp"
#include "ui_ChangeNamPopup.h"

ChangeNamPopup::ChangeNamPopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeNamPopup)
{
    ui->setupUi(this);
    connect(ui->Ok, &QPushButton::clicked, this, [&](){
        emit NameChange(ui->UsernameLine->text().toStdString());
        ui->UsernameLine->clear();
        close();
        hide();
    });
    connect(ui->No, &QPushButton::clicked, this, [&](){
        ui->UsernameLine->clear();
        close();
        hide();
    });
}



ChangeNamPopup::~ChangeNamPopup()
{
    delete ui;
}
