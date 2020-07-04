// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_xxxx_ContextMenuH
#define GUI_Main_xxxx_ContextMenuH
//---------------------------------------------------------------------------

#include <QPair>
#include <QList>
#include <QPoint>
#include <QObject>
#include <string>

class GUI_Main;
class Core;
using namespace std;

//---------------------------------------------------------------------------
class GUI_Main_xxxx_ContextMenu : public QObject
{
    Q_OBJECT

public:
    //Constructor/Destructor
    GUI_Main_xxxx_ContextMenu(GUI_Main* Main, Core* C, QObject *parent=0) : QObject(parent), C(C), Main(Main) {};
    ~GUI_Main_xxxx_ContextMenu() {};

    int showCoreMenu(const QPoint& globalPos, QList<QPair<string, string> > forItems);

private:
    Core* C;
    GUI_Main* Main;
};

#endif
