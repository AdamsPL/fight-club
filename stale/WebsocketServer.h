#ifndef _WEBSOCKET_SERVER_H
#define _WEBSOCKET_SERVER_H

#include <QByteArray>
#include <QTcpServer>
#include <QThread>
#include <QSet>

class WebsocketServer : public QObject
{
	Q_OBJECT

public:
	WebsocketServer();
	~WebsocketServer();
	void broadcast(QString msg);

signals:
	void init();
    void broadcastSignal(QString msg);

private slots:
	void onNewConnection();
	void onInit();
	void onBroadcast(QString msg);

private:
	QThread workerThread;
	QByteArray encode(QByteArray source);
	bool handshake(QTcpSocket *socket);

	QSet<QTcpSocket*> activeSockets;
    QTcpServer *server;
};

#endif
