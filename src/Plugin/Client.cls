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

Public Sub Start(LocalPort As Long, RemotePort As Long, Ini_File As String, Scripts_Path As String)

    iniFile = Ini_File
    ScriptsPath = Scripts_Path
    
    With Socket
        .CloseSck
        .Protocol = sckUDPProtocol
        .RemoteHost = "255.255.255.255"
        .LocalPort = LocalPort
        .RemotePort = RemotePort
        .Bind .LocalPort
    End With
    
    Locked = False
   
End Sub

Public Function Execute(ByVal Script As String, ByVal Parameters As String, Optional Version As Boolean = False) As String

    Dim Buffer As String
    Dim Command As String
    Dim Extension As String
    Dim Interpreter As String
    Dim ExecutionInterval As Integer
    Dim ExecutionTimeout As Integer
    
    Extension = GetExtension(Script)
   
    If Extension = "" Then
    
        Extension = Ini.Read(iniFile, "cgi4lcd", "default", "")
        Script = Script & "." & Extension
        
    End If
   
    If Ini.Read(iniFile, Extension, "language", "") = "" Then
    
        Execute = "[CGI4LCD] Interpreter for extension '" & Extension & "' not found"
        Exit Function
        
    End If
   
    Interpreter = Ini.Read(iniFile, Extension, "interpreter", "")
    Command = Ini.Read(iniFile, Extension, "command", "")
    ExecutionInterval = Int(Ini.Read(iniFile, Extension, "interval", 30000))
    ExecutionTimeout = Int(Ini.Read(iniFile, Extension, "timeout", 30000))
    
    If Dir(Interpreter, vbNormal) = "" Then
        
        Execute = "[CGI4LCD] Interpreter not found"
        Exit Function
        
    End If
    
    Script = ScriptsPath & Script
    
    If Dir(Script, vbNormal) = "" Then

        Execute = "[CGI4LCD] Script not found"
        Exit Function
    
    End If
    
    Command = FormatCommand(Command, Interpreter, Script, Parameters)

    Execute = Me.Request(Command, ExecutionInterval, ExecutionTimeout)
   
End Function


Public Function Request(ByVal CommandLine As String, ByVal Interval As Integer, ByVal Timeout As Integer) As String

    Dim RequestString As String
    Dim Command As Command
    Set Command = New Command
    
    Command.Line = CommandLine
    Command.Interval = Interval
    Command.Timeout = Timeout

    RequestString = Protocol.Build(Command)

    Debug.Print RequestString

    Socket.SendData RequestString

    TimerUnlock.Interval = 3000
    TimerUnlock.Enabled = True

    While Locked
        ' Wait for data arrival
        DoEvents
    Wend
    
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

Public Function FormatCommand(Template As String, Interpreter As String, Script As String, Params As String)

    FormatCommand = Template
    
    FormatCommand = Replace(FormatCommand, "%interpreter", Interpreter)
    FormatCommand = Replace(FormatCommand, "%scripts_path", ScriptsPath)
    FormatCommand = Replace(FormatCommand, "%bootstraps_path", ScriptsPath & "bootstraps\")
    FormatCommand = Replace(FormatCommand, "%script", Script)
    FormatCommand = Replace(FormatCommand, "%params", Params)
    
    FormatCommand = Replace(FormatCommand, "'", """")

End Function


