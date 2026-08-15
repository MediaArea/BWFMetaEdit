// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Main_xxxx_C2PADialog.h"
#include "GUI/Qt/GUI_Colors.h"
#include "Common/Core.h"
#include "ZenLib/Ztring.h"
#include "ZenLib/File.h"
#include "ThirdParty/json/json.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QFrame>
#include <QTabWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QApplication>
#include <QScreen>
#include <QList>
#include <cstdlib>
using namespace ZenLib;
using namespace std;
//---------------------------------------------------------------------------

//***************************************************************************
// JSON helpers
//***************************************************************************

//---------------------------------------------------------------------------
static json_value_s* Json_Object_Find(json_value_s* Value, const string &Key)
{
    if (!Value || Value->type!=json_type_object)
        return NULL;

    json_object_s* Object=json_value_as_object(Value);
    if (!Object)
        return NULL;

    for (json_object_element_s* Element=Object->start; Element; Element=Element->next)
        if (Element->name && Element->name->string && Key==Element->name->string)
            return Element->value;

    return NULL;
}

//---------------------------------------------------------------------------
static QString Json_String(json_value_s* Value)
{
    if (!Value)
        return QString();

    if (Value->type==json_type_string)
    {
        json_string_s* String=json_value_as_string(Value);
        if (String && String->string)
            return QString::fromUtf8(String->string, (int)String->string_size);
    }
    else if (Value->type==json_type_number)
    {
        json_number_s* Number=json_value_as_number(Value);
        if (Number && Number->number)
            return QString::fromUtf8(Number->number, (int)Number->number_size);
    }
    else if (Value->type==json_type_true)
        return "true";
    else if (Value->type==json_type_false)
        return "false";

    return QString();
}

//---------------------------------------------------------------------------
static QList<json_value_s*> Json_Members (json_value_s* Value)
{
    QList<json_value_s*> ToReturn;

    if (!Value)
        return ToReturn;

    if (Value->type==json_type_array)
    {
        json_array_s* Array=json_value_as_array(Value);
        if (Array)
            for (json_array_element_s* Element=Array->start; Element; Element=Element->next)
                ToReturn.append(Element->value);
    }
    else if (Value->type==json_type_object)
    {
        json_object_s* Object=json_value_as_object(Value);
        if (Object)
            for (json_object_element_s* Element=Object->start; Element; Element=Element->next)
                ToReturn.append(Element->value);
    }

    return ToReturn;
}

