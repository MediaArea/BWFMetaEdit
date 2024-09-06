// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Main_PerFile.h"
#include "GUI/Qt/GUI_Main_xxxx__Common.h"
#include "GUI/Qt/GUI_Main.h"
#include "GUI/Qt/GUI_Preferences.h"
#include "GUI/Qt/GUI_Main_xxxx_Bext.h"
#include "GUI/Qt/GUI_Main_xxxx_CodingHistoryDialog.h"
#include "GUI/Qt/GUI_Main_xxxx_DateDialog.h"
#include "GUI/Qt/GUI_Main_xxxx_Loudness.h"
#include "GUI/Qt/GUI_Main_xxxx_TextEditDialog.h"
#include "GUI/Qt/GUI_Main_xxxx_TimeReferenceDialog.h"
#include "GUI/Qt/GUI_Main_xxxx_UmidDialog.h"
#include "GUI/Qt/GUI_Main_xxxx_CueDialog.h"
#include "GUI/Qt/GUI_Main_xxxx_EditMenu.h"
#include "Common/Core.h"
#include "ZenLib/ZtringListList.h"
#include <QLabel>
#include <QEvent>
#include <QFont>
#include <QTextEdit>
#include <QDateEdit>
#include <QSpinBox>
#include <QItemDelegate>
#include <QStandardItemModel>
#include <QDate>
#include <QContextMenuEvent>
#include <QAction>
#include <QMenu>
using namespace ZenLib;
using namespace std;
//---------------------------------------------------------------------------

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
QString Human_Readable_Size(QString Size)
{
    QString ToReturn;

    float80 FSize=Ztring().From_UTF8(Size.toStdString()).To_float80();

    int8u Len=0;
    int8u Max=5;

    for (; Len<Max && FSize>1024.0; Len++)
        FSize/=1024.0;

    ToReturn=QString(Ztring().From_Number(FSize, 1, Ztring_NoZero).To_UTF8().c_str());
    switch (Len)
    {
        case 0: ToReturn+=" Byte"; break;
        case 1: ToReturn+=" KiB"; break;
        case 2: ToReturn+=" MiB"; break;
        case 3: ToReturn+=" GiB"; break;
        case 4: ToReturn+=" TiB"; break;
    }

    return ToReturn;
}

//---------------------------------------------------------------------------
QString Human_Readable_Rate(QString Rate, QString Suffix)
{
    QString ToReturn;

    int64u IRate=Ztring().From_UTF8(Rate.toStdString()).To_int64u();

    if (IRate>1000)
        ToReturn=QString(Ztring().From_Number(IRate/1000.0, 3, Ztring_NoZero).To_UTF8().c_str())+" K"+Suffix;
    else
        ToReturn=Rate+" "+Suffix;

    return ToReturn;
}

//***************************************************************************
// Model
//***************************************************************************

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
PerFileModel::PerFileModel(GUI_Main* Main, Core* _C, QObject *parent)
: Main(Main), C(_C), Count(0), QAbstractListModel(parent)
{
    MenuHandler=new GUI_Main_xxxx_EditMenu(Main, _C);
    connect(MenuHandler, SIGNAL(valuesChanged(bool)), this, SLOT(onValuesChanged(bool)));
}

//---------------------------------------------------------------------------
PerFileModel::~PerFileModel()
{
    delete MenuHandler;
}

//***************************************************************************
// Functions
//***************************************************************************

//---------------------------------------------------------------------------
int PerFileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return Count;
}

//---------------------------------------------------------------------------
QHash<int, QByteArray> PerFileModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(FileRole, "file");
    roles.insert(TechRole, "tech");
    roles.insert(ExpandedRole, "expanded");
    roles.insert(ModifiedRole, "modified");
    roles.insert(EditModeRole, "editMode");

    return roles;
}

