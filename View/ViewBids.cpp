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
    ui->BidsListWidget->setColumnCount(7);
    ui->BidsListWidget->setHorizontalHeaderLabels(
                    {"Bid ID", "Buyer Username", "Seller Username", "Quantity", "Rate", "Time", "Status"});
    ui->BidsListWidget->setColumnWidth(1, 150);
    ui->BidsListWidget->setColumnWidth(2, 150);
    ui->BidsListWidget->setShowGrid(true);
    ui->BidsListWidget->verticalHeader()->setVisible(false);
    connect(ui->Cancel, &QPushButton::clicked, this, [&](){
        if(!ui->BidsListWidget->selectedItems().empty()){
            emit CancelBid(GrabId());
            ui->BidsListWidget->removeRow(ui->BidsListWidget->currentRow());
            ui->bid_status_label->setText("Bid cancelled");
        }
    });
    connect(ui->Update, &QPushButton::clicked, this, [&](){
        if(!ui->BidsListWidget->selectedItems().empty()){
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
    if(jsons.empty()){
        ui->bid_status_label->setText("No " + QString(type) + " bids");
    }
    for(const auto& json : jsons){
        int row = ui->BidsListWidget->rowCount();
        ui->BidsListWidget->insertRow(row);
        ui->BidsListWidget->setItem(row, 0, new QTableWidgetItem(json.toObject().value(s21::BDNames::bid_id_for_join).toString()));
        ui->BidsListWidget->setItem(row, 1, new QTableWidgetItem(json.toObject().value(s21::BDNames::joined_buyer_name).toString()));
        ui->BidsListWidget->setItem(row, 2, new QTableWidgetItem(json.toObject().value(s21::BDNames::joined_seller_name).toString()));
        ui->BidsListWidget->setItem(row, 3, new QTableWidgetItem(json.toObject().value(s21::BDNames::bid_table_quantity).toString()));
        ui->BidsListWidget->setItem(row, 4, new QTableWidgetItem(json.toObject().value(s21::BDNames::bid_table_rate).toString()));
        ui->BidsListWidget->setItem(row, 5, new QTableWidgetItem(json.toObject().value(s21::BDNames::bid_table_create_update_time).toString()));
        ui->BidsListWidget->setItem(row, 6, new QTableWidgetItem(type));

    }
    ui->BidsListWidget->resizeColumnsToContents();
    ui->BidsListWidget->setColumnWidth(1, 150);
    ui->BidsListWidget->setColumnWidth(2, 150);
}

void ViewBids::InsertNewBid(const std::string &bid)
{
    if(bid.empty()){
        ui->bid_status_label->setText("Bid fullfilled");
        return;
    }
    emit ViewBid(bid.find(s21::ExtraJSONKeys::created_sell_bid_quantity) != std::string::npos
                 ? "Sell"
                 : "Buy");
    ui->bid_status_label->setText("Bid created");
}

void ViewBids::InsertUpdatedBidBack(const std::string &bid, size_t bid_index)
{
    ui->BidsListWidget->removeRow(bid_index);
    if(!bid.empty()){
        QJsonArray jsons = QJsonDocument::fromJson(QString::fromStdString("[{" + bid + "}]").toUtf8()).array();
        int row = ui->BidsListWidget->rowCount();
        ui->BidsListWidget->insertRow(row);
        ui->BidsListWidget->setItem(row, 0, new QTableWidgetItem(jsons[0].toObject().value(s21::BDNames::bid_id_for_join).toString()));
        ui->BidsListWidget->setItem(row, 1, new QTableWidgetItem(jsons[0].toObject().value(s21::BDNames::joined_buyer_name).toString()));
        ui->BidsListWidget->setItem(row, 2, new QTableWidgetItem(jsons[0].toObject().value(s21::BDNames::joined_seller_name).toString()));
        ui->BidsListWidget->setItem(row, 3, new QTableWidgetItem(jsons[0].toObject().value(s21::BDNames::bid_table_quantity).toString()));
        ui->BidsListWidget->setItem(row, 4, new QTableWidgetItem(jsons[0].toObject().value(s21::BDNames::bid_table_rate).toString()));
        ui->BidsListWidget->setItem(row, 5, new QTableWidgetItem(jsons[0].toObject().value(s21::BDNames::bid_table_create_update_time).toString()));
        ui->BidsListWidget->setItem(row, 6, new QTableWidgetItem("Updated"));
        ui->bid_status_label->setText("Bid updated");
    }else{
        ui->bid_status_label->setText("Bid fullfilled");
    }
}

void ViewBids::closeEvent(QCloseEvent *event)
{
    ui->BidsListWidget->clear();
    ui->BidsListWidget->setRowCount(0);
    ui->BidsListWidget->setHorizontalHeaderLabels(
                    {"Bid ID", "Buyer Username", "Seller Username", "Quantity", "Rate", "Time", "Status"});
    ui->bid_status_label->clear();
    event->accept();
}

std::string ViewBids::GrabId()
{
    return ui->BidsListWidget->item(ui->BidsListWidget->currentRow(), 0)->text().toStdString();
}

std::string ViewBids::GrabRate()
{
    return ui->BidsListWidget->item(ui->BidsListWidget->currentRow(), 4)->text().toStdString();
}

std::string ViewBids::GrabQuantity()
{
    return ui->BidsListWidget->item(ui->BidsListWidget->currentRow(), 3)->text().toStdString();
}

