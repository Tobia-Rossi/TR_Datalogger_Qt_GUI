// Rossi

// File:   MainWindow.cpp
// Author: Tobia Rossi
// Description: MainWindow form



///////////////////////////////////////////////////////////////////////////////
// application includes
#include "MainWindow.hpp"



///////////////////////////////////////////////////////////////////////////////
// system includes
#include "ui_MainWindow.h"



///////////////////////////////////////////////////////////////////////////////
// namespaces



///////////////////////////////////////////////////////////////////////////////
// creation / destruction

//------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent)
//------------------------------------------------------------------------------
	: QMainWindow(parent),
	  _ui(new Ui::MainWindow),
	  _aboutWindow(new AboutWindow(this, APP_NAME, APP_VERSION, APP_AUTHOR)),
	  _serial(new QSerialPort(this)),
	  _timerTimeout(new QTimer(this))
{
	// init forms
	_ui->setupUi(this);

	// init attributes
	_serialPortIsConnected = false;

	// set title
	setWindowTitle(APP_NAME "   v" APP_VERSION "   by " APP_AUTHOR);

	// refresh list of available serial ports
	refreshSerialPortsList();

	// connect signals
	connect(_serial, &QSerialPort::readyRead, this, &MainWindow::readData);
	connect(_timerTimeout, &QTimer::timeout, this, &MainWindow::timeoutSerialPort);

}

//------------------------------------------------------------------------------
MainWindow::~MainWindow()
//------------------------------------------------------------------------------
{
	delete _ui;
}



///////////////////////////////////////////////////////////////////////////////
// methods
///////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
void MainWindow::refreshUi()
//------------------------------------------------------------------------------
{
	if (_serialPortIsConnected) {
		_ui->pushButtonConnectOrDisconnectSerial->setText("Disconnect Serial Port");
		_ui->actionConnectOrDisconnectSerial->setChecked(true);
		_ui->iconConnectedOrDisconnected->setText("<html><head/><body><p><img src=\":/logos/Images/connected.png\"/></p></body></html>");
		_ui->comboBoxComPortSelection->setDisabled(true);
		_ui->comboBoxBaudRateSelection->setDisabled(true);
		_ui->comboBoxParityBitSelection->setDisabled(true);
		_ui->comboBoxStopBitsSelection->setDisabled(true);
		_ui->textEditConsole->setReadOnly(true);
	} else {
		_ui->pushButtonConnectOrDisconnectSerial->setText("Connect Serial Port");
		_ui->actionConnectOrDisconnectSerial->setChecked(false);;
		_ui->iconConnectedOrDisconnected->setText("<html><head/><body><p><img src=\":/logos/Images/disconnected.png\"/></p></body></html>");
		_ui->comboBoxComPortSelection->setDisabled(false);
		_ui->comboBoxBaudRateSelection->setDisabled(false);
		_ui->comboBoxParityBitSelection->setDisabled(false);
		_ui->comboBoxStopBitsSelection->setDisabled(false);
		_ui->textEditConsole->setReadOnly(false);
	}
}

//------------------------------------------------------------------------------
void MainWindow::saveToFile()
//------------------------------------------------------------------------------
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "/home/untitled", tr("Files (*)"));

	if (fileName == nullptr) {
		return;
	}

	QFile file(fileName);

	if (!file.open(QIODevice::WriteOnly)) {
		file.close();
	} else {
		file.write(_ui->textEditConsole->toPlainText().toUtf8());
		file.close();
	}
}

//------------------------------------------------------------------------------
void MainWindow::loadFromFile()
//------------------------------------------------------------------------------
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/", tr("Files (*)"));

	if (fileName == nullptr) {
		return;
	}

	QFile file(fileName);

	if (!file.open(QIODevice::ReadOnly)) {
		file.close();
	} else {
		_ui->textEditConsole->setText(file.readAll());
		file.close();
	}
}

//------------------------------------------------------------------------------
bool MainWindow::openSerialPort()
//------------------------------------------------------------------------------
{
	_serialPortIsConnected = true;
	refreshUi();

	_serial->setPortName(_ui->comboBoxComPortSelection->currentText());
	_serial->setBaudRate(_ui->comboBoxBaudRateSelection->currentText().toInt());
	switch (_ui->comboBoxParityBitSelection->currentIndex()) {
		case 0:
			_serial->setParity(QSerialPort::NoParity);
			break;

		case 1:
			_serial->setParity(QSerialPort::EvenParity);
			break;

		case 2:
			_serial->setParity(QSerialPort::OddParity);
			break;

		case 3:
			_serial->setParity(QSerialPort::SpaceParity);
			break;

		case 4:
			_serial->setParity(QSerialPort::MarkParity);
			break;

		default:
			_serial->setParity(QSerialPort::UnknownParity);
			break;
	}
	switch (_ui->comboBoxStopBitsSelection->currentIndex()) {
		case 0:
			_serial->setStopBits(QSerialPort::OneStop);
			break;

		case 1:
			_serial->setStopBits(QSerialPort::OneAndHalfStop);
			break;

		case 2:
			_serial->setStopBits(QSerialPort::TwoStop);
			break;

		default:
			_serial->setStopBits(QSerialPort::UnknownStopBits);
			break;
	}

	if (_serial->open(QIODevice::ReadWrite)) {
		return true;
	} else {
		_ui->textEditConsole->setText("Error connecting to Serial Port!");
		_serialPortIsConnected = true;
		refreshUi();
		return false;
	}
}

