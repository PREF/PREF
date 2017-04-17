#! /bin/sh

DEPLOY_DIR=deploy
OS_NAME=`uname`
ARCH=`uname -m`
BUILD_DATE=`date +%d%m%Y`
BUILD_ID=$OS_NAME"_"$ARCH"_"$BUILD_DATE

echo "Generating nightly build archive in '$PWD'"

mkdir $DEPLOY_DIR
cp PREF $DEPLOY_DIR #Copy executable
zip -r $BUILD_ID.zip $DEPLOY_DIR
rm -rf $DEPLOY_DIR

echo "$BUILD_ID.zip generated"