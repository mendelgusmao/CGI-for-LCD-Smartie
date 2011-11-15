VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "Server"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private Const DiscoveryPort As Long = 65432
Private Const ResponsePort As Long = 65433

Private Manager As Manager
Private WithEvents TimerExecute As TimerEx
Attribute TimerExecute.VB_VarHelpID = -1
Private WithEvents Socket As SocketMaster
Attribute Socket.VB_VarHelpID = -1
Public iniFile As String

Sub Class_Initialize()
    Set Manager = New Manager
    Set TimerExecute = New TimerEx
    Set Socket = New SocketMaster
    
    iniFile = App.Path & "\scripts\php.ini"
End Sub

Private Sub Class_Terminate()

    Socket.CloseSck
    Set Socket = Nothing
    Set Manager = Nothing
    Set TimerExecute = Nothing

End Sub

Public Sub Start()
    With Socket
        .CloseSck
        .Protocol = sckUDPProtocol
        .RemoteHost = "255.255.255.255"
        .LocalPort = mdlMain.Ini_Read(iniFile, "smartie", "local_port", 65432)
        .RemotePort = mdlMain.Ini_Read(iniFile, "smartie", "remote_port", 65433)
        .Bind .LocalPort
    End With
    
    TimerExecute.Interval = 1000
    TimerExecute.Enabled = True
End Sub

Private Sub Socket_DataArrival(ByVal bytesTotal As Long)

    Dim objCommand As Command
    Dim buffer As String
    
    Socket.GetData buffer, vbString
    
    Parse buffer, objCommand
    Manager.AddFunction objCommand
    
    Socket.SendData objCommand.Response
    
End Sub

Private Sub TimerExecute_OnTimer()
    Manager.ExecuteQueue
End Sub

Private Function Parse(ByVal data As String, ByRef objCommand As Command)
    
    Dim pieces() As String
    
    pieces = Split(data, Chr(1))
    
    If UBound(pieces) >= 1 Then
        With objCommand
            .Line = pieces(1)
            .Timeout = pieces(2)
            .Interval = pieces(3)
            .Response = ""
        End With
    Else
        objCommand.Response = "[PHP4LCD] Error parsing command"
    End If
    
End Function

