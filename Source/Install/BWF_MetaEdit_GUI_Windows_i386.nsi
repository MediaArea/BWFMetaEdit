; Request application privileges for Windows Vista
RequestExecutionLevel admin

; Some defines
!define PRODUCT_NAME "BWF MetaEdit"
!define PRODUCT_PUBLISHER "FADGI"
!define PRODUCT_VERSION "1.3.1"
!define PRODUCT_VERSION4 "${PRODUCT_VERSION}.0"
!define PRODUCT_WEB_SITE "http://www.digitizationguidelines.gov"
!define COMPANY_REGISTRY "Software\FADGI"
!define PRODUCT_REGISTRY "Software\FADGI\BWF MetaEdit"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\BWF_MetaEdit.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; Compression
SetCompressor /FINAL /SOLID lzma

; x64 stuff
!include "x64.nsh"

; Modern UI
!include "MUI2.nsh"
!define MUI_ABORTWARNING
!define MUI_ICON "..\..\Source\Resource\Image\FADGI\Logo.ico"

; Language Selection Dialog Settings
!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

; Installer pages
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!define MUI_FINISHPAGE_RUN "$INSTDIR\bwfedit-gui.exe"
!insertmacro MUI_PAGE_FINISH
; Uninstaller pages
!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

; Language files
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_RESERVEFILE_LANGDLL

; Info
VIProductVersion "${PRODUCT_VERSION4}"
VIAddVersionKey "ProductName" "${PRODUCT_NAME}" 
VIAddVersionKey "Comments" "All about editing your BWF files"
VIAddVersionKey "CompanyName" "FADGI"
VIAddVersionKey "LegalTrademarks" "BSD license" 
VIAddVersionKey "LegalCopyright" "" 
VIAddVersionKey "FileDescription" "All about editing your BWF files"
VIAddVersionKey "FileVersion" "${PRODUCT_VERSION}"
BrandingText " "

; Modern UI end

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "..\..\Release\BWF_MetaEdit_GUI_${PRODUCT_VERSION}_Windows_i386.exe"
InstallDir "$PROGRAMFILES\FADGI\BWF MetaEdit"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails nevershow
ShowUnInstDetails nevershow

Function .onInit
  ${If} ${RunningX64}
    MessageBox MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2 'You are trying to install the 32-bit version of ${PRODUCT_NAME} on 64-bit Windows.$\r$\nPlease download and use the 64-bit version instead.$\r$\nContinue with the installation of the 32-bit version?' IDYES noprob
  Quit
  noprob:
  ${Else}
  ${EndIf}
FunctionEnd

Section "SectionPrincipale" SEC01
  SetOverwrite ifnewer
  SetOutPath "$INSTDIR"
  CreateDirectory "$SMPROGRAMS\BWF MetaEdit"
  CreateShortCut "$SMPROGRAMS\BWF MetaEdit\BWF MetaEdit.lnk" "$INSTDIR\bwfedit-gui.exe" "" "" "" "" "" "BWF MetaEdit ${PRODUCT_VERSION}"
  File "/oname=bwfedit-gui.exe" "..\..\Project\MSVC2010\GUI\Win32\Release\BWF_MetaEdit_GUI.exe"
  File "/oname=History.txt" "..\..\History_GUI.txt"
  File "..\..\License.html"
  File  "/oname=ReadMe.txt""..\..\Release\ReadMe_GUI_Windows.txt"
SectionEnd

Section -AdditionalIcons
  SetOutPath $INSTDIR
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\BWF MetaEdit\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url" "" "" "" "" "" "Website"
  CreateShortCut "$SMPROGRAMS\BWF MetaEdit\Uninstall.lnk" "$INSTDIR\uninst.exe" "" "" "" "" "" "Uninstall MediaInfo"
  CreateShortCut "$SMPROGRAMS\BWF MetaEdit\History.lnk" "$INSTDIR\History.txt" "" "" "" "" "" "History"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\MediaInfo.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\MediaInfo.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
SectionEnd


Section Uninstall
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\bwfedit-gui.exe"
  Delete "$INSTDIR\History.txt"
  Delete "$INSTDIR\License.html"
  Delete "$INSTDIR\ReadMe.txt"
  Delete "$SMPROGRAMS\BWF MetaEdit\Uninstall.lnk"
  Delete "$SMPROGRAMS\BWF MetaEdit\Website.lnk"
  Delete "$SMPROGRAMS\BWF MetaEdit\BWF MetaEdit.lnk"
  Delete "$SMPROGRAMS\BWF MetaEdit\History.lnk"

  RMDir "$SMPROGRAMS\BWF MetaEdit"
  RMDir "$INSTDIR"

  DeleteRegKey HKLM "${PRODUCT_REGISTRY}"
  DeleteRegKey /ifempty HKLM "${COMPANY_REGISTRY}"
  DeleteRegKey HKCU "${PRODUCT_REGISTRY}"
  DeleteRegKey /ifempty HKCU "${COMPANY_REGISTRY}"
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd