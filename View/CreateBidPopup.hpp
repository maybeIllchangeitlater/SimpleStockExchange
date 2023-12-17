#ifndef CREATEBIDPOPUP_HPP
#define CREATEBIDPOPUP_HPP

#include <QDialog>

namespace Ui {
class CreateBidPopup;
}

class CreateBidPopup : public QDialog
{
    Q_OBJECT

public:
    explicit CreateBidPopup(QWidget *parent = nullptr);
    ~CreateBidPopup();

signals:
    void MakeBid(std::string quantity, std::string rate, std::string bid_type);

private:
    Ui::CreateBidPopup *ui;
};

#endif // CREATEBIDPOPUP_HPP
