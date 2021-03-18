#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_pServer = new QTcpServer( this );



	m_hexF = false;
	m_autosendF = false;



	ui->connectB->setText( tr( "OPEN" ) );
	ui->clearB->setIcon( this->style()->standardIcon(QStyle::SP_LineEditClearButton) );

	connect( ui->connectB, &QPushButton::clicked, this, [this](){
		if( !m_pServer->isListening() ){
			uint16_t port = ui->portBox->value();
			auto res = m_pServer->listen( QHostAddress::Any, port );
			if( res ){
				ui->connectB->setText( tr( "CLOSE" ) );
				ui->portBox->setEnabled( false );
			}
		}else{
			ui->connectB->setText( tr( "OPEN" ) );
			ui->portBox->setEnabled( true );
			if( m_pServer->isListening() ){
				emit signal_stopALL();
				m_pServer->close();
			}
		}
	} );

	connect( ui->clearB, &QPushButton::clicked, this, [this](){
		ui->logBox->clear();
	} );

	connect( ui->hexB, &QPushButton::toggled, this, [this](bool checked){
		m_hexF = checked;
	} );

	connect( ui->autosendB, &QPushButton::toggled, this, [this](bool checked){
		m_autosendF = checked;
	} );

	connect( ui->sendB, &QPushButton::clicked, this, &MainWindow::slot_sendData );

	connect( m_pServer, &QTcpServer::newConnection, this, &MainWindow::slot_newConnection );
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::slot_newConnection()
{
	QTcpSocket* socket = m_pServer->nextPendingConnection();

	Client* client = new Client( socket, this );

	connect( this, &MainWindow::signal_stopALL, client, &Client::slot_stop );
	connect( this, &MainWindow::signal_sendAll, client, &Client::slot_sendAll );
	connect( client, &Client::signal_stopped, client, &QTcpSocket::deleteLater );
	connect( client, &Client::signal_sendBye, this, [this, client](){
		QString str = QString( "<span style=\"color:gray;\">%1 [%2]</span>" ).arg( tr("Disconnected from") ).arg( client->getMyAddr() );
		ui->logBox->append( str );
	} );
	connect( client, &Client::signal_incommingData, this, &MainWindow::slot_incommingData );

	QString str = QString( "<span style=\"color:gray;\">%1 [%2:%3]</span>" ).arg( tr("New connect from") ).arg( socket->peerAddress().toString() ).arg( socket->peerPort() );
	ui->logBox->append( str );
}

void MainWindow::slot_incommingData(const int descriptor, const QString &addr, const QByteArray &data)
{
	QString str = QString( "[%1] [%2 bytes] >:" ).arg( addr ).arg( data.size() );
	ui->logBox->append( str );
	ui->logBox->append( QString( data ) );
	if( m_hexF ){
		str = QString( ">: %2" ).arg( addr ).arg( QString( data.toHex() ) );
		ui->logBox->append( str );
	}

	if( m_autosendF ){
		auto data = ui->sendTextBox->document()->toPlainText();
		emit signal_sendAll( descriptor, data.toUtf8() );

		ui->logBox->append( "<span style=\"color:orange;\"><:</span>" );
		ui->logBox->append( QString( data ) );
	}
}

void MainWindow::slot_sendData()
{
	auto data = ui->sendTextBox->document()->toPlainText();
	auto desc = ui->descriptorBox->value();
	emit signal_sendAll( desc, data.toUtf8() );

	ui->logBox->append( "<span style=\"color:orange;\"><:</span>" );
	ui->logBox->append( QString( data ) );
}
