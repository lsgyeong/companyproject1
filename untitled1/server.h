#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <iostream>

// TCP
#include <QList>
#include <QByteArray>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

// DB
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>

// 스레드
#include <Qthread>

#include <QDateTime>

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
    void newConnection();

    void Read_Data_From_Socket();

    void on_pushButton_clicked();

    void on_lineEdit_returnPressed();

    void test_db();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;

    void Add_New_Client_Connection(QTcpSocket *socket);
    // QTcpServer클래스의 포이터 변수 TCP_Server 선언
    QTcpServer *TCP_Server;
    // QTcpSocket클래스의 포인터를 저장하는 Client_Connection_List 선언
    QList<QTcpSocket*> Client_Connection_List;
};
#endif // SERVER_H
