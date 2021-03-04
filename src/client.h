#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

class Client : public QObject
{
	Q_OBJECT
public:
	explicit Client(QTcpSocket* socket, QObject *parent = nullptr);
signals:
	void signal_stopped();
	void signal_incommingData(const QString &addr, const QByteArray &data);
	void signal_sendBye(const QString &addr);
public slots:
	void slot_stop();
private slots:
	void slot_readyRead();
private:
	QTcpSocket* m_pClient;

	QString getMyAddr();
};

#endif // CLIENT_H
