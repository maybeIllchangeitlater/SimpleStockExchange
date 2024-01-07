#ifndef SIMPLESTOCKEXCHANGE_VIEW_BIDTRANSACTIONMADEPOPUP_HPP
#define SIMPLESTOCKEXCHANGE_VIEW_BIDTRANSACTIONMADEPOPUP_HPP

#include <vector>
#include <string>
#include <sstream>
#include <QDialog>
#include <QCloseEvent>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "../Utility/ExtraJSONKeys.hpp"

namespace Ui {
class BidTransactionMadePopup;
}

class BidTransactionMadePopup : public QDialog
{
    Q_OBJECT

public:
    explicit BidTransactionMadePopup(QWidget *parent = nullptr);
    ~BidTransactionMadePopup();

    std::string DisplayNewTransactions(const std::string &raw_response);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    std::string NewBid(const QJsonValueRef &json, QString bid_type);
    Ui::BidTransactionMadePopup *ui;
};

#endif // SIMPLESTOCKEXCHANGE_VIEW_BIDTRANSACTIONMADEPOPUP_HPP