//---------------------------------------------------------------------------
QVariant PerFileModel::data(const QModelIndex &index, int role) const
{
    QVariant toReturn = QVariant();
    if (index.row()<0 || index.row()>=Count)
        return toReturn;


    switch (role)
    {
    case FileRole:
        toReturn=FileNames.at(index.row());
        break;
    case TechRole:
        toReturn=Technical_Info(FileNames.at(index.row()));
        break;
    case ExpandedRole:
        toReturn=Expanded.value(FileNames.at(index.row()), true);
        break;
    case ModifiedRole:
        toReturn=C->IsModified_Get(FileNames.at(index.row()).toStdString());
        break;
    case EditModeRole:
        toReturn=EditMode.value(FileNames.at(index.row()), false);
        break;
    }

    return toReturn;
}

//---------------------------------------------------------------------------
bool PerFileModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool toReturn=false;

    if (index.row()<0 || index.row()>=Count)
        return toReturn;

    switch (role)
    {
    case ExpandedRole:
        Expanded.insert(FileNames.at(index.row()), value.type()==QVariant::Type::Bool ? value.toBool() : false);
        Q_EMIT dataChanged(index, index, QVector<int>(1, role));
        toReturn=true;
        break;
    case EditModeRole:
        EditMode.insert(FileNames.at(index.row()), value.type()==QVariant::Type::Bool ? value.toBool() : true);
        Q_EMIT dataChanged(index, index);
        toReturn=true;
        break;
    }

    return toReturn;
}

//---------------------------------------------------------------------------
Q_INVOKABLE void PerFileModel::saveFile(const QString& FileName)
{
    Main->OnMenu_File_Save_File(FileName);
};

//---------------------------------------------------------------------------
Q_INVOKABLE void PerFileModel::closeFile(const QString& FileName)
{
    C->Menu_File_Close_File_FileName_Clear();
    C->Menu_File_Close_File_FileName_Set(FileName.toStdString());

    Main->OnMenu_File_Close_Files();
    Fill();
};


//---------------------------------------------------------------------------
Q_INVOKABLE bool PerFileModel::valid(const QString& FileName) const
{
    return C->IsValid_Get(FileName.toStdString());
}

//---------------------------------------------------------------------------
Q_INVOKABLE bool PerFileModel::valid(const QString& FileName, const QString& Field, const QString& Value) const
{
    return C->IsValid(FileName.toStdString(), Field.toStdString(), Value.toStdString());
}

//---------------------------------------------------------------------------
Q_INVOKABLE QString PerFileModel::lastValidationError(const QString& FileName) const
{
    return QString::fromUtf8(C->IsValid_LastError(FileName.toStdString()).c_str());
}

//---------------------------------------------------------------------------
Q_INVOKABLE QString PerFileModel::lastValidationWarning(const QString& FileName) const
{
    return QString::fromUtf8(C->IsValid_LastWarning(FileName.toStdString()).c_str());
}

//---------------------------------------------------------------------------
Q_INVOKABLE QString PerFileModel::errors(const QString& FileName) const
{
    return Get_Technical_Field(FileName, "Errors");
}

//---------------------------------------------------------------------------
Q_INVOKABLE QString PerFileModel::warnings(const QString& FileName) const
{
    return Get_Technical_Field(FileName, "Warnings");
}
//---------------------------------------------------------------------------
Q_INVOKABLE QString PerFileModel::informations(const QString& FileName) const
{
    return Get_Technical_Field(FileName, "Information");
}

//---------------------------------------------------------------------------
Q_INVOKABLE QString PerFileModel::unsupportedChunks(const QString& FileName) const
{
    return Get_Technical_Field(FileName, "UnsupportedChunks");
}

//---------------------------------------------------------------------------
Q_INVOKABLE QString PerFileModel::value(const QString& FileName, const QString& Field) const
{
    string ModifiedContent=C->Get(FileName.toStdString(), Field.toStdString());
    AdaptEOL(ModifiedContent, adapt_n);

    if ((Field=="MD5Generated" || Field=="MD5Stored") && Main->Preferences->Group_Option_Checked_Get(Group_MD5, Option_MD5_SwapEndian) && !ModifiedContent.empty())
        ModifiedContent=Swap_MD5_Endianess(QString::fromUtf8(ModifiedContent.c_str())).toStdString();

    return QString::fromUtf8(ModifiedContent.c_str());
}

