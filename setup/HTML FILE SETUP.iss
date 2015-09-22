; Felix Setup

#define SourceDir       "C:\dev\C++\Projects\Assistant Suite 1.6\Felix 1.0"
#define ExeDir          "C:\dev\C++\Projects\Assistant Suite 1.6\Felix 1.0\Unicode_Release"
#define AddinSourceDir  "C:\dev\C++\Projects\Assistant Suite 1.6"
#define RedistSourceDir "C:\dev\c++\Common DLLs"
#define ExcelDir        "ExcelAssist\ExcelAssist\Release"
#define PptDir          "PowerPointAssist\PowerPointAssist\Release"
#define MemoryEngineDir "C:\dev\Python\FelixMemoryServes\MemoryEngine"

#define AppVer GetFileVersion(AddBackslash(ExeDir) + "Felix.exe")

[Setup]
AppCopyright=Copyright © 2008 Ginstrom IT Solutions (GITS)
AppName=Felix
AppVerName=Felix version {#AppVer}
AppPublisher=Ginstrom IT Solutions (GITS)
AppPublisherURL=http://www.ginstrom.com/
AppSupportURL=http://www.felix-cat.com/
AppVersion={#AppVer}
DefaultDirName={pf}\Assistant Suite\Felix
DefaultGroupName=Assistant Suite
UninstallDisplayName=Felix
UninstallFilesDir={app}
ShowLanguageDialog=yes
ShowComponentSizes=yes
OutputDir=.\Felix
WizardSmallImageFile=compiler:WIZMODERNSMALLIMAGE-IS.BMP
WizardImageFile=compiler:WizModernImage-IS.bmp
PrivilegesRequired=admin
OutputBaseFilename=Felix_HTML_Setup

[Languages]
Name: en; MessagesFile: compiler:English.isl; LicenseFile: {#SourceDir}\Eula.txt
Name: jp; MessagesFile: compiler:Japanese.isl; LicenseFile: C:\Dev\Docs\EULA_J.rtf

[Files]
; HTML stuff
Source: {#ExeDir}\html\*; DestDir: {localappdata}\Felix\html; Flags: ignoreversion recursesubdirs
Source: {#ExeDir}\html\*; DestDir: {app}\html; Flags: ignoreversion recursesubdirs

[Dirs]
Name: {localappdata}\Felix; Flags: uninsalwaysuninstall
Name: {localappdata}\Felix\html; Flags: uninsalwaysuninstall
Name: {localappdata}\Felix\html\en; Flags: uninsalwaysuninstall
Name: {localappdata}\Felix\html\jp; Flags: uninsalwaysuninstall
Name: {localappdata}\Felix\html\js; Flags: uninsalwaysuninstall
