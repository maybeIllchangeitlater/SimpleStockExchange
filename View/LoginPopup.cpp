#include "LoginPopup.hpp"
#include "ui_LoginPopup.h"

LoginPopup::LoginPopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginPopup)
{
    ui->setupUi(this);
    ui->PasswordLine->setEchoMode(QLineEdit::Password);
    connect(ui->eye, &QPushButton::clicked, this, [&](){
        ui->PasswordLine->echoMode() == QLineEdit::Password
                ? ui->PasswordLine->setEchoMode(QLineEdit::Normal)
                : ui->PasswordLine->setEchoMode(QLineEdit::Password);
    });
    connect(ui->Ok, &QPushButton::clicked, this, [&](){
                emit LoginAttempt(ui->LoginLine->text().toStdString(), ui->PasswordLine->text().toStdString());
                ui->LoginLine->clear();
                ui->PasswordLine->clear();
                ui->PasswordLine->setEchoMode(QLineEdit::Password);
                ui->LoginLine->SetFocus();
            });
}

LoginPopup::~LoginPopup()
{
    delete ui;
}