//------------------------------------------------------------------------------
void MainWindow::closeSerialPort()
//------------------------------------------------------------------------------
{
	if (_serial->isOpen()) {
		_serial->close();
		_serialPortIsConnected = false;
		refreshUi();
    }
}

//------------------------------------------------------------------------------
void MainWindow::writeData(const QByteArray &data)
//------------------------------------------------------------------------------
{
    _serial->write(data);
}

//------------------------------------------------------------------------------
void MainWindow::refreshSerialPortsList()
//------------------------------------------------------------------------------
{
	_ui->comboBoxComPortSelection->clear();

	const auto infos = QSerialPortInfo::availablePorts();
	for (const QSerialPortInfo &info : infos) {
		QString s = info.portName();
		_ui->comboBoxComPortSelection->addItem(s);
	}
}

//------------------------------------------------------------------------------
void MainWindow::clearConsole()
//------------------------------------------------------------------------------
{
	_ui->textEditConsole->clear();
}

//------------------------------------------------------------------------------
void MainWindow::connectOrDisconnect()
//------------------------------------------------------------------------------
{
	if (_serial->isOpen()) {
		 closeSerialPort();
		 _timerTimeout->stop();
	} else {
		openSerialPort();
		_timerTimeout->start(TimeoutTimeMs);
	}
}



///////////////////////////////////////////////////////////////////////////////
// events
///////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
void MainWindow::readData()
//------------------------------------------------------------------------------
{
	_timerTimeout->stop();
	const QByteArray data = _serial->readAll();
	_ui->textEditConsole->setText(_ui->textEditConsole->toPlainText() + data);
	_ui->textEditConsole->verticalScrollBar()->setValue(_ui->textEditConsole->verticalScrollBar()->maximum());
	_timerTimeout->start(TimeoutTimeMs);
}

//------------------------------------------------------------------------------
void MainWindow::timeoutSerialPort()
//------------------------------------------------------------------------------
{
	if (_serial->isOpen()) {
		closeSerialPort();
		_serialPortIsConnected = false;
		refreshUi();

		// Trows error
		QMessageBox messageBox;
		messageBox.setModal(true);
		messageBox.critical(0,"Serial connection Error","The serial connection has timed out!");
		messageBox.setFixedSize(500,200);
	}
}

//------------------------------------------------------------------------------
void MainWindow::on_actionExit_triggered()
//------------------------------------------------------------------------------
{
	close();
}

//------------------------------------------------------------------------------
void MainWindow::on_actionAbout_triggered()
//------------------------------------------------------------------------------
{
	_aboutWindow->setModal(true);
	_aboutWindow->show();
}

//------------------------------------------------------------------------------
void MainWindow::on_pushButtonWriteToFile_clicked()
//------------------------------------------------------------------------------
{
	saveToFile();
}

//------------------------------------------------------------------------------
void MainWindow::on_actionSave_triggered()
//------------------------------------------------------------------------------
{
	saveToFile();
}

//------------------------------------------------------------------------------
void MainWindow::on_pushButtonLoadFile_clicked()
//------------------------------------------------------------------------------
{
	loadFromFile();
}

//------------------------------------------------------------------------------
void MainWindow::on_actionLoad_triggered()
//------------------------------------------------------------------------------
{
	loadFromFile();
}

//------------------------------------------------------------------------------
void MainWindow::on_pushButtonConnectOrDisconnectSerial_clicked()
//------------------------------------------------------------------------------
{
	connectOrDisconnect();
}

//------------------------------------------------------------------------------
void MainWindow::on_actionConnectOrDisconnectSerial_triggered()
//------------------------------------------------------------------------------
{
	connectOrDisconnect();
}

//------------------------------------------------------------------------------
void MainWindow::on_pushButtonRefreshComList_clicked()
//------------------------------------------------------------------------------
{
	refreshSerialPortsList();
}

//------------------------------------------------------------------------------
void MainWindow::on_pushButtonClearConsole_clicked()
//------------------------------------------------------------------------------
{
	clearConsole();
}

//------------------------------------------------------------------------------
void MainWindow::on_actionClear_Console_triggered()
//------------------------------------------------------------------------------
{
	clearConsole();
}

//------------------------------------------------------------------------------
void MainWindow::on_checkBoxLiveMode_stateChanged(int arg1)
//------------------------------------------------------------------------------
{
	if (arg1 == Qt::Unchecked) {
		_ui->actionLive_Mode->setChecked(false);
	} else if (arg1 == Qt::Checked) {
		_ui->actionLive_Mode->setChecked(true);
	}
}

//------------------------------------------------------------------------------
void MainWindow::on_actionLive_Mode_triggered()
//------------------------------------------------------------------------------
{
	if (_ui->actionLive_Mode->isChecked()) {
		_ui->checkBoxLiveMode->setCheckState(Qt::Checked);
	} else {
		_ui->checkBoxLiveMode->setCheckState(Qt::Unchecked);
	}
}

