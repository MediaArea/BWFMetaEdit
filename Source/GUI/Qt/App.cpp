// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <QApplication>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#include "GUI/Qt/App.h"
#include "GUI/Qt/GUI_Main.h"
#include "Common/Core.h"
#include <vector>
#include <iostream>
using namespace std;

#if defined(_WIN32) && !defined(_DLL)
    #include <QtPlugin>
    Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif

#ifdef __MACOSX__
    #include <ApplicationServices/ApplicationServices.h>
#endif //__MACOSX__

int main (int argc, char** argv)
{
    /*
    #ifdef __MACOSX__
        ProcessSerialNumber PSN;
        GetCurrentProcess(&PSN);
        TransformProcessType(&PSN, kProcessTransformToForegroundApplication);
    #endif //__MACOSX__
    */

    setlocale(LC_ALL, "");

    QApplication App(argc, argv);
    Core C;
    C.Errors_Continue=true; //Always for the GUI

    // QApplication initialisation reset charset to default (ASCII when launched from finder) value.
    #ifdef __MACOSX__
    if (strcmp(setlocale(LC_CTYPE, NULL), "C")==0)
        setlocale(LC_CTYPE, "UTF-8");
    #endif

    GUI_Main Window(&C);
    Window.show();
    int ToReturn=App.exec();

    return ToReturn;
}
