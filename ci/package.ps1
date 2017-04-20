cd PREF\release

$archive = $env:appveyor_build_version + ".zip"

windeployqt --release PREF.exe
7z a -tzip $archive *
Move-Item -Path $archive -Destination ..\..

cd ..\..
