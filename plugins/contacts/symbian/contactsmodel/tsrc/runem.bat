@echo off
rem
rem Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
rem Contact: http://www.qt-project.org/legal
rem This component and the accompanying materials are made available
rem under the terms of "Eclipse Public License v1.0"
rem which accompanies this distribution, and is available
rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
rem
rem Initial Contributors:
rem Nokia Corporation - initial contribution.
rem
rem Contributors:
rem
rem Description:
rem
@echo on

if "%1"=="" set _whatvar_=udeb
call ..\tsrc\depend run
for %%i in (%_depend%) do \epoc32\release\wins\%_whatvar_%\%%i.exe
set _depend=
set _whatvar_=
