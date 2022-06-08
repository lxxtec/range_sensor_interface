#include "serialport.h"

SerialPort::SerialPort(QObject *parent) : QObject(parent)
{
    portlist  = QSerialPortInfo::availablePorts();
    port = new QSerialPort();
    QObject::connect(port,&QSerialPort::readyRead,this,&SerialPort::readyReadyProcess);
}

void SerialPort::readyReadyProcess(){
    bytes = port->readAll();
    emit dataready();
}

void SerialPort::sendData(QByteArray data){
    port->write(data);
}
bool SerialPort::openPort(QString portname,int baud){
    port->setPortName(portname);//选择串口号
    //打开串口
    if( !port->open(QIODevice::ReadWrite)){
        return false;
    }
    port->setBaudRate(baud);
    port->setDataBits(QSerialPort::Data8);//8位数据位
    port->setParity(QSerialPort::NoParity);//无检验
    port->setStopBits(QSerialPort::OneStop);//1位停止位
    port->setFlowControl(QSerialPort::NoFlowControl);//无硬件控制
    return true;
}
void SerialPort::closePort(){
    port->close();
}
bool SerialPort::isOpen(){
    return port->isOpen();
}

SerialPort::~SerialPort(){
    if(port->isOpen()){
        port->close();
    }
    delete port;
}
