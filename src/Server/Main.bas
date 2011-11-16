Attribute VB_Name = "mdlMain"
Private Server As Server
Public iniFile As String

Sub Main()
    
    If App.PrevInstance Then
        End
    End If
    
    iniFile = App.Path & "\scripts\cgi4lcd.ini"
    
    Set Server = New Server
    Server.Start _
        Val(Ini.Read(iniFile, "smartie", "local_port", 65432)), _
        Val(Ini.Read(iniFile, "smartie", "remote_port", 65433))
    
End Sub


