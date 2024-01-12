#ifndef SIMPLESTOCKEXCHANGE_VIEW_PASSWORDCONFIRMATIONPOPUP_HPP
#define SIMPLESTOCKEXCHANGE_VIEW_PASSWORDCONFIRMATIONPOPUP_HPP

#include <QCloseEvent>
#include <QDialog>

namespace Ui {
class PasswordConfirmationPopup;
}

class PasswordConfirmationPopup : public QDialog {
  Q_OBJECT

 public:
  explicit PasswordConfirmationPopup(QWidget *parent = nullptr);
  ~PasswordConfirmationPopup();

 signals:
  void ConfirmAction(const std::string user_password);

 protected:
  void closeEvent(QCloseEvent *event) override;

 private:
  Ui::PasswordConfirmationPopup *ui;
};

#endif  // SIMPLESTOCKEXCHANGE_VIEW_PASSWORDCONFIRMATIONPOPUP_HPP
