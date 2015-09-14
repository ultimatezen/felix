; Felix Setup

[Setup]
AppCopyright=Copyright (c) 2008-2015 Ginstrom IT Solutions (GITS)
AppName=Felix
AppVerName=Felix version 2
AppPublisher=Ginstrom IT Solutions (GITS)
AppPublisherURL=http://www.ginstrom.com/
AppSupportURL=http://www.felix-cat.com/
AppVersion=2
AppContact=support@felix-cat.com

VersionInfoVersion=2
VersionInfoCompany=Ginstrom IT Solutions(GITS)
VersionInfoDescription=Felix Setup Program

DefaultDirName={pf}\Assistant Suite\Felix
DefaultGroupName=Assistant Suite
UninstallDisplayName=Felix
UninstallDisplayIcon={app}\SkinHelper.exe
UninstallFilesDir={app}
ShowLanguageDialog=yes
ShowComponentSizes=yes
WizardSmallImageFile=compiler:WizModernSmallImage-IS.bmp
PrivilegesRequired=admin

OutputDir=.\Felix
OutputBaseFilename=Felix_Setup_2

[Registry]
Root: HKCU; Subkey: Software\AssistantSuite\Felix; Flags: uninsdeletekey
Root: HKCU; Subkey: Software\AssistantSuite\FelixWordInterface; Flags: uninsdeletekey
Root: HKCU; Subkey: Software\AssistantSuite\FelixExcelInterface; Flags: uninsdeletekey
Root: HKCU; Subkey: Software\AssistantSuite\FelixPptInterface; Flags: uninsdeletekey
Root: HKCU; Subkey: Software\AssistantSuite; Flags: uninsdeletekeyifempty

; ftm
Root: HKCR; Subkey: ".ftm"; ValueType: string; ValueName: ""; ValueData: "FelixTMFile"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "FelixTMFile"; ValueType: string; ValueName: ""; ValueData: "Felix TM File"; Flags: uninsdeletekey
; Root: HKCR; Subkey: "FelixTMFile\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\FELIX.EXE,0"
Root: HKCR; Subkey: "FelixTMFile\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\FELIX.EXE"" ""%%1"""

; fgloss
Root: HKCR; Subkey: ".fgloss"; ValueType: string; ValueName: ""; ValueData: "FelixGlossaryFile"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "FelixGlossaryFile"; ValueType: string; ValueName: ""; ValueData: "Felix Glossary File"; Flags: uninsdeletekey
; Root: HKCR; Subkey: "FelixGlossaryFile\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\FELIX.EXE,0"
Root: HKCR; Subkey: "FelixGlossaryFile\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\FELIX.EXE"" ""%%1"""

; fprefs
Root: HKCR; Subkey: ".fprefs"; ValueType: string; ValueName: ""; ValueData: "FelixPreferenceFile"; Flags: uninsdeletevalue
Root: HKCR; Subkey: "FelixPreferenceFile"; ValueType: string; ValueName: ""; ValueData: "Felix Preference File"; Flags: uninsdeletekey
; Root: HKCR; Subkey: "FelixPreferenceFile\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\FELIX.EXE,0"
Root: HKCR; Subkey: "FelixPreferenceFile\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\FELIX.EXE"" ""%%1"""

[Languages]
Name: en; MessagesFile: compiler:English.isl; LicenseFile: "D:\dev\cpp\Assistant Suite\LICENSE.txt"
Name: jp; MessagesFile: compiler:English.isl; LicenseFile: "D:\dev\cpp\Assistant Suite\LICENSE_JP.txt"

[Files]
; ===================
; MAIN COMPONENT
; ===================
; Felix
Source: D:\dev\cpp\Assistant Suite\Felix\Unicode_Release\Felix.exe; components: Main; DestDir: {app}; DestName: Felix.exe; Flags: ignoreversion
Source: D:\dev\cpp\Assistant Suite\Felix\Unicode_Release\lang\EngResource.dll; components: Main; DestDir: {app}\lang; Flags: ignoreversion
Source: D:\dev\cpp\Assistant Suite\Felix\Unicode_Release\lang\JpnResource.dll; components: Main; DestDir: {app}\lang; Flags: ignoreversion
Source: D:\dev\cpp\Assistant Suite\Felix\Unicode_Release\Felix.tlb; components: Main; DestDir: {app}; DestName: Felix.tlb; Flags: ignoreversion

; Memory Engine
Source: D:\dev\python\FelixMemoryServes\MemoryEngine\*; DestDir: {app}\pytools; Flags: ignoreversion recursesubdirs

; version info
Source: version.txt; DestDir: {localappdata}\Felix; Flags: ignoreversion

; Prefs
Source: D:\dev\cpp\Assistant Suite\settings\stringres.database; DestDir: {localappdata}\Felix\prefs; Flags: ignoreversion
Source: D:\dev\cpp\Assistant Suite\settings\felix_rules.frules; DestDir: {localappdata}\Felix\prefs; Flags: ignoreversion onlyifdoesntexist