//---------------------------------------------------------------------------
Q_INVOKABLE void PerFileModel::setValue(const QString& FileName, const QString& Field, const QString& Value)
{
    C->Set(FileName.toStdString(), Field.toStdString(), Value.toStdString());

    Q_EMIT dataChanged(index(FileNames.indexOf(FileName)), index(FileNames.indexOf(FileName)));

    Main->Menu_Update();
}

//---------------------------------------------------------------------------
Q_INVOKABLE bool PerFileModel::visible(const QString& FileName, const QString& Field) const {
    bool toReturn=true;

    if (Field=="MD5Generated" &&  !Main->Preferences->Group_Option_Checked_Get(Group_MD5, Option_MD5_Generate))
        toReturn=false;
   // if (Field=="MD5Stored" && (!C->Get(FileName.toStdString(), Field.toStdString()).empty() &&))
   //     toReturn=true;

    if (!EditMode.value(FileName, false) &&
        C->Get(FileName.toStdString(), Field.toStdString()).empty() &&
        C->IsValid(FileName.toStdString(), Field.toStdString(), "", true))
        toReturn=false;

    return toReturn;
}

//---------------------------------------------------------------------------
Q_INVOKABLE bool PerFileModel::modified(const QString& FileName, const QString& Field) const {
    return C->IsModified(FileName.toStdString(), Field.toStdString());
}

//---------------------------------------------------------------------------
Q_INVOKABLE bool PerFileModel::readOnly(const QString& FileName, const QString& Field) const {
    bool toReturn=false;

    if (Field=="MD5Generated")
        toReturn=true;
    if (Field=="MD5Stored" && (!C->Get(FileName.toStdString(), Field.toStdString()).empty() && !C->EmbedMD5_AuthorizeOverWritting))
        toReturn=true;

    if (!EditMode.value(FileName, false))
        toReturn=true;

    return toReturn;
};

//---------------------------------------------------------------------------
Q_INVOKABLE bool PerFileModel::readOnly(const QString& FileName) const {
    return C->IsReadOnly_Get(FileName.toStdString());
}

//---------------------------------------------------------------------------
Q_INVOKABLE bool PerFileModel::isWritable(const QString& FileName) const {
    return valid(FileName) && !readOnly(FileName);
}


