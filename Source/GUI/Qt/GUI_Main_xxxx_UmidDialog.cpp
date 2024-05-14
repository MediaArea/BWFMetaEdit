// BWF MetaEdit GUI - A GUI for BWF MetaEdit
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "GUI/Qt/GUI_Main_xxxx_UmidDialog.h"
#include "Common/Core.h"
#include "ZenLib/Ztring.h"
#include "ZenLib/File.h"
#include <QLabel>
#include <QEvent>
#include <QFont>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QTableWidget>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QGroupBox>
#include <QCheckBox>
#include <QDateEdit>
#include <QTimeEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QValidator>
#include <QScrollArea>
#include <QMessageBox>
//---------------------------------------------------------------------------

//***************************************************************************
// Smpte330_SA_Widget
//***************************************************************************

class Smpte330_SA_Widget : public QScrollArea
{
public :
    Smpte330_SA_Widget(QWidget* parent) : QScrollArea(parent){}

    QWidget* Smpte330;

    QSize sizeHint () const    
    {
        QSize ToReturn;
        ToReturn.setWidth(Smpte330->frameGeometry().width()+2);
        ToReturn.setHeight(Smpte330->frameGeometry().height()+2);
        return ToReturn;
    }
};

//***************************************************************************
// Validator
//***************************************************************************

class Umid_AlphaNumeric_Validator : public QValidator
{
public :
    Umid_AlphaNumeric_Validator(QObject * parent) : QValidator(parent) {}
         
    State validate (QString &input, int &pos) const
    {
        for (pos=0; pos<input.size(); pos++)
            if (input[pos]<0x20 || input[pos]>0x7F)
                return Invalid;

        return Acceptable;
    }
};

void Zone_Fill(QComboBox* Box)
{
    Box->addItem("UTC");
    Box->addItem("UTC-00:30");
    Box->addItem("UTC-01:00");
    Box->addItem("UTC-01:30");
    Box->addItem("UTC-02:00");
    Box->addItem("UTC-02:30");
    Box->addItem("UTC-03:00");
    Box->addItem("UTC-03:30");
    Box->addItem("UTC-04:00");
    Box->addItem("UTC-04:30");
    Box->addItem("UTC-05:00");
    Box->addItem("UTC-05:30");
    Box->addItem("UTC-06:00");
    Box->addItem("UTC-06:30");
    Box->addItem("UTC-07:00");
    Box->addItem("UTC-07:30");
    Box->addItem("UTC-08:00");
    Box->addItem("UTC-08:30");
    Box->addItem("UTC-09:00");
    Box->addItem("UTC-09:30");
    Box->addItem("UTC-10:00");
    Box->addItem("UTC-10:30");
    Box->addItem("UTC-11:00");
    Box->addItem("UTC-11:30");
    Box->addItem("UTC-12:00");
    Box->addItem("UTC+13:00");
    Box->addItem("UTC+12:45");
    Box->addItem("UTC+12:00");
    Box->addItem("UTC+11:30");
    Box->addItem("UTC+11:00");
    Box->addItem("UTC+10:30");
    Box->addItem("UTC+10:00");
    Box->addItem("UTC+09:30");
    Box->addItem("UTC+09:00");
    Box->addItem("UTC+08:30");
    Box->addItem("UTC+08:00");
    Box->addItem("UTC+07:30");
    Box->addItem("UTC+07:00");
    Box->addItem("UTC+06:30");
    Box->addItem("UTC+06:00");
    Box->addItem("UTC+05:30");
    Box->addItem("UTC+05:00");
    Box->addItem("UTC+04:30");
    Box->addItem("UTC+04:00");
    Box->addItem("UTC+03:30");
    Box->addItem("UTC+03:00");
    Box->addItem("UTC+02:30");
    Box->addItem("UTC+02:00");
    Box->addItem("UTC+01:30");
    Box->addItem("UTC+01:00");
    Box->addItem("UTC+00:30");
    Box->addItem("TP-3");
    Box->addItem("TP-2");
    Box->addItem("TP-1");
    Box->addItem("TP-0");
    Box->addItem("User defined");
    Box->addItem("26 - Reserved");
    Box->addItem("27 - Reserved");
    Box->addItem("33 - Reserved");
    Box->addItem("34 - Reserved");
    Box->addItem("35 - Reserved");
    Box->addItem("36 - Reserved");
    Box->addItem("37 - Reserved");
    Box->addItem("39 - Reserved");
}

int Zone_IndexToInt[64]=
{
    0x00,
    0x0A,
    0x01,
    0x0B,
    0x02,
    0x0C,
    0x03,
    0x0D,
    0x04,
    0x0E,
    0x05,
    0x0F,
    0x06,
    0x1A,
    0x07,
    0x1B,
    0x08,
    0x1C,
    0x09,
    0x1D,
    0x10,
    0x1E,
    0x11,
    0x1F,
    0x12,
    0x13,
    0x32,
    0x14,
    0x2A,
    0x15,
    0x2B,
    0x16,
    0x2C,
    0x17,
    0x2D,
    0x18,
    0x2E,
    0x19,
    0x2F,
    0x20,
    0x3A,
    0x21,
    0x3B,
    0x22,
    0x3C,
    0x23,
    0x3D,
    0x24,
    0x3E,
    0x25,
    0x3F,
    0x28,
    0x29,
    0x30,
    0x31,
    0x38,
    0x26,
    0x27,
    0x33,
    0x34,
    0x35,
    0x36,
    0x37,
    0x39,
};

