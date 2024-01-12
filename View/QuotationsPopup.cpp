#include "QuotationsPopup.hpp"

#include "ui_QuotationsPopup.h"

QuotationsPopup::QuotationsPopup(QWidget *parent)
    : QDialog(parent), ui(new Ui::QuotationsPopup) {
  ui->setupUi(this);
  connect(ui->day, &QPushButton::clicked, this, [&]() {
    ui->Quotationlist->addItem(s21::ClientDisplayMessages::daily_rate_for_usd);
    emit Quotations(DAY);
  });
  connect(ui->week, &QPushButton::clicked, this, [&]() {
    ui->Quotationlist->addItem(s21::ClientDisplayMessages::weekly_rate_for_usd);
    emit Quotations(WEEK);
  });
  connect(ui->month, &QPushButton::clicked, this, [&]() {
    ui->Quotationlist->addItem(
        s21::ClientDisplayMessages::monthly_rate_for_usd);
    emit Quotations(MONTH);
  });
  connect(ui->year, &QPushButton::clicked, this, [&]() {
    ui->Quotationlist->addItem(s21::ClientDisplayMessages::yearly_rate_for_usd);
    emit Quotations(YEAR);
  });
}

QuotationsPopup::~QuotationsPopup() { delete ui; }

void QuotationsPopup::SetQuotations(const std::string &rate) {
  auto qrate = QString::fromStdString(rate)
                   .remove(s21::ExtraJSONKeys::quotations)
                   .remove('"')
                   .remove(":");
  if (qrate == s21::ClientDisplayMessages::missing) {
    qrate = s21::ClientDisplayMessages::no_transactions;
  }
  ui->Quotationlist->addItem(qrate);
}

void QuotationsPopup::closeEvent(QCloseEvent *event) {
  ui->Quotationlist->clear();
  event->accept();
}
