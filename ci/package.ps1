cd PREF\release

windeployqt --release PREF.exe
7z a $($env:appveyor_build_version).zip *
Move-Item -Path $($env:appveyor_build_version).zip -Destination ..\..

cd ..\..