void Country_Fill(QComboBox* Box)
{
    Box->addItem("");
    Box->addItem("AFG - Afghanistan");
    Box->addItem("ALA - Aland Islands");
    Box->addItem("ALB - Albania");
    Box->addItem("DZA - Algeria");
    Box->addItem("ASM - American Samoa");
    Box->addItem("AND - Andorra");
    Box->addItem("AGO - Angola");
    Box->addItem("AIA - Anguilla");
    Box->addItem("ATA - Antarctica");
    Box->addItem("ATG - Antigua and Barbuda");
    Box->addItem("ARG - Argentina");
    Box->addItem("ARM - Armenia");
    Box->addItem("ABW - Aruba");
    Box->addItem("AUS - Australia");
    Box->addItem("AUT - Austria");
    Box->addItem("AZE - Azerbaijan");
    Box->addItem("BHS - Bahamas");
    Box->addItem("BHR - Bahrain");
    Box->addItem("BGD - Bangladesh");
    Box->addItem("BRB - Barbados");
    Box->addItem("BLR - Belarus");
    Box->addItem("BEL - Belgium");
    Box->addItem("BLZ - Belize");
    Box->addItem("BEN - Benin");
    Box->addItem("BMU - Bermuda");
    Box->addItem("BTN - Bhutan");
    Box->addItem("BOL - Bolivia, Plurinational State of");
    Box->addItem("BES - Bonaire, Sint Eustatius and Saba");
    Box->addItem("BIH - Bosnia and Herzegovina");
    Box->addItem("BWA - Botswana");
    Box->addItem("BVT - Bouvet Island");
    Box->addItem("BRA - Brazil");
    Box->addItem("IOT - British Indian Ocean Territory");
    Box->addItem("BRN - Brunei Darussalam");
    Box->addItem("BGR - Bulgaria");
    Box->addItem("BFA - Burkina Faso");
    Box->addItem("BDI - Burundi");
    Box->addItem("KHM - Cambodia");
    Box->addItem("CMR - Cameroon");
    Box->addItem("CAN - Canada");
    Box->addItem("CPV - Cape Verde");
    Box->addItem("CYM - Cayman Islands");
    Box->addItem("CAF - Central African Republic");
    Box->addItem("TCD - Chad");
    Box->addItem("CHL - Chile");
    Box->addItem("CHN - China");
    Box->addItem("CXR - Christmas Island");
    Box->addItem("CCK - Cocos (Keeling) Islands");
    Box->addItem("COL - Colombia");
    Box->addItem("COM - Comoros");
    Box->addItem("COG - Congo");
    Box->addItem("COD - Congo, the Democratic Republic of the");
    Box->addItem("COK - Cook Islands");
    Box->addItem("CRI - Costa Rica");
    Box->addItem("CIV - Côte d'Ivoire");
    Box->addItem("HRV - Croatia");
    Box->addItem("CUB - Cuba");
    Box->addItem("CUW - Curaçao");
    Box->addItem("CYP - Cyprus");
    Box->addItem("CZE - Czech Republic");
    Box->addItem("DNK - Denmark");
    Box->addItem("DJI - Djibouti");
    Box->addItem("DMA - Dominica");
    Box->addItem("DOM - Dominican Republic");
    Box->addItem("ECU - Ecuador");
    Box->addItem("EGY - Egypt");
    Box->addItem("SLV - El Salvador");
    Box->addItem("GNQ - Equatorial Guinea");
    Box->addItem("ERI - Eritrea");
    Box->addItem("EST - Estonia");
    Box->addItem("ETH - Ethiopia");
    Box->addItem("FLK - Falkland Islands (Malvinas)");
    Box->addItem("FRO - Faroe Islands");
    Box->addItem("FJI - Fiji");
    Box->addItem("FIN - Finland");
    Box->addItem("FRA - France");
    Box->addItem("GUF - French Guiana");
    Box->addItem("PYF - French Polynesia");
    Box->addItem("ATF - French Southern Territories");
    Box->addItem("GAB - Gabon");
    Box->addItem("GMB - Gambia");
    Box->addItem("GEO - Georgia");
    Box->addItem("DEU - Germany");
    Box->addItem("GHA - Ghana");
    Box->addItem("GIB - Gibraltar");
    Box->addItem("GRC - Greece");
    Box->addItem("GRL - Greenland");
    Box->addItem("GRD - Grenada");
    Box->addItem("GLP - Guadeloupe");
    Box->addItem("GUM - Guam");
    Box->addItem("GTM - Guatemala");
    Box->addItem("GGY - Guernsey");
    Box->addItem("GIN - Guinea");
    Box->addItem("GNB - Guinea-Bissau");
    Box->addItem("GUY - Guyana");
    Box->addItem("HTI - Haiti");
    Box->addItem("HMD - Heard Island and McDonald Islands");
    Box->addItem("VAT - Holy See (Vatican City State)");
    Box->addItem("HND - Honduras");
    Box->addItem("HKG - Hong Kong");
    Box->addItem("HUN - Hungary");
    Box->addItem("ISL - Iceland");
    Box->addItem("IND - India");
    Box->addItem("IDN - Indonesia");
    Box->addItem("IRN - Iran, Islamic Republic of");
    Box->addItem("IRQ - Iraq");
    Box->addItem("IRL - Ireland");
    Box->addItem("IMN - Isle of Man");
    Box->addItem("ISR - Israel");
    Box->addItem("ITA - Italy");
    Box->addItem("JAM - Jamaica");
    Box->addItem("JPN - Japan");
    Box->addItem("JEY - Jersey");
    Box->addItem("JOR - Jordan");
    Box->addItem("KAZ - Kazakhstan");
    Box->addItem("KEN - Kenya");
    Box->addItem("KIR - Kiribati");
    Box->addItem("PRK - Korea, Democratic People's Republic of");
    Box->addItem("KOR - Korea, Republic of");
    Box->addItem("KWT - Kuwait");
    Box->addItem("KGZ - Kyrgyzstan");
    Box->addItem("LAO - Lao People's Democratic Republic");
    Box->addItem("LVA - Latvia");
    Box->addItem("LBN - Lebanon");
    Box->addItem("LSO - Lesotho");
    Box->addItem("LBR - Liberia");
    Box->addItem("LBY - Libya");
    Box->addItem("LIE - Liechtenstein");
    Box->addItem("LTU - Lithuania");
    Box->addItem("LUX - Luxembourg");
    Box->addItem("MAC - Macao");
    Box->addItem("MKD - Macedonia, the former Yugoslav Republic of");
    Box->addItem("MDG - Madagascar");
    Box->addItem("MWI - Malawi");
    Box->addItem("MYS - Malaysia");
    Box->addItem("MDV - Maldives");
    Box->addItem("MLI - Mali");
    Box->addItem("MLT - Malta");
    Box->addItem("MHL - Marshall Islands");
    Box->addItem("MTQ - Martinique");
    Box->addItem("MRT - Mauritania");
    Box->addItem("MUS - Mauritius");
    Box->addItem("MYT - Mayotte");
    Box->addItem("MEX - Mexico");
    Box->addItem("FSM - Micronesia, Federated States of");
    Box->addItem("MDA - Moldova, Republic of");
    Box->addItem("MCO - Monaco");
    Box->addItem("MNG - Mongolia");
    Box->addItem("MNE - Montenegro");
    Box->addItem("MSR - Montserrat");
    Box->addItem("MAR - Morocco");
    Box->addItem("MOZ - Mozambique");
    Box->addItem("MMR - Myanmar");
    Box->addItem("NAM - Namibia");
    Box->addItem("NRU - Nauru");
    Box->addItem("NPL - Nepal");
    Box->addItem("NLD - Netherlands");
    Box->addItem("NCL - New Caledonia");
    Box->addItem("NZL - New Zealand");
    Box->addItem("NIC - Nicaragua");
    Box->addItem("NER - Niger");
    Box->addItem("NGA - Nigeria");
    Box->addItem("NIU - Niue");
    Box->addItem("NFK - Norfolk Island");
    Box->addItem("MNP - Northern Mariana Islands");
    Box->addItem("NOR - Norway");
    Box->addItem("OMN - Oman");
    Box->addItem("PAK - Pakistan");
    Box->addItem("PLW - Palau");
    Box->addItem("PSE - Palestinian Territory, Occupied");
    Box->addItem("PAN - Panama");
    Box->addItem("PNG - Papua New Guinea");
    Box->addItem("PRY - Paraguay");
    Box->addItem("PER - Peru");
    Box->addItem("PHL - Philippines");
    Box->addItem("PCN - Pitcairn");
    Box->addItem("POL - Poland");
    Box->addItem("PRT - Portugal");
    Box->addItem("PRI - Puerto Rico");
    Box->addItem("QAT - Qatar");
    Box->addItem("REU - Réunion");
    Box->addItem("ROU - Romania");
    Box->addItem("RUS - Russian Federation");
    Box->addItem("RWA - Rwanda");
    Box->addItem("BLM - Saint Barthélemy");
    Box->addItem("SHN - Saint Helena, Ascension and Tristan da Cunha");
    Box->addItem("KNA - Saint Kitts and Nevis");
    Box->addItem("LCA - Saint Lucia");
    Box->addItem("MAF - Saint Martin (French part)");
    Box->addItem("SPM - Saint Pierre and Miquelon");
    Box->addItem("VCT - Saint Vincent and the Grenadines");
    Box->addItem("WSM - Samoa");
    Box->addItem("SMR - San Marino");
    Box->addItem("STP - Sao Tome and Principe");
    Box->addItem("SAU - Saudi Arabia");
    Box->addItem("SEN - Senegal");
    Box->addItem("SRB - Serbia");
    Box->addItem("SYC - Seychelles");
    Box->addItem("SLE - Sierra Leone");
    Box->addItem("SGP - Singapore");
    Box->addItem("SXM - Sint Maarten (Dutch part)");
    Box->addItem("SVK - Slovakia");
    Box->addItem("SVN - Slovenia");
    Box->addItem("SLB - Solomon Islands");
    Box->addItem("SOM - Somalia");
    Box->addItem("ZAF - South Africa");
    Box->addItem("SGS - South Georgia and the South Sandwich Islands");
    Box->addItem("SSD - South Sudan");
    Box->addItem("ESP - Spain");
    Box->addItem("LKA - Sri Lanka");
    Box->addItem("SDN - Sudan");
    Box->addItem("SUR - Suriname");
    Box->addItem("SJM - Svalbard and Jan Mayen");
    Box->addItem("SWZ - Swaziland");
    Box->addItem("SWE - Sweden");
    Box->addItem("CHE - Switzerland");
    Box->addItem("SYR - Syrian Arab Republic");
    Box->addItem("TWN - Taiwan, Province of China");
    Box->addItem("TJK - Tajikistan");
    Box->addItem("TZA - Tanzania, United Republic of");
    Box->addItem("THA - Thailand");
    Box->addItem("TLS - Timor-Leste");
    Box->addItem("TGO - Togo");
    Box->addItem("TKL - Tokelau");
    Box->addItem("TON - Tonga");
    Box->addItem("TTO - Trinidad and Tobago");
    Box->addItem("TUN - Tunisia");
    Box->addItem("TUR - Turkey");
    Box->addItem("TKM - Turkmenistan");
    Box->addItem("TCA - Turks and Caicos Islands");
    Box->addItem("TUV - Tuvalu");
    Box->addItem("UGA - Uganda");
    Box->addItem("UKR - Ukraine");
    Box->addItem("ARE - United Arab Emirates");
    Box->addItem("GBR - United Kingdom");
    Box->addItem("USA - United States");
    Box->addItem("UMI - United States Minor Outlying Islands");
    Box->addItem("URY - Uruguay");
    Box->addItem("UZB - Uzbekistan");
    Box->addItem("VUT - Vanuatu");
    Box->addItem("VEN - Venezuela, Bolivarian Republic of");
    Box->addItem("VNM - Viet Nam");
    Box->addItem("VGB - Virgin Islands, British");
    Box->addItem("VIR - Virgin Islands, U.S.");
    Box->addItem("WLF - Wallis and Futuna");
    Box->addItem("ESH - Western Sahara");
    Box->addItem("YEM - Yemen");
    Box->addItem("ZMB - Zambia");
    Box->addItem("ZWE - Zimbabwe");

}

unsigned char CharToHexDigit(char c)
{
  if (c >= 'A')
    return c - 'A' + 10;
  else
    return c - '0';
}

char Pack(char c, bool b) {
  char res = c;
  if (b) {
    res |= 128;
  }
  return res;
}

char UnpackChar(char packed) {
  return packed & 127;
}

bool UnpackBool(char packed) {
  return ((packed & 128) > 0);
}

void Unpack(char packed, char &c, bool &b) {
  c = UnpackChar(packed);
  b = UnpackBool(packed);
}



