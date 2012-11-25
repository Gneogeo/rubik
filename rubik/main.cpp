//#include <QtGui/QApplication>
#include "rubik.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	rubik w;
	w.show();
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}
