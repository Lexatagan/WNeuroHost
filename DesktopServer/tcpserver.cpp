#include "tcpserver.h"
//#include "mainwindow.h"

TCPServer::TCPServer(QObject *parent) :
    QObject(parent)
{
    TcpServ = new QTcpServer(this);
    connect(TcpServ, SIGNAL(newConnection()), this, SLOT(newConnection()));
    if (!TcpServ->listen(QHostAddress::Any, TCP_LISTENING_PORT))
        qDebug() << OPENING_ERROR;
    else
    {
        qDebug() << OPENING_SUCCESS;
        qDebug() << BINDING_SUCCESS << TCP_LISTENING_PORT;
    }
}

void TCPServer::newConnection()
{
QTcpSocket *socket;

QByteArray TcpMessage;
QByteArray SDev, SFunc, SCom;
TWlanMessage Msg;

    if (!TcpServ->hasPendingConnections())
    {
        qDebug() << PENDINGCONNECTION_ERROR;
        return;
    }

    socket = TcpServ->nextPendingConnection();
    socket->waitForReadyRead();
    TcpMessage = socket->readLine(1000);

    qDebug() << RECIEVEDDATA << TcpMessage;

    if (Msg.LoadFromTcpRequest(TcpMessage))
    {
        qDebug() << PARSING_SUCCESS;
        emit(Send(Msg));
    }
    else
        qDebug() << PARSING_FAILED;

    socket->write("OK \r\n");
    socket->flush();
    socket->waitForBytesWritten(1000);
    socket->close();
}
