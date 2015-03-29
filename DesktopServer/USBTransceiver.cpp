#include "USBTransceiver.h"
#include <QTime>

USBTransceiver::USBTransceiver(QObject *parent) :
    Handle(NULL),
    PollingEnabled(FALSE)
{
    for (int i = 0; i < 1000; i++)
        RBuf[i] = 0;
    start();
}

USBTransceiver::~USBTransceiver()
{
    Close();
}

void USBTransceiver::USBInit()
{
    usb_init();
    usb_find_busses();
    usb_find_devices();
}

struct usb_device *USBTransceiver::Find()
{
    struct usb_bus *bus;
    struct usb_device *dev;

    for (bus = usb_get_busses(); bus; bus = bus->next)
    {
        for (dev = bus->devices; dev; dev = dev->next)
        {
            if (dev->descriptor.idVendor == DEV_VID && dev->descriptor.idProduct == DEV_PID)
                return dev;
        }
    }
    return NULL;
}

usb_dev_handle *USBTransceiver::Open()
{
    struct usb_device *Dev;
    usb_dev_handle *hndl;
    qDebug() << MSG_TRYINGTOCONNECT;
    Close();
    Dev = Find();
    if (Dev)
    {
        hndl = usb_open(Dev);
        if (hndl)
            if (usb_set_configuration(hndl, 1)>=0)
                if (usb_claim_interface(hndl,0)>=0)
                {
                    Handle = hndl;
                    qDebug() << MSG_TRANSCEIVEROPENED;
                    PollingEnabled = TRUE;
                }
                else
                    qDebug() << TRANSCEIVER_CAPTION << "Error claiming interface.";
            else
                qDebug() << TRANSCEIVER_CAPTION << "Error setting configuration.";
        else
            qDebug() << TRANSCEIVER_CAPTION << "Error opening device.";
    }
    else
        qDebug() << TRANSCEIVER_CAPTION << "Device not found.";

    return Handle;
}

void USBTransceiver::Close()
{
    if (Handle)
    {
        qDebug() << MSG_TRANSCEIVERCLOSED;
        usb_release_interface(Handle,0);
        usb_close(Handle);
        Handle = NULL;
    }
}

int USBTransceiver::SendCommand(int Command, int Data)
{
    int Buf[8];
    Buf[0] = 0x13;
    Buf[1] = Command % 256;
    Buf[2] = Command / 256;
    Buf[3] = Data % 256;
    Buf[4] = Data / 256;
    Buf[7] = 0x14;
    return usb_bulk_write(Handle, 0x03, (char*)Buf, 8, 100);
}

void USBTransceiver::run()
{
    char Buf[LAN_PACKET_LENGTH];
    char *pBuf = Buf;
    int result = 0;
    int cnt = 0;
    QTime time;
    time.start();

    while (1)
    {
        if (PollingEnabled)
        {
            result++;
                result = usb_bulk_read(Handle, IN_ENDPOINT, pBuf, LAN_PACKET_LENGTH, 100);
            if (result > 0)
                qDebug() << "Time:" << time.elapsed() << "    Recieved:" << result;
        }
    }
}

void USBTransceiver::ParseMessage(TWlanMessage Msg)
{
    WlanPacket *Pack;
    QByteArray Data;
    char *pData = (char*)&Data;
    Pack->setDirection(DIR_UPSTREAM);
    Pack->setFuncAddr(Msg.FuncAddr);
    Pack->setSafeCommand(Msg.Command);
    Data = Pack->getPacket();
    qDebug() << MSG_TRANSIEVED << Data;
    usb_bulk_write(Handle, OUT_ENDPOINT, pData, LAN_PACKET_LENGTH, 100);
}
