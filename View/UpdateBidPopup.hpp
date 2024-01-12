#ifndef SIMPLESTOCKEXCHANGE_VIEW_UPDATEBIDPOPUP_HPP
#define SIMPLESTOCKEXCHANGE_VIEW_UPDATEBIDPOPUP_HPP

#include <QDialog>

namespace Ui {
class UpdateBidPopup;
}

class UpdateBidPopup : public QDialog {
  Q_OBJECT

 public:
  explicit UpdateBidPopup(QWidget *parent = nullptr);
  ~UpdateBidPopup();
  void SetParameters(const std::string &id, const std::string &rate,
                     const std::string &quantity, int bid_index);
 signals:
  void UpdateBid(const std::string id, const std::string rate,
                 const std::string quantity, int bid_index);

 private:
  Ui::UpdateBidPopup *ui;
  int bid_index_;
};

#endif  // SIMPLESTOCKEXCHANGE_VIEW_UPDATEBIDPOPUP_HPP
