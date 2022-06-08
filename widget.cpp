#include "widget.h"
#include "ui_widget.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    qDebug()<<" init timer ";
    {
        timer = new QTimer();
        timer->setInterval(50);
        QObject::connect(timer,&QTimer::timeout,this,[this](){
            //qDebug()<<"time out";
            QSurfaceDataArray *data = new QSurfaceDataArray;
            QSurfaceDataRow *dataRow1 = new QSurfaceDataRow;
            QSurfaceDataRow *dataRow2 = new QSurfaceDataRow;
            static int i=0;
            if(i<500){
                *dataRow1 << QVector3D(-0.02f*i/10.0, 0.4f, 0.03f*i/10.0) << QVector3D(0.02f*i, 0.4f, 0.02f*i);
                *dataRow2 << QVector3D(-2.0f, 0.01f*i, 0.03f*i) << QVector3D(2.0f, 0.01f*i, 0.03f*i);
                *data << dataRow1 << dataRow2;
                i++;
            }
            modifier->m_dataProxy->resetArray(data);
          }
        );
        timer->start();
    }

    qDebug()<<" init graph ";
    {
        // graph
        graph = new Q3DSurface();
        container = QWidget::createWindowContainer(graph);
        //! [0]
        if (!graph->hasContext()) {
            QMessageBox msgBox;
            msgBox.setText("Couldn't initialize the OpenGL context.");
            msgBox.exec();
        }

        container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        container->setFocusPolicy(Qt::StrongFocus);

        ui->graphLayout->addWidget(container,1);

        qDebug()<<" init modifier ";
        modifier=new SurfaceGraph(graph);


        ui->comboxColor->addItem(QStringLiteral("Qt"));
        ui->comboxColor->addItem(QStringLiteral("Primary Colors"));
        ui->comboxColor->addItem(QStringLiteral("Digia"));
        ui->comboxColor->addItem(QStringLiteral("Stone Moss"));
        ui->comboxColor->addItem(QStringLiteral("Army Blue"));
        ui->comboxColor->addItem(QStringLiteral("Retro"));
        ui->comboxColor->addItem(QStringLiteral("Ebony"));
        ui->comboxColor->addItem(QStringLiteral("Isabelle"));

        QLinearGradient grBtoY(0, 0, 1, 100);
        grBtoY.setColorAt(1.0, Qt::black);
        grBtoY.setColorAt(0.67, Qt::blue);
        grBtoY.setColorAt(0.33, Qt::red);
        grBtoY.setColorAt(0.0, Qt::yellow);

        QLinearGradient grGtoR(0, 0, 1, 100);
        grGtoR.setColorAt(1.0, Qt::darkGreen);
        grGtoR.setColorAt(0.5, Qt::yellow);
        grGtoR.setColorAt(0.2, Qt::red);
        grGtoR.setColorAt(0.0, Qt::darkRed);
        QObject::connect(ui->comboxColor, SIGNAL(currentIndexChanged(int)),
                         modifier, SLOT(changeTheme(int)));

        QObject::connect(ui->redblue,&QRadioButton::clicked,modifier,&SurfaceGraph::setBlackToYellowGradient);
        QObject::connect(ui->redgreen, &QRadioButton::clicked,modifier, &SurfaceGraph::setGreenToRedGradient);
        //emit ui->redblue->clicked();
        // 必须要加slider
        modifier->setAxisMinSliderX(ui->slider1);
        modifier->setAxisMaxSliderX(ui->slider2);
        modifier->setAxisMinSliderZ(ui->slider3);
        modifier->setAxisMaxSliderZ(ui->slider4);
        // 绑定范围修改的槽
        QObject::connect(ui->slider1, &QSlider::valueChanged,
                         modifier, &SurfaceGraph::adjustXMin);
        QObject::connect(ui->slider2, &QSlider::valueChanged,
                         modifier, &SurfaceGraph::adjustXMax);
        QObject::connect(ui->slider3, &QSlider::valueChanged,
                         modifier, &SurfaceGraph::adjustZMin);
        QObject::connect(ui->slider4, &QSlider::valueChanged,
                         modifier, &SurfaceGraph::adjustZMax);
        // 绑定鼠标选择模式修改的槽
        QObject::connect(ui->selectNo, &QRadioButton::toggled,
                         modifier, &SurfaceGraph::toggleModeNone);
        QObject::connect(ui->selectDot,  &QRadioButton::toggled,
                         modifier, &SurfaceGraph::toggleModeItem);
        QObject::connect(ui->selectRow,  &QRadioButton::toggled,
                         modifier, &SurfaceGraph::toggleModeSliceRow);
        QObject::connect(ui->selectCol,  &QRadioButton::toggled,
                         modifier, &SurfaceGraph::toggleModeSliceColumn);


       // modifier->enableSqrtSinModel(true);
        modifier->enableDataModel(true);
    }

    qDebug()<<" init serial port ";
    {
        port=new SerialPort();
        foreach (const QSerialPortInfo & info, port->portlist) {
            ui->comSerialNum->addItem(info.portName());
        }
        ui->comSerialBaud->addItem(tr("4800"));
        ui->comSerialBaud->addItem(tr("9600"));
        ui->comSerialBaud->addItem(tr("19200"));
        ui->comSerialBaud->addItem(tr("38400"));
        ui->comSerialBaud->addItem(tr("115200"));
        ui->comSerialBaud->addItem(tr("256000"));
        ui->comSerialBaud->setCurrentIndex(4);

        QObject::connect(ui->btOpen,&QPushButton::clicked,this,[this](){
            QString name = ui->btOpen->text();//获取按钮上的文字
                if(name == tr("打开串口"))
                {
                    // 打开串口
                    if(!port->openPort(ui->comSerialNum->currentText(),ui->comSerialBaud->currentText().toInt())){
                        QMessageBox::information(this, tr("提示"),
                            tr("打开串口失败,请查看串口是否被占用"), QMessageBox::Ok);
                        return;
                    }
                    ui->btOpen->setText(tr("关闭串口"));//改变按钮上的文字
                    ui->comSerialNum->setDisabled(true);//串口号下拉列表变灰
                    ui->comSerialBaud->setDisabled(true);//波特率下接列表变灰
                }else
                {
                    port->closePort();//关闭串口
                    ui->comSerialNum->setEnabled(true);//串口号下拉列表变亮
                    ui->comSerialBaud->setEnabled(true);//串口号下拉列表变亮
                    ui->btOpen->setText(tr("打开串口"));
                }


        });
        QObject::connect(port,&SerialPort::dataready,this,&Widget::on_serial_recvd);
        QObject::connect(ui->btScan,&QPushButton::clicked,this,[this](){
            if(!port->isOpen()){
                QMessageBox::critical(this, tr("错误"),tr("请先打开串口"), QMessageBox::Ok);
                    return;
            }
        });
    }
}
void Widget::on_serial_recvd(){
    if (!port->bytes.isEmpty())
    {
        //QString strRecv = QString::fromLocal8Bit(port->bytes);
        QDataStream out(&port->bytes,QIODevice::ReadWrite);
        QString strHex;
        while(!out.atEnd())
        {
              qint8 outChar = 0;
              out>>outChar;
              QString str = QString("%1 ").arg(outChar&0xFF,2,16,QLatin1Char('0'));
              strHex += str;
         }
        ui->textRecv->appendPlainText(strHex);

        //qDebug()<<strRecv;
        process_data(port->bytes);
    }
}


