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
                    {s21::BDNames::bid_id_for_join, s21::BDNames::joined_buyer_name,
                     s21::BDNames::joined_seller_name, s21::BDNames::bid_table_quantity,
                     s21::BDNames::bid_table_rate, s21::BDNames::bid_table_create_update_time,
                     s21::ClientDisplayMessages::table_status});
    ui->BidsListWidget->setColumnWidth(1, 150);
    ui->BidsListWidget->setColumnWidth(2, 150);
    ui->BidsListWidget->setShowGrid(true);
    ui->BidsListWidget->verticalHeader()->setVisible(false);
    connect(ui->Cancel, &QPushButton::clicked, this, [&](){
        if(!ui->BidsListWidget->selectedItems().empty()){
            emit CancelBid(GrabId());
            ui->BidsListWidget->removeRow(ui->BidsListWidget->currentRow());
            ui->bid_status_label->setText(s21::ClientDisplayMessages::bid_cancelled);
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
    }else{
        ClearExistingBidsOfMatchingType(type);
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
        ui->bid_status_label->setText(s21::ClientDisplayMessages::bid_fulfilled);
        return;
    }
    emit ViewBid(bid.find(s21::ExtraJSONKeys::created_sell_bid_quantity) != std::string::npos
                 ? s21::ClientDisplayMessages::sell_bid_type
                 : s21::ClientDisplayMessages::buy_bid_type);
    ui->bid_status_label->setText(s21::ClientDisplayMessages::bid_created);
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
        ui->BidsListWidget->setItem(row, 6, new QTableWidgetItem(s21::ClientDisplayMessages::updated_bid_status));
        ui->bid_status_label->setText(s21::ClientDisplayMessages::bid_updated);
    }else{
        ui->bid_status_label->setText(s21::ClientDisplayMessages::bid_fulfilled);
    }
}

void ViewBids::closeEvent(QCloseEvent *event)
{
    ui->BidsListWidget->clear();
    ui->BidsListWidget->setRowCount(0);
    ui->BidsListWidget->setHorizontalHeaderLabels(
                {s21::BDNames::bid_id_for_join, s21::BDNames::joined_buyer_name,
                 s21::BDNames::joined_seller_name, s21::BDNames::bid_table_quantity,
                 s21::BDNames::bid_table_rate, s21::BDNames::bid_table_create_update_time, s21::ClientDisplayMessages::table_status});
    ui->bid_status_label->clear();
    event->accept();
}

void ViewBids::ClearExistingBidsOfMatchingType(const char *type)
{
    bool delete_sell = !std::strcmp(type, s21::ClientDisplayMessages::sell_bid_type);
    for(std::ptrdiff_t row = ui->BidsListWidget->rowCount() - 1; row >= 0; --row){
        if(ui->BidsListWidget->item(row, 1)->text() != s21::BDNames::missing_buyer && !delete_sell){
            ui->BidsListWidget->removeRow(row);
        }else if(ui->BidsListWidget->item(row, 2)->text() != s21::BDNames::missing_seller && delete_sell){
            ui->BidsListWidget->removeRow(row);
        }
    }
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

