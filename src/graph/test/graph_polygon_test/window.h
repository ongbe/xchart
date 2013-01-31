
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
        void contextMenuEvent(QContextMenuEvent *);

    private slots:
        void addPoint();
        void delPoint();

    private:
        void init();
        void createDataXml(QString &dataXml, QString &proXml);

    private:
        class PrivateData;
        PrivateData *m_data;
};

#endif
