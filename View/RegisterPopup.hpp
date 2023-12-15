#ifndef REGISTERPOPUP_HPP
#define REGISTERPOPUP_HPP

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
    void RegisterAttempt(std::string login, std::string password, std::string balance);

private:
    Ui::RegisterPopup *ui;
};

#endif // REGISTERPOPUP_HPP
