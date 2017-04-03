#ifndef WORKAREA_H
#define WORKAREA_H

#include <QWidget>
#include <QPainter>
#include <QVBoxLayout>

class WorkArea : public QWidget
{
    Q_OBJECT

public:
        WorkArea(QWidget *parent = 0);
        ~WorkArea();

public:
        QVBoxLayout *layout;

protected:
            virtual void paintEvent(QPaintEvent *event);
};

#endif // WORKAREA_H