std::string FloatToHexString(float d)
{
    unsigned char *buffer = (unsigned char*)&d;
    const int bufferSize = sizeof(float);

    char converted[bufferSize * 2 + 1];
	//char converted[bufferSize];

    int j = 0;
    for(int i = 0 ; i < bufferSize ; ++i)
    {
        sprintf(&converted[j*2], "%02X", buffer[i]);
		//sprintf(&converted[j], "%X", buffer[i]);
        ++j;
    }
    string hex_string(converted);
    return hex_string;
}



float HexStringToFloat(string &hexString)
{
    unsigned char byte_string[sizeof(float)];
    int number;

    int j = 0;

    for(int i = 0; i < hexString.size() ; i += 2)
    {
        sscanf(&hexString[i], "%02x", &number);
        byte_string[j] = (unsigned char)number;
        ++j;
    }

    float p = (float&)byte_string;
    return p;
}



char ToHex(int Value)
{
    if (Value<10)
        return '0'+(char)Value;
    if (Value<16)
        return 'A'+(char)(Value-10);
    return ' ';
}

int32u FromHex(const string &Value, size_t Pos, size_t Size)
{
    int Temp=0;
    for (size_t Offset=0; Offset<Size; Offset++)
        Temp|=(Value[Pos+Offset]-(Value[Pos+Offset]<='9'?'0':('A'-10)))<<(4*(Size-1-Offset));
    return Temp;
}

int32u FromBCD(const string &Value, size_t Pos, size_t Size, bool &Fail)
{
    if (Fail)
        return 0;
    
    int Temp=0;
    for (size_t Offset=0; Offset<Size; Offset++)
    {
        if (!(Value[Pos+Offset]<='9'))
        {
            Fail=true;
            return 0;
        }
        Temp|=(Value[Pos+Offset]-'0')<<(4*(Size-1-Offset));
    }
    return Temp;
}

