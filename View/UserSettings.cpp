#include "UserSettings.hpp"
#include "ui_UserSettings.h"

UserSettings::UserSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserSettings)
{
    ui->setupUi(this);
    connect(ui->DeleteAccount, &QPushButton::clicked, this, [&](){
        confirm_pop_->exec();
    });
    connect(confirm_pop_.get(), &PasswordConfirmationPopup::ConfirmAction, this, [&](const std::string user_password){
        emit DeleteAccount(user_password);
    });

    connect(ui->ChangeName, &QPushButton::clicked, this, [&](){
       namechange_pop_->exec();
    });
    connect(namechange_pop_.get(), &ChangeNamPopup::NameChange, this, [&](const std::string user_name){
       emit ChangeUsername(user_name);
    });

    connect(ui->ChangePassword, &QPushButton::clicked, this, [&](){
        passchange_pop_->exec();
    });
    connect(passchange_pop_.get(), &ChangePassPopup::ChangePassword, this,
            [&](const std::string new_password, const std::string old_password){
       emit ChangePassword(new_password, old_password);
    });
}

UserSettings::~UserSettings()
{
    delete ui;
}



void UserSettings::SetFields(const std::string &name, const std::string &id,
                            const std::string &usd_balance, const std::string &rub_balance)
{
    ui->Username->setText(QString::fromStdString(name));
    ui->Id->setText(QString::fromStdString(id));
    ui->BalanceUSD->setText(s21::ClientDisplayMessages::usd_balance + QString::fromStdString(usd_balance).remove('"'));
    ui->BalanceRUB->setText(s21::ClientDisplayMessages::rub_balance + QString::fromStdString(rub_balance).remove('"'));
}

void UserSettings::SetFields(const std::string &name)
{
    ui->Username->setText(QString::fromStdString(name));
}

void UserSettings::SetStatus(const std::string &status)
{
    ui->StatusLabel->setText(QString::fromStdString(status));
}

