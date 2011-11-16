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

Private Manager As Manager
Private WithEvents TimerExecute As TimerEx
Attribute TimerExecute.VB_VarHelpID = -1
Private WithEvents Socket As SocketMaster
Attribute Socket.VB_VarHelpID = -1

Sub Class_Initialize()
    Set Manager = New Manager
    Set TimerExecute = New TimerEx
    Set Socket = New SocketMaster
End Sub

Private Sub Class_Terminate()

    Socket.CloseSck
    Set Socket = Nothing
    Set Manager = Nothing
    Set TimerExecute = Nothing

End Sub

Public Sub Start(LocalPort As Long, RemotePort As Long)

    With Socket
        .CloseSck
        .Protocol = sckUDPProtocol
        .RemoteHost = "255.255.255.255"
        .LocalPort = LocalPort
        .RemotePort = RemotePort
        .Bind .LocalPort
    End With
    
    TimerExecute.Interval = 1000
    TimerExecute.Enabled = True
    
End Sub

Private Sub Socket_DataArrival(ByVal bytesTotal As Long)

    Dim objCommand As Command
    Dim buffer As String
    
    Socket.GetData buffer, vbString
    
    Debug.Print "Data arrived: " & buffer
    Protocol.Parse buffer, objCommand
    
    If Not objCommand.Malformed Then
        Manager.AddFunction objCommand
        Socket.SendData objCommand.Response
    End If
    
End Sub

Private Sub TimerExecute_OnTimer()

    Manager.ExecuteQueue
    
End Sub

