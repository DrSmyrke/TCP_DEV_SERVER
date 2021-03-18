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
	emit signal_sendBye();
	emit signal_stopped();
}

void Client::slot_sendAll(const int descriptor, const QByteArray &data)
{
	if( descriptor != m_pClient->socketDescriptor() ) return;

	sendData( data );
}

void Client::slot_readyRead()
{
	QByteArray ba;
	while( m_pClient->bytesAvailable() ){
		ba.append( m_pClient->readAll() );
	}

	emit signal_incommingData( m_pClient->socketDescriptor(),getMyAddr(), ba );
}

void Client::sendData(const QByteArray &data)
{
	if( data.size() == 0 ) return;
	if( m_pClient->state() == QAbstractSocket::ConnectingState ) m_pClient->waitForConnected(300);
	if( m_pClient->state() == QAbstractSocket::UnconnectedState ) return;
	m_pClient->write( data );
	m_pClient->waitForBytesWritten(100);
}

QString Client::getMyAddr()
{
	return QString( "%1:%2, %3" ).arg( m_pClient->peerAddress().toString() ).arg( m_pClient->peerPort() ).arg( m_pClient->socketDescriptor() );
}
