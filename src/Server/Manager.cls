VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "Manager"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Public Calls As Collection

Private Sub Class_Initialize()
    Set Calls = New Collection
End Sub

Private Sub Class_Terminate()
    Set Calls = Nothing
End Sub

Public Sub AddFunction(objPHPFunction As Command)

    If TypeOf Calls(objPHPFunction.Line) Is Command Then
        Calls(objPHPFunction.Line).CleanupTimer = 0
    Else
        Calls.Add objPHPFunction, objPHPFunction.Line
    End If
    
End Sub

Public Sub ExecuteQueue()

    Dim I As Integer
    
    Dim objCall As Command
    
    For I = 0 To Calls.Count() - 1
        
        Calls(I).Timer = Calls(I).Timer + 1
        Calls(I).CleanupTimer = Calls(I).CleanupTimer + 1
    
        If Calls(I).CleanupTimer >= Calls(I).Timeout Then
            Calls.Remove I
        Else
            If Calls(I).Timer >= Calls(I).Interval Then
                ExecuteFunction Calls(I)
                Calls(I).Timer = 0
            End If
        End If
        
    Next

End Sub

Private Sub ExecuteFunction(ByRef objCall As Command)

    Set objDOS = New DOSOutputs
    objCall.Response = objDOS.ExecuteCommand(objCall.Line)

End Sub
