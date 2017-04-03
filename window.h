#ifndef WINDOW_H
#define WINDOW_H

#include <QDialog>
#include <QToolBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QAction>
#include <QIcon>
#include <QFileDialog>
#include <QColorDialog>
#include <QSlider>
#include <QTimerEvent>
#include <QGroupBox>
#include <QKeyEvent>
#include <QVector>
#include <QLabel>

#include "workarea.h"
#include "workspace.h"

class WorkArea;
class WorkSpace;

class Window : public QDialog
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0);
    ~Window();

private:
        void Layout();
        QToolBar *toolbar(int action_count, QString *names, int icon_size, bool checkable = false, bool orientation = true);
        QSlider *slider(QString tooltip, int min, int max, int step, int pos);

private:
        WorkArea *work_area;
        WorkSpace *work_space;
        QSlider *size_slider, *size_line, *brightness, *lightness;
        QVector<QAction*> tools_array;
        QLabel *current_color;

protected:
            virtual void timerEvent(QTimerEvent *event);
            virtual void keyPressEvent(QKeyEvent *event);

private slots:
                void DifferentEvents();
                void SetCheckable();

signals:
                void resize_signal(int value);
                void color_signal(QColor &value);
                void line_size_signal(int value);
                void change_brightness(int value);
                void change_lightness(int value);
};

#endif // WINDOW_H
