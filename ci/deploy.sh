#! /bin/sh

DEPLOY_DIR=deploy
OS_NAME=`uname`
ARCH=`uname -m`
BUILD_DATE=`date +%d%m%Y`
BUILD_ID="PREF_"$OS_NAME"_"$ARCH"_"$BUILD_DATE
BUILD_REPO="pref.github.io"

mkdir $DEPLOY_DIR
cp PREF/PREF $DEPLOY_DIR  # Copy executable

cd $DEPLOY_DIR
zip -r ../$BUILD_ID.zip * # Generate archive
cd ..

# Cleanup temporary files
rm -rf PREF/
rm -rf PrefLib/
rm Makefile
rm -rf $DEPLOY_DIR

# Prepare deploy
rm -rf $BUILD_REPO
git clone -b builds https://${GH_TOKEN}@github.com/PREF/$BUILD_REPO.git

cd $BUILD_REPO
rm -rf *$OS_NAME*
mv ../$BUILD_ID.zip .

git config user.email "buildbot@none.io"
git config user.name "Travis Build Bot"
git add -A .
git commit -m "Updated Linux Nightly $BUILD_DATE"
git push origin builds