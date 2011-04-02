#include "viewportal.h"
#include "phantomwindow.h"

int main (int argc, char **argv)
{
   QApplication app (argc, argv);
/*
   Viewportal *viewportal = new Viewportal();
   viewportal->setWindowFlags(Qt::WindowStaysOnTopHint);
   viewportal->
   viewportal->show();
*/

   PhantomWindow *win = new PhantomWindow();
   win->show();

   return app.exec();
}
