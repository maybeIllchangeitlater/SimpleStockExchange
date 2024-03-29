#ifndef SIMPLESTOCKEXCHANGE_VIEW_VIEWBIDS_HPP
#define SIMPLESTOCKEXCHANGE_VIEW_VIEWBIDS_HPP

#include <QCloseEvent>
#include <QDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "../Repository/BdNames.hpp"
#include "../Utility/ClientDisplayMessages.hpp"
#include "../Utility/ExtraJSONKeys.hpp"

namespace Ui {
class ViewBids;
}

class ViewBids : public QDialog {
  Q_OBJECT

 public:
  explicit ViewBids(QWidget *parent = nullptr);
  ~ViewBids();

  void ShowBids(const std::string &bids, const char *type);
  void InsertNewBid(const std::string &bid);
  void InsertUpdatedBidBack(const std::string &bid, size_t bid_index);

 signals:
  void ViewBid(std::string bid_type);
  void CancelBid(std::string bid_id);
  void UpdateBid(std::string bid_id, std::string bid_rate,
                 std::string bid_quantity, int item_index);
  void CreateBid();

 protected:
  void closeEvent(QCloseEvent *event) override;

 private:
  void ClearExistingBidsOfMatchingType(const char *type);
  std::string GrabId();
  std::string GrabRate();
  std::string GrabQuantity();
  Ui::ViewBids *ui;
};

#endif  // SIMPLESTOCKEXCHANGE_VIEW_VIEWBIDS_HPP
