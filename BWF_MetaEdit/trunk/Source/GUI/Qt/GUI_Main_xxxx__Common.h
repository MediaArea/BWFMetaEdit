// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_xxxx_Table_CommonH
#define GUI_Main_xxxx_Table_CommonH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class Core;
#include "GUI/Qt/GUI_Preferences.h"
#include <QTableWidget>
#include <QItemDelegate>
#include <string>
class QEvent;
class GUI_Main;
using namespace std;
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_xxxx__Common
//***************************************************************************

class GUI_Main_xxxx__Common : public QTableWidget  
{
    Q_OBJECT

public:
    //Constructor/Destructor
    GUI_Main_xxxx__Common(Core* C, GUI_Main* parent);

protected Q_SLOTS:
    void OnItemSelectionChanged ();

protected:
    //Events
    bool                    event               (QEvent *Event); 
    void                    dataChanged         (const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> & roles = QVector<int> ()); 

    //Helpers
    void                    Colors_Update       ();
    void                    Colors_Update       (QTableWidgetItem* Item, const string &FileName, const string &Field);
    void                    SetEnabled          (int Row, const QString &Field);
    void                    SetEnabled          (const QModelIndex &Index, const QString &Field)       {SetEnabled(Index.row(), Field);}
    void                    SetEnabled          (const QTableWidgetItem &Index, const QString &Field)  {SetEnabled(Index.row(), Field);}
    void                    SetText             (int Row, const QString &Field);
    void                    SetText             (const QModelIndex &Index, const QString &Field)       {SetText(Index.row(), Field);}
    void                    SetText             (const QTableWidgetItem &Index, const QString &Field)  {SetText(Index.row(), Field);}

    void                    Fill                ();
    virtual const string   &Fill_Content        ()=0;
    virtual group           Fill_Group          ()=0;
    virtual bool            Fill_Enabled        (const string &FileName, const string &Field, const string &Value)=0;

    //Internal
    Core*       C;
    GUI_Main*   Main;
    bool        Updating;
    string      FileName_Before;
};

//***************************************************************************
// OriginationTimeDelegate
//***************************************************************************

class OriginationTimeDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    OriginationTimeDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

//***************************************************************************
// OriginationTimeDelegate
//***************************************************************************

class TimeReferenceDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    TimeReferenceDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif
