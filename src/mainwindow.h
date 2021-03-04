#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
signals:
	void signal_stopALL();
private slots:
	void slot_newConnection();
	void slot_incommingData(const QString& addr, const QByteArray &data);
private:
	Ui::MainWindow *ui;
	QTcpServer* m_pServer;
	QTcpSocket* m_pLastClient;

	bool m_hexF;
};
#endif // MAINWINDOW_H
