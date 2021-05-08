// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_PerFileH
#define GUI_Main_PerFileH
//---------------------------------------------------------------------------

#include "GUI/Qt/GUI_Main_xxxx_EditMenu.h"

#include <QQuickWidget>
#include <QAbstractListModel>
#include <QQmlContext>
#include <QValidator>
#include <QPoint>
#include <string>

#include "ZenLib/ZtringListList.h"

class QEvent;
class GUI_Main;
class Core;
using namespace std;
using namespace ZenLib;
//---------------------------------------------------------------------------

//***************************************************************************
// PerFileModel
//***************************************************************************

#define FETCH_COUNT 10

//---------------------------------------------------------------------------
class PerFileModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum userEventRoles
    {
        FileRole=Qt::UserRole+1,
        TechRole,
        ExpandedRole,
        ModifiedRole,
        EditModeRole
    };

    //Constructor/Destructor
    PerFileModel(GUI_Main* Main, Core* _C, QObject *parent=0);
    ~PerFileModel();

    int rowCount(const QModelIndex &parent) const;
    QHash<int, QByteArray> roleNames() const;
 
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    //Helpers
    Q_INVOKABLE void saveFile(const QString& FileName);
    Q_INVOKABLE void closeFile(const QString& FileName);
    Q_INVOKABLE bool valid(const QString& FileName) const;
    Q_INVOKABLE bool valid(const QString& FileName, const QString& Field, const QString& Value) const;
    Q_INVOKABLE QString lastValidationError(const QString& FileName) const;
    Q_INVOKABLE QString lastValidationWarning(const QString& FileName) const;
    Q_INVOKABLE QString errors(const QString& FileName) const;
    Q_INVOKABLE QString warnings(const QString& FileName) const;
    Q_INVOKABLE QString informations(const QString& FileName) const;
    Q_INVOKABLE QString unsupportedChunks(const QString& FileName) const;
    Q_INVOKABLE QString value(const QString& FileName, const QString& Field) const;
    Q_INVOKABLE void setValue(const QString& FileName, const QString& Field, const QString& Value);
    Q_INVOKABLE bool modified(const QString& FileName, const QString& Field) const;
    Q_INVOKABLE bool readOnly(const QString& FileName, const QString& Field) const;
    Q_INVOKABLE bool readOnly(const QString& FileName) const;
    Q_INVOKABLE bool isWritable(const QString& FileName) const;
    Q_INVOKABLE bool visible(const QString& FileName, const QString& Field) const;
    Q_INVOKABLE void editField(const QString& FileName, const QString& Field);
    Q_INVOKABLE void showCoreMenu(const QPoint& globalPos, const QString& FileName, const QString& Field);
    Q_INVOKABLE void setSelected(const QString& FileName, const QString& Field);
    Q_INVOKABLE void deselect(const QString& FileName, const QString& Field);

    void Fill();

public Q_SLOTS:
    void onValuesChanged(bool onlySelected);

protected:
    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);

private:
    QString Get_Technical_Field(const QString FileName, const QString FieldName) const;
    QString Technical_Info(const QString FileName) const;

    Core* C;
    GUI_Main* Main;
    GUI_Main_xxxx_EditMenu* MenuHandler;

    int Count;
    QStringList FileNames;
    ZtringListList TechnicalData;
    QHash<QString, bool> Expanded;
    QHash<QString, bool> EditMode;
    QPair<QString, QString> Selection;
};

//***************************************************************************
// GUI_Main_PerFile
//***************************************************************************

//---------------------------------------------------------------------------
class GUI_Main_PerFile : public QQuickWidget
{
    Q_OBJECT

public:
    //Constructor/Destructor
    GUI_Main_PerFile(Core* _C, GUI_Main* parent);
    ~GUI_Main_PerFile();

protected :
    //Events
    bool event(QEvent *Event);
    void dragEnterEvent(QDragEnterEvent* event);

private:
    Core* C;
    GUI_Main* Main;
    PerFileModel* Model;
};

#endif
