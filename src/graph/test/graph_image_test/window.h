
#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <QWidget>

class Window : public QWidget
{
    Q_OBJECT

    public:
        Window(QWidget *p = 0);
        ~Window();

    private:
        void init();
    protected:
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);

    private:
        class PrivateData;
        PrivateData *m_data;
};

#endif
