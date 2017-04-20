#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QApplication>
#include <QDebug>
#include <QtMath>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>
#include <QVector>
#include <QStack>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsBlurEffect>

#include "toolproperties.h"

class WorkSpace : public QLabel
{
    Q_OBJECT

public:
        WorkSpace(const QPixmap &pix, QWidget *parent = 0);
        ~WorkSpace();

        void SetImage(const QPixmap &pix);
        QImage *GetImage() const;
        void Clear();
        void Rotate();
        bool isDrawing() const;

public:
        int choice_tool;

protected:
            virtual void paintEvent(QPaintEvent *event);
            virtual void mousePressEvent(QMouseEvent *event);
            virtual void mouseMoveEvent(QMouseEvent *event);
            virtual void mouseReleaseEvent(QMouseEvent *event);

private:
        QImage *image;
        QPixmap work_holst;
        QTransform transform;

public:
        QVector<ToolProperties> pen_points;
        QVector<ToolProperties> rect_points;
        QVector<ToolProperties> polygon_points;
        QVector<ToolProperties> line_points;
        QVector<ToolProperties> spray_points;
        QVector<ToolProperties> ellipse_points;
        QVector<ToolProperties> brush_points;
        QVector<ToolProperties> eraser_points;
        QVector<ToolProperties> scissors_points;

        QColor color;


private:
        void DrawImage(QPaintDevice *device);
        void PenDraw(QPaintDevice *device);
        void PolygonDraw(QPaintDevice *device);
        void DrawLine(QPaintDevice *device);
        void DrawSpray(QPaintDevice *device);
        void DrawRect(QPaintDevice *device, QVector<ToolProperties> &points, int flag);
//        void Brush(QImage &img, int x, int y, QRgb newColor, QRgb oldColor);
//        void DrawEraser(QPaintDevice *device);
//        void Scissors(QPaintDevice *device);

        void SetBrightness();
        void SetLightness();

        void BlackWhite();
        void Inverse();
//        void Blur();

public:
         bool black_white, blur, inverse;

private:
         int WIDTH, HEIGHT;
         int line_size;

         int brightness_value, lightness_value;

private slots:
            void Resize(int kof);
            void SetColor(QColor &value);
            void SetLineSize(int value);

            void SetValueofBrigthess(int value);
            void SetValueofLightness(int value);
};

#endif // WORKSPACE_H
