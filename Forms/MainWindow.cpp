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
		_ui->label->setStyleSheet("font-weight: bold; color: red");
		_ui->label->setStyleSheet("QLabel {background-color: orange}");
	} else {
		_ui->label->setStyleSheet("font-weight: 100; color: black");
		_ui->label->setStyleSheet("QLabel {background-color: transparent}");
	}
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
void MainWindow::on_pushButton_pressed()
//------------------------------------------------------------------------------
{
	_isBold = !_isBold;
	refresh();
}

//------------------------------------------------------------------------------
void MainWindow::on_pushButton_released()
//------------------------------------------------------------------------------
{
	_isBold = !_isBold;
	refresh();
}

