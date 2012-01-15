VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "Command"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Private mvarTimeout As Integer
Private mvarInterval As Integer
Private mvarTimer As Integer
Private mvarCleanupTimer As Integer
Private mvarResponse As String
Private mvarisMalformed As Boolean
Private mvarExecutable As String
Private mvarArguments As String

Public Function Line()
    Line = mvarExecutable & " " & mvarArguments
End Function

Public Property Let Arguments(ByVal vData As String)
    mvarArguments = vData
End Property

Public Property Get Arguments() As String
    Arguments = mvarArguments
End Property

Public Property Let Executable(ByVal vData As String)
    mvarExecutable = vData
End Property

Public Property Get Executable() As String
    Executable = mvarExecutable
End Property

Public Property Let isMalformed(ByVal vData As Boolean)
    mvarisMalformed = vData
End Property

Public Property Get isMalformed() As Boolean
    isMalformed = mvarisMalformed
End Property

Public Property Let Response(ByVal vData As String)
    mvarResponse = vData
End Property

Public Property Get Response() As String
    Response = mvarResponse
End Property

Public Property Let CleanupTimer(ByVal vData As Integer)
    mvarCleanupTimer = vData
End Property

Public Property Get CleanupTimer() As Integer
    CleanupTimer = mvarCleanupTimer
End Property

Public Property Let Timer(ByVal vData As Integer)
    mvarTimer = vData
End Property

Public Property Get Timer() As Integer
    Timer = mvarTimer
End Property

Public Property Let Interval(ByVal vData As Integer)
    mvarInterval = vData
End Property

Public Property Get Interval() As Integer
    Interval = mvarInterval
End Property

Public Property Let Timeout(ByVal vData As Integer)
    mvarTimeout = vData
End Property

Public Property Get Timeout() As Integer
    Timeout = mvarTimeout
End Property
