#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "wlanPipe.h"

#define TCP_LISTENING_PORT 1811

#define TCP_CAPTION                     "TCPServer: "
#define TCP_ANSWEAR_OK                  "OK \r\n"
#define TCP_ANSWEAR_REJECTED            "Rejected \r\n"
#define TCP_STARTING                    TCP_CAPTION << "starting.."
#define OPENING_ERROR                   TCP_CAPTION << "Server opening failed."
#define OPENING_SUCCESS                 TCP_CAPTION << "Server opening successed."
#define BINDING_SUCCESS                 TCP_CAPTION << "Server binded on http://LocalIP:"
#define PENDINGCONNECTION_ERROR         TCP_CAPTION << "There is no pending connection."
#define PARSING_SUCCESS                 TCP_CAPTION << "Parsing successfull."
#define PARSING_FAILED                  TCP_CAPTION << "Parsing failed."
#define TCP_REQUEST_RECIEVED            TCP_CAPTION << "Recieved:"
#define TCP_REQUEST_CHECKING            TCP_CAPTION << "Checking..."
#define TCP_FORMAT_OK                   TCP_CAPTION << "-correct format."
#define TCP_FORMAT_FAIL                 TCP_CAPTION << "-incorrect format. Rejected."

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
    void Recieved(QByteArray *Request);

public slots:
private slots:
    void newConnection();

};

#endif // TCPSERVER_H
