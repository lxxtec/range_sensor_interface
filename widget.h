#ifndef WIDGET_H
#define WIDGET_H

#include "serialport.h"
#include "surfacegraph.h"
#include <QDebug>
#include <QList>
#include <QTimer>
#include <QWidget>
#include <QtDataVisualization>
#include <QtGui/QPainter>
#include <QtGui/QScreen>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QtSerialPort>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <math.h>
#include <vector>
#define _USE_MATH_DEFINES

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
    Q_OBJECT

  public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

  public:
    void on_serial_recvd();

    std::vector<float> process_data(QByteArray &bytes);

    QVector3D to3DPoint(float angleH, float angleV, float distance);

  private slots:
    void on_btClearR_clicked();

    void on_btClearS_clicked();

    void on_btSend_clicked();

    void on_btSaveR_clicked();

    void on_btStartScan_clicked();

  private:
    Ui::Widget *ui;
    Q3DSurface *graph;
    QWidget *container;
    SurfaceGraph *modifier;
    QTimer *timer;

  private:
    SerialPort *port;
    QList<QSerialPortInfo> portlist;
};
#endif // WIDGET_H
