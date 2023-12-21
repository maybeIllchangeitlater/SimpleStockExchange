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
    QString extracted_json_array = QString::fromStdString(raw_response.substr(raw_response.find('['), raw_response.find(']') + 1));
    QJsonArray  jsons = QJsonDocument::fromJson(extracted_json_array.toUtf8()).array();
    QString transaction_type = extracted_json_array.contains("Bought")
            ? s21::ExtraJSONKeys::buy_transaction
            : s21::ExtraJSONKeys::sell_transaction;
    for(const auto& json : jsons){
        if(json.toObject().contains(s21::ExtraJSONKeys::created_buy_bid_quantity)){
            return NewBid(json,  QString(s21::ExtraJSONKeys::created_buy_bid_quantity));
         }else if(json.toObject().contains(s21::ExtraJSONKeys::created_sell_bid_quantity)){
            return NewBid(json,  QString(s21::ExtraJSONKeys::created_sell_bid_quantity));
        }
        ui->TransactionViewer->addItem(transaction_type + " "
                                    +json.toObject().value(transaction_type).toString() + " " +
                                    QString(s21::ExtraJSONKeys::bid_transaction_rate) + " "
                                    + json.toObject().value(s21::ExtraJSONKeys::bid_transaction_rate).toString() + " RUB per 1 USD");
    }
    return "";

}

void BidTransactionMadePopup::closeEvent(QCloseEvent *event)
{
    ui->TransactionViewer->clear();
    event->accept();
}

std::string BidTransactionMadePopup::NewBid(const QJsonValueRef &json, QString bid_type)
{
    return (bid_type
            + json.toObject().value(bid_type).toString()
            + QString(s21::ExtraJSONKeys::bid_transaction_rate)
            + json.toObject().value(s21::ExtraJSONKeys::bid_transaction_rate).toString()
            + " RUB per 1 USD").toStdString();
}
