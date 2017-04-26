#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_udpSocket = new QUdpSocket(this);

    // Send
    connect(&m_sendTimer, SIGNAL(timeout()), this, SLOT(sendMessage()));

    // Receive
    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(receiveMessage()));

    m_udpSocket->bind(QHostAddress("192.168.2.1"), 45454, QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint);
//    m_udpSocket->setLocalPort(45454);
    //m_sendTimer.start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendMessage(void)
{
    qDebug() << "Sending message";
    QByteArray datagram = "\nBroadcast message";
    m_udpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress("192.168.2.1"), 45454);
}

void MainWindow::receiveMessage(void)
{
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(), datagram.size());
        qDebug() << QString("Received datagram: %1").arg(datagram.data());
    }
}
