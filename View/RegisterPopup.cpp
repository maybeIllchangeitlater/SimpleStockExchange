#include "RegisterPopup.hpp"
#include "ui_RegisterPopup.h"

RegisterPopup::RegisterPopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterPopup)
{
    ui->setupUi(this);
    ui->PasswordLine->setEchoMode(QLineEdit::Password);
    connect(ui->eye, &QPushButton::clicked, this, [&](){
        ui->PasswordLine->echoMode() == QLineEdit::Password
                ? ui->PasswordLine->setEchoMode(QLineEdit::Normal)
                : ui->PasswordLine->setEchoMode(QLineEdit::Password);
    });
    connect(ui->Ok, &QPushButton::clicked, this, [&](){
                emit RegisterAttempt(ui->LoginLine->text().toStdString(),
                                     ui->PasswordLine->text().toStdString(),
                                     ui->BalanceLineUSD->text().toStdString(),
                                     ui->BalanceLineRUB->text().toStdString());
            });
}

RegisterPopup::~RegisterPopup()
{
    delete ui;
}

void RegisterPopup::closeEvent(QCloseEvent *event)
{
    ui->LoginLine->clear();
    ui->PasswordLine->clear();
    ui->BalanceLineUSD->clear();
    ui->BalanceLineRUB->clear();
    ui->PasswordLine->setEchoMode(QLineEdit::Password);
    ui->LoginLine->setFocus();
    event->accept();
}
