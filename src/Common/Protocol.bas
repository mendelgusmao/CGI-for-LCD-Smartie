Attribute VB_Name = "Protocol"
Private Const DELIMITER = vbCrLf
Private Const HEADER = "cgi4lcd" & vbCrLf

Public Function Parse(ByVal data As String, ByRef objCommand As Command)
    
    If Left(data, Len(HEADER)) <> HEADER Then
        GoTo protocol_error
    End If
    
    Dim pieces As Variant
    
    pieces = Split(data, DELIMITER)
    
    If UBound(pieces) = 3 Then
        With objCommand
            .Line = pieces(1)
            .Timeout = Int(pieces(2))
            .Interval = Int(pieces(3))
            .Response = ""
        End With
        Exit Function
    End If
    
protocol_error:

    objCommand.Response = "[CGI4LCD] Error parsing command"
    objCommand.Malformed = True

End Function
    
Public Function Build(Command As Command) As String

    Build = HEADER & _
        Command.Line & DELIMITER & _
        Command.Interval & DELIMITER & _
        Command.Timeout & DELIMITER
        
End Function