//---------------------------------------------------------------------------
Q_INVOKABLE void PerFileModel::editField(const QString& FileName, const QString& Field)
{
    string ModifiedContent=C->Get(FileName.toStdString(), Field.toStdString());
    AdaptEOL(ModifiedContent, adapt_n);

    if (readOnly(FileName, Field)) {
        if ((Field=="MD5Generated" || Field=="MD5Stored") && Main->Preferences->Group_Option_Checked_Get(Group_MD5, Option_MD5_SwapEndian) && !ModifiedContent.empty())
            ModifiedContent=Swap_MD5_Endianess(QString::fromUtf8(ModifiedContent.c_str())).toStdString();

        QDialog* Edit=NULL;
        if (Field=="Cue")
            Edit=new GUI_Main_xxxx_CueDialog(C, FileName.toStdString(), true);
        else
            Edit=new GUI_Main_xxxx_TextEditDialog(C, FileName.toStdString(), Field.toStdString(), QString::fromUtf8(ModifiedContent.c_str()), true);
        Edit->exec();
        delete Edit; //Edit=NULL;
        return;
    }

    if (Field=="BextVersion")
    {
        if (Main->Bext_Toggle_Get())
        {
            int8u NewValue=Ztring().From_UTF8(C->Get(FileName.toStdString(), "BextVersion").c_str()).To_int8u();
            if (NewValue>=Main->Bext_MaxVersion_Get())
            {
                bool HasV1=C->Get(FileName.toStdString(), "LoudnessValue").empty() &&
                           C->Get(FileName.toStdString(), "LoudnessRange").empty() &&
                           C->Get(FileName.toStdString(), "MaxTruePeakLevel").empty() &&
                           C->Get(FileName.toStdString(), "MaxMomentaryLoudness").empty() &&
                           C->Get(FileName.toStdString(), "MaxShortTermLoudness").empty();
                bool HasV0=HasV1 && C->Get(FileName.toStdString(), "UMID").empty();
                if (HasV0)
                    NewValue=0;
                else if (HasV1)
                    NewValue=1;
                else
                    NewValue=2;
            }
            else
            {
                NewValue++;
            }

            //Filling
            C->Set(FileName.toStdString(), "BextVersion", Ztring::ToZtring(NewValue).To_UTF8());
        }
        else
        {
            GUI_Main_xxxx_Bext* Edit=new GUI_Main_xxxx_Bext(C, FileName.toStdString(), Main->Bext_MaxVersion_Get());
            Edit->exec();
            delete Edit; //Edit=NULL;
        }
    }
    else if (Field=="UMID")
    {
        GUI_Main_xxxx_UmidDialog* Edit=new GUI_Main_xxxx_UmidDialog(C, FileName.toStdString(), Field.toStdString(), QString::fromUtf8(ModifiedContent.c_str()));
        Edit->exec();
        delete Edit; //Edit=NULL;
    }
    else if (Field=="OriginationDate" || Field=="OriginationTime" || Field=="ICRD" || Field=="IDIT")
    {
        GUI_Main_xxxx_DateDialog* Edit=new GUI_Main_xxxx_DateDialog(C, FileName.toStdString(), Field.toStdString(), QString::fromUtf8(ModifiedContent.c_str()));
        Edit->exec();
        delete Edit; //Edit=NULL;
    }
    else if (Field=="CodingHistory")
    {
        GUI_Main_xxxx_CodingHistoryDialog* Edit=new GUI_Main_xxxx_CodingHistoryDialog(C, FileName.toStdString(), Field.toStdString(), QString::fromUtf8(ModifiedContent.c_str()), C->Rules);
        Edit->exec();
        delete Edit; //Edit=NULL;
    }
    else if (Field=="LoudnessValue" || Field=="LoudnessRange" || Field=="MaxTruePeakLevel" || Field=="MaxMomentaryLoudness" || Field=="MaxShortTermLoudness")
    {
        GUI_Main_xxxx_Loudness* Edit=new GUI_Main_xxxx_Loudness(C, FileName.toStdString(), Field.toStdString(), QString::fromUtf8(ModifiedContent.c_str()), C->Rules.Tech3285_Req);
        Edit->exec();
        delete Edit; //Edit=NULL;
    }
    else if (Field=="Cue") {
        GUI_Main_xxxx_CueDialog* Edit=new GUI_Main_xxxx_CueDialog(C, FileName.toStdString(), false);
        Edit->exec();
        delete Edit; //Edit=NULL;
    }
    else
    {
        GUI_Main_xxxx_TextEditDialog* Edit=new GUI_Main_xxxx_TextEditDialog(C, FileName.toStdString(), Field.toStdString(), QString::fromUtf8(ModifiedContent.c_str()), false);
        Edit->exec();
        delete Edit; //Edit=NULL;
    }

    Q_EMIT dataChanged(index(FileNames.indexOf(FileName)), index(FileNames.indexOf(FileName)));

    Main->Menu_Update();
}

Q_INVOKABLE void PerFileModel::showCoreMenu(const QPoint& globalPos, const QString& FileName, const QString& Field)
{
    MenuHandler->showContextMenu(globalPos);
}

