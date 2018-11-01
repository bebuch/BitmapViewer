#!/bin/bash

# exit script on any error
set -e
set -o xtrace

# set env variables
COMPILER=$1
COMPILER_VERSION=$($COMPILER --version | head -n1 | grep -Po '\d.\d.\d' | head -1)
MODE=$2
COMPILER_DIR=$COMPILER-$COMPILER_VERSION


PROJECT_DIR=$(pwd)
BIN_DIR=$PROJECT_DIR/bin/$COMPILER_DIR/$MODE
BUILD_DIR=$BIN_DIR/build

EXE_DIR=$BIN_DIR/AppDir/bin
USR_BIN_DIR=$BIN_DIR/AppDir/usr/bin
USR_LIB_DIR=$BIN_DIR/AppDir/usr/lib


# make build and deploy dirs
mkdir -p $BUILD_DIR
mkdir -p $EXE_DIR
mkdir -p $USR_BIN_DIR
mkdir -p $USR_LIB_DIR


# build
cd $BUILD_DIR
qmake "CONFIG+=$MODE" $PROJECT_DIR
make


# prepare AppImage

# add main executable
MAIN_EXE=$BUILD_DIR/BitmapViewer
cp $MAIN_EXE $EXE_DIR/

# add secondary executable
cp /bin/grep $USR_BIN_DIR

# add dependencies
ldd $MAIN_EXE
ldd $MAIN_EXE | grep -e "\.so" | grep -o '=> [^ ]*' | grep -o '[^=> ]*' | sort | sed -e 's/^/cp /' -e 's@$@ '"$USR_LIB_DIR/"'@' | bash


# create AppImage
appimagetool $BIN_DIR/AppDir
mv BitmapViewer-x86_64.AppImage BitmapViewer-$COMPILER_DIR-$MODE-x86_64.AppImage


# upload created files to Platin server
smbclient //$PLATIN_ADDRESS/Austausch -U "$PLATIN_USER%$PLATIN_PASS" -D //GitLab-Bot -c "mkdir BitmapViewer"
smbclient //$PLATIN_ADDRESS/Austausch -U "$PLATIN_USER%$PLATIN_PASS" -D //GitLab-Bot/BitmapViewer -c "put BitmapViewer-$COMPILER_DIR-$MODE-x86_64.AppImage"


# remove created files
rm BitmapViewer-$COMPILER_DIR-$MODE-x86_64.AppImage
rm -r $BIN_DIR/AppDir
