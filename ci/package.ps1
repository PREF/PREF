cd PREF\release

windeployqt --release PREF.exe
7z a -tzip %APPVEYOR_BUILD_VERSION%.zip *
Move-Item -Path %APPVEYOR_BUILD_VERSION%.zip -Destination ..\..

cd ..\..
