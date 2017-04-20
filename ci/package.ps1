cd PREF\release

windeployqt --release PREF.exe
7z a -tzip $($env:appveyor_build_version).zip *
Move-Item -Path $($env:appveyor_build_version).zip -Destination ..\..

cd ..\..
