#include "PasswordConfirmationPopup.hpp"

#include "ui_PasswordConfirmationPopup.h"

PasswordConfirmationPopup::PasswordConfirmationPopup(QWidget *parent)
    : QDialog(parent), ui(new Ui::PasswordConfirmationPopup) {
  ui->setupUi(this);
  ui->password_line->setEchoMode(QLineEdit::Password);
  connect(ui->eye, &QPushButton::clicked, this, [&]() {
    ui->password_line->echoMode() == QLineEdit::Password
        ? ui->password_line->setEchoMode(QLineEdit::Normal)
        : ui->password_line->setEchoMode(QLineEdit::Password);
  });
  connect(ui->Ok, &QPushButton::clicked, this, [&]() {
    emit ConfirmAction(ui->password_line->text().toStdString());
    hide();
    close();
  });
  connect(ui->No, &QPushButton::clicked, this, [&]() {
    hide();
    close();
  });
}

PasswordConfirmationPopup::~PasswordConfirmationPopup() { delete ui; }

void PasswordConfirmationPopup::closeEvent(QCloseEvent *event) {
  ui->password_line->clear();
  ui->password_line->setEchoMode(QLineEdit::Password);
  event->accept();
}
