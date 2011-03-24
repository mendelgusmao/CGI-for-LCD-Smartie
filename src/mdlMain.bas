Attribute VB_Name = "mdlMain"
'


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
' using:
'        el coco's template
'        wonderful DOSOutputs from someone I don't remember
'        mdlCGI, from an HTTPd I was coding in 2004
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

Private Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As String, ByVal lpDefault As String, ByVal lpReturnedString As String, ByVal nSize As Long, ByVal lpFileName As String) As Long

Public strPHPInterpreter As String
Public intExecutionTimeout As Integer
Public intMinRefreshInterval As Integer

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

    strPHPInterpreter = ReadINI(App.Path & "\config.ini", "PHP", "interpreter", "")
    intExecutionTimeout = Int(ReadINI(App.Path & "\config.ini", "PHP", "timeout", 5))
    intMinRefreshInterval = Int(ReadINI(App.Path & "\config.ini", "PHP", "refresh", 3000))

End Sub

Public Function GetMinRefreshInterval() As Integer

    GetMinRefreshInterval = intMinRefreshInterval
    
End Function

'test function
Public Function function1(ByVal strParam1 As String, ByVal strParam2 As String) As Long
    
    Dim strBuff As String
    Dim strScript As String
    Dim strParameters As String
    Dim strCommand As String
    
    strScript = App.Path & "\scripts\" & ConvertParam(strParam1)
    strParameters = ConvertParam(strParam2)
    
    If Mid(strScript, Len(strScript) - 4) <> ".php" Then
    
        strScript = strScript & ".php"
        
    End If
    
    If Dir(strPHPInterpreter) = "" Then
        
        strBuff = "[PHP] php interpreter not found"
        
    Else
    
        If Dir(strScript) = "" Then
    
            strBuff = "[PHP] script not found"
    
        Else
        
            strCommand = FormatCommand(strPHPInterpreter, strScript, strParameters)
            strBuff = mdlCGI.RunCGI(strCommand)
        
        End If
        
    End If
   
    function1 = CreateReturn(strBuff)
    
End Function

'=============================================================================================
' Private functions.
' you must not modify those
'=============================================================================================

Private Function FormatCommand(ByVal strExecutable As String, ByVal strScript As String, ByVal strParameters As String) As String
    
    FormatCommand = strExecutable & " """ & strScript & """" & IIf(strParameters = "", "", " """ & strParameters & """")

End Function

'Simple function to convert the parameter from unicode
Private Function ConvertParam(ByVal strParam As String) As String
    ConvertParam = StrConv(strParam, vbUnicode)
End Function

'Function to return a pointer to the desired data
Private Function CreateReturn(ByVal strData As String) As Long
    Dim lngPointer As Long
    Dim lngLen As Long

    lngLen = Len(strData) 'lenght of data
    '&H1000 = MEM_COMMIT    &H40 = PAGE_EXECUTE_READWRITE
    lngPointer = VirtualAlloc(ByVal 0&, lngLen, &H1000, &H40) 'alloc a pointer with data's lenght
    VirtualLock lngPointer, lngLen 'lock pointer
    SetString strData, lngPointer 'write unicode string on pointer
    
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

Private Function ReadINI(inifile As String, topic As String, subtopic As String, default As String)
    Dim sData As String
    Dim lDataLen As Long
    Dim inidados As String
    
    sData = Space$(255)
    lDataLen = GetPrivateProfileString(topic, subtopic, default, sData, Len(sData), inifile)
    ReadINI = Left$(sData, lDataLen)
    
End Function

'Private Function ReadString(ByVal lngPointer As Long, ByVal lngLen As Long) As String
'    Dim i As Long
'    Dim b As Byte
'
'    For i = 0 To lngLen - 1
'        GetMem1 lngPointer + i, b
'        ReadString = ReadString & Chr(b)
'    Next i
'End Function
