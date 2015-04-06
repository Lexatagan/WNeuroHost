#include "wlanpacket.h"
#include <QtDebug>
WlanPacket::WlanPacket()
{
    Direction = 0;
    RFAddr.fill(0, RF_ADDR_LENGTH);
    FuncAddr.fill(0, FUNC_ADDR_LENGTH);
}

void WlanPacket::setDirection(char Dir)
{
    Direction = Dir;
}

char WlanPacket::getDirection()
{
    return Direction;
}

bool WlanPacket::setRFAddr(QByteArray Addr)
{
    if (Addr.size() != RF_ADDR_LENGTH)
    {
        qDebug() << RF_ADDR_LENGTH_ERROR;
        return false;
    }
    RFAddr = Addr;
    return true;
}

QByteArray WlanPacket::getRFAddr()
{
    return RFAddr;
}

bool WlanPacket::setFuncAddr(QByteArray Addr)
{
    if (Addr.size() != FUNC_ADDR_LENGTH)
    {
        qDebug() << FUNC_ADDR_LENGTH_ERROR;
        return false;
    }
    FuncAddr = Addr;
    return true;
}

QByteArray WlanPacket::getFuncAddr()
{
    return FuncAddr;
}

bool WlanPacket::setCommand(QByteArray Com)
{
    if (Com.size() != getCommandLength())
    {
        qDebug() << COMMAND_LENGTH_ERROR;
        return false;
    }
    Command = Com;
    Assemble();
    return true;
}

bool WlanPacket::setSafeCommand(QByteArray Com)
{
    QByteArray s;
    s.fill(0, getCommandLength());
    s.replace(0, Com.size(), Com);
    return setCommand(s);
}

int WlanPacket::getCommandOffset()
{
    /*if ((Direction == DIR_UPSTREAM) || (Direction == DIR_SPECSTREAM))
        return LONGCOMMAND_OFFSET;
    if (Direction == DIR_DOWNSTREAM)
        return SHORTCOMMAND_OFFSET;
    qDebug() << DIRECTION_ERROR;
    return -1;*/
}

int WlanPacket::getCommandLength()
{
    /*if ((Direction == DIR_UPSTREAM) || (Direction == DIR_SPECSTREAM))
        return LONGCOMMAND_LENGTH;
    if (Direction == DIR_DOWNSTREAM)
        return SHORTCOMMAND_LENGTH;
    qDebug() << DIRECTION_ERROR;
    return -1;*/
}

bool WlanPacket::Assemble()
{
    /*Packet.resize(0);
    Packet.append(Direction);
    Packet.append(FuncAddr);
    if (Direction == DIR_DOWNSTREAM)
        Packet.append(RFAddr);
    Packet.append(Command);
    if (Packet.size() != PACKET_LENGTH)
    {
        qDebug() << COMMAND_LENGTH_ERROR;
        return false;
    }
    return true;*/
}

QByteArray WlanPacket::getPacket()
{
    return Packet;
}

bool WlanPacket::SpecSET_BRIDGE(QString Path)
{
    /*Direction = DIR_SPECSTREAM;
    FuncAddr = SPEC_FUNC_ADDR;
    QByteArray Com("");
    Com.append(SPEC_SET_BRIDGE);
    Com.append(Path);
    return setSafeCommand(Com);*/
}
