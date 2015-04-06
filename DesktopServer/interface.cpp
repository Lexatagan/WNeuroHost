#include "interface.h"
#include "USBTransceiver.h"
#include "lan.h"
//#include <QtSerialPort/QSerialPortInfo>

Interface::Interface(QObject *parent) :
    QObject(parent)
{
    /*qDebug() << IF_AVAILABLE_PORTS;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        qDebug() << "-" << info.portName();*/
    USBTransceiver *pUSBTransceiver = new USBTransceiver();
    pUSBTransceiver->USBInit();
    pUSBTransceiver->Open();

    Server = new TCPServer(this);

    TLAN *pLAN = new TLAN(this);

    connect(Server, SIGNAL(Recieved(QByteArray*)), pLAN, SLOT(PushTcpRequest(QByteArray*)));
    connect(pLAN, SIGNAL(SendPacketToUSB(TPacket*)), pUSBTransceiver, SLOT(SendPacket(TPacket*)));

    qDebug() << "--------Initialization done--------";
}
