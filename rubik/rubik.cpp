#include "rubik.h"
#include "mgl.h"

#include <QGridLayout>

rubik::rubik(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	QGridLayout *gd=new QGridLayout(ui.groupBox);
	Widget=new GLWidget(ui.groupBox);
	gd->addWidget(Widget);
	Widget->show();
	Widget->setFocusPolicy(Qt::StrongFocus);
	Widget->setFocus();
}

rubik::~rubik()
{
	
}
