#ifndef USBTRANSCEIVER_H
#define USBTRANSCEIVER_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include "lusb0_usb.h"
#include "wlanPipe.h"
#include "LAN.h"

#define DEV_VID                                     0x0483
#define DEV_PID                                     0x5740
#define IN_ENDPOINT                                 0x81
#define OUT_ENDPOINT                                0x02

#define USB_CAPTION                                 "USBTrans: "

#define USB_TRYINGTOCONNECT                         USB_CAPTION << "Trying to connect to VID:0x0483/PID:0x5740.."
#define USB_TRANSCEIVEROPENED                       USB_CAPTION << "-connected successfully"
#define USB_TRANSCEIVERCLOSED                       USB_CAPTION << "Disconnected"
#define USB_RECIEVED                                USB_CAPTION << "<- "
#define USB_TRANSIEVED                              USB_CAPTION << "-> "

#define USB_ERR                                     USB_CAPTION << "error: "
#define USB_ERR_NOT_OPENED                          USB_ERR << "USB transceiver not opened."
//#define ROUTING_ERROR                               USB_CAPTION << "Routing error!"
//#define ROUTING_SUCCESS                             USB_CAPTION << "Routing OK"
//#define ROUTING_REBUILDBRIDGE                       USB_CAPTION << "Rebuildiding bridge..."

class USBTransceiver : public QThread
{
    Q_OBJECT
public:
    explicit USBTransceiver();
    ~USBTransceiver();
    void USBInit();
    struct usb_device *Find();
    usb_dev_handle *Open();
    void Close();
    int SendCommand(int Command, int Data);
    char RBuf[2000];
    bool PollingEnabled;
    void Recieved(void);
    void run();

private:
    usb_dev_handle *Handle;

signals:

public slots:
    void SendPacket(TPacket *pPacket);
};

#endif // USBTRANSCEIVER_H
