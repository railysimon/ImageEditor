#ifndef TOOLPROPERTIES_H
#define TOOLPROPERTIES_H

#include <QPoint>
#include <QColor>

class ToolProperties
{
public:
        QPoint point;
        QColor color, old_color;
        int width;

public:
        ToolProperties(QPoint data_point = QPoint(0,0), QColor data_color = QColor(Qt::black), int d_width = 1)
            : point(data_point), color(data_color), width(d_width) {}

        void SetOldColor(QColor &c)
        {
            old_color = c;
        }

};

#endif // TOOLPROPERTIES_H
