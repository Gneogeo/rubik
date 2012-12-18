#ifndef RUBIK_H
#define RUBIK_H

#include <QtGui/QMainWindow>
#include <QGLWidget>
#include "ui_rubik.h"


class rubik : public QMainWindow
{
	Q_OBJECT

public:
	rubik(QWidget *parent = 0, Qt::WFlags flags = 0);
	~rubik();

	QGLWidget *Widget;
private:
	Ui::rubikClass ui;
};

#endif // RUBIK_H
