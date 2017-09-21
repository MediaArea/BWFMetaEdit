// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_xxxx_UmidDialogH
#define GUI_Main_xxxx_UmidDialogH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class Core;
#include <QDialog>
#include "ZenLib/ZtringListList.h"
#include <QItemDelegate>
#include <QTableWidget>
class QEvent;
class QComboBox;
class QLineEdit;
class QTabWidget;
class QLabel;
class QDialogButtonBox;
class QSpinBox;
class QDateEdit;
class QTimeEdit;
class QDoubleSpinBox;
class QCheckBox;
using namespace ZenLib;

#include<sstream>
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_xxxx_UmidDialog
//***************************************************************************

class GUI_Main_xxxx_UmidDialog : public QDialog
{
    Q_OBJECT

public:
    //Constructor/Destructor
    GUI_Main_xxxx_UmidDialog(Core* C, const std::string &FileName_, const std::string &Field_, const QString &OldText, QWidget* parent=NULL);

    //Widgets
    QTabWidget*                         Central;
    QLabel*                             Label;
    QDialogButtonBox*                   Dialog;
    QPushButton*                        Load;
    QPushButton*                        Save;
    QLineEdit*                          Free_Basic;
    QLineEdit*                          Free_Signature;
    QComboBox*                          Basic_Ul_Umid;
    QComboBox*                          Basic_Ul_MaterialMethod;
    QComboBox*                          Basic_Ul_InstanceMethod;
    QLabel*                             Basic_Ul_Length;
    QSpinBox*                           Basic_Ul_Instance;
    QDateEdit*                          Basic_Material_TimeDate_Date;
    QTimeEdit*                          Basic_Material_TimeDate_Time;
    QDoubleSpinBox*                     Basic_Material_TimeDate_Samples;
    QComboBox*                          Basic_Material_TimeDate_Zone;
    QSpinBox*                           Basic_Material_Random;
    QDoubleSpinBox*                     Basic_Material_Machine;
    QCheckBox*                          Signature_Enabled;
    QDateEdit*                          Signature_TimeDate_Date;
    QTimeEdit*                          Signature_TimeDate_Time;
    QDoubleSpinBox*                     Signature_TimeDate_Samples;
    QComboBox*                          Signature_TimeDate_Zone;
    QDoubleSpinBox*                     Signature_Spatial_Altitude;
    QDoubleSpinBox*                     Signature_Spatial_Longitudinal;
    QComboBox*                          Signature_Spatial_Longitudinal_Orientation;
    QDoubleSpinBox*                     Signature_Spatial_Latitude;
    QComboBox*                          Signature_Spatial_Latitude_Orientation;
    QComboBox*                          Signature_Other_Country;
    QLineEdit*                          Signature_Other_Organization;
    QLineEdit*                          Signature_Other_User;

private Q_SLOTS:
    //Actions
    void OnAccept                   ();
    void OnTextChanged              (const QString &);
    void OnMenu_Load                ();
    void OnMenu_Save                ();
    void OnMenu_List                ();
    void OnMenu_Text                ();
    void OnCurrentChanged           (int Index);
    void OnSignature_Toggled        (bool Checked);
    void OnvalueChanged_BM_T        (const QTime &);
    void OnvalueChanged_BM_S        (double);
    void OnvalueChanged_S_T         (const QTime &);
    void OnvalueChanged_S_S         (double);

protected:
    //Events
    void hideEvent (QHideEvent* Event);
    
private:
    //Internal
    Core*   C;
    std::string FileName;
    std::string Field;
    bool    Updating;
    ZtringListList List;
    int16u  SampleRate;
	bool bSigUpdated;
    //Helpers
    void Fill();
    void List2Text();
    void Text2List();
    bool TryList(Ztring Value);
};

#endif
