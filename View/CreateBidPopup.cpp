#include "CreateBidPopup.hpp"
#include "ui_CreateBidPopup.h"

CreateBidPopup::CreateBidPopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateBidPopup)
{
    ui->setupUi(this);
    connect(ui->Ok, &QPushButton::clicked, this, [&](){
                emit MakeBid(ui->QuantityLine->text().toStdString(), ui->RateLine->text().toStdString(), ui->comboBox->currentText().toStdString());
                ui->QuantityLine->clear();
                ui->RateLine->clear();
            });
}

CreateBidPopup::~CreateBidPopup()
{
    delete ui;
}
