#include "window.h"

Window::Window(QWidget *parent): QDialog(parent)
{
    Layout();

    this->setWindowTitle(" ");
    this->setStyleSheet("background: rgb(93, 93, 93)");

    this->startTimer(100);
}

Window::~Window()
{

}

void Window::Layout()
{
    QVBoxLayout *main_layout = new QVBoxLayout;

    QHBoxLayout *top_layout = new QHBoxLayout;
    top_layout->addSpacing(45);

    QString top_names[] = {"new", "open", "save", "save as"};
    top_layout->addWidget(toolbar(4, top_names, 30));

    QString settings_name[] = {"scissors", "rotate", "colors"};

    QHBoxLayout *settings = new QHBoxLayout;
    settings->setAlignment(Qt::AlignRight);
    settings->addWidget(toolbar(3, settings_name, 30));
    settings->addSpacing(20);

    top_layout->addLayout(settings);
    top_layout->addSpacing(22);

    QHBoxLayout *content = new QHBoxLayout;

    QVBoxLayout *left_menu = new QVBoxLayout;

    QString left_names[] = {"pen", "rectangle", "ellipse", "polygon", "line", "spray", "brush",
                            "eyedropper", "eraser", "cancel"};
    left_menu->addWidget(toolbar(10, left_names, 30, true, false));

    current_color = new QLabel;
    current_color->setMaximumHeight(40);
    current_color->setFrameStyle(QFrame::Panel | QFrame::Raised);
    current_color->setLineWidth(3);
    left_menu->addWidget(current_color);

    content->addLayout(left_menu);

    work_area = new WorkArea;
    work_space = new WorkSpace(QPixmap("default.png"), work_area);
    work_area->layout->addWidget(work_space);

    connect(this, SIGNAL(resize_signal(int)), work_space, SLOT(Resize(int)));
    connect(this, SIGNAL(color_signal(QColor&)), work_space, SLOT(SetColor(QColor&)));
    connect(this, SIGNAL(line_size_signal(int)), work_space, SLOT(SetLineSize(int)));
    connect(this, SIGNAL(change_brightness(int)), work_space, SLOT(SetValueofBrigthess(int)));
    connect(this, SIGNAL(change_lightness(int)), work_space, SLOT(SetValueofLightness(int)));

    content->addWidget(work_area);

    QVBoxLayout *effects = new QVBoxLayout;
    effects->setAlignment(Qt::AlignCenter);

    QString effects_name[] = {"blur", "black-white", "inverse"};
    effects->addWidget(toolbar(3, effects_name, 30, true, false));

    content->addLayout(effects);

    size_slider = slider("Holst Size", 0, 6, 1, 0);
    connect(size_slider, SIGNAL(valueChanged(int)), this, SIGNAL(resize_signal(int)));

    size_line = slider("Line sizes", 1, 10, 1, 0);
    connect(size_line, SIGNAL(valueChanged(int)), this, SIGNAL(line_size_signal(int)));

    brightness = slider("Brightness", -200, 0, 50, 0);
    connect(brightness, SIGNAL(valueChanged(int)), this, SIGNAL(change_brightness(int)));

    lightness = slider("Lightness", 0, 255, 1, 80);
    connect(lightness, SIGNAL(valueChanged(int)), this, SIGNAL(change_lightness(int)));

    QVBoxLayout *sizes_lay = new QVBoxLayout;
    sizes_lay->addWidget(size_slider);
    sizes_lay->addWidget(size_line);

    QGroupBox *sizes_group = new QGroupBox("Sizes");
    sizes_group->setStyleSheet("color: white; font-weight: bold;");
    sizes_group->setLayout(sizes_lay);

    QVBoxLayout *hue_lay = new QVBoxLayout;
    hue_lay->addWidget(brightness);
    hue_lay->addWidget(lightness);

    QGroupBox *hue_group = new QGroupBox("Hues");
    hue_group->setStyleSheet("color: white; font-weight: bold;");
    hue_group->setLayout(hue_lay);

    QHBoxLayout *global_settings = new QHBoxLayout;
    global_settings->addSpacing(40);
    global_settings->addWidget(sizes_group);
    global_settings->addWidget(hue_group);
    global_settings->addSpacing(40);

    main_layout->addLayout(top_layout);
    main_layout->addLayout(content);
    main_layout->addLayout(global_settings);
    this->setLayout(main_layout);
    this->setFixedSize(926, 656);
}

QToolBar *Window::toolbar(int action_count, QString *names, int icon_size, bool checkable, bool orientation)
{
    QToolBar *tools = new QToolBar();
    if (orientation) tools->setOrientation(Qt::Horizontal);
    else tools->setOrientation(Qt::Vertical);

    tools->setIconSize(QSize(icon_size, icon_size));
    tools->setStyleSheet("color: white; font-weight: bold;");

    QAction *act;
    for(int i=0; i<action_count; i++)
    {
        act = tools->addAction(QIcon(QPixmap(names[i])), names[i], this, SLOT(DifferentEvents()));
        if(checkable)act->setCheckable(true);
        connect(act, SIGNAL(triggered(bool)), this, SLOT(SetCheckable()));
        tools_array.push_back(act);
    }

    tools->setCursor(Qt::PointingHandCursor);

    return tools;
}

