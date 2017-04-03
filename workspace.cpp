#include "workspace.h"

WorkSpace::WorkSpace(const QPixmap &pix, QWidget *parent) : QLabel(parent), work_holst(pix), color(Qt::cyan)
{
    this->setMinimumSize(work_holst.size());
    this->setGraphicsEffect(new QGraphicsDropShadowEffect);
    this->setCursor(Qt::CrossCursor);
    image = new QImage(width(), height(), QImage::Format_ARGB32_Premultiplied);

    WIDTH = width();
    HEIGHT = height();

    choice_tool = 0;
    line_size = 1;
    brightness_value = 0;
    lightness_value = 80;

    black_white = false;
    blur = false;
    inverse = false;
}

WorkSpace::~WorkSpace()
{

}

void WorkSpace::SetImage(const QPixmap &pix)
{
    work_holst = pix;

    WIDTH = width();
    HEIGHT = height();

    delete image;
    image = new QImage(width(), height(), QImage::Format_ARGB32_Premultiplied);
}

QImage *WorkSpace::GetImage() const
{
    return image;
}

void WorkSpace::paintEvent(QPaintEvent *event)
{
    DrawImage(image);
    Scissors(image);
    DrawEraser(image);
    PenDraw(image);
    PolygonDraw(image);
    DrawLine(image);
    DrawSpray(image);
    DrawRect(image, rect_points, 0);
    DrawRect(image, ellipse_points, 1);

    QPainter painter(this);
    SetBrightness();
    SetLightness();

    for(int i=0; i<brush_points.size(); i++)
        Brush(*image, brush_points[i].point.x(), brush_points[i].point.y(),
              qRgb(brush_points[i].color.red(), brush_points[i].color.green(), brush_points[i].color.blue()),
              qRgb(brush_points[i].old_color.red(), brush_points[i].old_color.green(), brush_points[i].old_color.blue()));

    if(black_white) BlackWhite();
    if(inverse) Inverse();
    if(blur) Blur();

    painter.drawImage(0, 0, *image);
}

void WorkSpace::mousePressEvent(QMouseEvent *event)
{
    if(choice_tool == 5)
    {
        for(int i=0; i<2; i++)
            line_points.push_back(ToolProperties(QPoint(event->x(), event->y()), color, line_size));
    }

    if(choice_tool == 4)
    {
        for(int i=0; i<2; i++)
            polygon_points.push_back(ToolProperties(QPoint(event->x(), event->y()), color, line_size));
    }

    if(choice_tool == 1) pen_points.push_back(ToolProperties(QPoint(event->x(), event->y()), color, line_size));
    if(choice_tool == 6) spray_points.push_back(ToolProperties(QPoint(event->x(), event->y()), color, line_size));
    if(choice_tool == 2)
    {
        for(int i=0; i<2; i++)
            rect_points.push_back(ToolProperties(QPoint(event->x(), event->y()), color, line_size));
    }

    if(choice_tool == 3)
    {
        for(int i=0; i<2; i++)
            ellipse_points.push_back(ToolProperties(QPoint(event->x(), event->y()), color, line_size));
    }

    if(choice_tool == 7)
    {
        ToolProperties tmp(event->pos(), color);
        QColor click_color = image->pixel(event->pos());
        tmp.SetOldColor(click_color);
        brush_points.push_back(tmp);
    }

    if(choice_tool == 8) color = image->pixel(event->pos());
    if(choice_tool == 9) eraser_points.push_back(ToolProperties(event->pos()));

    if(choice_tool == 10)
    {
        for(int i=0; i<2; i++)
            scissors_points.push_back(ToolProperties(event->pos()));
    }


    QWidget::update();
}

void WorkSpace::mouseMoveEvent(QMouseEvent *event)
{
    if(choice_tool == 5)
    {
        line_points[line_points.size() - 1].point.setX(event->x());
        line_points[line_points.size() - 1].point.setY(event->y());
    }

    if(choice_tool == 4)
    {
        polygon_points[polygon_points.size() - 1].point.setX(event->x());
        polygon_points[polygon_points.size() - 1].point.setY(event->y());
    }

    if(choice_tool == 1) pen_points.push_back(ToolProperties(QPoint(event->x(), event->y()), color, line_size));
    if(choice_tool == 6) spray_points.push_back(ToolProperties(QPoint(event->x(), event->y()), color, line_size));
    if(choice_tool == 2)
    {
        rect_points[rect_points.size() - 1].point.setX(event->x());
        rect_points[rect_points.size() - 1].point.setY(event->y());
    }

    if(choice_tool == 3)
    {
        ellipse_points[ellipse_points.size() - 1].point.setX(event->x());
        ellipse_points[ellipse_points.size() - 1].point.setY(event->y());
    }

    if(choice_tool == 9) eraser_points.push_back(ToolProperties(event->pos()));

    if(choice_tool == 10)
    {
        scissors_points[scissors_points.size() - 1].point.setX(event->x());
        scissors_points[scissors_points.size() - 1].point.setY(event->y());
    }

    QWidget::update();
}

