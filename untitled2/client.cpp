#include "client.h"
#include "ui_client.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    TCPSocket = new QTcpSocket();
    TCPSocket->connectToHost(QHostAddress::LocalHost,5001);
    // QTcpSocket의 객체 TCPSocket에 데이터 송수신 가능한 상태 선언
    TCPSocket->open(QIODevice::ReadWrite);
    // readyRead()신호로 Read_Data_From_Socket() 호출 선언
    connect(TCPSocket,SIGNAL(readyRead()),this,SLOT(Read_Data_From_Socket()));
    // 서버와 소켓 연결 확인
    if (TCPSocket->isOpen())
        QMessageBox::information(this, "Qt With Ketan", "서버 연결");
    else
        QMessageBox::information(this, "Qt With Ketan", "서버 열결 실패");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Read_Data_From_Socket()
{
    //TCPSocket의 존재여부 확인
    if(TCPSocket)
    {
       if(TCPSocket->isOpen())
        {
            qDebug()<<"메시지 받음";
            // 소켓의 모든 데이터 받기
            QByteArray Data_from_Server = TCPSocket->readAll();
            // QString::fromStdString() 함수를 사용하여 std::string변환->QString 변환
            // QString MessageString = QString::fromStdString(Data_from_Server.toStdString());
            QString MessageString = QString::fromUtf8(Data_from_Server);
            ui->textEdit->append(MessageString);
        }
    }
}


void MainWindow::on_pushButton_clicked()
{
    if(TCPSocket)
    {
        if(TCPSocket->isOpen())
        {
            QString qmsg = ui->lineEdit->text();
            TCPSocket->write(qmsg.toStdString().c_str());
            qDebug()<<qmsg;
        }
        else
            QMessageBox::information(this,"Qr With Keten","error: "+TCPSocket->errorString());
    }
    else
        QMessageBox::information(this,"Qr With Keten","error: "+TCPSocket->errorString());
    ui->lineEdit->clear();
}


void MainWindow::on_lineEdit_returnPressed()
{
    on_pushButton_clicked();
}