QSlider *Window::slider(QString tooltip, int min, int max, int step, int pos)
{
    QSlider *slr = new QSlider(Qt::Horizontal);
    slr->setMinimum(min);
    slr->setMaximum(max);
    slr->setValue(pos);
    slr->setSingleStep(step);
    slr->setToolTip(tooltip);
    slr->setStyleSheet("color: white; font-weight: bold;");
    slr->setCursor(Qt::PointingHandCursor);

    return slr;
}

void Window::timerEvent(QTimerEvent *event)
{
    if(work_space->isDrawing()) size_slider->setEnabled(false);
    else size_slider->setEnabled(true);

    int r = work_space->color.red();
    int g = work_space->color.green();
    int b = work_space->color.blue();

    current_color->setStyleSheet("background: rgb(" + QString::number(r) + "," + QString::number(g)
                                 + "," + QString::number(b) + ");");

    Q_UNUSED(event);
}

void Window::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() == Qt::CTRL && event->key() == Qt::Key_Z)
    {
        if(work_space->choice_tool == 1)
            if(!work_space->pen_points.isEmpty())
                    work_space->pen_points.pop_back();

        if(work_space->choice_tool == 2)
            if(!work_space->rect_points.isEmpty())
                for(int i=0; i<2; i++)
                    work_space->rect_points.pop_back();

        if(work_space->choice_tool == 3)
            if(!work_space->ellipse_points.isEmpty())
                for(int i=0; i<2; i++)
                    work_space->ellipse_points.pop_back();

        if(work_space->choice_tool == 4)
            if(!work_space->polygon_points.isEmpty())
                for(int i=0; i<2; i++)
                    work_space->polygon_points.pop_back();

        if(work_space->choice_tool == 5)
            if(work_space->line_points.size() >= 2)
                for(int i=0; i<2; i++)
                    work_space->line_points.pop_back();

        if(work_space->choice_tool == 6)
            if(!work_space->spray_points.isEmpty())
                    work_space->spray_points.pop_back();

        if(work_space->choice_tool == 7)
            if(!work_space->brush_points.isEmpty())
                    work_space->brush_points.pop_back();

        if(work_space->choice_tool == 9)
            if(!work_space->eraser_points.isEmpty())
                    work_space->eraser_points.pop_back();

        if(work_space->choice_tool == 10)
            if(!work_space->scissors_points.isEmpty())
                for(int i=0; i<2; i++)
                    work_space->scissors_points.pop_back();

    }
}

void Window::DifferentEvents()
{
    QAction *action = qobject_cast<QAction*>(sender());
    static bool first_time = true;

    if(action->text() == "new")
    {
        work_space->Clear();
        first_time = true;
        size_slider->setEnabled(true);
    }

    if(action->text() == "open")
    {
        QString path = QFileDialog::getOpenFileName(this, "Open...", "", "*.png;; *.jpeg;; *.jpg;;");
        if(!path.isEmpty())
        {
            work_space->Clear();
            work_space->SetImage(QPixmap(path));
            first_time = true;
            size_slider->setEnabled(false);
        }

        work_space->update();
    }

    if(action->text() == "save")
    {
        static QString path;

        if(first_time)
        {
            path = QFileDialog::getSaveFileName(this, "Save...", "", "*.png;; *.jpeg;; *.jpg;;");
            if(!path.isEmpty()) work_space->GetImage()->save(path, "PNG");
            work_space->update();

            first_time = false;
        }
        else
        {
            work_space->GetImage()->save(path, "PNG");
        }
    }

    if(action->text() == "save as")
    {
        QString path = QFileDialog::getSaveFileName(this, "Save...", "", "*.png;;");
        if(!path.isEmpty()) work_space->GetImage()->save(path, "PNG");
        work_space->update();
    }

    if(action->text() == "pen") work_space->choice_tool = 1;
    if(action->text() == "polygon") work_space->choice_tool = 4;
    if(action->text() == "line") work_space->choice_tool = 5;
    if(action->text() == "spray") work_space->choice_tool = 6;
    if(action->text() == "cancel") work_space->choice_tool = 0;
    if(action->text() == "rectangle") work_space->choice_tool = 2;
    if(action->text() == "ellipse") work_space->choice_tool = 3;
    if(action->text() == "brush") work_space->choice_tool = 7;
    if(action->text() == "eyedropper") work_space->choice_tool = 8;
    if(action->text() == "eraser") work_space->choice_tool = 9;
    if(action->text() == "scissors") work_space->choice_tool = 10;
    if(action->text() == "rotate")
    {
        work_space->Rotate();
        size_slider->setEnabled(false);
    }
    if(action->text() == "colors")
    {
        QColor color = QColorDialog::getColor(QColor(Qt::black), this, "Colors...");
        emit color_signal(color);
    }

    if(action->text() == "black-white")
    {
        if(action->isChecked()) work_space->black_white = true;
        else work_space->black_white = false;
    }

    if(action->text() == "inverse")
    {
        if(action->isChecked()) work_space->inverse = true;
        else work_space->inverse = false;
    }

    if(action->text() == "blur")
    {
        if(action->isChecked()) work_space->blur = true;
        else work_space->blur = false;
    }

}

void Window::SetCheckable()
{
    QAction *action = qobject_cast<QAction*>(sender());

    for(int i=7; i<tools_array.size() - 3; i++)
    {
        tools_array[i]->setChecked(false);
        if(action->text() == tools_array[i]->text()) tools_array[i]->setChecked(true);
    }
}