void WorkSpace::mouseReleaseEvent(QMouseEvent *event)
{
    if(choice_tool == 5)
    {
        line_points[line_points.size() - 1].point.setX(event->x());
        line_points[line_points.size() - 1].point.setY(event->y());
    }

    if(choice_tool == 4)
    {
        polygon_points[polygon_points.size() - 1].point.setX(event->x());
        polygon_points[polygon_points.size() - 1].point.setY(event->y());
    }

    if(choice_tool == 1) pen_points.push_back(pen_points[pen_points.size() - 1]);
    if(choice_tool == 6) spray_points.push_back(ToolProperties(QPoint(event->x(), event->y()), color));
    if(choice_tool == 2)
    {
        rect_points[rect_points.size() - 1].point.setX(event->x());
        rect_points[rect_points.size() - 1].point.setY(event->y());
    }

    if(choice_tool == 3)
    {
        ellipse_points[ellipse_points.size() - 1].point.setX(event->x());
        ellipse_points[ellipse_points.size() - 1].point.setY(event->y());
    }

    QWidget::update();

}

void WorkSpace::DrawImage(QPaintDevice *device)
{
    QPainter painter(device);
    painter.setTransform(transform);
    painter.drawPixmap(0, 0, width(), height(), work_holst);
    QWidget::update();
}

void WorkSpace::PenDraw(QPaintDevice *device)
{
    QPainter painter(device);
    painter.setTransform(transform);

    for(int i=0; i<pen_points.size()-1; i++)
    {

        painter.setPen(QPen(pen_points[i].color, pen_points[i].width, Qt::SolidLine));
        painter.drawLine(pen_points[i].point, pen_points[i+1].point);
    }
}

void WorkSpace::PolygonDraw(QPaintDevice *device)
{
    QPainter painter(device);
    painter.setTransform(transform);

    for(int i=0; i<polygon_points.size() - 1; i++)
    {
        painter.setPen(QPen(polygon_points[i].color, polygon_points[i].width, Qt::SolidLine));
        painter.drawLine(polygon_points[i].point, polygon_points[i+1].point);
    }
}

void WorkSpace::DrawLine(QPaintDevice *device)
{
    QPainter painter(device);

    painter.setTransform(transform);

    for(int i=0; i<line_points.size() - 1; i++)
    {
        if(i % 2) continue;

        painter.setPen(QPen(line_points[i].color, line_points[i].width, Qt::SolidLine));
        painter.drawLine(line_points[i].point, line_points[i+1].point);
    }
}

void WorkSpace::DrawSpray(QPaintDevice *device)
{
    QPainter painter(device);
    painter.setTransform(transform);

    for(int i=0; i<spray_points.size(); i++)
    {
        painter.setPen(QPen(spray_points[i].color, spray_points[i].width, Qt::SolidLine));
        painter.setBrush(QBrush(QColor(color)));
        painter.drawEllipse(spray_points[i].point, 1, 1);
    }

}

void WorkSpace::DrawRect(QPaintDevice *device, QVector<ToolProperties> &points, int flag)
{
    QPainter painter(device);
    painter.setTransform(transform);

    for(int i=0; i<points.size() - 1; i++)
    {
        if(i % 2) continue;
        painter.setPen(QPen(points[i].color, points[i].width, Qt::SolidLine));
        if(!flag) painter.drawRect(QRect(points[i].point, points[i+1].point));
        else painter.drawEllipse(QRect(points[i].point, points[i+1].point));
    }
}

void WorkSpace::Brush(QImage &img, int x, int y, QRgb newColor, QRgb oldColor)
{
    if (oldColor == newColor) return;

            QStack<QPoint> stk;
            QPoint pt;

            int y1;
            bool spanLeft, spanRight;

            stk.push(QPoint(x, y));

            while (!stk.empty())
            {
                pt = stk.pop();
                x = pt.x();
                y = pt.y();

                y1 = y;
                while (y1 >= 0 && img.pixel(x, y1) == oldColor) y1--;
                y1++;

                spanLeft = spanRight = false;

                while (y1 < img.height() && img.pixel(x, y1) == oldColor)
                {
                    img.setPixel(x, y1, newColor);

                    if (!spanLeft && x > 0 && img.pixel(x-1, y1) == oldColor)
                    {
                        stk.push(QPoint(x-1, y1));
                        spanLeft = true;
                    }
                    else if(spanLeft && x > 0 && img.pixel(x-1, y1) != oldColor)
                    {
                        spanLeft = false;
                    }
                    if (!spanRight && x < (img.width() - 1) && img.pixel(x+1, y1) == oldColor)
                    {
                        stk.push(QPoint(x+1, y1));
                        spanRight = true;

                    }
                    else if(spanRight && (x < img.width() - 1) && img.pixel(x+1, y1) != oldColor)
                    {
                        spanRight = false;
                    }

                    y1++;
                }
            }
}

