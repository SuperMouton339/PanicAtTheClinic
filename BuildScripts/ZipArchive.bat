set UPROJECTDIR=%1
set ZIPNAME=%2
set ARCHIVEDIR=%UPROJECTDIR%\BuildMachineArchive\
set ZIPDIR=%UPROJECTDIR%\BuildMachineArchiveZip\
del /s /f /q %ZIPDIR%
"C:\Program Files\7-Zip\7z.exe" a -tzip %ZIPDIR%\%ZIPNAME%.zip %ARCHIVEDIR%
exit /B %ERRORLEVEL%