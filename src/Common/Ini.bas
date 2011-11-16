Attribute VB_Name = "Ini"
Option Explicit

Private Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" (ByVal lpApplicationName As String, ByVal lpKeyName As String, ByVal lpDefault As String, ByVal lpReturnedString As String, ByVal nSize As Long, ByVal lpFileName As String) As Long

Public Function Read(ByVal iniFile As String, topic As String, subtopic As String, default As String)
    Dim sData As String
    Dim lDataLen As Long
    Dim inidados As String
    
    sData = Space$(255)
    lDataLen = GetPrivateProfileString(topic, subtopic, default, sData, Len(sData), iniFile)
    Read = Left$(sData, lDataLen)
    
End Function

