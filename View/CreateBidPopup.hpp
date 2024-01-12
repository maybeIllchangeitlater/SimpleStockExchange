#ifndef SIMPLESTOCKEXCHANGE_VIEW_CREATEBIDPOPUP_HPP
#define SIMPLESTOCKEXCHANGE_VIEW_CREATEBIDPOPUP_HPP

#include <QCloseEvent>
#include <QDialog>

namespace Ui {
class CreateBidPopup;
}

class CreateBidPopup : public QDialog {
  Q_OBJECT

 public:
  explicit CreateBidPopup(QWidget *parent = nullptr);
  ~CreateBidPopup();

 signals:
  void MakeBid(std::string quantity, std::string rate, std::string bid_type);

 protected:
  void closeEvent(QCloseEvent *event) override;

 private:
  Ui::CreateBidPopup *ui;
};

#endif  // SIMPLESTOCKEXCHANGE_VIEW_CREATEBIDPOPUP_HPP
