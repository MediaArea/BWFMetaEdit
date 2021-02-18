// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_xxxx_EditMenuH
#define GUI_Main_xxxx_EditMenuH
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
class GUI_Main_xxxx_EditMenu : public QObject
{
    Q_OBJECT

public:
    //Constructor/Destructor
    GUI_Main_xxxx_EditMenu(GUI_Main* Main, Core* C, QObject *parent=0) : QObject(parent), C(C), Main(Main), Updating(false) {};
    ~GUI_Main_xxxx_EditMenu() {};

    void updateEditMenu(QList<QPair<string, string> > forItems);
    void showContextMenu(const QPoint& globalPos);

public Q_SLOTS:
    void onActionTriggered();

Q_SIGNALS:
    void valuesChanged(bool selectedOnly);

private:
    QList<QPair<string, string> > Items;
    Core* C;
    GUI_Main* Main;
    bool Updating;
};

#endif
