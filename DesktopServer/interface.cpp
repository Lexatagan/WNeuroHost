#include "interface.h"
#include "USBTransceiver.h"
//#include <QtSerialPort/QSerialPortInfo>

Interface::Interface(QObject *parent) :
    QObject(parent)
{
    /*qDebug() << IF_AVAILABLE_PORTS;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        qDebug() << "-" << info.portName();*/
    USBTransceiver *pUSBTransceiver = new USBTransceiver;
    pUSBTransceiver->USBInit();
    pUSBTransceiver->Open();

    WPipe = new WlanPipe(this);
    WPipe->OpenPort();
    Server = new TCPServer(this);
    connect(Server, SIGNAL(Send(TWlanMessage)), WPipe, SLOT(ParseMessage(TWlanMessage)));
    connect(Server, SIGNAL(Send(TWlanMessage)), pUSBTransceiver, SLOT(ParseMessage(TWlanMessage)));
}
