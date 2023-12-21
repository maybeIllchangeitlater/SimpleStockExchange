#include "ViewBids.hpp"
#include "ui_ViewBids.h"
#include <iostream>

ViewBids::ViewBids(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewBids)
{
    ui->setupUi(this);
    connect(ui->Ok, &QPushButton::clicked, this, [&](){
                emit ViewBid(ui->comboBox->currentText().toStdString());
            });
    connect(ui->Cancel, &QPushButton::clicked, this, [&](){
        if(!ui->BidsListWidget->selectedItems().empty()
                && ui->BidsListWidget->currentItem()->text().contains(s21::BDNames::bid_id_for_join)){
            emit CancelBid(GrabId());
            ui->BidsListWidget->takeItem(ui->BidsListWidget->currentRow());
        }
    });
    connect(ui->Update, &QPushButton::clicked, this, [&](){
        if(!ui->BidsListWidget->selectedItems().empty()
                && ui->BidsListWidget->currentItem()->text().contains(s21::BDNames::bid_id_for_join)){
            emit UpdateBid(GrabId(), GrabRate(), GrabQuantity());
            ui->BidsListWidget->takeItem(ui->BidsListWidget->currentRow()); //swap to grap by id and update
        }
    });
}

ViewBids::~ViewBids()
{
    delete ui;
}

void ViewBids::ShowBids(const std::string &bids, const char * type)
{
    QJsonArray  jsons = QJsonDocument::fromJson(QString::fromStdString("[{" + bids + "}]").toUtf8()).array();
    ui->BidsListWidget->addItem(QString(type) + " :");
    if(jsons.empty()){
        ui->BidsListWidget->addItem("No bids");
    }
    for(const auto& json : jsons){
        ui->BidsListWidget->addItem(QString(s21::BDNames::bid_id_for_join) + " : "
                                    +json.toObject().value(s21::BDNames::bid_id_for_join).toString() + "  " +
                                    QString(s21::BDNames::joined_buyer_name) + " : "
                                    + json.toObject().value(s21::BDNames::joined_buyer_name).toString() + "  " +
                                    QString(s21::BDNames::joined_seller_name) + " : "
                                    + json.toObject().value(s21::BDNames::joined_seller_name).toString() + "  " +
                                    QString(s21::BDNames::bid_table_quantity) + " : "
                                    + json.toObject().value(s21::BDNames::bid_table_quantity).toString() + "  " +
                                    QString(s21::BDNames::bid_table_rate) + " : "
                                    + json.toObject().value(s21::BDNames::bid_table_rate).toString() + "  " +
                                    QString(s21::BDNames::bid_table_create_update_time) + " : "
                                    + json.toObject().value(s21::BDNames::bid_table_create_update_time).toString());
    }
}

void ViewBids::closeEvent(QCloseEvent *event)
{
    ui->BidsListWidget->clear();
    event->accept();
}

std::string ViewBids::GrabId()
{
    auto str = ui->BidsListWidget->currentItem()->text().toStdString().substr(std::string(s21::BDNames::bid_id_for_join).length() + 3); // " : "
    return str.substr(0, str.find(' '));
}

std::string ViewBids::GrabRate()
{
    auto str = ui->BidsListWidget->currentItem()->text().toStdString().substr(std::string(s21::BDNames::bid_table_rate).length() + 3); // " : "
    return str.substr(0, str.find(' '));
}

std::string ViewBids::GrabQuantity()
{
    auto str = ui->BidsListWidget->currentItem()->text().toStdString().substr(std::string(s21::BDNames::bid_table_quantity).length() + 3); // " : "
    return str.substr(0, str.find(' '));
}