; Resources
Source: D:\dev\cpp\Assistant Suite\settings\Felix.ico; DestDir: {localappdata}\Felix\res; Flags: ignoreversion

; HTML stuff
Source: D:\dev\cpp\Assistant Suite\Felix\html\*; components: Main; DestDir: {localappdata}\Felix\html; Flags: ignoreversion recursesubdirs
Source: D:\dev\cpp\Assistant Suite\Felix\html\*; components: Main; DestDir: {app}\html; Flags: ignoreversion recursesubdirs

; Docs
Source: D:\dev\cpp\Assistant Suite\LICENSE.txt; components: Main; DestDir: {app}; Flags: ignoreversion
Source: D:\dev\cpp\Assistant Suite\LICENSE_JP.txt; components: Main; DestDir: {app}; Flags: ignoreversion

; DLLs
Source: D:\dev\cpp\Common DLLs\DbgHelp.Dll; components: Main; DestDir: {sys}; Flags: sharedfile
Source: D:\dev\cpp\Common DLLs\SciLexer.Dll; components: Main; DestDir: {sys}; Flags: sharedfile

;; Stuff for Win 2000 Only
Source: D:\dev\cpp\Common DLLs\DbgHelp.Dll; components: Main; DestDir: {app}; OnlyBelowVersion: 4.9.3000,5.01.2600
Source: D:\dev\cpp\Common DLLs\MSVCR90.DLL; components: Main; DestDir: {sys}; OnlyBelowVersion: 4.9.3000,5.01.2600
Source: D:\dev\cpp\Common DLLs\MSVCP90.dll; components: Main; DestDir: {sys}; OnlyBelowVersion: 4.9.3000,5.01.2600

; ===================
; OFFICE INTERFACES
; ===================
; word
Source: D:\dev\cpp\Assistant Suite\WordAssist\WordAssist\Release\WordAssist.dll; components: WordIF; DestDir: {app}; Flags: ignoreversion

; excel
Source: D:\dev\cpp\Assistant Suite\ExcelAssist\ExcelAssist\Release\ExcelAssist.dll; components: ExcelIF; DestDir: {app}; Flags: ignoreversion

; powerpoint
Source: D:\dev\cpp\Assistant Suite\PowerPointAssist\PowerPointAssist\Release\PowerPointAssist.dll; components: PowerPointIF; DestDir: {app}; Flags: ignoreversion

; CRT junk
Source: D:\dev\cpp\Common DLLs\vcredist_x86.exe; components: Main; DestDir: {tmp}
Source: D:\dev\cpp\Common DLLs\vcredist_2013_x86.exe; components: Main; DestDir: {tmp}


[Run]
; VC Redistributables
Filename: "{tmp}\vcredist_x86.exe"; StatusMsg: {cm:Registering,VC 2008 Redistributable Files}; components: Main; Parameters: "/qn"
Filename: "{tmp}\vcredist_2013_x86.exe"; StatusMsg: {cm:Registering,VC 2013 Redistributable Files}; components: Main; Parameters: "/q /norestart"

; Register Office interfaces
Filename: {app}\pytools\RegDll.exe; StatusMsg: {cm:Registering,Excel Interface}; WorkingDir: {app}; components: ExcelIF; Parameters: """{app}\ExcelAssist.dll"""
Filename: {app}\pytools\RegDll.exe; StatusMsg: {cm:Registering,PowerPoint Interface}; WorkingDir: {app}; components: PowerPointIF; Parameters: """{app}\PowerPointAssist.dll"""
Filename: {app}\pytools\RegDll.exe; StatusMsg: {cm:Registering,Word Interface}; WorkingDir: {app}; components: WordIF; Parameters: """{app}\WordAssist.dll"""

; others
Filename: {app}\pytools\MemoryEngine.exe; StatusMsg: {cm:Registering,MemoryEngine}; WorkingDir: {app}\pytools; components: Main; Parameters: /register
Filename: {app}\pytools\FelixUtilities.exe; StatusMsg: {cm:Registering,FelixUtilities}; WorkingDir: {app}\pytools; components: Main; Parameters: /register
Filename: {app}\pytools\ExcelHistory.exe; StatusMsg: {cm:Registering,TransHistoryExcel}; WorkingDir: {app}\pytools; components: Main; Parameters: /register
Filename: {app}\pytools\WordHistory.exe; StatusMsg: {cm:Registering,TransHistoryWord}; WorkingDir: {app}\pytools; components: Main; Parameters: /register
Filename: {app}\pytools\FelixPrefs.exe; StatusMsg: {cm:Registering,FelixPrefs}; WorkingDir: {app}\pytools; components: Main; Parameters: /register

; Felix
Filename: {app}\Felix.exe; StatusMsg: {cm:Registering,Felix}; WorkingDir: {app}; components: Main; Parameters: /RegServer
Filename: {app}\Felix.exe; Description: {cm:LaunchProgram,Felix}; WorkingDir: {app}; components: Main; Flags: nowait postinstall skipifsilent; Parameters: -lang {cm:LanguageID}

