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
    ui->TransactionViewer->setColumnCount(5);
    ui->TransactionViewer->setHorizontalHeaderLabels(
                    {s21::BDNames::joined_buyer_name,
                     s21::BDNames::joined_seller_name, s21::BDNames::transaction_table_quantity,
                     s21::BDNames::transaction_table_rate, s21::BDNames::transaction_table_create_update_time});
    ui->TransactionViewer->setColumnWidth(1, 150);
    ui->TransactionViewer->setColumnWidth(2, 150);
    ui->TransactionViewer->setShowGrid(true);
    ui->TransactionViewer->verticalHeader()->setVisible(false);
}



ViewTransactionsPopup::~ViewTransactionsPopup()
{
    delete ui;
}

void ViewTransactionsPopup::DisplayTransactions(const std::string &raw_response)
{
    QJsonArray  jsons = QJsonDocument::fromJson(QString::fromStdString(raw_response).toUtf8()).array();
    for(const auto& json : jsons){
       int row = ui->TransactionViewer->rowCount();
        ui->TransactionViewer->insertRow(row);
        ui->TransactionViewer->setItem(row, 0, new QTableWidgetItem(json.toObject().value(s21::BDNames::joined_buyer_name).toString()));
        ui->TransactionViewer->setItem(row, 1, new QTableWidgetItem(json.toObject().value(s21::BDNames::joined_seller_name).toString()));
        ui->TransactionViewer->setItem(row, 2, new QTableWidgetItem(json.toObject().value(s21::BDNames::bid_table_quantity).toString()));
        ui->TransactionViewer->setItem(row, 3, new QTableWidgetItem(json.toObject().value(s21::BDNames::bid_table_rate).toString()));
        ui->TransactionViewer->setItem(row, 4, new QTableWidgetItem(json.toObject().value(s21::BDNames::bid_table_create_update_time).toString()));
    }
        ui->TransactionViewer->resizeColumnsToContents();
        ui->TransactionViewer->setColumnWidth(0, 150);
        ui->TransactionViewer->setColumnWidth(1, 150);
}

void ViewTransactionsPopup::closeEvent(QCloseEvent *event)
{
    ui->TransactionViewer->clear();
    ui->TransactionViewer->setRowCount(0);
    ui->TransactionViewer->setHorizontalHeaderLabels(
                    {s21::BDNames::joined_buyer_name,
                     s21::BDNames::joined_seller_name, s21::BDNames::transaction_table_quantity,
                     s21::BDNames::transaction_table_rate, s21::BDNames::transaction_table_create_update_time});
    event->accept();
}
