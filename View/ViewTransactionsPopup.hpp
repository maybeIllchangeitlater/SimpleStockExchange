#ifndef SIMPLESTOCKEXCHANGE_VIEW_VIEWTRANSACTIONSPOPUP_HPP
#define SIMPLESTOCKEXCHANGE_VIEW_VIEWTRANSACTIONSPOPUP_HPP

#include <QDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QCloseEvent>
#include "../Repository/BdNames.hpp"

namespace Ui {
class ViewTransactionsPopup;
}

class ViewTransactionsPopup : public QDialog
{
    Q_OBJECT

public:
    explicit ViewTransactionsPopup(QWidget *parent = nullptr);
    ~ViewTransactionsPopup();

    void DisplayTransactions(const std::string &type, const std::string &raw_response);

signals:
    void ShowBuyTransactions();
    void ShowSellTransactions();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QString GrabTransactionFromJson(const QJsonValueRef &json);
    Ui::ViewTransactionsPopup *ui;
};

#endif // SIMPLESTOCKEXCHANGE_VIEW_VIEWTRANSACTIONSPOPUP_HPP
