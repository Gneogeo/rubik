#ifndef MGL_H
#define MGL_H

#include <QGLWidget>
#include <QMouseEvent>

 class GLWidget : public QGLWidget
 {
     Q_OBJECT

 public:
     GLWidget(QWidget *parent = 0);
     ~GLWidget();

     QSize minimumSizeHint() const;
     QSize sizeHint() const;


	 void rot1();
	 void rot2();
	 void rot3();
	 void rot4();
	 void rot5();
	 void rot6();

  
 protected:
     void initializeGL();
     void paintGL();
     void resizeGL(int width, int height);
     void mousePressEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
	 void keyPressEvent( QKeyEvent * event );

 private:
     GLuint makeObject();

	 float xRotOld,yRotOld;
     float xRot;
     float yRot;
	 float zRot;
     QPoint lastPos;

 };

 #endif
