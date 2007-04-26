/****************************************************************
**
** Qt tutorial 8 (adopted)
**
****************************************************************/

#include "stdafx.h"
#include "rendererLoader/rendererloader.h"

#include <qapplication.h>
#include <qpushbutton.h>
#include <qlcdnumber.h>
#include <qfont.h>
#include <qlayout.h>

#include "lcdrange.h"
#include "teapot.h"

class QMyApplication : public QApplication {
public:
	inline QMyApplication(int argc, char **argv) : QApplication(argc, argv), m_ri(NULL), m_rendererLoader(NULL) {}
	inline ~QMyApplication() {
		if ( m_rendererLoader ) {
			if ( m_ri ) {
				try {
					m_ri->end();
				} catch (TSystemException &t) {
					t.displayMessage();
				} catch (TRendererError &t) {
					t.displayMessage();
				} catch (...) {
					MessageBoxA(NULL, "Unexpected Error in ~QMyApplication() while ending renderer.", NULL, MB_OK);
				}
			}
			delete m_rendererLoader;
		}
	}

	inline bool initRenderer() {
		try {
			m_rendererLoader = new TRendererLoader("contexts");
			if ( !m_rendererLoader )
				return 0;
		} catch (TSystemException &t) {
			t.displayMessage();
			if ( m_rendererLoader )
				delete m_rendererLoader;
			return 0;
		} catch (...) {
			MessageBoxA(NULL, "Unexpected Error in QMyApplication::initRenderer() while establishing renderer loader.", NULL, MB_OK);
			return 0;
		}
		if ( m_rendererLoader ) {

            try {
                m_ri = m_rendererLoader->loadRenderer("glrenderer");
				if ( m_ri ) {
                    m_ri->errorHandler(m_ri->errorPrint);
					m_ri->begin();
				}
            } catch (TSystemException &t) {
                t.displayMessage();
            } catch (TRendererError &t) {
                t.displayMessage();
            } catch (...) {
                MessageBoxA(NULL, "Unexpected Error in MyWndProc() while loading glrenderer.", NULL, MB_OK);
            }
		}

		return m_ri != NULL;
	}

	TRi *m_ri;
private:
	TRendererLoader *m_rendererLoader;
};
QMyApplication *app = NULL;

class MyWidget: public QWidget
{
public:
    MyWidget( QWidget *parent=0, const char *name=0 );
};


MyWidget::MyWidget( QWidget *parent, const char *name )
        : QWidget( parent, name )
{
    QPushButton *quit = new QPushButton( "Quit", this, "quit" );
    quit->setFont( QFont( "Times", 18, QFont::Bold ) );

    connect( quit, SIGNAL(clicked()), qApp, SLOT(quit()) );

    LCDRange *angle = new LCDRange( this, "angle" );
    angle->setRange( 5, 70 );

    TeapotField *teapotField 
	= new TeapotField( this, "teapotField" );

	teapotField->m_ri = app->m_ri;

	connect( angle, SIGNAL(valueChanged(int)),
	     teapotField, SLOT(setAngle(int)) );
    connect( teapotField, SIGNAL(angleChanged(int)),
	     angle, SLOT(setValue(int)) );

    QGridLayout *grid = new QGridLayout( this, 2, 2, 10 );
    //2x2, 10 pixel border

    grid->addWidget( quit, 0, 0 );
    grid->addWidget( angle, 1, 0, Qt::AlignTop );
    grid->addWidget( teapotField, 1, 1 );
    grid->setColStretch( 1, 10 );

    angle->setValue( 60 );
    angle->setFocus();
}

int main( int argc, char **argv )
{
    QMyApplication a( argc, argv );
	app = &a;

	app->initRenderer();

    MyWidget w;
    w.setGeometry( 100, 100, 500, 355 );
    app->setMainWidget( &w );
    w.show();
    return app->exec();
}
