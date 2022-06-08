#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "surfacegraph.h"
#include "serialport.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtGui/QScreen>
#include <QtDataVisualization>
#include <QDebug>
#include <QtSerialPort/QtSerialPort>
#include <QList>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <vector>
#include <math.h>
#define _USE_MATH_DEFINES

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public:
    void on_serial_recvd();

    std::vector<float> process_data(QByteArray &bytes);

    QVector3D to3DPoint(float angleH,float angleV,float distance);

private slots:
    void on_btClearR_clicked();

    void on_btClearS_clicked();

    void on_btSend_clicked();

    void on_btSaveR_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_4_clicked();

    void on_btStartScan_clicked();

private:
    Ui::Widget *ui;
    Q3DSurface *graph;
    QWidget *container;
    SurfaceGraph *modifier;
    QTimer *timer;
private:
    SerialPort  *port;
    QList<QSerialPortInfo> portlist;
};
#endif // WIDGET_H
