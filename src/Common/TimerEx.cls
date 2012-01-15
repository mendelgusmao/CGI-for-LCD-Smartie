VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "TimerEx"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

'* The GetTickCount function retrieves the number of milliseconds
'* that have elapsed since Windows was started.
Private Declare Function GetTickCount Lib "kernel32" () As Long

Private mblnEnabled             As Boolean
Private mlngInterval            As Long
Private mstrTag                 As String
Private mlngTickCount           As Long

'* This is the timer event that will fire in a given interval
Public Event OnTimer()

'* A type that will hold the extended information about the interval you want to set
'* If you set different types of intervals, the total interval will
'* be calculated combining all types


'* You can see whether the timer is Enabled by this property
Public Property Get Enabled() As Boolean
    Enabled = mblnEnabled
End Property

'* You can start / stop the timer by this property
Public Property Let Enabled(blnEnabled As Boolean)
    mblnEnabled = blnEnabled
    If blnEnabled Then
        mlngTickCount = GetTickCount
        Call TimerLoop
    End If
End Property

'* Conventional Interval property of the timer, you can check how many milliseconds
'* have been set for the timer
Public Property Get Interval() As Long
    Interval = mlngInterval
End Property

'* Conventional Interval property of the timer, you can set interval of the timer
'* in milliseconds
Public Property Let Interval(lngInterval As Long)
    mlngInterval = lngInterval
End Property

'* Check what info is in the Tag property in the timer, you can store any string data
'* into this property
Public Property Get Tag() As String
    Tag = mstrTag
End Property

'* You can store any string data into this property as extra info of your timer
Public Property Let Tag(strTag As String)
    mstrTag = strTag
End Property

'* Core of the timer. It fires the OnTimer event in a timely fashion according to
'* the Interval / IntervalInfo you have set
Private Sub TimerLoop()
    Do While Not mblnEnabled = False
        If GetTickCount - mlngTickCount >= mlngInterval Then
            RaiseEvent OnTimer
            mlngTickCount = GetTickCount
        '* Like GetTickCount has exceeded its capacity,
        '* run over from the beginning
        ElseIf GetTickCount = 0 Then
            mlngTickCount = 0
        ElseIf GetTickCount < mlngTickCount Then
            mlngTickCount = 0
        End If
        DoEvents
    Loop
End Sub

'* ENJOY!!


