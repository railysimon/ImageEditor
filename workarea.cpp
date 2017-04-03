#include "workarea.h"

WorkArea::WorkArea(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(800, 500);

    layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignTop | Qt:: AlignLeft);
    this->setLayout(layout);
}

WorkArea::~WorkArea()
{

}

void WorkArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setPen(QPen(Qt::NoPen));
    painter.setBrush(QBrush(QColor(180, 180, 180), Qt::Dense4Pattern));
    painter.drawRect(0, 0, width(), height());
}
