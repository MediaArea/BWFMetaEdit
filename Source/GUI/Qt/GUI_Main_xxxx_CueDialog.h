// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_xxxx_CueDialogH
#define GUI_Main_xxxx_CueDialogH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class Core;
class QLabel;
class QLineEdit;
class QTimeEdit;
class QCheckBox;
class QDoubleSpinBox;
class QDialogButtonBox;
#include "ZenLib/ZtringListList.h"
#include <QStandardItemModel>
#include <QItemDelegate>
#include <QTableWidget>
#include <QValidator>
#include <QComboBox>
#include <QDialog>

using namespace ZenLib;

//***************************************************************************
// CueDialog_SampleRateDialog
//***************************************************************************

class CueDialog_SampleRateDialog : public QDialog
{
    Q_OBJECT
public:
    //Constructor/Destructor
    CueDialog_SampleRateDialog(int32u Value, int32u SampleRate, QWidget* parent=0);

    //Widgets
    QTimeEdit*         TimeEdit;
    QDoubleSpinBox*    LineEdit;

    ////Actions
    int32u GetValue();

private Q_SLOTS:
    //Actions
    void OnTimeChanged(const QTime& Time);
    void OnValueEdited(const QString& Value);
private:
    //Internal
    int64u    SampleRate;
    bool      IsChanging;
};

//***************************************************************************
// Validators
//***************************************************************************

//---------------------------------------------------------------------------
class CC4Validator : public QValidator
{
    Q_OBJECT
public:
    //Constructor/Destructor
    CC4Validator(QObject* parent=nullptr);

    //Validator
    QValidator::State validate(QString& input, int& pos) const;
};

//---------------------------------------------------------------------------
class IDValidator : public QValidator
{
    Q_OBJECT
public:
    //Constructor/Destructor
    IDValidator(QModelIndex index, QObject* parent=0);

    //Validator
    QValidator::State validate(QString& input, int& pos) const;

private:
    //Internal
    QModelIndex    index;
};

//---------------------------------------------------------------------------
class CountryValidator : public QValidator
{
    Q_OBJECT
public:
    //Constructor/Destructor
    CountryValidator(QObject* parent=nullptr);

    //Validator
    QValidator::State validate(QString& input, int& pos) const;
};

//---------------------------------------------------------------------------
class LanguageValidator : public QValidator
{
    Q_OBJECT
public:
    //Constructor/Destructor
    LanguageValidator(QObject* parent=nullptr);

    //Validator
    QValidator::State validate(QString& input, int& pos) const;
};

//***************************************************************************
// CueDialogEdit_Delegate
//***************************************************************************

class CueDialogEdit_Delegate : public QItemDelegate
{
    Q_OBJECT
public:
    //Constructor/Destructor
    CueDialogEdit_Delegate(QObject* parent=0) : QItemDelegate(parent) {};

    //Delegate
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

//***************************************************************************
// CueDialogCombo_Delegate
//***************************************************************************

class CueDialogCombo_Delegate : public QItemDelegate
{
    Q_OBJECT
public:
    //Constructor/Destructor
    CueDialogCombo_Delegate(QObject* parent=0) : QItemDelegate(parent) {};

    //Delegate
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

//***************************************************************************
// CueDialog_TableWidget
//***************************************************************************

class CueDialog_TableWidget : public QTableWidget
{
    Q_OBJECT
public:
    //Constructor/Destructor
    CueDialog_TableWidget(Core* C, const std::string& FileName, bool ReadOnly, QWidget* parent=0);

protected:
    bool edit(const QModelIndex& index, QAbstractItemView::EditTrigger trigger, QEvent* event);

private:
    //Internal
    std::string FileName;
    Core*       C;
    int32u      SampleRate;
    bool        ReadOnly;
};


//***************************************************************************
// GUI_Main_xxxx_CueDialog
//***************************************************************************

class GUI_Main_xxxx_CueDialog : public QDialog
{
    Q_OBJECT
public:
    //Constructor/Destructor
    GUI_Main_xxxx_CueDialog(Core* C, const std::string& FileName, bool ReadOnly=false, QWidget* parent=NULL);

    //Widgets
    CueDialog_TableWidget*    Table;
    QDialogButtonBox*         Dialog;
    QCheckBox*                DavidMode;
    QLabel*                   Message;

private Q_SLOTS:
    //Actions
    void OnCloseClicked();
    void OnMenu_Save();
    void OnMenu_Load();
    void OnEdited(QTableWidgetItem* Item);
    void OnAccept();

protected:
    //Events
    void hideEvent(QHideEvent* Event);

private:
    //Internal
    Core*          C;
    std::string    FileName;
    std::string    Xml;
    int32u         SampleRate;
    int16u         BitsPerSample;
    bool           IsAccepted;
    bool           Updating;
    bool           ReadOnly;

    //Helpers
    void UpdateItem(QTableWidgetItem* Item);
    void UpdateRows();
    void Xml2List();
    void List2Xml();
};
#endif //GUI_Main_xxxx_CueDialogH
