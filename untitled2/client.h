#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
// TCP
// 이진데이터를 다루는데 사용
#include <QDataStream>
#include <QByteArray>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_lineEdit_returnPressed();

    void Read_Data_From_Socket();

private:
    Ui::MainWindow *ui;
    // TCPSocket 포인터 변수 선언
    QTcpSocket *TCPSocket;
};
#endif // CLIENT_H
