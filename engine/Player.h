#ifndef _PLAYER_H
#define _PLAYER_H

#include <QObject>
#include <QString>

class GameState;

class Player : public QObject
{

Q_OBJECT

public:
	Player(GameState *state);
	virtual void receiveMsg(QString msg);

	inline int getId() const { return id; }
	inline GameState *getGameState() const { return state; }

	void sendMsg(QString msg);
	virtual bool init();
	virtual bool cleanup();

signals:
	void sendMsgSignal(int id, QString msg);
	void msgReceivedSignal(QString msg);
	void playerLeftSignal(int id);

protected:
	void leave();

private:
	GameState *state;
	const int id;
};

#endif
