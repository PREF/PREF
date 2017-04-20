cd PREF\release

windeployqt --release PREF.exe
7z a -tzip (get-item env:appveyor_build_version).zip *
Move-Item -Path (get-item env:appveyor_build_version).zip -Destination ..\..

cd ..\..
