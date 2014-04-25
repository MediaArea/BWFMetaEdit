// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digitization Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_xxxx_CodingHistoryDialogH
#define GUI_Main_xxxx_CodingHistoryDialogH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class Core;
#include <QDialog>
#include "ZenLib/ZtringListList.h"
#include <QItemDelegate>
#include <QTableWidget>
class QEvent;
class QComboBox;
class QTextEdit;
class QTabWidget;
using namespace ZenLib;
//---------------------------------------------------------------------------

//***************************************************************************
// CodingHistoryDialog_TableWidget
//***************************************************************************

class CodingHistoryDialog_TableWidget : public QTableWidget
{
public:
    //In/Out
    bool            Updating;

    //Constructor/Destructor
    CodingHistoryDialog_TableWidget(ZtringListList &List, QWidget *parent = 0);

private:
    //Events
    void dataChanged (const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> & roles = QVector<int> ());

    //Internal;
    ZtringListList* List;
};

//***************************************************************************
// GUI_Main_xxxx_CodingHistoryDialog
//***************************************************************************

class GUI_Main_xxxx_CodingHistoryDialog : public QDialog
{
    Q_OBJECT

public:
    //Constructor/Destructor
    GUI_Main_xxxx_CodingHistoryDialog(Core* C, const std::string &FileName_, const std::string &Field_, const QString &OldText, bool Rules_Recommendations, QWidget* parent=NULL);

    //Widgets
    QTabWidget*                         Central;
    CodingHistoryDialog_TableWidget*    Table;
    QTextEdit*                          TextEdit;
    QPushButton*                        Cancel;
    QPushButton*                        Valid;
    QPushButton*                        Load;
    QPushButton*                        Save;

private Q_SLOTS:
    //Actions
    void OnAccept                   ();
    void OnMenu_Load                ();
    void OnMenu_Save                ();
    void OnMenu_List                ();
    void OnMenu_Text                ();
    void OnCurrentChanged           (int Index);

protected:
    //Events
    void hideEvent (QHideEvent* Event);
    
private:
    //Internal
    Core*   C;
    std::string FileName;
    std::string Field;
    bool    Rules_Recommendations;
    ZtringListList List;

    //Helpers
    void Fill();
    void List2Text();
    void Text2List();
};

//***************************************************************************
// CodingHistoryDialog_Delegate
//***************************************************************************

class CodingHistoryDialog_Delegate : public QItemDelegate
{
    Q_OBJECT

public:
    CodingHistoryDialog_Delegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:
    virtual void Fill(QComboBox* Editor) const {}
};

//***************************************************************************
// xxxDelegate
//***************************************************************************

#define DELEGATE(_NAME) \
class _NAME##Delegate : public CodingHistoryDialog_Delegate \
    { \
    public: \
        _NAME##Delegate(QObject *parent = 0) : CodingHistoryDialog_Delegate() {} \
    \
    protected: \
        void Fill(QComboBox* Editor) const; \
    }; \

DELEGATE(CodingAlgorithm)
DELEGATE(SamplingFrequency)
DELEGATE(BitRate)
DELEGATE(WordLength)
DELEGATE(Mode)

#endif
