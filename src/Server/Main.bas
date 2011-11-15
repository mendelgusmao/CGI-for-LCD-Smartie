Attribute VB_Name = "mdlMain"
Private Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As String, ByVal lpDefault As String, ByVal lpReturnedString As String, ByVal nSize As Long, ByVal lpFileName As String) As Long

Private Server As Server

Sub Main()
    
    Set Server = New Server
    Server.Start
    
End Sub

Public Function Ini_Read(iniFile As String, topic As String, subtopic As String, default As String)
    Dim sData As String
    Dim lDataLen As Long
    Dim inidados As String
    
    sData = Space$(255)
    lDataLen = GetPrivateProfileString(topic, subtopic, default, sData, Len(sData), iniFile)
    ReadINI = Left$(sData, lDataLen)
    
End Function
