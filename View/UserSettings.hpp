#ifndef SIMPLESTOCKEXCHANGE_VIEW_USERSETTINGS_HPP
#define SIMPLESTOCKEXCHANGE_VIEW_USERSETTINGS_HPP

#include <ChangeNamPopup.hpp>
#include <ChangePassPopup.hpp>
#include <PasswordConfirmationPopup.hpp>
#include <QDialog>
#include <boost/make_unique.hpp>

#include "../Utility/ClientDisplayMessages.hpp"

namespace Ui {
class UserSettings;
}

class UserSettings : public QDialog {
  Q_OBJECT

 public:
  explicit UserSettings(QWidget *parent = nullptr);
  ~UserSettings();
  void SetFields(const std::string &name, const std::string &id,
                 const std::string &usd_balance,
                 const std::string &rub_balance);
  void SetFields(const std::string &name);
  void SetStatus(const std::string &status);
 signals:
  void ChangeUsername(const std::string user_name);
  void ChangePassword(const std::string new_password,
                      const std::string old_password);
  void DeleteAccount(const std::string user_password);

 private:
  Ui::UserSettings *ui;
  std::unique_ptr<ChangeNamPopup> namechange_pop_ =
      boost::make_unique<ChangeNamPopup>();
  std::unique_ptr<ChangePassPopup> passchange_pop_ =
      boost::make_unique<ChangePassPopup>();
  std::unique_ptr<PasswordConfirmationPopup> confirm_pop_ =
      boost::make_unique<PasswordConfirmationPopup>();
};

#endif  // SIMPLESTOCKEXCHANGE_VIEW_USERSETTINGS_HPP
