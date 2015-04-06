#include "tcpserver.h"
//#include "mainwindow.h"

TCPServer::TCPServer(QObject *parent) :
    QObject(parent)
{
    qDebug() << TCP_STARTING;
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

    qDebug() << TCP_REQUEST_RECIEVED << TcpMessage;
    qDebug() << TCP_REQUEST_CHECKING;

    if ((TcpMessage.indexOf(TCP_DEVICETAG, 0) > 0) &&
        (TcpMessage.indexOf(TCP_FUNCTIONTAG, 0) > 0) &&
        (TcpMessage.indexOf(TCP_COMMANDTAG, 0) > 0))
    {
        qDebug() << TCP_FORMAT_OK;
        emit(Recieved(&TcpMessage));
        socket->write(TCP_ANSWEAR_OK);
    }
    else
    {
        qDebug() << TCP_FORMAT_FAIL;
        socket->write(TCP_ANSWEAR_REJECTED);
    }

    socket->flush();
    socket->waitForBytesWritten(1000);
    socket->close();
}
