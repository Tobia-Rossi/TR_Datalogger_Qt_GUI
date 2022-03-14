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
	: QMainWindow(parent)
	, _ui(new Ui::MainWindow),
		_aboutWindow(new AboutWindow(this, APP_NAME, APP_VERSION, APP_AUTHOR))
{
	_ui->setupUi(this);

	// init attributes
	_isBold = false;

	// set title
	setWindowTitle(APP_NAME "   v" APP_VERSION "   by " APP_AUTHOR);

	// refresh GUI
	refresh();
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
void MainWindow::refresh()
//------------------------------------------------------------------------------
{
	if (_isBold == true) {
		_ui->labelHelloWorld->setStyleSheet("font-weight: bold; color: red");
		_ui->labelHelloWorld->setStyleSheet("QLabel {background-color: orange}");
	} else {
		_ui->labelHelloWorld->setStyleSheet("font-weight: 100; color: black");
		_ui->labelHelloWorld->setStyleSheet("QLabel {background-color: transparent}");
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
		file.write(_ui->textEditTestDataInput->toPlainText().toUtf8());
		file.close();
	}

	// _ui->labelHelloWorld->setText(fileName);
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
		_ui->textEditTestDataInput->setText(file.readAll());
		file.close();
	}

	// _ui->labelHelloWorld->setText(fileName);
}


///////////////////////////////////////////////////////////////////////////////
// events
///////////////////////////////////////////////////////////////////////////////

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
void MainWindow::on_pushButtonPressMe_pressed()
//------------------------------------------------------------------------------
{
	_isBold = !_isBold;
	refresh();
}

//------------------------------------------------------------------------------
void MainWindow::on_pushButtonPressMe_released()
//------------------------------------------------------------------------------
{
	_isBold = !_isBold;
	refresh();
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

