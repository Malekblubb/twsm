//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#include <twsm/main_window.hpp>

#include <QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	twsm::main_window w;
	w.show();

	return a.exec();
}
