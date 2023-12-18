#include "DeleteAccountPopup.hpp"
#include "ui_DeleteAccountPopup.h"

DeleteAccountPopup::DeleteAccountPopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteAccountPopup)
{
    ui->setupUi(this);
    connect(ui->Yes, &QPushButton::clicked, this, [&](){
       emit DeleteAccount();
        hide();
        close();
    });
    connect(ui->No, &QPushButton::clicked, this, [&](){
        hide();
        close();
    });
}

DeleteAccountPopup::~DeleteAccountPopup()
{
    delete ui;
}
