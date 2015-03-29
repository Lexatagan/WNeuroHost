#include "wlanPipe.h"

#include <QDebug>
#include <QQueue>

#include "wlanpacket.h"
#include "tcpserver.h"

WlanPipe::WlanPipe(QObject *parent) :
    QObject(parent),
    Pack(new WlanPacket)
{
    routingTable["Nd1"] = "Ud1";
    routingTable["Nd2"] = "Nd1";
    routingTable["Nd3"] = "Nd1";

    BridgedRFAddr = DEFAULT_NONE_LAN_ADDR;
    BridgeThroughtRFAddr = DEFAULT_NONE_LAN_ADDR;

    qDebug() << MSG_STARTING << DEFAULT_HOST_LAN_ADDR;

    connect(&serial, SIGNAL(readyRead()),
            this, SLOT(readResponse()));
}

void WlanPipe::OpenPort(void)
{
    qDebug() << MSG_TRYINGTOCONNECT << COM_DEFAULTPORT;

    serial.close();
    serial.setPortName(COM_DEFAULTPORT);

    if (!serial.open(QSerialPort::ReadWrite)) {
        qDebug() << (tr("Can't open %1, error code %2")
                     .arg(serial.portName()).arg(serial.error()));
        return;
    }

    if (!serial.setBaudRate(QSerialPort::Baud38400)) {
        qDebug() << (tr("Can't set rate 38400 baud to port %1, error code %2")
                     .arg(serial.portName()).arg(serial.error()));
        return;
    }

    if (!serial.setDataBits(QSerialPort::Data8)) {
        qDebug() << (tr("Can't set 8 data bits to port %1, error code %2")
                     .arg(serial.portName()).arg(serial.error()));
        return;
    }

    if (!serial.setParity(QSerialPort::NoParity)) {
        qDebug() << (tr("Can't set no patity to port %1, error code %2")
                     .arg(serial.portName()).arg(serial.error()));
        return;
    }

    if (!serial.setStopBits(QSerialPort::OneStop)) {
        qDebug() << (tr("Can't set 1 stop bit to port %1, error code %2")
                     .arg(serial.portName()).arg(serial.error()));
        return;
    }

    if (!serial.setFlowControl(QSerialPort::NoFlowControl)) {
        qDebug() << (tr("Can't set no flow control to port %1, error code %2")
                     .arg(serial.portName()).arg(serial.error()));
        return;
    }
    qDebug() << MSG_UARTOPENED;
}

void WlanPipe::ClosePort()
{
    serial.close();
}

void WlanPipe::readResponse()
{
    QByteArray S;
    QByteArray Sentence;
    S = serial.readAll();
    response.append(S);

    while ((response.contains(MSG_PREFIX)) && response.length() >= MSG_LENGTH)
    {
        if (response.contains(MSG_PREFIX))
            response.remove(0, response.indexOf(MSG_PREFIX));
        if (response.length() >= MSG_LENGTH)
        {
            Sentence = response.mid(MSG_PREFIX_LENGTH, PACKET_LENGTH);
            Sentence.replace(0, 'x');
            qDebug() << MSG_RECIEVED << Sentence;
            response.remove(0, MSG_LENGTH);
            BridgedRFAddr = DEFAULT_NONE_LAN_ADDR;
        }

    }
}

void WlanPipe::SendMessage(TWlanMessage Msg)
{
    if ((Msg.RFAddr != BridgedRFAddr))
    {
        qDebug() << ROUTING_REBUILDBRIDGE;
        if (SetBridge(Msg.RFAddr))
            qDebug() << ROUTING_SUCCESS;
        else
        {
            qDebug() << ROUTING_ERROR;
            return;
        }

    }

    Pack->setDirection(DIR_UPSTREAM);
    Pack->setFuncAddr(Msg.FuncAddr);
    Pack->setSafeCommand(Msg.Command);
    SendPacket(Pack->getPacket());
}

bool WlanPipe::SetBridge(QByteArray DestRFAddr)
{
    int Cnt(1);
    QByteArray RFAddr(DestRFAddr);
    QByteArray Path(RFAddr);
    WlanPacket SpecPacket;

    QMap<QByteArray, QByteArray>::iterator it = routingTable.begin();

    Path.append("LAD");

    while((it != routingTable.end()) && (RFAddr != DEFAULT_HOST_LAN_ADDR) && (Cnt < BRIDGE_MAX_LENGTH))
    {
        if (RFAddr == it.key())
        {
            Cnt++;
            Path.prepend(it.value());
            RFAddr = it.value();
            it = routingTable.begin();
        }
        else
            it++;
    }

    if (RFAddr != DEFAULT_HOST_LAN_ADDR)
        return false;

    BridgedRFAddr = DestRFAddr;
    BridgeThroughtRFAddr = Path.mid(3, 3);

    SpecPacket.SpecSET_BRIDGE(Path);
    SendPacket(SpecPacket.getPacket());

    return true;
}

void WlanPipe::SendPacket(QByteArray packet)
{
    if (BridgeThroughtRFAddr == DEFAULT_UART_BRIDGE_LAN_ADDR)
    {
        serial.write("->" + packet);
        packet.replace(0, ' ');
        qDebug() << MSG_TRANSIEVED << packet;
    }
    else
        qDebug() << ROUTING_ERROR;
}

void WlanPipe::ParseMessage(TWlanMessage Msg)
{
    if (Msg.RFAddr == DEFAULT_HOST_LAN_ADDR)
    {
        qDebug() << "Xxx";
        if (Msg.FuncAddr == FUNC_ADDR_R0)
            if (Msg.Command == FUNC_R0_RESET)
            {
                BridgedRFAddr = DEFAULT_NONE_LAN_ADDR;
            }
    }
    else
        SendMessage(Msg);
}

bool TWlanMessage::LoadFromTcpRequest(QByteArray Request)
{
qint16 index;
bool Res = true;
    RFAddr = "";
    FuncAddr = "";
    Command = "";

    index = Request.indexOf(TCP_DEVICETAG, 0);
    if (index != 0)
        RFAddr = Request.mid(index + strlen(TCP_DEVICETAG), RF_ADDR_LENGTH);
    else
        Res = false;

    index = Request.indexOf(TCP_FUNCTIONTAG, 0);
    if (index != 0)
        FuncAddr = Request.mid(index + strlen(TCP_FUNCTIONTAG), FUNC_ADDR_LENGTH);
    else
        Res = false;

    index = Request.indexOf(TCP_COMMANDTAG, 0);
    if (index != 0)
        Command = Request.mid(index + strlen(TCP_COMMANDTAG), Request.indexOf(TCP_ENDOFCOMMANDTAG, index) - index - strlen(TCP_COMMANDTAG));
    else
        Res = false;

    return Res;
}