qint64 FromHex64(const string &Value, size_t Pos, size_t Size)
{
    qint64 Temp=0;
    for (size_t Offset=0; Offset<Size; Offset++)
        Temp|=((qint64)(Value[Pos+Offset]-(Value[Pos+Offset]<='9'?'0':('A'-10))))<<(4*(Size-1-Offset));
    return Temp;
}

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_xxxx_UmidDialog::GUI_Main_xxxx_UmidDialog(Core* C_, const std::string &FileName_, const std::string &Field_, const QString &OldText, QWidget* parent)
: QDialog(parent)
{
    //Internal
    C=C_;
    FileName=FileName_;
    Field=Field_;
    Updating=false;
	bSigUpdated=true;

    SampleRate=Ztring().From_UTF8(C->Get(FileName, "SampleRate")).To_int16u();
    while (((qint64)SampleRate)*24*60*60>=0x100000000LL)
        SampleRate/=2;

    //Configuration
    setWindowFlags(windowFlags()&(0xFFFFFFFF-Qt::WindowContextHelpButtonHint));
    setWindowTitle("Unique Material IDentifier");
    setWindowIcon (QIcon(":/Image/Logo/Logo.png"));

    //Buttons
    Load=new QPushButton("&Load file...");
    Save=new QPushButton("&Save file...");
    Dialog=new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    Dialog->addButton(Load, QDialogButtonBox::ResetRole);
    Dialog->addButton(Save, QDialogButtonBox::ResetRole);
    connect(Dialog, SIGNAL(accepted()), this, SLOT(OnAccept()));
    connect(Dialog, SIGNAL(rejected()), this, SLOT(reject()));
    connect(Load, SIGNAL(clicked()), this, SLOT(OnMenu_Load()));
    connect(Save, SIGNAL(clicked()), this, SLOT(OnMenu_Save()));

    //Basic_UL
    QLabel* Basic_Ul_Begin_Label=new QLabel(tr("Fixed part"), this);
    QLabel* Basic_Ul_Begin=new QLabel(tr("06 0A 2B 34 01 01 01 01 01 01 "), this);
    QLabel* Basic_Ul_Umid_Label=new QLabel(tr("Type"), this);
    Basic_Ul_Umid=new QComboBox(this);
    Basic_Ul_Umid->addItem("0 - Reserved");
    Basic_Ul_Umid->addItem("Picture");
    Basic_Ul_Umid->addItem("Audio");
    Basic_Ul_Umid->addItem("Data");
    Basic_Ul_Umid->addItem("Other");
    Basic_Ul_Umid->addItem("5 - Reserved");
    Basic_Ul_Umid->addItem("6 - Reserved");
    Basic_Ul_Umid->addItem("7 - Reserved");
    Basic_Ul_Umid->addItem("8 - Reserved");
    Basic_Ul_Umid->addItem("9 - Reserved");
    Basic_Ul_Umid->addItem("A - Reserved");
    Basic_Ul_Umid->addItem("B - Reserved");
    Basic_Ul_Umid->addItem("C - Reserved");
    Basic_Ul_Umid->addItem("D - Reserved");
    Basic_Ul_Umid->addItem("E - Reserved");
    Basic_Ul_Umid->addItem("F - Reserved");
    Basic_Ul_Umid->setCurrentIndex(2);
    Basic_Ul_Umid->setEnabled(false);
    QLabel* Basic_Ul_MaterialMethod_Label=new QLabel(tr("Material Number Generation Method"), this);
    Basic_Ul_MaterialMethod=new QComboBox(this);
    Basic_Ul_MaterialMethod->addItem("No defined method");
    Basic_Ul_MaterialMethod->addItem("SMPTE method");
    Basic_Ul_MaterialMethod->addItem("As per ISO/IEC 11578 Annex A");
    Basic_Ul_MaterialMethod->addItem("3 - Reserved");
    Basic_Ul_MaterialMethod->addItem("4 - Reserved");
    Basic_Ul_MaterialMethod->addItem("5 - Reserved");
    Basic_Ul_MaterialMethod->addItem("6 - Reserved");
    Basic_Ul_MaterialMethod->addItem("7 - Reserved");
    Basic_Ul_MaterialMethod->addItem("8 - Reserved");
    Basic_Ul_MaterialMethod->addItem("9 - Reserved");
    Basic_Ul_MaterialMethod->addItem("A - Reserved");
    Basic_Ul_MaterialMethod->addItem("B - Reserved");
    Basic_Ul_MaterialMethod->addItem("C - Reserved");
    Basic_Ul_MaterialMethod->addItem("D - Reserved");
    Basic_Ul_MaterialMethod->addItem("E - Reserved");
    Basic_Ul_MaterialMethod->addItem("F - Reserved");
    Basic_Ul_MaterialMethod->setCurrentIndex(1);
    Basic_Ul_MaterialMethod->setEnabled(false);
    QLabel* Basic_Ul_InstanceMethod_Label=new QLabel(tr("Instance Generation Method"), this);
    Basic_Ul_InstanceMethod=new QComboBox(this);
    Basic_Ul_InstanceMethod->addItem("No defined method");
    Basic_Ul_InstanceMethod->addItem("Local registration");
    Basic_Ul_InstanceMethod->addItem("24-bit seeded PRS generator");
    Basic_Ul_InstanceMethod->addItem("3 - Reserved");
    Basic_Ul_InstanceMethod->addItem("4 - Reserved");
    Basic_Ul_InstanceMethod->addItem("5 - Reserved");
    Basic_Ul_InstanceMethod->addItem("6 - Reserved");
    Basic_Ul_InstanceMethod->addItem("7 - Reserved");
    Basic_Ul_InstanceMethod->addItem("8 - Reserved");
    Basic_Ul_InstanceMethod->addItem("9 - Reserved");
    Basic_Ul_InstanceMethod->addItem("A - Reserved");
    Basic_Ul_InstanceMethod->addItem("B - Reserved");
    Basic_Ul_InstanceMethod->addItem("C - Reserved");
    Basic_Ul_InstanceMethod->addItem("D - Reserved");
    Basic_Ul_InstanceMethod->addItem("E - Reserved");
    Basic_Ul_InstanceMethod->addItem("F - Reserved");
    QGridLayout *Basic_Ul_Layout=new QGridLayout;
    Basic_Ul_Layout->addWidget(Basic_Ul_Begin_Label, 0, 0, 1, 1);
    Basic_Ul_Layout->addWidget(Basic_Ul_Begin, 1, 0, 1, 1);
    Basic_Ul_Layout->addWidget(Basic_Ul_Umid_Label, 0, 1, 1, 1);
    Basic_Ul_Layout->addWidget(Basic_Ul_Umid, 1, 1, 1, 1);
    Basic_Ul_Layout->addWidget(Basic_Ul_MaterialMethod_Label, 0, 2, 1, 1);
    Basic_Ul_Layout->addWidget(Basic_Ul_MaterialMethod, 1, 2, 1, 1);
    Basic_Ul_Layout->addWidget(Basic_Ul_InstanceMethod_Label, 0, 3, 1, 1);
    Basic_Ul_Layout->addWidget(Basic_Ul_InstanceMethod, 1, 3, 1, 1);
    Basic_Ul_Layout->setColumnStretch(4, 1);
    QGroupBox *Basic_Ul=new QGroupBox(tr("Universal Label"));
    Basic_Ul->setLayout(Basic_Ul_Layout);

    //Basic_Other
    QLabel* Basic_Ul_Length_Label=new QLabel(tr("Length"), this);
    Basic_Ul_Length=new QLabel(tr("13"), this);
    QLabel* Basic_Ul_Instance_Label=new QLabel(tr("Instance"), this);
    Basic_Ul_Instance=new QSpinBox(this);
    Basic_Ul_Instance->setMaximum(16777215); //3-byte
    QGridLayout *Basic_Other_Layout=new QGridLayout;
    Basic_Other_Layout->addWidget(Basic_Ul_Length_Label, 0, 0, 1, 1);
    Basic_Other_Layout->addWidget(Basic_Ul_Length, 1, 0, 1, 1);
    Basic_Other_Layout->addWidget(Basic_Ul_Instance_Label, 0, 1, 1, 1);
    Basic_Other_Layout->addWidget(Basic_Ul_Instance, 1, 1, 1, 1);
    Basic_Other_Layout->setColumnStretch(2, 1);
    QGroupBox *Basic_Other=new QGroupBox(tr("Length and Instance"));
    Basic_Other->setLayout(Basic_Other_Layout);

    //Basic_Material
    QLabel* Basic_Material_TimeDate_Date_Label=new QLabel(tr("Time Snap: Date,"), this);
    Basic_Material_TimeDate_Date=new QDateEdit(this);
    Basic_Material_TimeDate_Date->setDisplayFormat("yyyy-MM-dd");
    Basic_Material_TimeDate_Date->setDate(QDate::fromJulianDay(2400001));
    Basic_Material_TimeDate_Date->setMinimumDate(QDate::fromJulianDay(2400001));
    QLabel* Basic_Material_TimeDate_Time_Label=new QLabel(tr("Time HH:MM:SS.mmm (UTC),"), this);
    Basic_Material_TimeDate_Time=new QTimeEdit(this);
    Basic_Material_TimeDate_Time->setDisplayFormat("hh:mm:ss.zzz");
    connect(Basic_Material_TimeDate_Time, SIGNAL(timeChanged(const QTime &)), this, SLOT(OnvalueChanged_BM_T(const QTime &)));
    QLabel* Basic_Material_TimeDate_Samples_Label=new QLabel(tr(("Time "+Ztring::ToZtring(SampleRate).To_UTF8()+" Hz (UTC),").c_str()), this);
    Basic_Material_TimeDate_Samples=new QDoubleSpinBox(this);
    Basic_Material_TimeDate_Samples->setDecimals(0);
    Basic_Material_TimeDate_Samples->setMaximum((double)(((qint64)SampleRate)*24*60*60-1));
    connect(Basic_Material_TimeDate_Samples, SIGNAL(valueChanged(double)), this, SLOT(OnvalueChanged_BM_S(double)));
    QLabel* Basic_Material_TimeDate_Zone_Label=new QLabel(tr("Time zone (informational)."), this);
    Basic_Material_TimeDate_Zone=new QComboBox(this);
    Zone_Fill(Basic_Material_TimeDate_Zone);
    QLabel* Basic_Material_Random_Label=new QLabel(tr("Random"), this);
    Basic_Material_Random=new QSpinBox(this);
    Basic_Material_Random->setMaximum(65535);
    QLabel* Basic_Material_Machine_Label=new QLabel(tr("Machine Node"), this);
    Basic_Material_Machine=new QDoubleSpinBox(this);
    Basic_Material_Machine->setDecimals(0);
    Basic_Material_Machine->setMaximum(281474976710655.0);
    QGridLayout *Basic_Material_Layout=new QGridLayout;
    Basic_Material_Layout->addWidget(Basic_Material_TimeDate_Date_Label, 0, 0, 1, 1);
    Basic_Material_Layout->addWidget(Basic_Material_TimeDate_Date, 1, 0, 1, 1);
    Basic_Material_Layout->addWidget(Basic_Material_TimeDate_Time_Label, 0, 1, 1, 1);
    Basic_Material_Layout->addWidget(Basic_Material_TimeDate_Time, 1, 1, 1, 1);
    Basic_Material_Layout->addWidget(Basic_Material_TimeDate_Samples_Label, 0, 2, 1, 1);
    Basic_Material_Layout->addWidget(Basic_Material_TimeDate_Samples, 1, 2, 1, 1);
    Basic_Material_Layout->addWidget(Basic_Material_TimeDate_Zone_Label, 0, 3, 1, 1);
    Basic_Material_Layout->addWidget(Basic_Material_TimeDate_Zone, 1, 3, 1, 1);
    Basic_Material_Layout->addWidget(Basic_Material_Random_Label, 0, 4, 1, 1);
    Basic_Material_Layout->addWidget(Basic_Material_Random, 1, 4, 1, 1);
    Basic_Material_Layout->addWidget(Basic_Material_Machine_Label, 0, 5, 1, 1);
    Basic_Material_Layout->addWidget(Basic_Material_Machine, 1, 5, 1, 1);
    Basic_Material_Layout->setColumnStretch(6, 1);
    QGroupBox *Basic_Material=new QGroupBox(tr("Material Number"));
    Basic_Material->setLayout(Basic_Material_Layout);

    //Signature_TimeDate
    QLabel* Signature_TimeDate_Date_Label=new QLabel(tr("Date"), this);
    Signature_TimeDate_Date=new QDateEdit(this);
    Signature_TimeDate_Date->setDisplayFormat("yyyy-MM-dd");
    Signature_TimeDate_Date->setDate(QDate::fromJulianDay(2400001));
    Signature_TimeDate_Date->setMinimumDate(QDate::fromJulianDay(2400001));
    QLabel* Signature_TimeDate_Time_Label=new QLabel(tr("Time (UTC)"), this);
    Signature_TimeDate_Time=new QTimeEdit(this);
    Signature_TimeDate_Time->setDisplayFormat("hh:mm:ss.zzz");
    connect(Signature_TimeDate_Time, SIGNAL(timeChanged(const QTime &)), this, SLOT(OnvalueChanged_S_T(const QTime &)));
    QLabel* Signature_TimeDate_Samples_Label=new QLabel(tr(("Time "+Ztring::ToZtring(SampleRate).To_UTF8()+" Hz (UTC)").c_str()), this);
    Signature_TimeDate_Samples=new QDoubleSpinBox(this);
    Signature_TimeDate_Samples->setDecimals(0);
    Signature_TimeDate_Samples->setMaximum((double)(((qint64)SampleRate)*24*60*60-1));
    connect(Signature_TimeDate_Samples, SIGNAL(valueChanged(double)), this, SLOT(OnvalueChanged_S_S(double)));
    QLabel* Signature_TimeDate_Zone_Label=new QLabel(tr("Time zone (informational)"), this);
    Signature_TimeDate_Zone=new QComboBox(this);
    Zone_Fill(Signature_TimeDate_Zone);
    QGridLayout *Signature_TimeDate_Layout=new QGridLayout;
    Signature_TimeDate_Layout->addWidget(Signature_TimeDate_Date_Label, 0, 0, 1, 1);
    Signature_TimeDate_Layout->addWidget(Signature_TimeDate_Date, 1, 0, 1, 1);
    Signature_TimeDate_Layout->addWidget(Signature_TimeDate_Time_Label, 0, 1, 1, 1);
    Signature_TimeDate_Layout->addWidget(Signature_TimeDate_Time, 1, 1, 1, 1);
    Signature_TimeDate_Layout->addWidget(Signature_TimeDate_Samples_Label, 0, 2, 1, 1);
    Signature_TimeDate_Layout->addWidget(Signature_TimeDate_Samples, 1, 2, 1, 1);
    Signature_TimeDate_Layout->addWidget(Signature_TimeDate_Zone_Label, 0, 3, 1, 1);
    Signature_TimeDate_Layout->addWidget(Signature_TimeDate_Zone, 1, 3, 1, 1);
    Signature_TimeDate_Layout->setColumnStretch(4, 1);
    QGroupBox *Signature_TimeDate=new QGroupBox(tr("Time/Date"));
    Signature_TimeDate->setLayout(Signature_TimeDate_Layout);

    //Signature_Spatial
    QLabel* Signature_Spatial_Altitude_Label=new QLabel(tr("Altitude (Center of Earth)"), this);
    Signature_Spatial_Altitude=new QDoubleSpinBox(this);
    Signature_Spatial_Altitude->setDecimals(0);
    Signature_Spatial_Altitude->setMaximum(99999999);
	QLabel* Signature_Spatial_Longitudinal_Label=new QLabel(tr("Longitudinal"), this);
    Signature_Spatial_Longitudinal=new QDoubleSpinBox(this);
    Signature_Spatial_Longitudinal->setDecimals(5);
    Signature_Spatial_Longitudinal->setMinimum(-180);
    Signature_Spatial_Longitudinal->setMaximum(180);
    Signature_Spatial_Longitudinal_Orientation=new QComboBox(this);
    Signature_Spatial_Longitudinal_Orientation->addItem("West");
    Signature_Spatial_Longitudinal_Orientation->addItem("East");
    QLabel* Signature_Spatial_Latitude_Label=new QLabel(tr("Latitude"), this);
    Signature_Spatial_Latitude=new QDoubleSpinBox(this);
    Signature_Spatial_Latitude->setDecimals(5);
    Signature_Spatial_Latitude->setMinimum(-90);
    Signature_Spatial_Latitude->setMaximum(90);
    Signature_Spatial_Latitude_Orientation=new QComboBox(this);
    Signature_Spatial_Latitude_Orientation->addItem("North");
    Signature_Spatial_Latitude_Orientation->addItem("South");
    QGridLayout *Signature_Spatial_Layout=new QGridLayout;
    Signature_Spatial_Layout->addWidget(Signature_Spatial_Altitude_Label, 0, 0, 1, 1);
    Signature_Spatial_Layout->addWidget(Signature_Spatial_Altitude, 1, 0, 1, 1);
    Signature_Spatial_Layout->addWidget(Signature_Spatial_Longitudinal_Label, 0, 1, 1, 2);
    Signature_Spatial_Layout->addWidget(Signature_Spatial_Longitudinal, 1, 1, 1, 1);
    Signature_Spatial_Layout->addWidget(Signature_Spatial_Longitudinal_Orientation, 1, 2, 1, 1);
    Signature_Spatial_Layout->addWidget(Signature_Spatial_Latitude_Label, 0, 3, 1, 1);
    Signature_Spatial_Layout->addWidget(Signature_Spatial_Latitude, 1, 3, 1, 1);
    Signature_Spatial_Layout->addWidget(Signature_Spatial_Latitude_Orientation, 1, 4, 1, 1);
    Signature_Spatial_Layout->setColumnStretch(5, 1);
    QGroupBox *Signature_Spatial=new QGroupBox(tr("Spatial coordinates"));
    Signature_Spatial->setLayout(Signature_Spatial_Layout);

    //Signature_Other
    QLabel* Signature_Other_Country_Label=new QLabel(tr("Country"), this);
    Signature_Other_Country=new QComboBox(this);
    Signature_Other_Country->setEditable(true);
    QLineEdit* Signature_Other_Country_Edit=new QLineEdit(this); 
    Signature_Other_Country_Edit->setMaxLength(4);
    Signature_Other_Country_Edit->setValidator(new Umid_AlphaNumeric_Validator(this));
    Signature_Other_Country->setLineEdit(Signature_Other_Country_Edit);
    Country_Fill(Signature_Other_Country);
    QLabel* Signature_Other_Organization_Label=new QLabel(tr("Organization"), this);
    Signature_Other_Organization=new QLineEdit(tr(""));
    Signature_Other_Organization->setMaxLength(4);
    Signature_Other_Organization->setValidator(new Umid_AlphaNumeric_Validator(this));
    QLabel* Signature_Other_User_Label=new QLabel(tr("User"), this);
    Signature_Other_User=new QLineEdit(tr(""));
    Signature_Other_User->setMaxLength(4);
    Signature_Other_User->setValidator(new Umid_AlphaNumeric_Validator(this));
    QGridLayout *Signature_Other_Layout=new QGridLayout;
    Signature_Other_Layout->addWidget(Signature_Other_Country_Label, 0, 0, 1, 1);
    Signature_Other_Layout->addWidget(Signature_Other_Country, 1, 0, 1, 1);
    Signature_Other_Layout->addWidget(Signature_Other_Organization_Label, 0, 1, 1, 1);
    Signature_Other_Layout->addWidget(Signature_Other_Organization, 1, 1, 1, 1);
    Signature_Other_Layout->addWidget(Signature_Other_User_Label, 0, 2, 1, 1);
    Signature_Other_Layout->addWidget(Signature_Other_User, 1, 2, 1, 1);
    Signature_Other_Layout->setColumnStretch(3, 1);
    QGroupBox *Signature_Other=new QGroupBox(tr("Country / Organization / User"));
    Signature_Other->setLayout(Signature_Other_Layout);

    //All together - Basic
    QGroupBox *Basic=new QGroupBox(tr("Basic UMID"));
    QVBoxLayout *Basic_Layout=new QVBoxLayout;
    Basic_Layout->addWidget(Basic_Ul);
    Basic_Layout->addWidget(Basic_Other);
    Basic_Layout->addWidget(Basic_Material);
    Basic->setLayout(Basic_Layout);

    //All together - Signature
    Signature_Enabled=new QCheckBox(tr("With signature")); Signature_Enabled->setChecked(false);
    connect(Signature_Enabled, SIGNAL(toggled(bool)), this, SLOT(OnSignature_Toggled(bool)));
    QGroupBox *Signature=new QGroupBox(tr("Signature"));
    QVBoxLayout *Signature_Layout=new QVBoxLayout;
    Signature_Layout->addWidget(Signature_Enabled);
    Signature_Layout->addWidget(Signature_TimeDate);
    Signature_Layout->addWidget(Signature_Spatial);
    Signature_Layout->addWidget(Signature_Other);
    Signature->setLayout(Signature_Layout);

    //All together
    Smpte330_SA_Widget* Smpte330_SA=new Smpte330_SA_Widget(this);
    Smpte330_SA->Smpte330=new QWidget(this);
    Smpte330_SA->setWidget(Smpte330_SA->Smpte330); Smpte330_SA->setWidgetResizable(true);
    QVBoxLayout *UmidLayout=new QVBoxLayout;
    UmidLayout->addWidget(Basic);
    UmidLayout->addWidget(Signature);
    UmidLayout->addWidget(new QWidget);
    #if (QT_VERSION >= 0x040500)
        UmidLayout->setStretch(2, 1);
    #endif //(QT_VERSION >= 0x040500)
    Smpte330_SA->Smpte330->setLayout(UmidLayout);

    //Free Text
    QLabel* Free_Basic_Label=new QLabel(tr("Basic UMID"), this);
    Free_Basic=new QLineEdit(this);
    Free_Basic->setMaxLength(64);
    connect(Free_Basic, SIGNAL(textChanged(const QString &)), this, SLOT(OnTextChanged(const QString &)));
    QLabel *Free_Signature_Enabled=new QLabel(tr("Signature"));
    Free_Signature=new QLineEdit(this);
    Free_Signature->setMaxLength(64);
    connect(Free_Signature, SIGNAL(textChanged(const QString &)), this, SLOT(OnTextChanged(const QString &)));
    QGridLayout *FreeText_Layout=new QGridLayout;
    FreeText_Layout->addWidget(Free_Basic_Label, 0, 0);
    FreeText_Layout->addWidget(Free_Basic, 1, 0);
    FreeText_Layout->addWidget(Free_Signature_Enabled, 2, 0);
    FreeText_Layout->addWidget(Free_Signature, 3, 0);
    FreeText_Layout->setRowStretch(4, 1);
    QWidget* FreeText=new QWidget(this);
    FreeText->setLayout(FreeText_Layout);


        
    //Central
    Central=new QTabWidget(this);
    Central->addTab(Smpte330_SA, tr("S&MPTE 330M-2000"));
    Central->addTab(FreeText, tr("&Free text"));
    connect(Central, SIGNAL(currentChanged (int)), this, SLOT(OnCurrentChanged(int)));
        
    Label=new QLabel(this);
    QVBoxLayout* L=new QVBoxLayout();
    L->addWidget(Central);
    L->addWidget(Label);
    L->addWidget(Dialog);
    OnSignature_Toggled(false);

    setLayout(L);

    show();
    hide();
    show();
    if (size().width()<Smpte330_SA->Smpte330->size().width())
    {
        int ScreenNumber=QApplication::desktop()->screenNumber(this);
        int Extra_x=frameGeometry().width()-geometry().width();
        int Extra_y=frameGeometry().height()-geometry().height();
        move(QApplication::desktop()->screenGeometry(ScreenNumber).left()+40, QApplication::desktop()->screenGeometry(ScreenNumber).top()+40);
        resize(QApplication::desktop()->screenGeometry(ScreenNumber).width()-80-Extra_x, QApplication::desktop()->screenGeometry(ScreenNumber).height()-80-Extra_y);
    }

    //Filling
    Updating=true;
    string Value=C->Get(FileName, Field);
    if (!Value.empty())
        Free_Basic->setText(Value.substr(0, 64).c_str());
    if (Value.size()>=64)
        Free_Signature->setText(Value.substr(64, 64).c_str());
    Updating=false;
    if (TryList(Ztring().From_UTF8(Value)))
    {
        Central->setCurrentIndex(0);
        Label->setText("Limitations: SMPTE ST330-2000 only, Audio UMID only, SMPTE Material Number Generation Method only");
    }
    else
    {
        Central->setCurrentIndex(1);
        Central->setTabEnabled(0, false);
    }
}

