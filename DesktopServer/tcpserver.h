#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "wlanPipe.h"

#define TCP_LISTENING_PORT 1811

#define TCP_HEADLINE                    "TCPServer: "
#define OPENING_ERROR                   TCP_HEADLINE << "Server opening failed."
#define OPENING_SUCCESS                 TCP_HEADLINE << "Server opening successed."
#define BINDING_SUCCESS                 TCP_HEADLINE << "Server binded on http://LocalIP:"
#define PENDINGCONNECTION_ERROR         TCP_HEADLINE << "There is no pending connection."
#define RECIEVEDDATA                    TCP_HEADLINE << "Recieving:"
#define PARSING_SUCCESS                 TCP_HEADLINE << "Parsing successfull."
#define PARSING_FAILED                  TCP_HEADLINE << "Parsing failed."

#define TCP_DEVICETAG                   "dev="
#define TCP_FUNCTIONTAG                 "func="
#define TCP_COMMANDTAG                  "com="
#define TCP_ENDOFCOMMANDTAG             " "

class TCPServer : public QObject
{
    Q_OBJECT
public:
    explicit TCPServer(QObject *parent = 0);

private:
    QTcpServer *TcpServ;

signals:
    void Send(TWlanMessage Msg);

public slots:
private slots:
    void newConnection();

};

#endif // TCPSERVER_H
