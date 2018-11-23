#include <qapplication.h>
#include "qtmixform.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    QTMixForm w;
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