void WorkSpace::DrawEraser(QPaintDevice *device)
{
    QPainter painter(device);
    painter.setBrush(QBrush(Qt::white));
    painter.setPen(QPen(Qt::NoPen));

    for(int i=0; i<eraser_points.size(); i++)
        painter.drawEllipse(eraser_points[i].point, 20, 20);
}

void WorkSpace::Scissors(QPaintDevice *device)
{
    QPainter painter(device);
    painter.setBrush(QBrush(Qt::white));
    painter.setPen(QPen(Qt::NoPen));

    for(int i=0; i<scissors_points.size() - 1; i++)
    {
        if(i % 2) continue;
        painter.drawRect(QRect(scissors_points[i].point, scissors_points[i+1].point));
    }
}

void WorkSpace::Rotate()
{
    transform.rotate(90);
    transform.translate(0, -1*(WIDTH));
    QWidget::update();
}

bool WorkSpace::isDrawing() const
{
    if(!pen_points.isEmpty() || !polygon_points.isEmpty() || !line_points.isEmpty() || !spray_points.isEmpty()
            || !rect_points.isEmpty() || !ellipse_points.isEmpty() || !brush_points.isEmpty() || !brush_points.isEmpty())
        return true;

    return false;
}

void WorkSpace::Clear()
{
    work_holst = QPixmap("default.png");

    pen_points.clear();
    rect_points.clear();
    polygon_points.clear();
    line_points.clear();
    spray_points.clear();


    QWidget::update();
}

void WorkSpace::Resize(int kof)
{
    static int new_width = width(), new_height = height();

    if(kof == 6)
    {
        this->resize(780, 480);

        WIDTH = 780;
        HEIGHT = 480;
    }
    else
    {
        kof *= 50;
        this->resize(new_width + kof, new_height + kof);

        WIDTH = width();
        HEIGHT = height();

    }

    delete image;
    image = new QImage(width(), height(), QImage::Format_ARGB32_Premultiplied);

    QWidget::update();
}

void WorkSpace::SetColor(QColor &value)
{
    color = value;
}

void WorkSpace::SetLineSize(int value)
{
    line_size = value;
}

void WorkSpace::SetValueofBrigthess(int value)
{
    brightness_value = value;
}

void WorkSpace::SetValueofLightness(int value)
{
    lightness_value = value;
}

void WorkSpace::SetBrightness()
{
    for(int i=0; i < image->height(); i++)
    {
        QRgb* tempLine = reinterpret_cast<QRgb*>(image->scanLine(i));

        for(int j=0; j < image->width(); j++)
        {
            int r = qRed(*tempLine) + brightness_value;
            int g = qGreen(*tempLine) + brightness_value;
            int b = qBlue(*tempLine) + brightness_value;
            int a = qAlpha(*tempLine) + brightness_value;

            *tempLine++ = qRgba(r > 255 ? 255 : r < 0 ? 0 : r,
            g > 255 ? 255 : g < 0 ? 0 : g,
            b > 255 ? 255 : b < 0 ? 0 : b,
            a
            );
        }
    }

    QWidget::update();
}

void WorkSpace::SetLightness()
{
    for (int y = 0; y < image->height(); ++y)
        {
            for(int x = 0; x < image->width(); ++x)
            {
                QColor pixel = image->pixel(x, y);
                pixel.setHsl(pixel.hue(), lightness_value, pixel.lightness(), pixel.alpha());
                image->setPixel(x, y, pixel.rgba());
            }
        }

    QWidget::update();
}

void WorkSpace::BlackWhite()
{
    for(int i=0; i<image->height(); i++)
        for(int j=0; j<image->width(); j++)
        {
            QColor col(image->pixel(i, j));
            col.setHsv(col.hue(), 0, col.value(), col.alpha());
            image->setPixel(i, j, col.rgb());
        }

    QWidget::update();
}

void WorkSpace::Inverse()
{
    image->invertPixels(QImage::InvertRgb);
}

void WorkSpace::Blur()
{

}
