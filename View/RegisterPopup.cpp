#include "RegisterPopup.hpp"
#include "ui_RegisterPopup.h"

RegisterPopup::RegisterPopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterPopup)
{
    ui->setupUi(this);
    ui->PasswordLine->setEchoMode(QLineEdit::Password);
    connect(ui->Ok, &QPushButton::clicked, this, [&](){
                emit RegisterAttempt(ui->LoginLine->text().toStdString(),
                                     ui->PasswordLine->text().toStdString(), ui->BalanceLine->text().toStdString());
                ui->LoginLine->clear();
                ui->PasswordLine->clear();
                ui->BalanceLine->clear();
            });
}

RegisterPopup::~RegisterPopup()
{
    delete ui;
}
