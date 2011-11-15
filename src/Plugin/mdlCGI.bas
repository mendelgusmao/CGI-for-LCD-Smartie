Attribute VB_Name = "mdlCGI"
Option Explicit
Option Base 0

Private Const INFINITE = -1&

Private Type STARTUPINFO
  cb As Long
  lpReserved As String
  lpDesktop As String
  lpTitle As String
  dwX As Long
  dwY As Long
  dwXSize As Long
  dwYSize As Long
  dwXCountChars As Long
  dwYCountChars As Long
  dwFillAttribute As Long
  dwFlags As Long
  wShowWindow As Integer
  cbReserved2 As Integer
  lpReserved2 As Long
  hStdInput As Long
  hStdOutput As Long
  hStdError As Long
End Type

Private Type PROCESS_INFORMATION
  hProcess As Long
  hThread As Long
  dwProcessID As Long
  dwThreadID As Long
End Type

Private objDOS As DOSOutputs

Public Function RunCGI(executable As String) As String

    Set objDOS = New DOSOutputs

    RunCGI = objDOS.ExecuteCommand(executable)
    
End Function
