#include "server.h"
#include "ui_server.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // QTcpServer 개체를 구성
    TCP_Server = new QTcpServer(this);
    // QHostAddress::Any인 경우 모든 네트워크 인터페이스에서 수신대기
    // 불자료형으로 반환
    // 연결 대기
    if(TCP_Server->listen(QHostAddress::Any,5001))
    {
        // newConnection()시그널을 받아 this.newConnection()함수 호출
        connect(TCP_Server, SIGNAL(newConnection()),this,SLOT(newConnection()));
        // 현재의 시간을 받아옴
        QDateTime currentDateTiem = QDateTime::currentDateTime();
        // 현재 시간 출력
        qDebug()<<"Server Open: "+currentDateTiem.toString("yyyy-MM-dd")+" " + currentDateTiem.toString("hh:mm:ss");
    }
    // DB에 연결
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("10.10.21.105");
    db.setDatabaseName("franchise");
    db.setUserName("network");
    db.setPassword("aaaa");
    if(!db.open())
        QMessageBox::information(this,"Error", "데이터베이스 연결 실패");

}

MainWindow::~MainWindow()
{
    delete ui;
}

// DB에서 데이터 받기 테스트 함수
void MainWindow::test_db()
{
    QString object_qstr = QString::fromStdString("달서구");
    QString queryStr;
    queryStr = QString("select cafe_name from cafe where address1 like '%%1%'").arg(object_qstr);
    QSqlQuery query;
    query.prepare(queryStr);
    query.exec();
    while(query.next())
        qDebug()<<query.value(0).toString();
}


// 대기중인 연결을 받는 함수
void MainWindow::newConnection()
{   // 대기중인 연결을 확인
    while (TCP_Server->hasPendingConnections())
    {   // 다음 대기중인 연결의 소켓객체를 받아 Add_New_Client_Connection()함수에 전달
        Add_New_Client_Connection(TCP_Server->nextPendingConnection());
    }
}

// 새로운 클라이언트 연결을 추가
void MainWindow::Add_New_Client_Connection(QTcpSocket *socket)
{   // 매개변수로 받은 socket객체를 Client_Connection_List에 추가
    Client_Connection_List.append(socket);
    // 소켓에서 readyRead() 신호를 읽어 this.Read_Data_From_Socket()를 호출
    connect(socket,SIGNAL(readyRead()),this,SLOT(Read_Data_From_Socket()));
    // 콤보박스에 연결된 소켓의 디스크립터(descriptor)를 추가
    // 디스트립터 : 소켓을 숫자로 표현한 추상표현(1번 누구, 2번 누구 에서 번호)
    ui-> comboBox ->addItem(QString::number(socket->socketDescriptor()));
    // QString Client 선언(소켓의 연결 확인용)
    QString Client = "Client : "+QString::number(socket->socketDescriptor())+" connected with The Server.";
    // textEdit에 QString Client 추가
    ui -> textEdit ->append(Client);
}

// 소켓으로 부터 데이터를 받기
void MainWindow::Read_Data_From_Socket()
{
    // 현재 함수를 호출한 객체의 포인터(sender())를 QTcpSocket* 형태로 변환
    QTcpSocket *socket =reinterpret_cast<QTcpSocket*>(sender());
    // 소켓에서 읽어온 모든 데이터를 QByteArray 형태로 받음
    QByteArray Message_From_Server = socket -> readAll();
    // QByteArray -> std::string -> QString으로 변환
    // QString Message = "Client : " + QString::number(socket->socketDescriptor())+"::"+QString::fromStdString(Message_From_Server.toStdString());
    QString Message = QString::number(socket->socketDescriptor())+"::"+ QString::fromStdString(Message_From_Server.toStdString());
    // socket->socketDescriptor()의 크기 확인 : 8 출력
    qDebug()<<sizeof socket->socketDescriptor();
    // QByteArray -> std::string 으로 변환
    string str_message = Message_From_Server.toStdString();
    // 받은 데이터가 DB인 경우
    if(str_message == "DB")
    {
        qDebug()<<"DB연동";
        // test_db 함수 호출
        test_db();
    }
    // 받은 데이터를 textEdit에 추가
    ui -> textEdit -> append(Message);
    // Client_Connection_List에 있는 모든 소켓에 데이터를 전송
    foreach(QTcpSocket * socket, Client_Connection_List)
    {
        socket->write(Message_From_Server);
    }
}

// 메시지 전송
void MainWindow::on_pushButton_clicked()
{
    // QString형의 변수 Message_For_Client 선언및 초기화
    QString Message_For_Client = ui->lineEdit->text();
    // QString형의 변수 Receiver 선언및 초기화
    QString Receiver = ui->comboBox->currentText();
    // 모든 클라이언트에 메시지를 모내는 경우
    if(ui->comboBox_2->currentText()=="ALL")
    {
        foreach(QTcpSocket *socket, Client_Connection_List)
        {
            // .c_str() : C++에서 문자열(string)을 char배열로 변환
            socket->write(Message_For_Client.toStdString().c_str());
        }
    }
    else
    {
        foreach(QTcpSocket *socket, Client_Connection_List)
        {
            // .toLongLong() : Qstring -> long long integer로 변환
            if(socket->socketDescriptor() == Receiver.toLongLong())
            {
                socket->write(Message_For_Client.toStdString().c_str());
            }
        }
    }
    ui->lineEdit->clear();
}


void MainWindow::on_lineEdit_returnPressed()
{
    on_pushButton_clicked();
}


void MainWindow::on_pushButton_2_clicked()
{
    test_db();
}

