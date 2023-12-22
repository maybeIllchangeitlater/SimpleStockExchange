#ifndef VIEWBIDS_HPP
#define VIEWBIDS_HPP

#include <QDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QCloseEvent>
#include "../Repository/BdNames.hpp"

namespace Ui {
class ViewBids;
}

class ViewBids : public QDialog
{
    Q_OBJECT

public:
    explicit ViewBids(QWidget *parent = nullptr);
    ~ViewBids();

    void ShowBids(const std::string &bids, const char *type);
    void InsertUpdatedBidBack(const std::string &bid, size_t bid_index);

signals:
    void ViewBid(std::string bid_type);
    void CancelBid(std::string bid_id);
    void UpdateBid(std::string bid_id, std::string bid_rate, std::string bid_quantity, int item_index);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    std::string GrabId();
    std::string GrabRate();
    std::string GrabQuantity();
    QString GrabBidFromJson(const QJsonValueRef &json);
    Ui::ViewBids *ui;
};

#endif // VIEWBIDS_HPP
