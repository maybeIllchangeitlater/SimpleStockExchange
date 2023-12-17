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

signals:
    void ViewBid(std::string bid_type);
    void CancelBid(std::string bid_id);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    std::string GrabId();
    Ui::ViewBids *ui;
};

#endif // VIEWBIDS_HPP