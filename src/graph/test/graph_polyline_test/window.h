
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

    private slots:
        void add();
        void remove();
        void moveline();
        void copy();

    private:
        class PrivateData;
        PrivateData *m_data;
};

#endif