//***************************************************************************
// Menu actions
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_xxxx_UmidDialog::OnAccept ()
{
    bSigUpdated = true;

	if (Central->currentIndex()!=1)
        OnCurrentChanged(1);
    
    std::string Value=(Free_Basic->text()+Free_Signature->text()).toUtf8().data();
    if (!C->IsValid(FileName, Field, Value))
    {
        QMessageBox MessageBox;
        MessageBox.setWindowTitle("BWF MetaEdit");
        MessageBox.setText((string("Field does not conform to rules:\n")+C->IsValid_LastError(FileName)).c_str());
        #if (QT_VERSION >= 0x040200)
            MessageBox.setStandardButtons(QMessageBox::Ok);
        #endif // (QT_VERSION >= 0x040200)
        MessageBox.setIcon(QMessageBox::Warning);
        MessageBox.setWindowIcon(QIcon(":/Image/Logo/Logo.png"));
        MessageBox.exec();
        return;
    }

    C->Set(FileName, Field, Value);

    accept();
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_UmidDialog::OnTextChanged (const QString &)
{
    std::string Value=Free_Basic->text().toUtf8().data();
    if (Value.size()==64)
        Value+=Free_Signature->text().toUtf8().data();

    if (!C->IsValid(FileName, Field, Value))
    {
        Label->setText(QString::fromUtf8(C->IsValid_LastError(FileName).c_str()));
        Dialog->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else
    {
        Label->setText(QString());
        Dialog->button(QDialogButtonBox::Ok)->setEnabled(true);
    }

    Central->setTabEnabled(0, TryList((Free_Basic->text()+Free_Signature->text()).toUtf8().data()));
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_UmidDialog::OnMenu_Load()
{
    //User interaction
    QString FileNamesQ = QFileDialog::getOpenFileName(this, "", QString::fromUtf8(C->OpenSaveFolder.c_str()), "");
    
    if (FileNamesQ.isEmpty())
        return;

    File F;
    if (!F.Open(ZenLib::Ztring().From_UTF8(FileNamesQ.toUtf8().data())))
        return;
    int64u F_Size=F.Size_Get();
    if (F_Size>((size_t)-1)-1)
        return;

    //Creating buffer
    int8u* Buffer=new int8u[(size_t)F_Size+1];
    size_t Buffer_Offset=0;

    //Reading the file
    while(Buffer_Offset<F_Size)
    {
        size_t BytesRead=F.Read(Buffer+Buffer_Offset, (size_t)F_Size-Buffer_Offset);
        if (BytesRead==0)
            break; //Read is finished
        Buffer_Offset+=BytesRead;
    }
    if (Buffer_Offset<F_Size)
        return;

    //Filling
    Field.assign((const char*)Buffer, Buffer_Offset);
    delete[] Buffer;
    AdaptEOL(Field, adapt_n);

    if (Field.empty())
        Free_Basic->setText(Field.substr(0, 64).c_str());
    if (Field.size()>=64)
        Free_Signature->setText(Field.substr(64, 64).c_str());
    if (Central->currentIndex()==0)
        OnMenu_List();
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_UmidDialog::OnMenu_Save()
{
    //User interaction
    QString FileNamesQ = QFileDialog::getSaveFileName(this, "", QString::fromUtf8(C->OpenSaveFolder.c_str()), "");
    
    if (FileNamesQ.isEmpty())
        return;

    File F;
    if (!F.Create(ZenLib::Ztring().From_UTF8(FileNamesQ.toUtf8().data())))
        return;

    //Filling
    F.Write(Ztring((Free_Basic->text()+Free_Signature->text()).toUtf8().data()));
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_UmidDialog::OnMenu_List ()
{
    Text2List();
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_UmidDialog::OnMenu_Text ()
{
    Label->setText(QString());
    List2Text();
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_UmidDialog::OnCurrentChanged (int Index)
{
    //Showing
    switch (Index)
    {
        case 0  : OnMenu_List(); break;
        case 1  : OnMenu_Text(); break;
        default : ;
    }
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_UmidDialog::OnSignature_Toggled (bool Checked)
{
    Basic_Ul_Length->setText(Checked?"33":"13");
    Signature_TimeDate_Date->setEnabled(Checked);
    Signature_TimeDate_Time->setEnabled(Checked);
    Signature_TimeDate_Samples->setEnabled(Checked);
    Signature_TimeDate_Zone->setEnabled(Checked);
    Signature_Spatial_Altitude->setEnabled(Checked);
    Signature_Spatial_Longitudinal->setEnabled(Checked);
    Signature_Spatial_Longitudinal_Orientation->setEnabled(Checked);
    Signature_Spatial_Latitude->setEnabled(Checked);
    Signature_Spatial_Latitude_Orientation->setEnabled(Checked);
    Signature_Other_Country->setEnabled(Checked);
    Signature_Other_Organization->setEnabled(Checked);
    Signature_Other_User->setEnabled(Checked);
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_UmidDialog::OnvalueChanged_BM_T (const QTime &Value)
{
    if (Updating || SampleRate==0)
        return;

    Basic_Material_TimeDate_Samples->setValue(float64_int64s(((double)QTime(0, 0, 0, 0).msecsTo(Value))*SampleRate/1000));
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_UmidDialog::OnvalueChanged_BM_S (double Value)
{
    if (Updating || SampleRate==0)
        return;

    int64s TimeReference=float64_int64s(Value/SampleRate*1000);
    Updating=true;
    if (TimeReference>23*60*60*1000+59*60*1000+59*1000+999)
    {
        Basic_Material_TimeDate_Time->setEnabled(false);
        Basic_Material_TimeDate_Time->setTime(QTime(0, 0, 0, 0).addMSecs(23*60*60*1000+59*60*1000+59*1000+999));
    }
    else
    {
        Basic_Material_TimeDate_Time->setEnabled(true);
        Basic_Material_TimeDate_Time->setTime(QTime(0, 0, 0, 0).addMSecs((int)TimeReference));
    }
    Updating=false;
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_UmidDialog::OnvalueChanged_S_T (const QTime &Value)
{
    if (Updating || SampleRate==0)
        return;

    Signature_TimeDate_Samples->setValue(float64_int64s(((double)QTime(0, 0, 0, 0).msecsTo(Value))*SampleRate/1000));
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_UmidDialog::OnvalueChanged_S_S (double Value)
{
    if (Updating || SampleRate==0)
        return;

    int64s TimeReference=float64_int64s(Value/SampleRate*1000);
    Updating=true;
    if (TimeReference>23*60*60*1000+59*60*1000+59*1000+999)
    {
        Signature_TimeDate_Time->setEnabled(false);
        Signature_TimeDate_Time->setTime(QTime(0, 0, 0, 0).addMSecs(23*60*60*1000+59*60*1000+59*1000+999));
    }
    else
    {
        Signature_TimeDate_Time->setEnabled(true);
        Signature_TimeDate_Time->setTime(QTime(0, 0, 0, 0).addMSecs((int)TimeReference));
    }
    Updating=false;
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_UmidDialog::List2Text ()
{
    if (Updating)
        return;    
        
    std::string Temp;
    
    //Basic_UL
    Temp+="060A2B340101010101010";
    Temp+='0'+Basic_Ul_Umid->currentIndex();
    Temp+='0'+Basic_Ul_MaterialMethod->currentIndex();
    Temp+='0'+Basic_Ul_InstanceMethod->currentIndex();

    //Basic_Other
    Temp+=Basic_Ul_Length->text().at(0).toLatin1();
    Temp+=Basic_Ul_Length->text().at(1).toLatin1();
    Temp+=ToHex((Basic_Ul_Instance->value()>>20)&0xF);
    Temp+=ToHex((Basic_Ul_Instance->value()>>16)&0xF);
    Temp+=ToHex((Basic_Ul_Instance->value()>>12)&0xF);
    Temp+=ToHex((Basic_Ul_Instance->value()>> 8)&0xF);
    Temp+=ToHex((Basic_Ul_Instance->value()>> 4)&0xF);
    Temp+=ToHex((Basic_Ul_Instance->value()    )&0xF);

    //Basic_Material
	
	
    int32u TempI=Basic_Material_TimeDate_Samples->value(); //Time
    Temp+=ToHex((TempI>> 4)&0xF);
    Temp+=ToHex((TempI    )&0xF);
    Temp+=ToHex((TempI>>12)&0xF);
    Temp+=ToHex((TempI>> 8)&0xF);
    Temp+=ToHex((TempI>>20)&0xF);
    Temp+=ToHex((TempI>>16)&0xF);
    Temp+=ToHex((TempI>>28)&0xF);
    Temp+=ToHex((TempI>>24)&0xF);	


    TempI=Basic_Material_TimeDate_Date->date().toJulianDay()-2400001;
    Temp+=ToHex((TempI/      10)%10); //Date
    Temp+=ToHex((TempI         )%10);
    Temp+=ToHex((TempI/    1000)%10);
    Temp+=ToHex((TempI/     100)%10);
    Temp+=ToHex((TempI/  100000)%10);
    Temp+=ToHex((TempI/   10000)%10);
    TempI=Zone_IndexToInt[Basic_Material_TimeDate_Zone->currentIndex()];
    Temp+=ToHex(0x8|(TempI>>4)); //Julian date bit + 2 bits of time zone
    Temp+=ToHex(TempI&0xF);
    Temp+=ToHex((Basic_Material_Random->value()>>12)&0xF);
    Temp+=ToHex((Basic_Material_Random->value()>> 8)&0xF);
    Temp+=ToHex((Basic_Material_Random->value()>> 4)&0xF);
    Temp+=ToHex((Basic_Material_Random->value()    )&0xF);
    qint64 Machine=(qint64)Basic_Material_Machine->value();
    Temp+=ToHex((Machine>>44)&0xF);
    Temp+=ToHex((Machine>>40)&0xF);
    Temp+=ToHex((Machine>>36)&0xF);
    Temp+=ToHex((Machine>>32)&0xF);
    Temp+=ToHex((Machine>>28)&0xF);
    Temp+=ToHex((Machine>>24)&0xF);
    Temp+=ToHex((Machine>>20)&0xF);
    Temp+=ToHex((Machine>>16)&0xF);
    Temp+=ToHex((Machine>>12)&0xF);
    Temp+=ToHex((Machine>> 8)&0xF);
    Temp+=ToHex((Machine>> 4)&0xF);
    Temp+=ToHex((Machine    )&0xF);

	Free_Basic->setText(Temp.c_str());
	Temp.clear();
	
    if (Signature_Enabled->isChecked())
    {
        //Signature_TimeDate
		bSigUpdated = true;
		/*
        int32u Milliseconds=Signature_TimeDate_Time->time().hour()*60*60*1000
                   + Signature_TimeDate_Time->time().minute()*60*1000
                   + Signature_TimeDate_Time->time().second()*1000
                   + Signature_TimeDate_Time->time().msec();
        TempI=Milliseconds*48;
        Temp+=ToHex((TempI>> 4)&0xF);
        Temp+=ToHex((TempI    )&0xF);
        Temp+=ToHex((TempI>>12)&0xF);
        Temp+=ToHex((TempI>> 8)&0xF);
        Temp+=ToHex((TempI>>20)&0xF);
        Temp+=ToHex((TempI>>16)&0xF);
        Temp+=ToHex((TempI>>28)&0xF);
        Temp+=ToHex((TempI>>24)&0xF);
	
		*/

		int32u TempI=Signature_TimeDate_Samples->value();
		Temp+=ToHex((TempI>> 4)&0xF);
		Temp+=ToHex((TempI    )&0xF);
		Temp+=ToHex((TempI>>12)&0xF);
		Temp+=ToHex((TempI>> 8)&0xF);
		Temp+=ToHex((TempI>>20)&0xF);
		Temp+=ToHex((TempI>>16)&0xF);
		Temp+=ToHex((TempI>>28)&0xF);
		Temp+=ToHex((TempI>>24)&0xF);

        TempI=Signature_TimeDate_Date->date().toJulianDay()-2400001;
        Temp+=ToHex((TempI/      10)%10); //Date
        Temp+=ToHex((TempI         )%10);
        Temp+=ToHex((TempI/    1000)%10);
        Temp+=ToHex((TempI/     100)%10);
        Temp+=ToHex((TempI/  100000)%10);
        Temp+=ToHex((TempI/   10000)%10);
        TempI=Zone_IndexToInt[Signature_TimeDate_Zone->currentIndex()];
        Temp+=ToHex(0x8|(TempI>>4)); //Julian date bit + 2 bits of time zone
        Temp+=ToHex(TempI&0xF);

        //Signature_Spatial
        TempI=(int)Signature_Spatial_Altitude->value();
        Temp+=ToHex((TempI/      10)%10);
        Temp+=ToHex((TempI         )%10);
        Temp+=ToHex((TempI/    1000)%10);
        Temp+=ToHex((TempI/     100)%10);
        Temp+=ToHex((TempI/  100000)%10);
        Temp+=ToHex((TempI/   10000)%10);
        Temp+=ToHex((TempI/10000000)%10);
        Temp+=ToHex((TempI/ 1000000)%10);

        /*
		TempI=(int)Signature_Spatial_Longitudinal->value();
        Temp+=ToHex((TempI/      10)%10);
        Temp+=ToHex((TempI         )%10);
        Temp+=ToHex((TempI/    1000)%10);
        Temp+=ToHex((TempI/     100)%10);
        Temp+=ToHex((TempI/  100000)%10);
        Temp+=ToHex((TempI/   10000)%10);
        Temp+=ToHex((Signature_Spatial_Longitudinal_Orientation->currentIndex()?0xE:0x0)|((TempI/10000000)%10));
        Temp+=ToHex((TempI/ 1000000)%10);
		*/
	
		
		float TempF=(float)Signature_Spatial_Longitudinal->value();
		bool index = Signature_Spatial_Longitudinal_Orientation->currentIndex();

		string hexStr = FloatToHexString(TempF);
		unsigned char c = hexStr.at(hexStr.length()-1);
		c = Pack(c,index);
		if(index)
			c = c - 128;
		
		hexStr.at(hexStr.length()-1) = c;
		Temp += hexStr;
		
/*
        TempI=(int)Signature_Spatial_Latitude->value();
        Temp+=ToHex((TempI/      10)%10);
        Temp+=ToHex((TempI         )%10);
        Temp+=ToHex((TempI/    1000)%10);
        Temp+=ToHex((TempI/     100)%10);
        Temp+=ToHex((TempI/  100000)%10);
        Temp+=ToHex((TempI/   10000)%10);
        Temp+=ToHex((Signature_Spatial_Latitude_Orientation->currentIndex()?0xF:0x0)|((TempI/10000000)%10));
        Temp+=ToHex((TempI/ 1000000)%10);
*/

		TempF=(float)Signature_Spatial_Latitude->value();
		index = Signature_Spatial_Latitude_Orientation->currentIndex();
		hexStr="";
		hexStr = FloatToHexString(TempF);
		c = hexStr.at(hexStr.length()-1);
		c = Pack(c,index);
		if(index)
			c = c - 128;
		
		hexStr.at(hexStr.length()-1) = c;
		Temp += hexStr;


        string TempS=Signature_Other_Country->lineEdit()->text().toStdString();
        if (TempS.size()<4)
            TempS.append(4-TempS.size(), ' ');
        Temp+=ToHex((TempS[0]>> 4)&0xF);
        Temp+=ToHex((TempS[0]    )&0xF);
        Temp+=ToHex((TempS[1]>> 4)&0xF);
        Temp+=ToHex((TempS[1]    )&0xF);
        Temp+=ToHex((TempS[2]>> 4)&0xF);
        Temp+=ToHex((TempS[2]    )&0xF);
        Temp+=ToHex((TempS[3]>> 4)&0xF);
        Temp+=ToHex((TempS[3]    )&0xF);

        TempS=Signature_Other_Organization->text().toStdString();
        if (TempS.size()<4)
            TempS.append(4-TempS.size(), ' ');
        Temp+=ToHex((TempS[0]>> 4)&0xF);
        Temp+=ToHex((TempS[0]    )&0xF);
        Temp+=ToHex((TempS[1]>> 4)&0xF);
        Temp+=ToHex((TempS[1]    )&0xF);
        Temp+=ToHex((TempS[2]>> 4)&0xF);
        Temp+=ToHex((TempS[2]    )&0xF);
        Temp+=ToHex((TempS[3]>> 4)&0xF);
        Temp+=ToHex((TempS[3]    )&0xF);

        TempS=Signature_Other_User->text().toStdString();
        if (TempS.size()<4)
            TempS.append(4-TempS.size(), ' ');
        Temp+=ToHex((TempS[0]>> 4)&0xF);
        Temp+=ToHex((TempS[0]    )&0xF);
        Temp+=ToHex((TempS[1]>> 4)&0xF);
        Temp+=ToHex((TempS[1]    )&0xF);
        Temp+=ToHex((TempS[2]>> 4)&0xF);
        Temp+=ToHex((TempS[2]    )&0xF);
        Temp+=ToHex((TempS[3]>> 4)&0xF);
        Temp+=ToHex((TempS[3]    )&0xF);
    }

    Free_Signature->setText(Temp.c_str());
}

//---------------------------------------------------------------------------
void GUI_Main_xxxx_UmidDialog::Text2List ()
{
    if (Updating)
        return;    
        
    TryList((Free_Basic->text()+Free_Signature->text()).toUtf8().data());
    Label->setText("Limitations: SMPTE ST330-2000 only, Audio UMID only, SMPTE Material Number Generation Method only");
}

//---------------------------------------------------------------------------
bool GUI_Main_xxxx_UmidDialog::TryList (Ztring Value)
{
    Value.MakeUpperCase();
    bool Fail=false;
    
    if (!C->IsValid(FileName, Field, Value.To_UTF8()))
        return false;

    if (Value.empty())
        return true;

    //Basic_Ul
    if (Value.find(__T("060A2B34010101010101021"))!=0)
        return false;
    Basic_Ul_InstanceMethod->setCurrentIndex(Value[23]-(Value[23]<='9'?'0':'A'));
    
    //Basic_Other
    switch (FromHex(Value.To_UTF8(), 24, 2))
    {
        case 0x13 : if (Value.size()!=64)
                        return false;
                    Basic_Ul_Length->setText("13");
                    break;
        case 0x33 : if (Value.size()!=128)
                        return false;
                    Basic_Ul_Length->setText("33");
                    break;
        default  : return false;
    }
    Basic_Ul_Instance->setValue(FromHex(Value.To_UTF8(), 26, 6));

    //Basic_Material
    double bm_var = (double)(FromHex(Value.To_UTF8(), 32, 2)+(FromHex(Value.To_UTF8(), 34, 2)<<8)+(FromHex(Value.To_UTF8(), 36, 2)<<16)+(FromHex(Value.To_UTF8(), 38, 2)<<24));
	Basic_Material_TimeDate_Samples->setValue(bm_var);
    if (SampleRate)
    {
        int64s TimeReference=float64_int64s(bm_var/SampleRate*1000);
        Basic_Material_TimeDate_Time->setTime(QTime(0, 0, 0, 0).addMSecs((int)TimeReference));
    }

    Basic_Material_TimeDate_Date->setDate(QDate::fromJulianDay(2400001).addDays(FromBCD(Value.To_UTF8(), 40, 1, Fail)*10
                                                                              + FromBCD(Value.To_UTF8(), 41, 1, Fail)
                                                                              + FromBCD(Value.To_UTF8(), 42, 1, Fail)*1000
                                                                              + FromBCD(Value.To_UTF8(), 43, 1, Fail)*100
                                                                              + FromBCD(Value.To_UTF8(), 44, 1, Fail)*100000
                                                                              + FromBCD(Value.To_UTF8(), 45, 1, Fail)*10000));

    if (Fail)
        return false;
    if ((FromHex(Value.To_UTF8(), 46, 1)&0x8)!=0x8)
        return false;    
    int8u Zone=(int8u)(((FromHex(Value.To_UTF8(), 46, 1)&0x3)<<4)|FromHex(Value.To_UTF8(), 47, 1));
    for (int Pos=0; Pos<64; Pos++)
        if (Zone_IndexToInt[Pos]==Zone)
            Basic_Material_TimeDate_Zone->setCurrentIndex(Pos);
    Basic_Material_Random->setValue(FromHex(Value.To_UTF8(), 48, 4));
    Basic_Material_Machine->setValue(FromHex64(Value.To_UTF8(), 52, 12));

    //Signature
    if (Value.size()==128 && bSigUpdated )
    {
        string str="";
		Signature_Enabled->setChecked(true);

        //Signature_TimeDate
        double sig_var = (double)(FromHex(Value.To_UTF8(), 64, 2)+(FromHex(Value.To_UTF8(), 66, 2)<<8)+(FromHex(Value.To_UTF8(), 68, 2)<<16)+(FromHex(Value.To_UTF8(), 70, 2)<<24));
		Signature_TimeDate_Samples->setValue(sig_var);
        if (SampleRate)
        {
            int64s TimeReference=float64_int64s(sig_var/SampleRate*1000);
            Signature_TimeDate_Time->setTime(QTime(0, 0, 0, 0).addMSecs((int)TimeReference));
        }

        Signature_TimeDate_Date->setDate(QDate::fromJulianDay(2400001).addDays(FromBCD(Value.To_UTF8(), 72, 1, Fail)*10
                                                                             + FromBCD(Value.To_UTF8(), 73, 1, Fail)
                                                                             + FromBCD(Value.To_UTF8(), 74, 1, Fail)*1000
                                                                             + FromBCD(Value.To_UTF8(), 75, 1, Fail)*100
                                                                             + FromBCD(Value.To_UTF8(), 76, 1, Fail)*100000
                                                                             + FromBCD(Value.To_UTF8(), 77, 1, Fail)*10000));
        if (Fail)
            return false;
        if ((FromHex(Value.To_UTF8(), 78, 1)&0x8)!=0x8)
            return false;    
        int8u TempI8=(int8u)(((FromHex(Value.To_UTF8(), 78, 1)&0x3)<<4)|FromHex(Value.To_UTF8(), 79, 1));
        for (int Pos=0; Pos<64; Pos++)
            if (Zone_IndexToInt[Pos]==TempI8)
                Signature_TimeDate_Zone->setCurrentIndex(Pos);

        //Signature_Spatial
        Signature_Spatial_Altitude->setValue(FromBCD(Value.To_UTF8(), 80, 1, Fail)*10
                                           + FromBCD(Value.To_UTF8(), 81, 1, Fail)
                                           + FromBCD(Value.To_UTF8(), 82, 1, Fail)*1000
                                           + FromBCD(Value.To_UTF8(), 83, 1, Fail)*100
                                           + FromBCD(Value.To_UTF8(), 84, 1, Fail)*100000
                                           + FromBCD(Value.To_UTF8(), 85, 1, Fail)*10000
                                           + FromBCD(Value.To_UTF8(), 86, 1, Fail)*10000000
                                           + FromBCD(Value.To_UTF8(), 87, 1, Fail)*1000000);
        if (Fail)
            return false;

		/*
        Signature_Spatial_Longitudinal->setValue(FromBCD(Value, 88, 1, Fail)*10
                                               + FromBCD(Value, 89, 1, Fail)
                                               + FromBCD(Value, 90, 1, Fail)*1000
                                               + FromBCD(Value, 91, 1, Fail)*100
                                               + FromBCD(Value, 92, 1, Fail)*100000
                                               + FromBCD(Value, 93, 1, Fail)*10000
                                               + (FromHex(Value, 94, 1)&0x1)*10000000
                                               + FromBCD(Value, 95, 1, Fail)*1000000);
        if (Fail)
            return false;
        switch ((FromHex(Value, 94, 1)&0xE))
        {
            case 0x0: Signature_Spatial_Longitudinal_Orientation->setCurrentIndex(0); break;
            case 0xE: Signature_Spatial_Longitudinal_Orientation->setCurrentIndex(1); break;
            default : return false;
        }
		*/

		str="";
		for(int i=88; i<88+8; i++)
		str += Value[i];
		
		unsigned char c;
		char res;
		bool orientation;

		c = str.at(str.length()-1);
		bool checkOrientation = UnpackBool(c+128);
		if(checkOrientation)
			c = c + 128;
		Unpack(c,res,orientation);
		
		str.at(str.length()-1) = res;
		float fLongitude = HexStringToFloat(str);


		Signature_Spatial_Longitudinal->setValue(fLongitude);
		Signature_Spatial_Longitudinal_Orientation->setCurrentIndex(orientation);



/*
		Signature_Spatial_Latitude->setValue(FromBCD(Value, 96, 1, Fail)*10
                                           + FromBCD(Value, 97, 1, Fail)
                                           + FromBCD(Value, 98, 1, Fail)*1000
                                           + FromBCD(Value, 99, 1, Fail)*100
                                           + FromBCD(Value, 100, 1, Fail)*100000
                                           + FromBCD(Value, 101, 1, Fail)*10000
                                           + FromBCD(Value, 103, 1, Fail)*1000000);
        if (Fail)
            return false;
        switch (FromHex(Value, 102, 1))
        {
            case 0x0: Signature_Spatial_Latitude_Orientation->setCurrentIndex(0); break;
            case 0xF: Signature_Spatial_Latitude_Orientation->setCurrentIndex(1); break;
            default : return false;
        }
*/


		str="";
		for(int i=96; i<96+8; i++)
		str += Value[i];
		
		c = str.at(str.length()-1);
		checkOrientation = UnpackBool(c+128);
		if(checkOrientation)
			c = c + 128;
		Unpack(c,res,orientation);
		
		str.at(str.length()-1) = res;
		float fLatitude = HexStringToFloat(str);


		Signature_Spatial_Latitude->setValue(fLatitude);
		Signature_Spatial_Latitude_Orientation->setCurrentIndex(orientation);



       string TempS;
        TempI8=FromHex(Value.To_UTF8(), 104, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        TempI8=FromHex(Value.To_UTF8(), 106, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        TempI8=FromHex(Value.To_UTF8(), 108, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        TempI8=FromHex(Value.To_UTF8(), 110, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        Signature_Other_Country->lineEdit()->setText(TempS.c_str());
        TempS.clear();
        TempI8=FromHex(Value.To_UTF8(), 112, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        TempI8=FromHex(Value.To_UTF8(), 114, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        TempI8=FromHex(Value.To_UTF8(), 116, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        TempI8=FromHex(Value.To_UTF8(), 118, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        Signature_Other_Organization->setText(TempS.c_str());
        TempS.clear();
        TempI8=FromHex(Value.To_UTF8(), 120, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        TempI8=FromHex(Value.To_UTF8(), 122, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        TempI8=FromHex(Value.To_UTF8(), 124, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        TempI8=FromHex(Value.To_UTF8(), 126, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        Signature_Other_User->setText(TempS.c_str());
		
		
        //Signature_Other
        //string TempS;
        TempI8=FromHex(Value.To_UTF8(), 104, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        TempI8=FromHex(Value.To_UTF8(), 106, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        TempI8=FromHex(Value.To_UTF8(), 108, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        TempI8=FromHex(Value.To_UTF8(), 110, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        Signature_Other_Country->lineEdit()->setText(TempS.c_str());
        TempS.clear();
        TempI8=FromHex(Value.To_UTF8(), 112, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        TempI8=FromHex(Value.To_UTF8(), 114, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        TempI8=FromHex(Value.To_UTF8(), 116, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        TempI8=FromHex(Value.To_UTF8(), 118, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        Signature_Other_Organization->setText(TempS.c_str());
        TempS.clear();
        TempI8=FromHex(Value.To_UTF8(), 120, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        TempI8=FromHex(Value.To_UTF8(), 122, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        TempI8=FromHex(Value.To_UTF8(), 124, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        TempI8=FromHex(Value.To_UTF8(), 126, 2);
        if (TempI8<0x20 || TempI8>0x7E)
            return false;
        TempS+=(char)TempI8;
        Signature_Other_User->setText(TempS.c_str());

		bSigUpdated = false;
		
    }
    
	if(Value.size() != 128)
        Signature_Enabled->setChecked(false);

	// else
	// Signature_Enabled->setChecked(false);

    return true;
}

//***************************************************************************
// Events
//***************************************************************************

void GUI_Main_xxxx_UmidDialog::hideEvent (QHideEvent*)
{
    if (Central->currentIndex()==0)
        List2Text();
}