struct Protocal{
    char head;
    char tail;
    uint16_t data[8];
};

std::vector<float> Widget::process_data(QByteArray &bytes){
    // 得到完整一帧数据
    std::vector<float> res;
    bool stflag=false,endflag=false;
    for(int i=0;i<bytes.length();i++){
        if((bytes[i]&0xff)==0x31){
            // 遇到帧头跳过
            stflag=true;
            endflag=false;
            continue;
        }else if((bytes[i]&0xff)==0x40){
            // 遇到帧尾跳过
            stflag=false;
            endflag=true;
            continue;
        }
        if(stflag && !endflag && i<bytes.length()-1){
            //qDebug()<<QString("%1").arg(bytes[i]&0xFF,2,16,QLatin1Char('0'));
            //qDebug()<<QString("%1").arg(bytes[i+1]&0xFF,2,16,QLatin1Char('0'));
            unsigned char d1=bytes[i]&0xFF;
            unsigned char d2=bytes[i+1]&0xFF;
            uint16_t dat=d1<<8|d2;
            //qDebug()<<"data is "<<dat;
            i+=1;
            res.push_back(dat*15.0/10000); //解算距离
        }
    }
    return res;
}

Widget::~Widget()
{
    delete ui;
}



void Widget::on_btClearR_clicked()
{
    ui->textRecv->clear();
}

void Widget::on_btClearS_clicked()
{
    ui->textSend->clear();
}

void Widget::on_btSend_clicked()
{
    port->sendData(ui->textSend->placeholderText().toLocal8Bit());
}

void Widget::on_btSaveR_clicked()
{
    QDateTime time = QDateTime::currentDateTime();
    QString dateTime = time.toString("yyyy-MM-dd-hh-mm");
    QString str= QString("data-%1.txt").arg(dateTime);
    QFile file(str);
    if(file.open(QIODevice::WriteOnly)){
        QString str=ui->textRecv->toPlainText();
        //qDebug()<<str;
        file.write(str.toUtf8(),str.length());
        file.close();
        QMessageBox::information(this,"提示","保存成功");
    }else{
        qDebug()<<"failed open";
    }
}




QVector3D Widget::to3DPoint(float angleH, float angleV, float distance){
    float z=distance*sinf(angleV);
     qDebug()<<z;
    float tmp=distance*cosf(angleV);
    float x=tmp*cosf(angleH);
    float y=tmp*sinf(angleH);
    return QVector3D(x,z,y);
}

void Widget::on_btStartScan_clicked()
{
    //modifier->enableDataModel(true);
    //timer->start();
}
