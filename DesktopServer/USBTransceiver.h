#ifndef USBTRANSCEIVER_H
#define USBTRANSCEIVER_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include "lusb0_usb.h"
#include"wlanPipe.h"

#define DEV_VID                                     0x0483
#define DEV_PID                                     0x5740
#define IN_ENDPOINT                                 0x81
#define OUT_ENDPOINT                                0x02

#define LAN_PACKET_LENGTH                           32
#define TRANSCEIVER_CAPTION                         "USBTrans:  "

#define MSG_TRYINGTOCONNECT                         TRANSCEIVER_CAPTION << "trying to connect to VID:0x0483/PID:0x5740"
#define MSG_TRANSCEIVEROPENED                       TRANSCEIVER_CAPTION << "connected successfully"
#define MSG_TRANSCEIVERCLOSED                       TRANSCEIVER_CAPTION << "disconnected"
#define MSG_RECIEVED                                TRANSCEIVER_CAPTION << "<- "
#define MSG_TRANSIEVED                              TRANSCEIVER_CAPTION << "-> "

#define ROUTING_ERROR                               TRANSCEIVER_CAPTION << "Routing error!"
#define ROUTING_SUCCESS                             TRANSCEIVER_CAPTION << "Routing OK"
#define ROUTING_REBUILDBRIDGE                       TRANSCEIVER_CAPTION << "Rebuildiding bridge..."

class USBTransceiver : public QThread
{
public:
    explicit USBTransceiver(QObject *parent = 0);
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
    void ParseMessage(TWlanMessage Msg);
};

#endif // USBTRANSCEIVER_H
