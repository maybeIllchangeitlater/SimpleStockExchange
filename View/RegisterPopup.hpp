#ifndef SIMPLESTOCKEXCHANGE_VIEW_REGISTERPOPUP_HPP
#define SIMPLESTOCKEXCHANGE_VIEW_REGISTERPOPUP_HPP

#include <QDialog>

namespace Ui {
class RegisterPopup;
}

class RegisterPopup : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterPopup(QWidget *parent = nullptr);
    ~RegisterPopup();

signals:
    void RegisterAttempt(std::string login, std::string password, std::string balance_usd, std::string balance_rub);

private:
    Ui::RegisterPopup *ui;
};

#endif // SIMPLESTOCKEXCHANGE_VIEW_REGISTERPOPUP_HPP
