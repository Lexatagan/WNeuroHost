#ifndef WLANPACKET_H
#define WLANPACKET_H

#include <QByteArray>


#define DIR_UPSTREAM                    'U'
#define DIR_DOWNSTREAM                  'D'
#define DIR_SPECSTREAM                  'S'

#define SPEC_FUNC_ADDR                  "R0"
#define SPEC_SET_BRIDGE                 "SB"

#define BRIDGE_MAX_LENGTH               9

//#define DEFAULT_NONE_RFADDR             "   "
//#define DEFAULT_HOST_RFADDR             "hs "

#define PACKET_LENGTH                   32
#define DIR_LENGTH                      1
#define RF_ADDR_LENGTH                  3
#define FUNC_ADDR_LENGTH                2
#define LONGCOMMAND_LENGTH              (PACKET_LENGTH - DIR_LENGTH - FUNC_ADDR_LENGTH)
#define SHORTCOMMAND_LENGTH             (LONGCOMMAND_LENGTH - RF_ADDR_LENGTH)

#define DIR_OFFSET                      0
#define FUNC_ADDR_OFFSET                DIR_LENGTH
#define DEV_RF_ADDR_OFFSET              (FUNC_ADDR_OFFSET + FUNC_ADDR_LENGTH)
#define LONGCOMMAND_OFFSET              (FUNC_ADDR_OFFSET + FUNC_ADDR_LENGTH)
#define SHORTCOMMAND_OFFSET             (FUNC_ADDR_OFFSET + FUNC_ADDR_LENGTH + RF_ADDR_LENGTH)

/*
#define PACKET_SPEC_TYPE_OFFSET         (PACKET_DIR_LENGTH + PACKET_FUNC_ADDR_LENGTH)
#define PACKET_SPEC_PATH_OFFSET         (PACKET_DIR_LENGTH + PACKET_FUNC_ADDR_LENGTH + PACKET_SPEC_TYPE_LENGTH)
#define PACKET_STOP_OFFSET              PACKET_DIR_LENGTH
*/
#define PACKET_STOP_LENGTH              2
#define PACKET_SPEC_TYPE_LENGTH         2

#define WLANPACKET_ERROR                "WlanPacket: "
#define RF_ADDR_LENGTH_ERROR            WLANPACKET_ERROR << "RFAddr length error!"
#define FUNC_ADDR_LENGTH_ERROR          WLANPACKET_ERROR << "FuncAddr length error!"
#define COMMAND_LENGTH_ERROR            WLANPACKET_ERROR << "Command length error!"
#define ASSEMBLING_ERROR                WLANPACKET_ERROR << "Error occurs during assembling!"
#define DIRECTION_ERROR                 WLANPACKET_ERROR << "Unknown Direction!"

class WlanPacket
{
public:
    WlanPacket();
    void setDirection(char Dir);
    char getDirection();
    bool setRFAddr(QByteArray Addr);
    QByteArray getRFAddr();
    bool setFuncAddr(QByteArray Addr);
    QByteArray getFuncAddr();
    bool setCommand(QByteArray Com);
    bool setSafeCommand(QByteArray Com);
    QByteArray getCommand();
    bool Assemble();
    bool Disassemble();
    QByteArray getPacket();

    bool SpecSET_BRIDGE(QString Path);


private:
    int getCommandLength();
    int getCommandOffset();

    QByteArray Packet;
    char Direction;
    QByteArray RFAddr;
    QByteArray FuncAddr;
    QByteArray Command;
};

#endif // WLANPACKET_H
