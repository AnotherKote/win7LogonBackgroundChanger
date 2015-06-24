#ifndef MENUIMAGE_H
#define MENUIMAGE_H

#include <QLabel>

class MenuImage : public QLabel
{
   Q_OBJECT
public:
   MenuImage(QWidget *parent = 0);
   ~MenuImage();
   // QWidget interface
protected:
   void mouseReleaseEvent(QMouseEvent *);

signals:
   void mouseReleased();
};

#endif // MENUIMAGE_H