//---------------------------------------------------------------------------
static QString Json_CommonName (json_value_s* Value)
{
    QString CommonName=Json_String(Json_Object_Find(Value, "CN"));
    if (!CommonName.isEmpty())
        return CommonName;

    return Json_String(Json_Object_Find(Value, "O"));
}

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_xxxx_C2PADialog::GUI_Main_xxxx_C2PADialog(Core* _C, const std::string &FileName_, QWidget* parent, bool Writable_)
: QDialog(parent)
{
    //Internal
    C=_C;
    FileName=FileName_;
    Status=C->Get(FileName, "C2PA");
    Manifest=C->Get(FileName, "c2pajson");
    Writable=Writable_;
    //SignManifestPath is only known for the lifetime of the dialog instance that just imported a file (set in
    //OnMenu_Import); reopening the dialog on an already-staged file still detects the staged state (via
    //Update_PendingBanner querying Core directly) but can no longer show the original file name

    //Configuration
    setWindowFlags(windowFlags()&(~Qt::WindowContextHelpButtonHint));
    setWindowTitle("C2PA Content Credentials");
    setWindowIcon (QIcon(":/Image/Logo/Logo.png"));

    //Buttons
    Export=new QPushButton("&Export manifest...");
    Import=new QPushButton("&Import signing manifest...");
    ClearImport=new QPushButton("&Clear staged manifest");
    Dialog=new QDialogButtonBox(QDialogButtonBox::Close);
    if (!Manifest.empty())
        Dialog->addButton(Export, QDialogButtonBox::ResetRole);
    if (Writable)
    {
        Dialog->addButton(Import, QDialogButtonBox::ActionRole);
        bool HasSigningCredentials=!C->C2PA_SignCertificate.empty() && !C->C2PA_SignPrivateKey.empty();
        Import->setEnabled(HasSigningCredentials);
        if (!HasSigningCredentials)
            Import->setToolTip(tr("Configure a signing certificate and private key in Preferences → C2PA first."));
        Dialog->addButton(ClearImport, QDialogButtonBox::ActionRole);
    }
    connect(Dialog, SIGNAL(rejected()), this, SLOT(reject()));
    connect(Export, SIGNAL(clicked()), this, SLOT(OnMenu_Export()));
    connect(Import, SIGNAL(clicked()), this, SLOT(OnMenu_Import()));
    connect(ClearImport, SIGNAL(clicked()), this, SLOT(OnMenu_ClearImport()));

    //Central - Summary
    QScrollArea* Scroll=new QScrollArea(this);
    Scroll->setWidgetResizable(true);
    Scroll->setFrameShape(QFrame::NoFrame);
    Scroll->setWidget(Build_Summary());
    Update_PendingBanner();

    //Central - Raw JSON
    QTextEdit* RawText=new QTextEdit(this);
    RawText->setReadOnly(true);
    static const int MaxSize=0x100000;
    if (Manifest.empty())
        RawText->setPlainText(Status=="Absent"?tr("No C2PA manifest present in this file."):tr("A C2PA chunk is present, but its manifest could not be read."));
    else if (Manifest.size()<MaxSize)
        RawText->setPlainText(QString::fromUtf8(Manifest.c_str()));
    else
        RawText->setPlainText(QString::fromUtf8(Manifest.substr(0, MaxSize).c_str())+"\n...");

    Central=new QTabWidget(this);
    Central->addTab(Scroll,  tr("&Summary"));
    Central->addTab(RawText, tr("&Raw JSON"));

    QVBoxLayout* L=new QVBoxLayout();
    L->addWidget(Central);
    L->addWidget(Dialog);

    setLayout(L);

    QScreen* Screen=QApplication::screenAt(mapToGlobal(QPoint(0,0)));
    if (Screen)
        resize(Screen->availableGeometry().width()/2, Screen->availableGeometry().height()/2);
}