//---------------------------------------------------------------------------
Q_INVOKABLE void PerFileModel::setSelected(const QString& FileName, const QString& Field)
{
    Selection=qMakePair(FileName, Field);

    C->Menu_File_Close_File_FileName_Clear();
    C->Menu_File_Close_File_FileName_Set(FileName.toStdString());

    QList<QPair<string, string> > Items;
    Items.append(qMakePair(FileName.toUtf8(), Field.toUtf8()));
    MenuHandler->updateEditMenu(Items);

    Main->Menu_Update();
}

//---------------------------------------------------------------------------
Q_INVOKABLE void PerFileModel::deselect(const QString& FileName, const QString& Field)
{
    Selection=qMakePair(QString(), QString());
    if (Selection==qMakePair(FileName, Field))
    {
        C->Menu_File_Close_File_FileName_Clear();
        QList<QPair<string, string> > Items;
        MenuHandler->updateEditMenu(Items);

        Main->Menu_Update();
    }
}

//---------------------------------------------------------------------------
void PerFileModel::onValuesChanged(bool onlySelected)
{
    if (onlySelected)
        Q_EMIT dataChanged(index(FileNames.indexOf(Selection.first)), index(FileNames.indexOf(Selection.first)));
    else
        Q_EMIT dataChanged(index(0), index(Count-1));

    Main->Menu_Update();
}


//---------------------------------------------------------------------------
void PerFileModel::Fill()
{
    //Showing
    ZtringListList List;
    List.Separator_Set(0, EOL);
    List.Separator_Set(1, __T(","));
    List.Write(Ztring().From_UTF8(C->Core_Get().c_str()));

    QHash<QString, bool> Expanded_Old=Expanded;
    QHash<QString, bool> EditMode_Old=EditMode;

    beginResetModel();

    TechnicalData.clear();
    FileNames.clear();
    Expanded.clear();
    EditMode.clear();
    Count=0;

    TechnicalData.Separator_Set(0, EOL);
    TechnicalData.Separator_Set(1, __T(","));
    TechnicalData.Write(Ztring().From_UTF8(C->Technical_Get().c_str()));

    if (List.empty() || List[0].empty())
    {
        endResetModel();
        return;
    }

    for (size_t Pos=1; Pos<List.size(); Pos++)
    {
        if (List[Pos][0].empty())
            continue;

        QString FileName = QString::fromUtf8(List[Pos][0].To_UTF8().c_str());
        FileNames.append(FileName);
        Expanded.insert(FileName, Expanded_Old.value(FileName, true));
        EditMode.insert(FileName, EditMode_Old.value(FileName, false));
    }

    C->Menu_File_Close_File_FileName_Clear();
    if (FileNames.size()==1)
         C->Menu_File_Close_File_FileName_Set(FileNames[0].toStdString());

    Count=FileNames.size()<FETCH_COUNT?FileNames.size():FETCH_COUNT;

    Main->Menu_Update();

    endResetModel();
}

//---------------------------------------------------------------------------
QString PerFileModel::Get_Technical_Field(const QString FileName, const QString FieldName) const
{
    QString toReturn;

    //Showing
    if (TechnicalData.empty() || TechnicalData[0].empty())
        return toReturn;

    size_t FileIndex=TechnicalData.Find(Ztring().From_UTF8(FileName.toStdString()), 0, 1);
    if (FileIndex>TechnicalData.size())
        return toReturn;

    size_t FieldIndex=TechnicalData[0].Find(Ztring().From_UTF8(FieldName.toStdString()));
    if (FieldIndex<TechnicalData[FileIndex].size())
        toReturn+=QString::fromUtf8(TechnicalData[FileIndex][FieldIndex].To_UTF8().c_str());

    return toReturn;
}

