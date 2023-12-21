#ifndef UPDATEBIDPOPUP_HPP
#define UPDATEBIDPOPUP_HPP

#include <QDialog>

namespace Ui {
class UpdateBidPopup;
}

class UpdateBidPopup : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateBidPopup(QWidget *parent = nullptr);
    ~UpdateBidPopup();
     void SetParameters(const std::string &id, const std::string &rate, const std::string &quantity);
signals:
     void UpdateBid(const std::string id, const std::string rate, const std::string quantity);
private:
    Ui::UpdateBidPopup *ui;
};

#endif // UPDATEBIDPOPUP_HPP
