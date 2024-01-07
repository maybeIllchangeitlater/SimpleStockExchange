#include "ChangePassPopup.hpp"
#include <iostream>
#include "ui_ChangePassPopup.h"

ChangePassPopup::ChangePassPopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePassPopup)
{
    ui->setupUi(this);
    ui->newpass->setEchoMode(QLineEdit::Password);
    ui->oldpass->setEchoMode(QLineEdit::Password);
    connect(ui->Ok, &QPushButton::clicked, this, [&](){
       emit ChangePassword(ui->newpass->text().toStdString(), ui->oldpass->text().toStdString());
        ui->newpass->clear();
        ui->oldpass->clear();
        ui->newpass->setEchoMode(QLineEdit::Password);
        ui->oldpass->setEchoMode(QLineEdit::Password);
        ui->newpass->setFocus();
        close();
        hide();
    });
    connect(ui->No, &QPushButton::clicked, this, [&](){
        ui->newpass->clear();
        ui->oldpass->clear();
        ui->newpass->setEchoMode(QLineEdit::Password);
        ui->oldpass->setEchoMode(QLineEdit::Password);
        ui->newpass->setFocus();
        close();
        hide();
    });
    connect(ui->eye_new, &QPushButton::clicked, this, [&](){
        ui->newpass->echoMode() == QLineEdit::Password
                ? ui->newpass->setEchoMode(QLineEdit::Normal)
                : ui->newpass->setEchoMode(QLineEdit::Password);
    });
    connect(ui->eye_old, &QPushButton::clicked, this, [&](){
        ui->oldpass->echoMode() == QLineEdit::Password
                ? ui->oldpass->setEchoMode(QLineEdit::Normal)
                : ui->oldpass->setEchoMode(QLineEdit::Password);
    });
}

ChangePassPopup::~ChangePassPopup()
{
    delete ui;
}
