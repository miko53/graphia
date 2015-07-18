; example1.nsi
;
; This script is perhaps one of the simplest NSIs you can make. All of the
; optional settings are left to their default settings. The installer simply 
; prompts the user asking them where to install, and drops a copy of example1.nsi
; there. 

;--------------------------------
!include "MUI2.nsh"

; The name of the installer
Name "graphia"
XPStyle on
ShowInstDetails hide

; The file to write
OutFile "install.exe"

; The default installation directory
InstallDir $PROGRAMFILES\graphia
InstallDirRegKey HKLM "Software\graphia" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel admin

Var StartMenuFolder

!define MUI_ABORTWARNING
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "..\licence.txt"
!insertmacro MUI_PAGE_DIRECTORY

!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\graphia" 
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"

!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

;--------------------------------
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"

;--------------------------------

; The stuff to install
Section "install" ;No components page, name is not important

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File ../build-mingw32-debug\graphia\core\graphia.exe
  File C:\Qt\5.3\mingw482_32\bin\icudt52.dll
  File C:\Qt\5.3\mingw482_32\bin\icuin52.dll
  File C:\Qt\5.3\mingw482_32\bin\icuuc52.dll
  File C:\Qt\5.3\mingw482_32\bin\libgcc_s_dw2-1.dll
  File C:\Qt\5.3\mingw482_32\bin\libstdc++-6.dll
  File C:\Qt\5.3\mingw482_32\bin\libwinpthread-1.dll
  File C:\Qt\5.3\mingw482_32\bin\Qt5Cored.dll
  File C:\Qt\5.3\mingw482_32\bin\Qt5Guid.dll
  File C:\Qt\5.3\mingw482_32\bin\Qt5PrintSupportd.dll  
  File C:\Qt\5.3\mingw482_32\bin\Qt5Widgetsd.dll
  File C:\Qt\5.3\mingw482_32\bin\Qt5Xmld.dll
  File C:\Qt\5.3\mingw482_32\bin\QtSolutions_PropertyBrowser-headd.dll
  
  SetOutPath $INSTDIR\platforms
  File C:\Qt\5.3\mingw482_32\plugins\platforms\qminimald.dll
  File C:\Qt\5.3\mingw482_32\plugins\platforms\qoffscreend.dll
  File C:\Qt\5.3\mingw482_32\plugins\platforms\qwindowsd.dll  
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\graphia "Install_Dir" "$INSTDIR"
  
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
	  CreateDirectory "$SMPROGRAMS\Graphia"
	  CreateShortCut "$SMPROGRAMS\Graphia\graphia.lnk" "$INSTDIR\graphia.exe" "" "$INSTDIR\graphia.exe" 0
	  CreateShortCut "$SMPROGRAMS\Graphia\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0    
  !insertmacro MUI_STARTMENU_WRITE_END
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\graphia" "Graphia" "Graphia SW uninstall"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\graphia" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\graphia" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\graphia" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd ; end the section

LangString DESC_SecDummy ${LANG_ENGLISH} "A test section."

;Assign language strings to sections
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
!insertmacro MUI_DESCRIPTION_TEXT ${SecDummy} $(DESC_SecDummy)
!insertmacro MUI_FUNCTION_DESCRIPTION_END
  
Section "Uninstall"

  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\graphia"
  DeleteRegKey HKLM SOFTWARE\graphia

  ; Remove files and uninstaller
  Delete $INSTDIR\uninstall.exe
  Delete $INSTDIR\graphia.exe
  Delete $INSTDIR\icudt52.dll
  Delete $INSTDIR\icuin52.dll
  Delete $INSTDIR\icuuc52.dll
  Delete $INSTDIR\libgcc_s_dw2-1.dll
  Delete $INSTDIR\libstdc++-6.dll
  Delete $INSTDIR\libwinpthread-1.dll
  Delete $INSTDIR\Qt5Cored.dll
  Delete $INSTDIR\Qt5Guid.dll
  Delete $INSTDIR\Qt5PrintSupportd.dll  
  Delete $INSTDIR\Qt5Widgetsd.dll
  Delete $INSTDIR\Qt5Xmld.dll
  Delete $INSTDIR\QtSolutions_PropertyBrowser-headd.dll
  Delete $INSTDIR\platforms\qminimald.dll
  Delete $INSTDIR\platforms\qoffscreend.dll
  Delete $INSTDIR\platforms\qwindowsd.dll  
  RMDir  $INSTDIR\platforms
  
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
    
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\graphia.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"  
  
  RMDir "$INSTDIR"

SectionEnd

  
