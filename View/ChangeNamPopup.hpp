#ifndef SIMPLESTOCKEXCHANGE_VIEW_CHANGENAMPOPUP_HPP
#define SIMPLESTOCKEXCHANGE_VIEW_CHANGENAMPOPUP_HPP

#include <QDialog>

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

private:
    Ui::ChangeNamPopup *ui;
};

#endif // SIMPLESTOCKEXCHANGE_VIEW_CHANGENAMPOPUP_HPP
