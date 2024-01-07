#ifndef SIMPLESTOCKEXCHANGE_VIEW_CHANGENAMPOPUP_HPP
#define SIMPLESTOCKEXCHANGE_VIEW_CHANGENAMPOPUP_HPP

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class ChangeNamPopup;
}

class ChangeNamPopup : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeNamPopup(QWidget *parent = nullptr);
    ~ChangeNamPopup();
signals:
    void NameChange(const std::string username);
protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::ChangeNamPopup *ui;
};

#endif // SIMPLESTOCKEXCHANGE_VIEW_CHANGENAMPOPUP_HPP
