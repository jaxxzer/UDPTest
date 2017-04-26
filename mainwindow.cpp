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

    m_udpSocket->bind(QHostAddress("192.168.2.1"), 45454, QAbstractSocket::ShareAddress);

    m_sendTimer.start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendMessage(void)
{
    if (!m_senderPort) {
        return;
    }
    qDebug() << "Sending message";
    QByteArray datagram = "\nAloha Back";
    m_udpSocket->writeDatagram(datagram.data(), datagram.size(), m_sender, m_senderPort);
}

void MainWindow::receiveMessage(void)
{
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        m_udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        qDebug() << "Received datagram:" << datagram.data() << sender << senderPort;

        if (!m_senderPort) {
            m_sender = sender;
            m_senderPort = senderPort;
        }
    }
}
