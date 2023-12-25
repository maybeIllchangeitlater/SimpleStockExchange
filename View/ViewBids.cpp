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
            ui->bid_status_label->setText("Bid cancelled");
        }
    });
    connect(ui->Update, &QPushButton::clicked, this, [&](){
        if(!ui->BidsListWidget->selectedItems().empty()
                && ui->BidsListWidget->currentItem()->text().contains(s21::BDNames::bid_id_for_join)){
            emit UpdateBid(GrabId(), GrabRate(), GrabQuantity(), ui->BidsListWidget->currentRow());
        }
    });
    connect(ui->Create, &QPushButton::clicked, this, [&](){
        emit CreateBid();
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
        ui->BidsListWidget->addItem(GrabBidFromJson(json));
    }
}

void ViewBids::InsertNewBid(const std::string &bid)
{
    if(bid.empty()){
        ui->bid_status_label->setText("Bid fullfilled");
        return;
    }
    std::cout << "bid is : " << bid << "\n";
    emit ViewBid(bid.find(s21::ExtraJSONKeys::created_sell_bid_quantity) != std::string::npos
                 ? ui->comboBox->itemText(0).toStdString()
                 : ui->comboBox->itemText(1).toStdString());
    ui->bid_status_label->setText("Bid created");
}

void ViewBids::InsertUpdatedBidBack(const std::string &bid, size_t bid_index)
{
    ui->BidsListWidget->takeItem(bid_index);
    if(!bid.empty()){
        QJsonArray jsons = QJsonDocument::fromJson(QString::fromStdString("[{" + bid + "}]").toUtf8()).array();
        ui->BidsListWidget->insertItem(bid_index, GrabBidFromJson(jsons[0]));
        ui->bid_status_label->setText("Bid updated");
    }else{
        ui->bid_status_label->setText("Bid fullfilled");
    }
}

void ViewBids::closeEvent(QCloseEvent *event)
{
    ui->BidsListWidget->clear();
    ui->bid_status_label->clear();
    event->accept();
}

std::string ViewBids::GrabId()
{
    auto str = ui->BidsListWidget->currentItem()->text().toStdString().substr(std::string(s21::BDNames::bid_id_for_join).length() + 3); // " : "
    return str.substr(0, str.find(' '));
}

std::string ViewBids::GrabRate()
{
    auto str = ui->BidsListWidget->currentItem()->text().toStdString();
    str = str.substr(str.find(s21::BDNames::bid_table_rate) + std::string(s21::BDNames::bid_table_rate).length() + 3);
    return str.substr(0, str.find(' '));
}

std::string ViewBids::GrabQuantity()
{
    auto str = ui->BidsListWidget->currentItem()->text().toStdString();
    str = str.substr(str.find(s21::BDNames::bid_table_quantity) + std::string(s21::BDNames::bid_table_quantity).length() + 3);
    return str.substr(0, str.find(' '));
}

QString ViewBids::GrabBidFromJson(const QJsonValueRef &json)
{
    return QString(s21::BDNames::bid_id_for_join) + " : "
            +json.toObject().value(s21::BDNames::bid_id_for_join).toString() + "  " +
            s21::BDNames::joined_buyer_name + " : "
            + json.toObject().value(s21::BDNames::joined_buyer_name).toString() + "  " +
            s21::BDNames::joined_seller_name + " : "
            + json.toObject().value(s21::BDNames::joined_seller_name).toString() + "  " +
            s21::BDNames::bid_table_quantity + " : "
            + json.toObject().value(s21::BDNames::bid_table_quantity).toString() + "  " +
            s21::BDNames::bid_table_rate + " : "
            + json.toObject().value(s21::BDNames::bid_table_rate).toString() + "  " +
            s21::BDNames::bid_table_create_update_time + " : "
            + json.toObject().value(s21::BDNames::bid_table_create_update_time).toString();
}
