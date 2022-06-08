#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QtSerialPort/QtSerialPort>
#include <QList>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = nullptr);
    ~SerialPort();
public:
    QList<QSerialPortInfo> portlist;
    QByteArray bytes;
public:
    // 打开串口
    bool openPort(QString portname,int baud=9600);
    // 关闭串口
    void closePort();
    // 信号转发
    void readyReadyProcess();
    // 检查连接状态
    bool isOpen();

    // 发送指令
    void sendData(QByteArray data);
private:
    QSerialPort *port;
signals:
    void dataready();
};

#endif // SERIALPORT_H
