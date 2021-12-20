#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "connect.h"



#define SIZE_PREAM 32

#define INTERVAL 10000

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString getLinkName() const;


    unsigned long getCountReq() const;
    void setCountReq(unsigned long _countReq = 0);

public slots:

    void showResult(bool, unsigned long);
    void startFunc();
    void successConn(unsigned long num);
    void errorConn(unsigned long num);
    void timerReq();
    void setLinkName(QString _linkName = "");

private slots:

    void textProc(const QString&);
    QString strProc(const QString&);

    void on_pushButton_clear_clicked();

private:

    Ui::MainWindow *ui;
    connectYt * m_channell;
    QTimer * m_timer;

    QString linkName;

    unsigned long countReq;

signals:

    void sendRequest(QString &);

};
#endif // MAINWINDOW_H
