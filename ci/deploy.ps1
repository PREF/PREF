git config core.autocrlf true
git config --global credential.helper store
Add-Content "$env:USERPROFILE\.git-credentials" "https://$($env:github_token):x-oauth-basic@github.com`n"

if(Test-Path $($env:build_repo)) {
    Remove-Item $($env:build_repo) -Recurse -Force 
}

git clone -b builds https://$($env:github_token)@github.com/PREF/$($env:build_repo).git > $null 2>&1
cd $($env:build_repo)
Remove-Item *Windows* 
Move-Item -Path ../$($env:appveyor_build_version).zip -Destination .
    
if(Test-Path $($env:appveyor_build_version))
{
    git config --global user.email "buildbot@none.io"
    git config --global user.name "AppVeyor Build Bot"
    git add -A .
    git commit -m "Updated Windows Nightly $(Get-Date -format ddMMyyyy)"
    git push --quiet origin builds > $null 2>&1 
}