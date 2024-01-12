#include "UpdateBidPopup.hpp"

#include "ui_UpdateBidPopup.h"

UpdateBidPopup::UpdateBidPopup(QWidget *parent)
    : QDialog(parent), ui(new Ui::UpdateBidPopup) {
  ui->setupUi(this);
  connect(ui->update, &QPushButton::clicked, this, [&]() {
    emit UpdateBid(ui->bid_id->text().toStdString(),
                   ui->bid_rate->text().toStdString(),
                   ui->bid_quantity->text().toStdString(), bid_index_);
  });
}

UpdateBidPopup::~UpdateBidPopup() { delete ui; }

void UpdateBidPopup::SetParameters(const std::string &id,
                                   const std::string &rate,
                                   const std::string &quantity, int bid_index) {
  ui->bid_id->setText(QString::fromStdString(id));
  ui->bid_rate->setText(QString::fromStdString(rate));
  ui->bid_quantity->setText(QString::fromStdString(quantity));
  bid_index_ = bid_index;
}
