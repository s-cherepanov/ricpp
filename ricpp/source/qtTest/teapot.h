/****************************************************************
**
** Definition of TeapotField class, Qt tutorial 8 (adopted)
**
****************************************************************/

#ifndef TEAPOT_H
#define TEAPOT_H

#include <qwidget.h>
class TRi;

class TeapotField : public QWidget
{
    Q_OBJECT
public:
    TeapotField( QWidget *parent=0, const char *name=0 );

    int angle() const { return ang; }
    QSizePolicy sizePolicy() const;

    TRi *m_ri;

public slots:
    void setAngle( int degrees );

signals:
    void angleChanged( int );

protected:
    void paintEvent( QPaintEvent * );

private:
    int ang;
};


#endif // TEAPOT_H
