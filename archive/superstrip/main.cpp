#include <qapplication.h>
#include "superstrip2form.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    SuperStrip2Form w;
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
