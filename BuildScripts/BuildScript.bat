set UPROJECTDIR=%1
set UPROJECTFILENAME=%2
set UPROJECTPATH=%UPROJECTDIR%\%UPROJECTFILENAME%
set ARCHIVEDIR=%UPROJECTDIR%\BuildMachineArchive
set BUILDCONFIG=%3
set TARGETPLATFORMS=%4
set RUNUATPATH=%5
del /s /f /q %ARCHIVEDIR%
%RUNUATPATH% BuildCookRun -project=%UPROJECTPATH% -platform=%TARGETPLATFORMS% -clientconfig=%BUILDCONFIG% -build -cook -archive -archivedirectory=%ARCHIVEDIR% -stage -pak -prereqs
exit /B %ERRORLEVEL%