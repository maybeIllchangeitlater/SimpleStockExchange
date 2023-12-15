#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "../Client/Client.hpp"
#include "LoginPopup.hpp"
#include "RegisterPopup.hpp"
#include <memory.h>
#include <boost/make_unique.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(s21::Client &client, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void HandleLoginAttempt(const std::string username, const std::string password);
    void HandleRegisterAttempt(const std::string username, const std::string password, const std::string balance);

private:
    void SetLoginnedButtons();
    void SetNotLoginnedButtons();
    void Connect();
    Ui::MainWindow *ui;
    std::unique_ptr<LoginPopup> log_pop_ = boost::make_unique<LoginPopup>();
    std::unique_ptr<RegisterPopup> reg_pop_ = boost::make_unique<RegisterPopup>();
    s21::Client &client_;
};
#endif // MAINWINDOW_HPP
