#include "lan.h"
#include "tcpserver.h"
#include <QDebug>

TLAN::TLAN(QObject *parent) : QObject(parent)
{
    RTable["d1"].SrcLANAddr = "dH";
    RTable["d1"].Interface = "S";

    RTable["d2"].SrcLANAddr = "d1";
    RTable["d2"].Interface = "N";

    RTable["d3"].SrcLANAddr = "d2";
    RTable["d3"].Interface = "U";

    RTable["d4"].SrcLANAddr = "d2";
    RTable["d4"].Interface = "N";

    RTable["d5"].SrcLANAddr = "d4";
    RTable["d5"].Interface = "N";

    RTable["d6"].SrcLANAddr = "d4";
    RTable["d6"].Interface = "N";
}

TLAN::~TLAN()
{

}

bool TLAN::BuildSpecSBPacket(TPacket *Packet, QByteArray Path)
{
    Packet->setDir(DIR_SPECSTREAM);
    Packet->setFunc(SPEC_FUNC_R0);
    Packet->setUpStreamCommand(SPEC_SET_BRIDGE + Path);
    return true;
}

bool TLAN::BuildUpStreamPacket(TPacket *Packet, TFunc Func, QByteArray Command)
{
    Packet->setDir(DIR_UPSTREAM);
    Packet->setFunc(Func);
    Packet->setUpStreamCommand(Command);
    return true;
}

void TLAN::PushTcpRequest(QByteArray *Request)
{
TLAN_Addr DestAddr;
TPacket Packet;
TFunc Func;
QByteArray Command;

    Func = Request->mid(Request->indexOf(TCP_FUNCTIONTAG, 0) + strlen(TCP_FUNCTIONTAG), FUNC_ADDR_LENGTH);
    Command = Command = Request->mid(Request->indexOf(TCP_COMMANDTAG, 0) + strlen(TCP_COMMANDTAG), Request->indexOf(TCP_ENDOFCOMMANDTAG, Request->indexOf(TCP_COMMANDTAG, 0)) - Request->indexOf(TCP_COMMANDTAG, 0) - strlen(TCP_COMMANDTAG));
    DestAddr = Request->mid(Request->indexOf(TCP_DEVICETAG, 0) + strlen(TCP_DEVICETAG), LAN_ADDR_LENGTH);
    if (DestAddr != BridgedNode.SrcLANAddr)
        SetBridge(DestAddr);

    BuildUpStreamPacket(&Packet, Func, Command);
    SendPacket(&Packet);
}

bool TLAN::SendPacket(TPacket *pPacket)
{
    if (BridgedNode.Interface == INTERFACE_USB)
    {
        emit(SendPacketToUSB(pPacket));
        return true;
    }
    else
        return false;
}

QByteArray TLAN::SetBridge(TLAN_Addr DestLANAddr)
{
    QByteArray Path = "";
    QByteArray DbgPath = "";
    TLAN_Addr NodeLANAddr = DestLANAddr;
    char Cnt = 0;

    qDebug() << LAN_REBUILDBRIDGE;

    Path.append(DestLANAddr);
    Path.append(INTERFACE_LOCAL);

    DbgPath =  DestLANAddr;

    QMap<TLAN_Addr, TRTableVal>::iterator it = RTable.begin();
    while((it != RTable.end()) && (NodeLANAddr != HOST_LAN_ADDR) && (Cnt < BRIDGE_MAX_LENGTH))
    {
        if (NodeLANAddr == it.key())
        {
            Cnt++;
            DbgPath.prepend(it.value().SrcLANAddr+"<-"+it.value().Interface+"->");
            Path.prepend(it.value().SrcLANAddr + it.value().Interface);
            NodeLANAddr = it.value().SrcLANAddr;
            it = RTable.begin();
        }
        else
            it++;
    }
    qDebug() << LAN_CAPTION << DbgPath;

    if (NodeLANAddr != HOST_LAN_ADDR)
    {
        qDebug() << LAN_ERR_RTABLE_FAIL;
        return "";
    }
    else
    {
        qDebug() << LAN_REBUILDBRIDGE_OK << Path;
        TPacket SetBridgePacket;
        BuildSpecSBPacket(&SetBridgePacket, Path);

        BridgedNode.SrcLANAddr = DestLANAddr;
        BridgedNode.Interface = it.value().Interface;

        SendPacket(&SetBridgePacket);

        return Path;
    }
}

