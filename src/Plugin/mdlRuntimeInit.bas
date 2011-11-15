Attribute VB_Name = "mdlRuntimeInit"
'---------------------------------------------------------------------------------------
' Module    :   MRuntimeInit
' Purpose   :   VB runtime initialization.
' Warnings  :   DO NOT MODIFY THIS CODE UNLESS YOU KNOW EXACTLY WHAT YOU ARE DOING.
'
' This file depends on:
'   References:
'     vbAdvance.tlb (vbAdvance Type Library) Needed only at compile-time.
'   Files:
'     CRuntimeInit.cls
'
'---------------------------------------------------------------------------------------
Option Explicit

'==============================================================================
'FunctionDelegator code from Feb.2000 VBPJ article "Call Function Pointers"
'by Matthew Curland
'http://www.powervb.com
'------------------------------------------------------------------------------
Private Const cDelegateASM As Currency = -368956918007638.6215@
Private m_DelegateASM As Currency
Private Type DelegatorVTables
    VTable(7) As Long
End Type
Private m_VTables As DelegatorVTables
Private m_pVTableOKQI As Long
Private m_pVTableFailQI As Long
Public Type FunctionDelegator
    pVTable As Long
    pfn As Long
End Type
'END FUNCTION DELEGATOR CODE
'==============================================================================

'Object reference which keeps runtime alive:
Private moInitObject As clsRuntimeInit

'------------------------------------------------------------------------------
' Procedure :   RuntimeInitialize
' Purpose   :   VB runtime initialization. Pass handle to the DLL that this
'               code resides in.
'
'               DO NOT MODIFY THIS ROUTINE UNLESS YOU KNOW EXACTLY WHAT YOU
'               ARE DOING.
'
'------------------------------------------------------------------------------
Public Sub RuntimeInitialize(ByVal hMod As Long)
    
    Dim sFile As String
    Dim lLen As Long
    Dim lRet As Long
    Dim i As Long
    Dim lpTypeLib As Long
    Dim TLI As ITypeLib
    Dim lppTypeInfo As Long
    Dim TI As ITypeInfo
    Dim sName As String
    Dim pAttr As Long
    Dim TA As TYPEATTR
    Dim IID_ClassFactory As VBGUID
    Dim IID_IUnknown As VBGUID
    Dim pGetClass As Long
    Dim pCall As ICallDLLGetClassObject
    Dim FD As FunctionDelegator
    Dim pICF As IClassFactory
    Dim pUnk As IUnknown
    
    'Make sure parent process is not VB IDE:
    If GetModuleHandle("VBA6.DLL") <> 0 Then Exit Sub
    If GetModuleHandle("VBA5.DLL") <> 0 Then Exit Sub
    
    sFile = Space$(260)
    lLen = Len(sFile)
    lRet = GetModuleFileName(hMod, sFile, lLen)
    If lRet Then
        sFile = Left$(sFile, lLen - 1)
        lpTypeLib = LoadTypeLibEx(sFile, REGKIND_NONE)
        CopyMemory TLI, lpTypeLib, 4
        For i = 0 To TLI.GetTypeInfoCount - 1
            If TLI.GetTypeInfoType(i) = TKIND_COCLASS Then
                lppTypeInfo = TLI.GetTypeInfo(i)
                CopyMemory TI, lppTypeInfo, 4
                TI.GetDocumentation DISPID_UNKNOWN, sName, "", 0, ""
                If lstrcmp(sName, "clsRuntimeInit") = 0 Then
                    pAttr = TI.GetTypeAttr
                    CopyMemory TA, ByVal pAttr, Len(TA)
                    TI.ReleaseTypeAttr pAttr
                    If TA.wTypeFlags Then
                        Exit For
                    End If
                End If
            End If
        Next i
        With IID_ClassFactory
            .Data1 = 1
            .Data4(0) = &HC0
            .Data4(7) = &H46
        End With
        With IID_IUnknown
            .Data4(0) = &HC0
            .Data4(7) = &H46
        End With
        pGetClass = GetProcAddress(hMod, "DllGetClassObject")
        If pGetClass Then
            CopyMemory pCall, InitDelegator(FD, pGetClass), 4
            lRet = pCall.Call(TA.iid, IID_ClassFactory, pICF)
            If lRet <> CLASS_E_CLASSNOTAVAILABLE Then
                lRet = pICF.CreateInstance(0&, IID_IUnknown, pUnk)
                If lRet = S_OK Then
                    Set moInitObject = pUnk
                    moInitObject.InitVBCall
                    CopyMemory pCall, 0&, 4
                    Set pICF = Nothing
                    Set pUnk = Nothing
                End If
            End If
        End If
    End If
End Sub

'==============================================================================
'FunctionDelegator code from Feb.2000 VBPJ article "Call Function Pointers"
'by Matthew Curland
'http://www.powervb.com
'------------------------------------------------------------------------------
Public Function InitDelegator(Delegator As FunctionDelegator, Optional ByVal pfn As Long) As IUnknown
    If m_pVTableOKQI = 0 Then InitVTables
    With Delegator
        .pVTable = m_pVTableOKQI
        .pfn = pfn
    End With
    CopyMemory InitDelegator, VarPtr(Delegator), 4
End Function
Private Sub InitVTables()
Dim pAddRefRelease As Long
    With m_VTables
        .VTable(0) = FuncAddr(AddressOf QueryInterfaceOK)
        .VTable(4) = FuncAddr(AddressOf QueryInterfaceFail)
        pAddRefRelease = FuncAddr(AddressOf AddRefRelease)
        .VTable(1) = pAddRefRelease
        .VTable(5) = pAddRefRelease
        .VTable(2) = pAddRefRelease
        .VTable(6) = pAddRefRelease
        m_DelegateASM = cDelegateASM
        .VTable(3) = VarPtr(m_DelegateASM)
        .VTable(7) = .VTable(3)
        m_pVTableOKQI = VarPtr(.VTable(0))
        m_pVTableFailQI = VarPtr(.VTable(4))
    End With
End Sub
Private Function QueryInterfaceOK(This As FunctionDelegator, riid As Long, pvObj As Long) As Long
    pvObj = VarPtr(This)
    This.pVTable = m_pVTableFailQI
End Function
Private Function AddRefRelease(ByVal This As Long) As Long
End Function
Private Function QueryInterfaceFail(ByVal This As Long, riid As Long, pvObj As Long) As Long
    pvObj = 0
    QueryInterfaceFail = E_NOINTERFACE
End Function
Private Function FuncAddr(ByVal pfn As Long) As Long
    FuncAddr = pfn
End Function
'END FUNCTION DELEGATOR CODE
'==============================================================================
