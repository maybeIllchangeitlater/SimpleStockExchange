#include "ViewTransactionsPopup.hpp"
#include "ui_viewtransactionspopup.h"

ViewTransactionsPopup::ViewTransactionsPopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewTransactionsPopup)
{
    ui->setupUi(this);
    connect(ui->Buy, &QPushButton::clicked, this, [&](){
        emit ShowBuyTransactions();
    });
    connect(ui->Sell, &QPushButton::clicked, this, [&](){
        emit ShowSellTransactions();
    });
}



ViewTransactionsPopup::~ViewTransactionsPopup()
{
    delete ui;
}

void ViewTransactionsPopup::DisplayTransactions(const std::string &type, const std::string &raw_response)
{
    QJsonArray  jsons = QJsonDocument::fromJson(QString::fromStdString("[{" + raw_response + "}]").toUtf8()).array();
    ui->TransactionViewer->addItem(QString::fromStdString(type) + " :");
    if(jsons.empty()){
        ui->TransactionViewer->addItem("No transactions");
    }
    for(const auto& json : jsons){
        ui->TransactionViewer->addItem(GrabTransactionFromJson(json));
    }
}

QString ViewTransactionsPopup::GrabTransactionFromJson(const QJsonValueRef &json)
{
    return QString(s21::BDNames::transaction_id_for_join) + " : "
            +json.toObject().value(s21::BDNames::transaction_id_for_join).toString() + "  " +
            s21::BDNames::joined_buyer_name + " : "
            + json.toObject().value(s21::BDNames::joined_buyer_name).toString() + "  " +
            s21::BDNames::joined_seller_name + " : "
            + json.toObject().value(s21::BDNames::joined_seller_name).toString() + "  " +
            s21::BDNames::transaction_table_quantity + " : "
            + json.toObject().value(s21::BDNames::transaction_table_quantity).toString() + "  " +
            s21::BDNames::transaction_table_rate + " : "
            + json.toObject().value(s21::BDNames::transaction_table_rate).toString() + "  " +
            s21::BDNames::transaction_table_create_update_time + " : "
            + json.toObject().value(s21::BDNames::transaction_table_create_update_time).toString();
}

