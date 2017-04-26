#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void sendMessage(void);
    void receiveMessage(void);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QTimer m_sendTimer;
    QUdpSocket* m_udpSocket;

    // Can be Remote or Local
    QHostAddress m_client;
    quint16 m_clientPort;

    // Us
    static const QHostAddress m_host;
    static const quint16 m_hostPort;
};

#endif // MAINWINDOW_H
