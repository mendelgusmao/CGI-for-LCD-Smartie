VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "Client"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private WithEvents TimerUnlock As TimerEx
Attribute TimerUnlock.VB_VarHelpID = -1
Private Socket As SocketMaster
Private Locked As Boolean
Private iniFile As String
Private Response As String
Private ScriptsPath As String

Private Sub Class_Initialize()
    Set Socket = New SocketMaster
    Set TimerUnlock = New TimerEx
End Sub

Public Sub Start(RemotePort As Long, Ini_File As String, Scripts_Path As String)

    iniFile = Ini_File
    ScriptsPath = Scripts_Path
    
    With Socket
        .CloseSck
        .Protocol = sckUDPProtocol
        .RemoteHost = "255.255.255.255"
        .RemotePort = RemotePort
    End With
    
    Locked = False
   
End Sub

Public Function Execute(ByVal Script As String, ByVal Parameters As String, Optional Version As Boolean = False) As String

    Dim Buffer As String
    Dim Arguments As String
    Dim Extension As String
    Dim Interpreter As String
    Dim ExecutionInterval As Integer
    Dim ExecutionTimeout As Integer
    
    Dim Vars As Scripting.Dictionary
    
    Extension = GetExtension(Script)
   
    If Extension = "" Then
    
        Extension = Ini.Read(iniFile, "cgi4lcd", "default", "")
        Script = Script & "." & Extension
        
    End If
   
    Interpreter = Ini.Read(iniFile, Extension, "interpreter", "")
   
    If Interpreter = "" Or Dir(Interpreter) = "" Then
    
        Execute = "[CGI4LCD] Interpreter for extension '" & Extension & "' not found"
        Exit Function
        
    End If
   
    
    ExecutionInterval = Int(Ini.Read(iniFile, "cgi4lcd", "interval", 15)) * 1000
    ExecutionTimeout = Int(Ini.Read(iniFile, "cgi4lcd", "timeout", 60)) * 1000
    
    If Version Then
    
        Arguments = Ini.Read(iniFile, Extension, "version", "")
    
    Else
    
        Arguments = Ini.Read(iniFile, Extension, "command", "")
    
        If Dir(ScriptsPath & Script, vbNormal) = "" Then
    
            Execute = "[CGI4LCD] Script '" & Script & "' not found"
            Exit Function
        
        End If
        
    End If
    
    Set Vars = New Scripting.Dictionary
    With Vars
        .Add "interpreter", Interpreter
        .Add "scripts_path", ScriptsPath
        .Add "bootstraps_path", ScriptsPath & "bootstraps\"
        .Add "script", Script
        .Add "arguments", Arguments
        .Add "'", """"
    End With
    
    Arguments = FormatCommand(Arguments, Vars)

    Execute = Request(Interpreter, Arguments, ExecutionInterval, ExecutionTimeout)
   
End Function

Public Function Request(ByVal Interpreter As String, ByVal Arguments As String, ByVal Interval As Integer, ByVal Timeout As Integer) As String

    Dim RequestString As String
    Dim Command As Command
    Set Command = New Command
    
    Command.Executable = Interpreter
    Command.Arguments = Arguments
    Command.Interval = Interval
    Command.Timeout = Timeout

    RequestString = Protocol.Build(Command)

    Debug.Print RequestString

    Socket.SendData RequestString

    TimerUnlock.Interval = 1000
    TimerUnlock.Enabled = True

    'While Locked
    '    ' Wait for data arrival
    '    DoEvents
    'Wend
    
    Request = Response

End Function

Private Sub Socket_DataArrival(ByVal bytesTotal As Long)

    Dim Buffer As String
  
    Socket.GetData Buffer, vbString
    Locked = False
    Response = Buffer
    
End Sub

Private Sub Socket_OnTimer()
    Locked = False
    Response = "[CGI4LCD] Server timeout"
End Sub

Public Function GetExtension(File As String)

    If InStr(File, ".") > 0 Then
        Dim pieces As Variant
        pieces = Split(File, ".")
        
        GetExtension = pieces(UBound(pieces))
    Else
        GetExtension = ""
    End If
    
End Function

Public Function FormatCommand(ByVal Template As String, ByRef Vars As Scripting.Dictionary)
    
    Dim Variable

    FormatCommand = Template

    For Each Variable In Vars
        FormatCommand = Replace(FormatCommand, "%" & Variable, Vars.Item(Variable))
    Next Variable

End Function


