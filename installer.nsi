; ============================================================
;  FileEcho NSIS Installer Script
;  Version: 1.1.1
;  Requires: NSIS 3.x  (https://nsis.sourceforge.io)
; ============================================================

!define APP_NAME      "FileEcho"
!define APP_VERSION   "1.1.1"
!define APP_PUBLISHER  "CEQ151"
!define APP_URL        "https://github.com/CEQ151/FileEcho"
!define APP_EXE        "FileEcho.exe"
!define APP_ICON       "resources\logo.ico"
!define DIST_DIR       "dist\FileEcho-v${APP_VERSION}"
!define UNINSTALL_KEY  "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"

; --- Output ---
OutFile "FileEcho-v${APP_VERSION}-Setup.exe"
InstallDir "$PROGRAMFILES64\${APP_NAME}"
InstallDirRegKey HKLM "${UNINSTALL_KEY}" "InstallLocation"
RequestExecutionLevel admin
SetCompressor /SOLID lzma

; --- UI ---
!include "MUI2.nsh"
!include "FileFunc.nsh"

!define MUI_ICON   "${APP_ICON}"
!define MUI_UNICON "${APP_ICON}"
!define MUI_ABORTWARNING

; --- Pages ---
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "LICENSE"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!define MUI_FINISHPAGE_RUN "$INSTDIR\${APP_EXE}"
!define MUI_FINISHPAGE_RUN_TEXT "Launch FileEcho"
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; --- Languages ---
!insertmacro MUI_LANGUAGE "SimpChinese"
!insertmacro MUI_LANGUAGE "English"

; ============================================================
;  Installation
; ============================================================
Section "Install"
    SetOutPath "$INSTDIR"

    ; --- Application files (statically linked, no DLLs needed) ---
    File "${DIST_DIR}\${APP_EXE}"
    File "${DIST_DIR}\logo.ico"

    ; --- Uninstaller ---
    WriteUninstaller "$INSTDIR\Uninstall.exe"

    ; --- Start Menu shortcuts ---
    CreateDirectory "$SMPROGRAMS\${APP_NAME}"
    CreateShortCut  "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk"   "$INSTDIR\${APP_EXE}" "" "$INSTDIR\logo.ico"
    CreateShortCut  "$SMPROGRAMS\${APP_NAME}\Uninstall.lnk"     "$INSTDIR\Uninstall.exe"

    ; --- Desktop shortcut ---
    CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${APP_EXE}" "" "$INSTDIR\logo.ico"

    ; --- Registry: Add/Remove Programs ---
    WriteRegStr   HKLM "${UNINSTALL_KEY}" "DisplayName"      "${APP_NAME} v${APP_VERSION}"
    WriteRegStr   HKLM "${UNINSTALL_KEY}" "UninstallString"   '"$INSTDIR\Uninstall.exe"'
    WriteRegStr   HKLM "${UNINSTALL_KEY}" "QuietUninstallString" '"$INSTDIR\Uninstall.exe" /S'
    WriteRegStr   HKLM "${UNINSTALL_KEY}" "InstallLocation"   "$INSTDIR"
    WriteRegStr   HKLM "${UNINSTALL_KEY}" "DisplayIcon"       "$INSTDIR\logo.ico"
    WriteRegStr   HKLM "${UNINSTALL_KEY}" "Publisher"         "${APP_PUBLISHER}"
    WriteRegStr   HKLM "${UNINSTALL_KEY}" "URLInfoAbout"      "${APP_URL}"
    WriteRegStr   HKLM "${UNINSTALL_KEY}" "DisplayVersion"    "${APP_VERSION}"
    WriteRegDWORD HKLM "${UNINSTALL_KEY}" "NoModify"          1
    WriteRegDWORD HKLM "${UNINSTALL_KEY}" "NoRepair"          1

    ; --- Estimated size (KB) ---
    ${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
    IntFmt $0 "0x%08X" $0
    WriteRegDWORD HKLM "${UNINSTALL_KEY}" "EstimatedSize" $0

SectionEnd

; ============================================================
;  Uninstallation
; ============================================================
Section "Uninstall"

    ; --- Remove files ---
    Delete "$INSTDIR\${APP_EXE}"
    Delete "$INSTDIR\logo.ico"
    Delete "$INSTDIR\Uninstall.exe"

    ; --- Remove shortcuts ---
    Delete "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk"
    Delete "$SMPROGRAMS\${APP_NAME}\Uninstall.lnk"
    RMDir  "$SMPROGRAMS\${APP_NAME}"
    Delete "$DESKTOP\${APP_NAME}.lnk"

    ; --- Remove registry ---
    DeleteRegKey HKLM "${UNINSTALL_KEY}"

    ; --- Remove install directory (only if empty) ---
    RMDir "$INSTDIR"

SectionEnd
