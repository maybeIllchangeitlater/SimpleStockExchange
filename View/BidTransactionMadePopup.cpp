#include "BidTransactionMadePopup.hpp"
#include "ui_BidTransactionMadePopup.h"
#include <iostream>

BidTransactionMadePopup::BidTransactionMadePopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BidTransactionMadePopup)
{
    ui->setupUi(this);

}

BidTransactionMadePopup::~BidTransactionMadePopup()
{
    delete ui;
}

std::string BidTransactionMadePopup::DisplayNewTransactions(const std::string &raw_response)
{

    QJsonArray  jsons = QJsonDocument::fromJson(QString::fromStdString(raw_response).toUtf8()).array();
    QString transaction_type = raw_response.find("Bought") != std::string::npos
            ? s21::ExtraJSONKeys::buy_transaction
            : s21::ExtraJSONKeys::sell_transaction;
    for(const auto& json : jsons){
        auto token = json.toString();
        if(token.contains(s21::ExtraJSONKeys::bid_creation_check)){
            return NewBid(json, token.contains(s21::ExtraJSONKeys::created_buy_bid_quantity)
                          ? QString(s21::ExtraJSONKeys::created_buy_bid_quantity)
                          : QString(s21::ExtraJSONKeys::created_sell_bid_quantity));
            }
        ui->TransactionViewer->addItem(transaction_type + " "
                                    +json.toObject().value(transaction_type).toString() + " " +
                                    QString(s21::ExtraJSONKeys::bid_transaction_rate) + " "
                                    + json.toObject().value(s21::ExtraJSONKeys::bid_transaction_rate).toString() + " RUB");
    }
    return "";

}

std::string BidTransactionMadePopup::NewBid(const QJsonValueRef &json, QString bid_type)
{
    return (bid_type
            + json.toObject().value(bid_type).toString()
            + QString(s21::ExtraJSONKeys::bid_transaction_rate)
            + json.toObject().value(s21::ExtraJSONKeys::bid_transaction_rate).toString()
            + "RUB").toStdString();
}
