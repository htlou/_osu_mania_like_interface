#include "stagehint.h"

stageHint::stageHint(const QString &name)
{
    QPixmap image(":/element/resources/"+name+".png");
    setPixmap(image);
    setOffset(-image.width()/2, -image.height()/2);

    // set up animation
    zoomInAnimation = new QPropertyAnimation(this, "MyOpacity");
    zoomInAnimation->setStartValue(0);
    zoomInAnimation->setEndValue(1);
    zoomInAnimation->setDuration(100);

    zoomOutAnimation = new QPropertyAnimation(this, "MyOpacity");
    zoomInAnimation->setStartValue(1);
    zoomInAnimation->setEndValue(0);
    zoomInAnimation->setDuration(50);
}

qreal stageHint::MyScale()
{
    return this->scale();
}

qreal stageHint::MyOpacity()
{
    return this->opacity();
}


void stageHint::setMyScale(qreal scale)
{
    setScale(scale);
}

void stageHint::setMyOpacity(qreal _opacity)
{
    setOpacity(_opacity);
}


void stageHint::startAnimation()
{
    zoomInAnimation->start();
}

void stageHint::vanish()
{
    zoomOutAnimation->start();
    deleteLater();
}
