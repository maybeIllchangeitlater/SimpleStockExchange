#ifndef QUOTATIONSPOPUP_HPP
#define QUOTATIONSPOPUP_HPP

#include <QDialog>
#include <QCloseEvent>
#include "../Utility/ExtraJSONKeys.hpp"

namespace Ui {
class QuotationsPopup;
}

class QuotationsPopup : public QDialog
{
    Q_OBJECT

public:
    explicit QuotationsPopup(QWidget *parent = nullptr);
    ~QuotationsPopup();

    void SetQuotations(const std::string &rate);

signals:
    void Quotations(size_t time_period);
protected:
    void closeEvent(QCloseEvent *event) override;

private:
    enum TimePeriod{
        DAY = 1,
        WEEK = 7,
        MONTH = 30,
        YEAR = 365,
    };
    Ui::QuotationsPopup *ui;
};

#endif // QUOTATIONSPOPUP_HPP