//***************************************************************************
// Menu actions
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_xxxx_C2PADialog::OnMenu_Export ()
{
    //User interaction
    QString FileNamesQ=QFileDialog::getSaveFileName(this, tr("Export C2PA manifest..."), QString::fromUtf8(C->OpenSaveFolder.c_str()), "JSON files (*.json);;All files (*.*)");

    if (FileNamesQ.isEmpty())
        return;

    File F;
    if (!F.Create(ZenLib::Ztring().From_UTF8(FileNamesQ.toUtf8().data())))
        return;

    F.Write((const int8u*)Manifest.c_str(), Manifest.size());
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_C2PADialog::OnMenu_Import ()
{
    //User interaction
    QString FileNameQ=QFileDialog::getOpenFileName(this, tr("Import C2PA signing manifest..."), QString::fromUtf8(C->OpenSaveFolder.c_str()), "JSON files (*.json);;All files (*.*)");

    if (FileNameQ.isEmpty())
        return;

    //The manifest is staged as text (read now), not as a path: the file may not be reachable anymore by the
    //time Save() actually signs (portal-style file access on Flatpak, macOS sandboxed bookmarks...)
    File F;
    string Content;
    bool Ok=F.Open(ZenLib::Ztring().From_UTF8(FileNameQ.toUtf8().data()));
    if (Ok)
    {
        int64u F_Size=F.Size_Get();
        if (F_Size && F_Size<=((size_t)-1)-1)
        {
            int8u* Buffer=new int8u[(size_t)F_Size];
            size_t Buffer_Offset=0;
            while (Buffer_Offset<F_Size)
            {
                size_t BytesRead=F.Read(Buffer+Buffer_Offset, (size_t)F_Size-Buffer_Offset);
                if (BytesRead==0)
                    break;
                Buffer_Offset+=BytesRead;
            }
            Ok=Buffer_Offset==F_Size;
            if (Ok)
                Content.assign((const char*)Buffer, Buffer_Offset);
            delete[] Buffer;
        }
        else
            Ok=false;
    }

    if (!Ok)
    {
        QMessageBox::warning(this, tr("Import C2PA signing manifest..."), tr("Unable to read the selected manifest file."));
        return;
    }

    C->Set(FileName, "C2PASignManifest", Content);
    SignManifestPath=FileNameQ.toStdString();
    Update_PendingBanner();
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_C2PADialog::OnMenu_ClearImport ()
{
    C->Set(FileName, "C2PASignManifest", string());
    SignManifestPath.clear();
    Update_PendingBanner();
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
QLabel* GUI_Main_xxxx_C2PADialog::Build_StatusBanner ()
{
    QString Text;
    QString Color;
    if (Status=="Absent")
    {
        Text=tr("No C2PA manifest found in this file.");
        Color=GUI_Colors::Neutral();
    }
    else if (Manifest.empty())
    {
        //The chunk is present but its content could not be extracted (eg the C2PA library is missing or failed)
        Text=QString(QChar(0x26A0))+" "+tr("A C2PA chunk is present, but its manifest could not be read.");
        Color=GUI_Colors::Warning();
    }
    else if (Status=="Valid")
    {
        Text=QString(QChar(0x2714))+" "+tr("Signature is valid");
        Color=GUI_Colors::Success();
    }
    else if (Status=="Invalid")
    {
        Text=QString(QChar(0x2716))+" "+tr("Signature is invalid");
        Color=GUI_Colors::Error();
    }
    else
    {
        Text=QString(QChar(0x2139))+" "+tr("C2PA manifest present (not verified)");
        Color=GUI_Colors::Info();
    }

    QLabel* Banner=new QLabel(Text);
    Banner->setStyleSheet("font-size: 13pt; font-weight: bold; color: "+Color+";");

    return Banner;
}

//---------------------------------------------------------------------------
QLabel* GUI_Main_xxxx_C2PADialog::Build_ValidationItem (json_value_s* Item, const QString &Color, const QString &Glyph)
{
    QString Code=Json_String(Json_Object_Find(Item, "code"));
    QString Explanation=Json_String(Json_Object_Find(Item, "explanation"));
    QString Url=Json_String(Json_Object_Find(Item, "url"));

    QString Text="<span style='color:"+Color+";'>"+Glyph+"</span> <b>"+Code.toHtmlEscaped()+"</b>";
    if (!Explanation.isEmpty())
        Text+=" &mdash; "+Explanation.toHtmlEscaped();

    QLabel* Label=new QLabel();
    Label->setTextFormat(Qt::RichText);
    Label->setWordWrap(true);
    Label->setText(Text);
    if (!Url.isEmpty())
        Label->setToolTip(Url);

    return Label;
}

//---------------------------------------------------------------------------
QWidget* GUI_Main_xxxx_C2PADialog::Build_ValidationResults (json_value_s* ValidationResults)
{
    QList<json_value_s*> Success=Json_Members(Json_Object_Find(ValidationResults, "success"));
    QList<json_value_s*> Informational=Json_Members(Json_Object_Find(ValidationResults, "informational"));
    QList<json_value_s*> Failure=Json_Members(Json_Object_Find(ValidationResults, "failure"));

    QGroupBox* Group=new QGroupBox(tr("Validation results"));
    QVBoxLayout* Layout=new QVBoxLayout(Group);

    QLabel* Summary=new QLabel(tr("%1 check(s) passed, %2 warning(s), %3 failure(s)").arg(Success.count()).arg(Informational.count()).arg(Failure.count()));
    Summary->setStyleSheet("color: "+(Failure.isEmpty()?GUI_Colors::Success():GUI_Colors::Error())+"; font-weight: bold;");
    Layout->addWidget(Summary);

    for (int Pos=0; Pos<Failure.count(); Pos++)
        Layout->addWidget(Build_ValidationItem(Failure[Pos], GUI_Colors::Error(), QString(QChar(0x2717))));

    for (int Pos=0; Pos<Informational.count(); Pos++)
        Layout->addWidget(Build_ValidationItem(Informational[Pos], GUI_Colors::Warning(), QString(QChar(0x2139))));

    QString SpecVersion=Json_String(Json_Object_Find(ValidationResults, "specVersion"));
    QString ValidationTime=Json_String(Json_Object_Find(ValidationResults, "validationTime"));
    if (!SpecVersion.isEmpty() || !ValidationTime.isEmpty())
    {
        QLabel* Footer=new QLabel(tr("C2PA spec %1, validated %2").arg(SpecVersion, ValidationTime));
        Footer->setStyleSheet("color: "+GUI_Colors::Neutral()+";");
        Layout->addWidget(Footer);
    }

    return Group;
}

//---------------------------------------------------------------------------
QWidget* GUI_Main_xxxx_C2PADialog::Build_Manifest (json_value_s* ManifestValue, int Index, int Count)
{
    QGroupBox* Group=new QGroupBox(Count>1?tr("Manifest %1 of %2").arg(Index).arg(Count):tr("Manifest"));
    QVBoxLayout* Layout=new QVBoxLayout(Group);

    json_value_s* Claim=Json_Object_Find(ManifestValue, "claim.v2");
    if (!Claim)
        Claim=Json_Object_Find(ManifestValue, "claim.v1");

    QFormLayout* Form=new QFormLayout();

    QString Title=Json_String(Json_Object_Find(Claim, "dc:title"));
    if (!Title.isEmpty())
        Form->addRow(tr("Title:"), new QLabel(Title));

    QString Label=Json_String(Json_Object_Find(ManifestValue, "label"));
    if (!Label.isEmpty())
    {
        QLabel* LabelWidget=new QLabel(Label);
        LabelWidget->setTextInteractionFlags(Qt::TextSelectableByMouse);
        Form->addRow(tr("Manifest ID:"), LabelWidget);
    }

    QString InstanceID=Json_String(Json_Object_Find(Claim, "instanceID"));
    if (!InstanceID.isEmpty())
    {
        QLabel* InstanceIDWidget=new QLabel(InstanceID);
        InstanceIDWidget->setTextInteractionFlags(Qt::TextSelectableByMouse);
        Form->addRow(tr("Instance ID:"), InstanceIDWidget);
    }

    json_value_s* GeneratorInfo=Json_Object_Find(Claim, "claim_generator_info");
    QString Generator=(Json_String(Json_Object_Find(GeneratorInfo, "name"))+" "+Json_String(Json_Object_Find(GeneratorInfo, "version"))).trimmed();
    if (!Generator.isEmpty())
        Form->addRow(tr("Claim generator:"), new QLabel(Generator));

    json_value_s* Assertions=Json_Object_Find(ManifestValue, "assertions");
    json_value_s* ActionsAssertion=Json_Object_Find(Assertions, "c2pa.actions.v2");
    if (!ActionsAssertion)
        ActionsAssertion=Json_Object_Find(Assertions, "c2pa.actions");
    QList<json_value_s*> Actions=Json_Members(Json_Object_Find(ActionsAssertion, "actions"));
    if (!Actions.isEmpty())
    {
        QStringList ActionNames;
        for (int Pos=0; Pos<Actions.count(); Pos++)
        {
            QString ActionName=Json_String(Json_Object_Find(Actions[Pos], "action"));
            if (!ActionName.isEmpty())
                ActionNames.append(ActionName);
        }
        if (!ActionNames.isEmpty())
            Form->addRow(tr("Actions:"), new QLabel(ActionNames.join(", ")));
    }

    json_value_s* Signature=Json_Object_Find(ManifestValue, "signature");
    QString Algorithm=Json_String(Json_Object_Find(Signature, "algorithm"));
    if (!Algorithm.isEmpty())
        Form->addRow(tr("Signature algorithm:"), new QLabel(Algorithm.toUpper()));

    json_value_s* CertificateInfo=Json_Object_Find(Signature, "certificateInfo");
    QString Subject=Json_CommonName(Json_Object_Find(CertificateInfo, "subject"));
    if (!Subject.isEmpty())
        Form->addRow(tr("Signed by:"), new QLabel(Subject));

    QString Issuer=Json_CommonName(Json_Object_Find(CertificateInfo, "issuer"));
    if (!Issuer.isEmpty())
        Form->addRow(tr("Issued by:"), new QLabel(Issuer));

    json_value_s* Validity=Json_Object_Find(CertificateInfo, "validity");
    QString NotBefore=Json_String(Json_Object_Find(Validity, "notBefore"));
    QString NotAfter=Json_String(Json_Object_Find(Validity, "notAfter"));
    if (!NotBefore.isEmpty() || !NotAfter.isEmpty())
        Form->addRow(tr("Certificate validity:"), new QLabel(NotBefore+" - "+NotAfter));

    Layout->addLayout(Form);

    json_value_s* ValidationResults=Json_Object_Find(ManifestValue, "validationResults");
    if (ValidationResults)
        Layout->addWidget(Build_ValidationResults(ValidationResults));

    return Group;
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_C2PADialog::Update_PendingBanner ()
{
    bool HasStagedManifest=!C->Get(FileName, "C2PASignManifest").empty();
    ClearImport->setEnabled(HasStagedManifest);

    if (!HasStagedManifest)
    {
        PendingBanner->hide();
        return;
    }

    if (SignManifestPath.empty())
        PendingBanner->setText(QString(QChar(0x270D))+" "+tr("A signing manifest is staged and will be applied when this file is saved."));
    else
    {
        QString Base=QFileInfo(QString::fromUtf8(SignManifestPath.c_str())).fileName();
        PendingBanner->setText(QString(QChar(0x270D))+" "+tr("A signing manifest is staged and will be applied when this file is saved: %1").arg(Base));
    }
    PendingBanner->show();
}

//---------------------------------------------------------------------------
QWidget* GUI_Main_xxxx_C2PADialog::Build_Summary ()
{
    QWidget* Widget=new QWidget();
    QVBoxLayout* Layout=new QVBoxLayout(Widget);
    Layout->setAlignment(Qt::AlignTop);

    Layout->addWidget(Build_StatusBanner());

    PendingBanner=new QLabel();
    PendingBanner->setWordWrap(true);
    PendingBanner->setStyleSheet("color: "+GUI_Colors::Notice()+"; font-weight: bold;");
    PendingBanner->hide();
    Layout->addWidget(PendingBanner);

    if (Manifest.empty())
    {
        if (Status!="Absent")
        {
            QLabel* Hint=new QLabel(tr("Verify that the C2PA library (libc2pa_c) is installed and reachable by this application."));
            Hint->setWordWrap(true);
            Hint->setStyleSheet("color: "+GUI_Colors::Neutral()+";");
            Layout->addWidget(Hint);
        }
        return Widget;
    }

    json_value_s* Root=json_parse(Manifest.data(), Manifest.size());
    if (!Root)
    {
        Layout->addWidget(new QLabel(tr("The C2PA manifest could not be parsed.")));
        return Widget;
    }

    QList<json_value_s*> Manifests=Json_Members(Json_Object_Find(Root, "manifests"));
    for (int Pos=0; Pos<Manifests.count(); Pos++)
        Layout->addWidget(Build_Manifest(Manifests[Pos], Pos+1, Manifests.count()));

    json_value_s* Generator=Json_Object_Find(Root, "jsonGenerator");
    QString GeneratorText=(Json_String(Json_Object_Find(Generator, "name"))+" "+Json_String(Json_Object_Find(Generator, "version"))).trimmed();
    if (!GeneratorText.isEmpty())
    {
        QLabel* Footer=new QLabel(tr("Generated by %1").arg(GeneratorText));
        Footer->setStyleSheet("color: "+GUI_Colors::Neutral()+"; font-style: italic;");
        Layout->addWidget(Footer);
    }

    free(Root);

    return Widget;
}
