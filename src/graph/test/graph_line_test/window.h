
#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <QWidget>

class Window : public QWidget
{
    Q_OBJECT

    public:
        Window(QWidget *p = 0);
        ~Window();

    protected:
        void paintEvent(QPaintEvent *event);
        void mousePressEvent(QMouseEvent *);
        void mouseReleaseEvent(QMouseEvent *);
        void mouseMoveEvent(QMouseEvent *);

    private:
        void init();
        void contentXml(QString &dataXml,
                        QString &proXml);

    private:
        class PrivateData;
        PrivateData *m_data;
};

#endif
