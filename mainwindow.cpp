#include "mainwindow.h"
#include "ui_mainwindow.h"

// Listen for udp traffic here, once we get traffic we start replying to the client
const QHostAddress MainWindow::m_host = QHostAddress("localhost");
const quint16 MainWindow::m_hostPort = 8989;

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

    // Listen here
    m_udpSocket->bind(m_host, m_hostPort, QAbstractSocket::ShareAddress);

    m_sendTimer.start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Sends messages to the client port
void MainWindow::sendMessage(void)
{
    if (!m_clientPort) {
        return; // We don't send anything until a client connects
    }
    qDebug() << "Sending message";
    QByteArray datagram = "\nAloha Back";
    m_udpSocket->writeDatagram(datagram.data(), datagram.size(), m_client, m_clientPort);
}

// Listens for messages on the bound port
void MainWindow::receiveMessage(void)
{
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        m_udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        qDebug() << "Received datagram:" << datagram.data() << sender << senderPort;

        // Start replying to the first client we hear from
        if (!m_clientPort) {
            m_client = sender;
            m_clientPort = senderPort;
        }
    }
}
