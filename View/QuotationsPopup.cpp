#include "QuotationsPopup.hpp"
#include "ui_QuotationsPopup.h"

QuotationsPopup::QuotationsPopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuotationsPopup)
{
    ui->setupUi(this);
    connect(ui->day, &QPushButton::clicked, this, [&](){
        ui->Quotationlist->addItem("Today's rate for USD is :");
        emit Quotations(DAY);
    });
    connect(ui->week, &QPushButton::clicked, this, [&](){
        ui->Quotationlist->addItem("This week rate for USD is :");
        emit Quotations(WEEK);
    });
    connect(ui->month, &QPushButton::clicked, this, [&](){
        ui->Quotationlist->addItem("This month rate for USD is :");
        emit Quotations(MONTH);
    });
    connect(ui->year, &QPushButton::clicked, this, [&](){
        ui->Quotationlist->addItem("This year rate for USD is :");
        emit Quotations(YEAR);
    });
}

QuotationsPopup::~QuotationsPopup()
{
    delete ui;
}

void QuotationsPopup::SetQuotations(const std::string &rate)
{
    auto qrate = QString::fromStdString(rate).remove(s21::ExtraJSONKeys::quotations).remove('"').remove(":");
    if(qrate == "null"){
        qrate = "No transactions found";
    }
    ui->Quotationlist->addItem(qrate);
}

void QuotationsPopup::closeEvent(QCloseEvent *event)
{
    ui->Quotationlist->clear();
    event->accept();
}
