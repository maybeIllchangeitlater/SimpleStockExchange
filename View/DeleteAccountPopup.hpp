#ifndef DELETEACCOUNTPOPUP_HPP
#define DELETEACCOUNTPOPUP_HPP

#include <QDialog>

namespace Ui {
class DeleteAccountPopup;
}

class DeleteAccountPopup : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteAccountPopup(QWidget *parent = nullptr);
    ~DeleteAccountPopup();

signals:
    void DeleteAccount();

private:
    Ui::DeleteAccountPopup *ui;
};

#endif // DELETEACCOUNTPOPUP_HPP
