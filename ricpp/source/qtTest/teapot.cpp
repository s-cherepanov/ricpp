/****************************************************************
**
** Implementation TeapotField class, Qt tutorial 8 (adopted)
**
****************************************************************/
#include "stdafx.h"
#include "rendererloader/rendererloader.h"

#include "teapot.h"
#include <qpainter.h>


TeapotField::TeapotField( QWidget *parent, const char *name )
        : QWidget( parent, name )
{
    ang = 45;
    setPalette( QPalette( QColor( 250, 250, 200) ) );
	setBackgroundMode(NoBackground);

	m_ri = NULL;
}


void TeapotField::setAngle( int degrees )
{
    if ( degrees < 5 )
	degrees = 5;
    if ( degrees > 70 )
	degrees = 70;
    if ( ang == degrees )
	return;
    ang = degrees;
    repaint();
    emit angleChanged( ang );
}


void TeapotField::paintEvent( QPaintEvent * )
{
//    QString s = "Angle = " + QString::number( ang );
   QPainter p( this );
//    p.drawText( 200, 200, s );
   if ( m_ri ) {
	   TRi &ri = *m_ri;
       try {
           HDC hdc = p.handle();
           if ( !hdc )
              return;
           WId hwnd = this->winId();
           if ( !hwnd )
             return;

           ri.display("wxTest", ri.RI_FRAMEBUFFER, ri.RI_RGB,
                      "devicecontext", &hdc, "windowhandle", &hwnd, ri.RI_NULL);
           ri.clipping((RtFloat).1, (RtFloat)100.0);
           ri.frameBegin(1);
               RtFloat d = (RtFloat)ang; // 39.6; // normal objektive 50mm on 35mm film
               ri.projection(ri.RI_PERSPECTIVE, ri.RI_FOV, &d, ri.RI_NULL);
               ri.worldBegin();
                   RtFloat intensity = (RtFloat)27.0;
                   ri.lightSource(ri.RI_POINTLIGHT, ri.RI_INTENSITY, &intensity, ri.RI_NULL);
                   ri.translate((RtFloat)0.0, (RtFloat)-1.0, (RtFloat)10.0);
                   ri.rotate((RtFloat)-135.0, (RtFloat)1.0, (RtFloat)0.0, (RtFloat)0.0);
                   ri.surface(ri.RI_MATTE, ri.RI_NULL);
                   ri.geometry("teapot", ri.RI_NULL);
               ri.worldEnd();
           ri.frameEnd();
       } catch (TSystemException &t) {
          t.displayMessage();
          try {
             ri.synchronize(ri.RI_ABORT);
          } catch (...) {
          }
       } catch (TRendererError &t) {
          t.displayMessage();
          try {
             ri.synchronize(ri.RI_ABORT);
          } catch (...) {
          }
       } catch (...) {
          try {
             ri.synchronize(ri.RI_ABORT);
          } catch (...) {
          }
       }
   }
}


QSizePolicy TeapotField::sizePolicy() const
{
    return QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
}

