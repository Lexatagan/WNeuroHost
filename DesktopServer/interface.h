#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>
#include <QDebug>
#include "wlanPipe.h"
#include "tcpserver.h"

#define IF_AVAILABLE_PORTS           "Available ports:"

class Interface : public QObject
{
    Q_OBJECT
private:
    TCPServer *Server;
public:
    explicit Interface(QObject *parent = 0);


signals:

public slots:

};

#endif // INTERFACE_H
