#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

class Client : public QObject
{
	Q_OBJECT
public:
	explicit Client(QTcpSocket* socket, QObject *parent = nullptr);
	QString getMyAddr();
signals:
	void signal_stopped();
	void signal_incommingData(const QString &addr, const QByteArray &data);
	void signal_sendBye();
public slots:
	void slot_stop();
	void slot_sendAll(const int descriptor, const QByteArray &data);
private slots:
	void slot_readyRead();
private:
	QTcpSocket* m_pClient;

	void sendData(const QByteArray &data);
};

#endif // CLIENT_H
