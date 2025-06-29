// Rossi

// File:   MainWindow.hpp
// Author: Tobia Rossi
// Description: MainWindow form



#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

///////////////////////////////////////////////////////////////////////////////
// application includes
#include "AboutWindow.hpp"



///////////////////////////////////////////////////////////////////////////////
// system includes
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QScrollBar>
#include <QTimer>



///////////////////////////////////////////////////////////////////////////////
// namespaces
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



///////////////////////////////////////////////////////////////////////////////
// class
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	///////////////////////////////////////////////////////////////////////////////
	// creation / destruction
	///////////////////////////////////////////////////////////////////////////////

	MainWindow(QWidget *parent = nullptr);
	~MainWindow();



private:
	///////////////////////////////////////////////////////////////////////////////
	// Forms
	///////////////////////////////////////////////////////////////////////////////

	Ui::MainWindow *_ui;
	AboutWindow *_aboutWindow;



	///////////////////////////////////////////////////////////////////////////////
	// attributes
	///////////////////////////////////////////////////////////////////////////////

	QSerialPort *_serial = nullptr;
	QTimer *_timerTimeout = nullptr;

	bool _serialPortIsConnected;
	const int TimeoutTimeMs = 100;



	///////////////////////////////////////////////////////////////////////////////
	// methods
	///////////////////////////////////////////////////////////////////////////////

	void refreshUi();
	void saveToFile();
	void loadFromFile();
	void refreshSerialPortsList();

	bool openSerialPort();
	void closeSerialPort();
	void writeData(const QByteArray &data);
	void clearConsole();
	void connectOrDisconnect();



private slots:
	///////////////////////////////////////////////////////////////////////////////
	// events
	///////////////////////////////////////////////////////////////////////////////

	// Serial
	void readData();
	void timeoutSerialPort();

	// menues
	void on_actionSave_triggered();
	void on_actionLoad_triggered();
	void on_actionExit_triggered();
	void on_actionAbout_triggered();
	void on_actionConnectOrDisconnectSerial_triggered();
	void on_actionClear_Console_triggered();

	// buttons
	void on_pushButtonWriteToFile_clicked();
	void on_pushButtonLoadFile_clicked();
	void on_pushButtonConnectOrDisconnectSerial_clicked();
	void on_pushButtonRefreshComList_clicked();
	void on_pushButtonClearConsole_clicked();
	void on_checkBoxLiveMode_stateChanged(int arg1);
	void on_actionLive_Mode_triggered();
	void on_actionSerialPortInfo_triggered();
};
#endif // MAINWINDOW_HPP