[UninstallRun]

Filename: {app}\pytools\UnRegDll.exe; StatusMsg: Unregistering Excel Interface; WorkingDir: {app}; components: ExcelIF; Parameters: """{app}\ExcelAssist.dll"""
Filename: {app}\pytools\UnRegDll.exe; StatusMsg: Unregistering PowerPoint Interface; WorkingDir: {app}; components: PowerPointIF; Parameters: """{app}\PowerPointAssist.dll"""
Filename: {app}\pytools\UnRegDll.exe; StatusMsg: Unregistering Word Interface; WorkingDir: {app}; components: WordIF; Parameters: """{app}\WordAssist.dll"""

Filename: {app}\pytools\MemoryEngine.exe; StatusMsg: Unregistering  MemoryEngine; WorkingDir: {app}\pytools; components: Main; Parameters: /unregister
; Unregister Felix as server
Filename: {app}\Felix.exe; StatusMsg: Unregistering Felix; WorkingDir: {app}\res; RunOnceId: DelReg; components: Main; Parameters: /UnRegServer
Filename: {app}\pytools\FelixUtilities.exe; StatusMsg: Unregistering  FelixUtilities; WorkingDir: {app}\pytools; components: Main; Parameters: /unregister
Filename: {app}\pytools\ExcelHistory.exe; StatusMsg: Unregistering  TransHistoryExcel; WorkingDir: {app}\pytools; components: Main; Parameters: /unregister
Filename: {app}\pytools\WordHistory.exe; StatusMsg: Unregistering  TransHistoryWord; WorkingDir: {app}\pytools; components: Main; Parameters: /unregister
Filename: {app}\pytools\FelixPrefs.exe; StatusMsg: Unregistering  FelixPrefs; WorkingDir: {app}\pytools; components: Main; Parameters: /unregister

; delete menus
Filename: {app}\pytools\CleanupOffice.exe; StatusMsg: {cm:RemovingMenus}; WorkingDir: {app}\pytools; RunOnceId: DelMenus; components: WordIF ExcelIF

[UninstallDelete]
Type: files; Name: "{app}\excel_errlog.txt"
Type: filesandordirs; Name: "{localappdata}\Felix"

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Components: main

[Icons]
Name: {group}\Felix\{cm:UninstallProgram,Felix}; Filename: {uninstallexe}
  ; Comment: {cm:UninstallProgram,Felix}; WorkingDir: {app}; Components: Main
Name: {group}\Felix\Felix; Filename: {app}\Felix.exe
  ; Comment: {cm:LaunchProgram,Felix}; WorkingDir: {app}; Components: Main; IconFilename: {app}\Felix.exe; IconIndex: 0
Name: {userdesktop}\Felix; Filename: {app}\Felix.exe
   ; Comment: {cm:LaunchProgram,Felix}; WorkingDir: {app}; Components: Main; IconFilename: {app}\Felix.exe; IconIndex: 0; Tasks: desktopicon
Name: {group}\Felix\Show Logs; Filename: {app}\pytools\ShowLogs.exe
  ; Comment: {cm:LaunchProgram,Show Logs}; WorkingDir: {app}\pytools; Components: Main; IconFilename: {app}\ShowLogs.exe; IconIndex: 0; Parameters: {cm:LanguageID}
Name: {group}\Felix\Check Updates; Filename: {app}\pytools\CheckUpdates.exe
  ; Comment: {cm:LaunchProgram,Check Updates}; WorkingDir: {app}\pytools; Components: Main; IconFilename: {app}\CheckUpdates.exe; IconIndex: 0

[Components]
Name: Main; Description: {cm:MainFiles}; Types: full custom compact; Flags: fixed
Name: WordIF; Description: {cm:InterfaceFor,MS Word}; Types: full
Name: ExcelIF; Description: {cm:InterfaceFor,MS Excel}; Types: full
Name: PowerPointIF; Description: {cm:InterfaceFor,MS PowerPoint}; Types: full

[Dirs]
Name: {group}\Felix; Flags: uninsalwaysuninstall
Name: {app}; Flags: uninsalwaysuninstall
Name: {app}\lang; Flags: uninsalwaysuninstall
Name: {app}\docs; Flags: uninsalwaysuninstall
Name: {app}\html; Flags: uninsalwaysuninstall
Name: {app}\pytools; Flags: uninsalwaysuninstall
Name: {localappdata}\Felix; Flags: uninsalwaysuninstall
Name: {localappdata}\Felix\logs; Flags: uninsalwaysuninstall
Name: {localappdata}\Felix\prefs; Flags: uninsalwaysuninstall
Name: {localappdata}\Felix\html; Flags: uninsalwaysuninstall
Name: {localappdata}\Felix\html\en; Flags: uninsalwaysuninstall
Name: {localappdata}\Felix\html\jp; Flags: uninsalwaysuninstall
Name: {localappdata}\Felix\html\js; Flags: uninsalwaysuninstall
