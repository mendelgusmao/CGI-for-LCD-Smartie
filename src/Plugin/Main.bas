Attribute VB_Name = "mdlMain"
Option Explicit

'---------------------------------------------------------------------------------------
' Module    : mdlMain
' Author    : el coco
' Date      : 23/08/2009 10:25 pm
' Purpose   : Plugin example for LCDSmartie
'
' ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
'
' PHP for LCD Smartie developed by @MendelGusmao 23/03/2011
' CGI for LCD Smartie developed by @MendelGusmao 15/11/2011
' using:
'        el coco's template [ http://forums.lcdsmartie.org/viewtopic.php?f=2&t=1963 ]
'        wonderful DOSOutputs from someone I don't remember
'        TimerEx
'        CSocketMaster
'
' www.pvrehaavok.wordpress.com
'
'---------------------------------------------------------------------------------------

'Needed APIs
Private Declare Function VirtualAlloc Lib "kernel32" (ByVal lpAddress As Long, ByVal dwSize As Long, ByVal flAllocationType As Long, ByVal flProtect As Long) As Long
Private Declare Function VirtualFree Lib "kernel32" (ByVal lpAddress As Long, ByVal dwSize As Long, ByVal dwFreeType As Long) As Long
Private Declare Function VirtualLock Lib "kernel32" (ByVal lpAddress As Long, ByVal dwSize As Long) As Long
Private Declare Function VirtualUnlock Lib "kernel32" (ByVal lpAddress As Long, ByVal dwSize As Long) As Long

'APIs for testing function
Private Declare Function GetWindowText Lib "user32" Alias "GetWindowTextA" (ByVal hwnd As Long, ByVal lpString As String, ByVal cch As Long) As Long
Private Declare Function SetWindowText Lib "user32" Alias "SetWindowTextA" (ByVal hwnd As Long, ByVal lpString As String) As Long
Private Declare Function GetForegroundWindow Lib "user32" () As Long

Private m_lngLastPointer As Long
Private m_lngLastLenght As Long

Public ScriptsPath As String
Public iniFile As String

Public ExecutionTimeout As Integer
Public ExecutionInterval As Integer
Public minRefreshInterval As Integer

Private Client As Client

'=============================================================================================
' Public functions.
' you must modify those, add or edit code
' REMEMBER ADD THE FUNCTIONS EXPORTED ON vbAdvance CONFIGURATION!
'=============================================================================================

'Exported functions. (configure those on vbAdvance configuration)

Public Function DllMain(ByVal hinstDLL As Long, ByVal fdwReason As Long, ByVal lpvReserved As Long) As Long
    
    Const DLL_PROCESS_ATTACH As Long = 1
    
    If fdwReason = DLL_PROCESS_ATTACH Then
        'Initialize the VB Runtime when this DLL is first
        'loaded by a process:
        RuntimeInitialize hinstDLL
            
        'Must return TRUE for success:
        DllMain = 1
        
    End If
    
End Function

Public Sub SmartieInit()

    ScriptsPath = App.Path & "\scripts\"
    iniFile = ScriptsPath & "cgi4lcd.ini"
        
    Set Client = New Client
    
    ' Local and remote ports are swapped in client
    Client.Start _
        Ini.Read(iniFile, "cgi4lcd", "remote_port", 6543), _
        Ini.Read(iniFile, "cgi4lcd", "local_port", 6542), _
        iniFile, ScriptsPath
    
    ExecutionTimeout = Int(Ini.Read(iniFile, "smartie", "timeout", 60000))
    ExecutionInterval = Int(Ini.Read(iniFile, "smartie", "interval", 5000))
    minRefreshInterval = Int(Ini.Read(iniFile, "smartie", "refresh", 3000))
   
End Sub

Public Function GetMinRefreshInterval() As Integer

    GetMinRefreshInterval = minRefreshInterval
    
End Function

' Execute through server
' strParam1 - Filename
' strParam2 - Function name and function parameters
Public Function function1(ByVal strParam1 As String, ByVal strParam2 As String) As Long
    
    Dim Buffer As String
    Dim Script As String
    Dim Parameters As String
    Dim Command As String
    Dim Extension As String
    Dim Interpreter As String
    
    Script = ScriptsPath & ConvertParam(strParam1)
    Parameters = ConvertParam(strParam2)
    
    function1 = Client.Execute(Script, Parameters)
    
End Function

' Execute directly
' strParam1 - Filename
' strParam2 - Function name and function parameters
Public Function function2(ByVal strParam1 As String, ByVal strParam2 As String) As Long

End Function

' Retrieve stats
' strParam1 - Desired data
' -> requests, executions, cleanups, uptime (...)
Public Function function3(ByVal strParam1 As String, ByVal strParam2 As String) As Long

End Function

' Version of interpreter
Public Function function4(ByVal strParam1 As String, ByVal strParam2 As String) As Long

    Dim Buffer As String
    Dim VersionCommand As String
    Dim Extension As String
    Dim Language As String
    Dim Interpreter As String
    
    Extension = LCase(strParam1)
    
    If Not Extension Like "[a-z0-9]" Then
        
        Buffer = "[CGI4LCD] Invalid extension"
        GoTo display
        
    End If
    
    Language = Ini.Read(iniFile, Extension, "language", "")
    Interpreter = Ini.Read(iniFile, Extension, "interpreter", "")
    
    If Language = "" Or Dir(Interpreter, vbNormal) = "" Then
    
        Buffer = "[CGI4LCD] Interpreter for extension '" & Extension & "' not found"
        GoTo display
        
    End If

    VersionCommand = Ini.Read(iniFile, Extension, "version", "")
    VersionCommand = Client.FormatCommand(VersionCommand, Interpreter, "", "")

    Buffer = Client.Request(Command, ExecutionInterval, ExecutionTimeout)

display:

    function2 = CreateReturn(Buffer)

End Function

'=============================================================================================
' Private functions.
' you must not modify those
'=============================================================================================

'Simple function to convert the parameter from unicode
Private Function ConvertParam(ByVal strParam As String) As String
    ConvertParam = StrConv(strParam, vbUnicode)
End Function

'Function to return a pointer to the desired data
Private Function CreateReturn(ByVal strdata As String) As Long
    Dim lngPointer As Long
    Dim lngLen As Long

    lngLen = Len(strdata) 'lenght of data
    '&H1000 = MEM_COMMIT    &H40 = PAGE_EXECUTE_READWRITE
    lngPointer = VirtualAlloc(ByVal 0&, lngLen, &H1000, &H40) 'alloc a pointer with data's lenght
    VirtualLock lngPointer, lngLen 'lock pointer
    SetString strdata, lngPointer 'write unicode string on pointer
    
    If Not (m_lngLastPointer = 0) Then 'release previous pointer
        '&H4000 = MEM_DECOMMIT    &H8000 = MEM_RELEASE
        VirtualUnlock m_lngLastPointer, m_lngLastLenght
        VirtualFree m_lngLastPointer, m_lngLastLenght, &H4000
        VirtualFree m_lngLastPointer, m_lngLastLenght, &H8000
    End If
    
    m_lngLastLenght = lngLen
    m_lngLastPointer = lngPointer
    
    CreateReturn = lngPointer 'return current pointer
End Function

'Interaction with UNICODE and ANSI
'by Cobein (thanks!)
Private Function SetString(ByVal sData As String, ByVal lMem As Long)
    Dim bvData() As Byte
    Dim i As Long

    bvData = StrConv(sData, vbFromUnicode)
    
    For i = 0 To UBound(bvData)
        CopyMemory ByVal lMem + i, bvData(i), 1
    Next
End Function

