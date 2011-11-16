VERSION 5.00
Begin VB.Form frmMain 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "CGI 4 LCD Smartie - Test"
   ClientHeight    =   2445
   ClientLeft      =   45
   ClientTop       =   465
   ClientWidth     =   10560
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2445
   ScaleWidth      =   10560
   StartUpPosition =   2  'CenterScreen
   Begin VB.CommandButton txtExecute 
      Caption         =   "Execute"
      Height          =   375
      Left            =   3360
      TabIndex        =   9
      Top             =   1800
      Width           =   1215
   End
   Begin VB.TextBox txtParams 
      Height          =   285
      Left            =   1200
      TabIndex        =   8
      Text            =   "14;08;2011"
      Top             =   1320
      Width           =   3375
   End
   Begin VB.TextBox Text4 
      Height          =   285
      Left            =   1200
      TabIndex        =   4
      Text            =   "do_some_math"
      Top             =   960
      Width           =   3375
   End
   Begin VB.OptionButton Option2 
      Caption         =   "Version"
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   720
      Width           =   855
   End
   Begin VB.OptionButton Option1 
      Caption         =   "Function"
      Height          =   255
      Left            =   120
      TabIndex        =   5
      Top             =   960
      Value           =   -1  'True
      Width           =   1215
   End
   Begin VB.TextBox txtResponse 
      Height          =   1935
      Left            =   4680
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   2
      Top             =   360
      Width           =   5775
   End
   Begin VB.TextBox txtScript 
      Height          =   285
      Left            =   120
      TabIndex        =   1
      Text            =   "test"
      Top             =   360
      Width           =   4455
   End
   Begin VB.Label Label4 
      Caption         =   "Parameters"
      Height          =   255
      Left            =   120
      TabIndex        =   7
      Top             =   1320
      Width           =   1335
   End
   Begin VB.Label Label3 
      Caption         =   "Response"
      Height          =   255
      Left            =   4680
      TabIndex        =   3
      Top             =   120
      Width           =   855
   End
   Begin VB.Label Label2 
      Caption         =   "Script"
      Height          =   255
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   855
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim Client As Client

Public Sub Form_Load()

    Dim ScriptsPath As String
    Dim iniFile As String

    ScriptsPath = App.Path & "\scripts\"
    iniFile = ScriptsPath & "cgi4lcd.ini"
        
    Set Client = New Client
    
    ' Local and remote ports are swapped in client
    Client.Start _
        Val(Ini.Read(iniFile, "cgi4lcd", "remote_port", 65433)), _
        Val(Ini.Read(iniFile, "cgi4lcd", "local_port", 65432)), _
        iniFile, ScriptsPath
   
End Sub

Private Sub txtExecute_Click()

    txtResponse.Text = Client.Execute(txtScript.Text, txtParams.Text)

End Sub