//---------------------------------------------------------------------------
QString PerFileModel::Technical_Info(const QString FileName) const
{
    QString toReturn;
    QString FileSize=Get_Technical_Field(FileName, "FileSize");
    QString Format=Get_Technical_Field(FileName, "Format");
    QString CodecID=Get_Technical_Field(FileName, "CodecID");
    QString Duration=Get_Technical_Field(FileName, "Duration");
    QString Channels=Get_Technical_Field(FileName, "Channels");
    QString SampleRate=Get_Technical_Field(FileName, "SampleRate");
    QString BitRate=Get_Technical_Field(FileName, "BitRate");
    QString BitPerSample=Get_Technical_Field(FileName, "BitPerSample");
    QString Encoding=Get_Technical_Field(FileName, "Encoding");

    if (!FileSize.isEmpty())
    {
        toReturn+=Human_Readable_Size(FileSize);
    }

    if (!Format.isEmpty())
    {
        toReturn+=toReturn.isEmpty()?"":" ";
        toReturn+=Format;
    }

    if (!CodecID.isEmpty())
    {
        toReturn+=toReturn.isEmpty()?"":" ";
        toReturn+=CodecID;
    }

    if (!Duration.isEmpty())
    {
        toReturn+=toReturn.isEmpty()?"":", ";
        toReturn+=Duration;
    }

    if (!Channels.isEmpty())
    {
        toReturn+=toReturn.isEmpty()?"":", ";
        toReturn+=Channels;
        toReturn+=" Channel(s)";
    }

    if (!toReturn.isEmpty() && (!SampleRate.isEmpty() || !BitRate.isEmpty() || !BitPerSample.isEmpty()))
        toReturn+=",";

    if (!SampleRate.isEmpty())
    {
        toReturn+=toReturn.isEmpty()?"":" ";
        toReturn+=Human_Readable_Rate(SampleRate, "Hz");
    }

    if (!BitRate.isEmpty())
    {
        toReturn+=toReturn.isEmpty()?"":" ";
        toReturn+=Human_Readable_Rate(BitRate, "bps");
    }

    if (!BitPerSample.isEmpty())
    {
        toReturn+=toReturn.isEmpty()?"":" ";
        toReturn+=BitPerSample;
        toReturn+=" bit";
    }

    if (!Encoding.isEmpty())
    {
        toReturn+=toReturn.isEmpty()?"":" ";
        toReturn+=Encoding;
    }

    return toReturn;
}

//---------------------------------------------------------------------------
bool PerFileModel::canFetchMore(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return (Count<FileNames.size());
}

//---------------------------------------------------------------------------
void PerFileModel::fetchMore(const QModelIndex &parent)
{
    Q_UNUSED(parent);

    int ToFetch=qMin(FETCH_COUNT, FileNames.size()-Count);
    if (ToFetch<=0)
        return;

    beginInsertRows(QModelIndex(), Count, Count+ToFetch-1);
    Count+=ToFetch;
    endInsertRows();
}

//***************************************************************************
// Widget
//***************************************************************************

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_PerFile::GUI_Main_PerFile(Core* _C, GUI_Main* parent)
: QQuickWidget(parent), C(_C), Main(parent)
{
    Model=new PerFileModel(Main, C, this);
    //Model->Set_Mode(PerFileModel::DisplayMode);

    rootContext()->setContextProperty("Model", Model);
    setResizeMode(QQuickWidget::SizeRootObjectToView);

    setSource(QUrl("qrc:/PerFile.qml"));
    setAcceptDrops(true);
}

//---------------------------------------------------------------------------
GUI_Main_PerFile::~GUI_Main_PerFile()
{
    delete Model;
}

//***************************************************************************
// Functions
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_PerFile::dragEnterEvent(QDragEnterEvent* event)
{
}

//---------------------------------------------------------------------------
bool GUI_Main_PerFile::event (QEvent* Event)
{
    //Should fill data
    if (Event->type()==QEvent::User || Event->type()==(QEvent::Type)(QEvent::User+1) || Event->type()==(QEvent::Type)(QEvent::User+2) || Event->type()==(QEvent::Type)(QEvent::User+3))
    {
        Model->Fill();

        setFocus();
        Event->accept();
        return true;
    }

    return QQuickWidget::event(Event);
}
