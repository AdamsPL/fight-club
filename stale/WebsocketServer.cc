#include "WebsocketServer.h"

#include <QtEndian>
#include <QCryptographicHash>
#include <QStringList>
#include <QTcpSocket>

static const QString magicString = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

WebsocketServer::WebsocketServer()
	: QObject(NULL), server(NULL)
{
	moveToThread(&workerThread);

	connect(this, SIGNAL(init()), this, SLOT(onInit()), Qt::QueuedConnection);
	connect(this, SIGNAL(broadcastSignal(QString)), this, SLOT(onBroadcast(QString)));
	workerThread.start();
	emit init();
}

WebsocketServer::~WebsocketServer()
{
	delete server;
}

void WebsocketServer::onInit()
{
	server = new QTcpServer(this);
	connect(server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    server->listen(QHostAddress::Any, 54321);
}

QByteArray WebsocketServer::encode(QByteArray source)
{
    QByteArray result;
    unsigned char hdr = 0;

    hdr = (1 << 7) | (1);
    result.append(hdr);

    if (source.length() < 126)
        hdr = source.length();
    else if (source.length() <= 0xFFFF)
        hdr = 126;
    else
        hdr = 127;
    result.append(hdr);

    short lens;
    int lenl;

    switch(hdr)
    {
        case 126:
            lens = qToBigEndian<short>(source.length());
            result.append((char*)&lens, sizeof(lens));
            break;
        case 127:
            lenl = qToBigEndian<short>(source.length());
            result.append((char*)&lenl, sizeof(lenl));
            break;
    }
    result.append(source);

    return result;
}

bool WebsocketServer::handshake(QTcpSocket *socket)
{
	QString line;
	QString key;

    while(1)
    {
        while(!socket->canReadLine())
        {
            socket->waitForReadyRead(200);
        }
        line = socket->readLine();
        if (line.startsWith("Sec-WebSocket-Key:"))
        {
            line.remove("Sec-WebSocket-Key:");
            line = line.simplified();
            line.append(magicString);
            QCryptographicHash hash(QCryptographicHash::Sha1);
            hash.addData(line.toLocal8Bit());
            key = hash.result().toBase64();
        }
        if (line.simplified().isEmpty())
            break;
    }
    QStringList response;
    response << "HTTP/1.1 101 Switching Protocols";
    response << "Upgrade: websocket";
    response << "Connection: Upgrade";
    response << QString("Sec-WebSocket-Accept: ") + key;
    response << "";
    response << "";

    QByteArray responseBuf = response.join("\r\n").toLocal8Bit();

    socket->write(responseBuf);
    socket->flush();

	return true;
}

void WebsocketServer::onNewConnection()
{
	QTcpSocket *socket = NULL;

	while ((socket = server->nextPendingConnection())) {
		if (!handshake(socket))
			delete socket;
		else
			activeSockets << socket;
	}
}

void WebsocketServer::onBroadcast(QString msg)
{
	QTcpSocket *socket;
	QSet<QTcpSocket *> deadSockets;

	foreach (socket, activeSockets) {
        if (socket->write(encode(msg.toLocal8Bit())) == -1) {
			deadSockets << socket;
            continue;
        }
        socket->flush();
	}
	foreach (socket, deadSockets) {
		activeSockets.remove(socket);
		delete socket;
	}
}

void WebsocketServer::broadcast(QString msg)
{
    emit broadcastSignal(msg);
}
