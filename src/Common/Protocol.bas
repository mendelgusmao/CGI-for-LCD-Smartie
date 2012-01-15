Attribute VB_Name = "Protocol"
Private Const DELIMITER = "|"
Private Const HEADER = "cgi4lcd"

Public Sub Parse(ByVal data As String, ByRef Command As Command)
    
    If Left(data, Len(HEADER)) = HEADER Then
        Dim pieces As Variant
        
        pieces = Split(data, DELIMITER)
        
        If UBound(pieces) = 3 Then
            With Command
                .Executable = pieces(1)
                .Arguments = pieces(2)
                .Timeout = Int(pieces(3))
                .Interval = Int(pieces(4))
                .Response = ""
            End With
            Exit Sub
        End If
    End If

    Command.Response = "[CGI4LCD] Error parsing command"
    Command.isMalformed = True

End Sub
    
Public Function Build(Command As Command) As String

    Build = HEADER & DELIMITER & _
        Command.Executable & DELIMITER & _
        Command.Arguments & DELIMITER & _
        Command.Interval & DELIMITER & _
        Command.Timeout & DELIMITER
        
End Function
