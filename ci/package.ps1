$archive = $env:appveyor_build_version + ".zip"

cd PREF\release
mkdir deploy

Copy-Item PREF.exe deploy
cd deploy
windeployqt --release PREF.exe

7z a -tzip $archive *
Move-Item -Path $archive -Destination ..\..\..

cd ..
Remove-Item deploy -Recurse -Force 
cd ..\..
