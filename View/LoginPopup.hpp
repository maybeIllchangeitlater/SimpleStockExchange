#ifndef SIMPLESTOCKEXCHANGE_VIEW_LOGINPOPUP_HPP
#define SIMPLESTOCKEXCHANGE_VIEW_LOGINPOPUP_HPP

#include <QCloseEvent>
#include <QDialog>
#include <string>

namespace Ui {
class LoginPopup;
}

class LoginPopup : public QDialog {
  Q_OBJECT

 public:
  explicit LoginPopup(QWidget *parent = nullptr);
  ~LoginPopup();

 signals:
  void LoginAttempt(std::string login, std::string password);

 protected:
  void closeEvent(QCloseEvent *event) override;

 private:
  Ui::LoginPopup *ui;
};

#endif  // SIMPLESTOCKEXCHANGE_VIEW_LOGINPOPUP_HPP
