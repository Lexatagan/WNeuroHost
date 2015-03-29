#ifndef WLANPIPE_H
#define WLANPIPE_H

#include <QMap>
#include <QObject>
#include <QQueue>

#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include "wlanpacket.h"

#define PACKET_LENGTH                   32
#define MSG_PREFIX_LENGTH               2
#define MSG_LENGTH                      PACKET_LENGTH + MSG_PREFIX_LENGTH
#define MSG_PREFIX                      "->"

#define COM_DEFAULTPORT                  "COM2"
#define PACKET_LAN_ADDR_LENGTH           3
#define DEFAULT_NONE_LAN_ADDR            "   "
#define DEFAULT_HOST_LAN_ADDR            "Ud1"
#define DEFAULT_UART_BRIDGE_LAN_ADDR     "Nd1"
#define BRIDGE_MAX_LENGTH                9

#define WLANPIPE_ERROR                  "WlanPipe:  "
#define MSG_TRYINGTOCONNECT             WLANPIPE_ERROR << "Trying to connect to"
#define ROUTING_ERROR                   WLANPIPE_ERROR << "Routing error!"
#define ROUTING_SUCCESS                 WLANPIPE_ERROR << "Routing OK"
#define ROUTING_REBUILDBRIDGE           WLANPIPE_ERROR << "Rebuildiding bridge..."
#define MSG_RECIEVED                    WLANPIPE_ERROR << "<- "
#define MSG_TRANSIEVED                  WLANPIPE_ERROR << "-> "
#define MSG_UARTOPENED                  WLANPIPE_ERROR << COM_DEFAULTPORT << "opened successfully"
#define MSG_STARTING                    WLANPIPE_ERROR << "Starting host. Local RFAddr - "

#define FUNC_ADDR_R0                    "R0"
#define FUNC_R0_RESET                   "reset"

class TWlanMessage;

class WlanPipe : public QObject
{
    Q_OBJECT
public:
    WlanPipe(QObject *parent = 0);
    void OpenPort(void);
    void ClosePort();

    QMap <QByteArray, QByteArray> routingTable;
    QByteArray BridgedRFAddr;
    QByteArray BridgeThroughtRFAddr;
    void SendPacket(QByteArray packet);
    void SendMessage(TWlanMessage Msg);

signals:
    
public slots:
    void readResponse();
    void ParseMessage(TWlanMessage Msg);
private:
    WlanPacket *Pack;
    QSerialPort serial;
    QByteArray response;
    bool SetBridge(QByteArray DestRFAddr);
};

class TWlanMessage
{
public:
    bool LoadFromTcpRequest(QByteArray Request);
    QByteArray RFAddr;
    QByteArray FuncAddr;
    QByteArray Command;
private:
};

#endif // QWLAN_H
