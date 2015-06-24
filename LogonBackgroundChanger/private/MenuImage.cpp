#include <QMouseEvent>

#include "MenuImage.hpp"

#include <QDebug>

MenuImage::MenuImage(QWidget *parent/* = 0*/)
: QLabel(parent)
{
}

MenuImage::~MenuImage()
{
}


void MenuImage::mouseReleaseEvent(QMouseEvent *)
{
   emit mouseReleased();
}
