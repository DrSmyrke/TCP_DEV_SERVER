#include "client.h"
#include <QHostAddress>

Client::Client(QTcpSocket *socket, QObject *parent)
	: QObject(parent)
	, m_pClient(socket)
{
	connect( m_pClient, &QTcpSocket::disconnected, this, &Client::slot_stop );
	connect( m_pClient, &QTcpSocket::readyRead, this, &Client::slot_readyRead );
}

void Client::slot_stop()
{
	emit signal_sendBye( getMyAddr() );
	emit signal_stopped();
}

void Client::slot_readyRead()
{
	QByteArray ba;
	while( m_pClient->bytesAvailable() ){
		ba.append( m_pClient->readAll() );
	}

	emit signal_incommingData( getMyAddr(), ba );
}

QString Client::getMyAddr()
{
	return QString( "%1:%2" ).arg( m_pClient->peerAddress().toString() ).arg( m_pClient->peerPort() );
}
