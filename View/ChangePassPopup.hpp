#ifndef SIMPLESTOCKEXCHANGE_VIEW_CHANGEPASSPOPUP_HPP
#define SIMPLESTOCKEXCHANGE_VIEW_CHANGEPASSPOPUP_HPP

#include <QCloseEvent>
#include <QDialog>

namespace Ui {
class ChangePassPopup;
}

class ChangePassPopup : public QDialog {
  Q_OBJECT

 public:
  explicit ChangePassPopup(QWidget *parent = nullptr);
  ~ChangePassPopup();

 signals:
  void ChangePassword(const std::string new_password,
                      const std::string old_password);

 protected:
  void closeEvent(QCloseEvent *event) override;

 private:
  Ui::ChangePassPopup *ui;
};

#endif  // SIMPLESTOCKEXCHANGE_VIEW_CHANGEPASSPOPUP_HPP