TPacket::TPacket()
{
    Clear();
}

void TPacket::Clear()
{
    Packet.fill(0, LAN_PACKET_LENGTH);
}

TDir TPacket::getDir()
{
    return Packet.mid(0, DIR_LENGTH);
}

void TPacket::setDir(TDir Dir)
{
    Packet.replace(0, DIR_LENGTH, Dir);
}

TFunc TPacket::getFunc()
{
    return Packet.mid(DIR_LENGTH, FUNC_LENGTH);
}

void TPacket::setFunc(TFunc Func)
{
    Packet.replace(DIR_LENGTH, FUNC_LENGTH, Func);
}

TLAN_Addr TPacket::getDevLANAddr()
{
    return Packet.mid(LAN_PACKET_LENGTH - LAN_ADDR_LENGTH, LAN_ADDR_LENGTH);
}

void TPacket::setDevLANAddr(TLAN_Addr DevLAN_Addr)
{
    Packet.replace(LAN_PACKET_LENGTH - LAN_ADDR_LENGTH, LAN_ADDR_LENGTH, DevLAN_Addr);
}

TFunc TPacket::getUpStreamCommand()
{
    return Packet.mid(DIR_LENGTH + FUNC_LENGTH, UP_STREAM_COMMAND_LENGTH);
}

void TPacket::setUpStreamCommand(QByteArray UpStreamCommand)
{
    if (UpStreamCommand.length() > UP_STREAM_COMMAND_LENGTH)
    {
        qDebug() << LAN_ERR_LONG_COMMAND;
        UpStreamCommand = UpStreamCommand.left(UP_STREAM_COMMAND_LENGTH);
    }
    if (UpStreamCommand.length() < UP_STREAM_COMMAND_LENGTH)
    {
        qDebug() << LAN_ERR_SHORT_COMMAND;
        while (UpStreamCommand.length() < UP_STREAM_COMMAND_LENGTH)
            //UpStreamCommand.append('*');
            UpStreamCommand.append((char)0);
    }
    Packet.replace(DIR_LENGTH + FUNC_LENGTH, UP_STREAM_COMMAND_LENGTH, UpStreamCommand);
}

TFunc TPacket::getDownStreamCommand()
{
    return Packet.mid(DIR_LENGTH + FUNC_LENGTH, DOWN_STREAM_COMMAND_LENGTH);
}

void TPacket::setDownStreamCommand(QByteArray DownStreamCommand)
{
    if (DownStreamCommand.length() > DOWN_STREAM_COMMAND_LENGTH)
    {
        qDebug() << LAN_ERR_LONG_COMMAND;
        DownStreamCommand = DownStreamCommand.left(DOWN_STREAM_COMMAND_LENGTH);
    }
    if (DownStreamCommand.length() < DOWN_STREAM_COMMAND_LENGTH)
    {
        qDebug() << LAN_ERR_SHORT_COMMAND;
        while (DownStreamCommand.length() < DOWN_STREAM_COMMAND_LENGTH)
            //DownStreamCommand.append('*');
            DownStreamCommand.append((char)0);
    }
    Packet.replace(DIR_LENGTH + FUNC_LENGTH, DOWN_STREAM_COMMAND_LENGTH, DownStreamCommand);
}

QByteArray TPacket::getValue()
{
    if (Packet.length() != LAN_PACKET_LENGTH)
        qDebug() << LAN_ERR_PACKET_LENGTH_MISMATCH;
    return Packet;
}

void TPacket::setValue(QByteArray Val)
{
    if (Val.length() != LAN_PACKET_LENGTH)
        qDebug() << LAN_ERR_PACKET_LENGTH_MISMATCH;
    Packet.replace(0, LAN_PACKET_LENGTH, Val);
}

QByteArray TPacket::print()
{
    QByteArray res;
    res = getValue();
    res.replace(0, '*');
    return res;
}

