#ifndef LAN_H
#define LAN_H

#include <QObject>
#include <QMap>

#define LAN_PACKET_LENGTH               32
#define DIR_LENGTH                      1
#define FUNC_LENGTH                     2
#define LAN_ADDR_LENGTH                 2
#define UP_STREAM_COMMAND_LENGTH        29
#define DOWN_STREAM_COMMAND_LENGTH      27

#define BRIDGE_MAX_LENGTH               9
#define HOST_LAN_ADDR                   "dH"
#define INTERFACE_RF                    "N"
#define INTERFACE_UART                  "U"
#define INTERFACE_USB                   "S"
#define INTERFACE_LOCAL                 "L"
#define DIR_UPSTREAM                    "U"
#define DIR_DOWNSTREAM                  "D"
#define DIR_SPECSTREAM                  "S"
#define SPEC_FUNC_R0                    "R0"
#define SPEC_SET_BRIDGE                 "SB"

#define LAN_CAPTION                     "LAN: "
#define LAN_ERROR                       LAN_CAPTION << "error: "
#define LAN_REBUILDBRIDGE               LAN_CAPTION << "rebuilding bridge..."
#define LAN_REBUILDBRIDGE_OK            LAN_CAPTION << "ok -"
#define LAN_ERR_PACKET_LENGTH_MISMATCH  LAN_ERROR << "packet size error"
#define LAN_ERR_LONG_COMMAND            LAN_ERROR << "too long command, truncating..."
#define LAN_ERR_SHORT_COMMAND           LAN_ERROR << "too short command, extending..."
#define LAN_ERR_BRIDGE_MAX_LENGTH       LAN_ERROR << "too long bridge."
#define LAN_ERR_RTABLE_FAIL             LAN_ERROR << "routing table failed."

typedef QByteArray TLAN_Addr;
typedef QByteArray TInterface;
typedef QByteArray TDir;
typedef QByteArray TFunc;

struct TRTableVal
{
    TLAN_Addr SrcLANAddr;
    TInterface Interface;
};

class TPacket
{
public:
    TPacket();
    void Clear();

    TDir getDir();
    void setDir(TDir Direction);

    TFunc getFunc();
    void setFunc(TFunc Func);

    TLAN_Addr getDevLANAddr();
    void setDevLANAddr(TLAN_Addr DevLAN_Addr);

    QByteArray getDownStreamCommand();
    void setDownStreamCommand(QByteArray Command);

    QByteArray getUpStreamCommand();
    void setUpStreamCommand(QByteArray Command);

    QByteArray getValue();
    void setValue(QByteArray Val);

    QByteArray print();

private:
    QByteArray Packet;

};

class TLAN : public QObject
{
    Q_OBJECT
public:
    explicit TLAN(QObject *parent = 0);
    ~TLAN();
    QByteArray SetBridge(TLAN_Addr DestLANAddr);
    bool BuildUpStreamPacket(TPacket *Packet, TFunc Func, QByteArray Command);
    bool BuildSpecSBPacket(TPacket *Packet, QByteArray Path);
    bool SendPacket(TPacket *pPacket);
    TRTableVal BridgedNode;
    QMap <TLAN_Addr, TRTableVal> RTable;

signals:
    void SendPacketToUSB(TPacket *Packet);

public slots:
    void PushTcpRequest(QByteArray *Request);
};

#endif // LAN_H
