#ifndef LOGINPOPUP_HPP
#define LOGINPOPUP_HPP

#include <QDialog>
#include <string>

namespace Ui {
class LoginPopup;
}

class LoginPopup : public QDialog
{
    Q_OBJECT

public:
    explicit LoginPopup(QWidget *parent = nullptr);
    ~LoginPopup();

signals:
    void LoginAttempt(std::string login, std::string password);

private:
    Ui::LoginPopup *ui;
};

#endif // LOGINPOPUP_HPP
