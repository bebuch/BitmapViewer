#!/bin/bash

# exit script on any error
set -e
set -o xtrace

PROJECT_DIR=$(pwd)

# Parse compiler versions from build directories
cd bin
GCC_VERSION=$(ls | grep -Po 'gcc-\d.\d.\d' | grep -Po '\d.\d.\d')
CLANG_VERSION=$(ls | grep -Po 'clang-\d.\d.\d' | grep -Po '\d.\d.\d')

# create AppImages
cd $PROJECT_DIR

cp -r appimage/* bin/gcc-$GCC_VERSION/debug/AppDir
appimagetool bin/gcc-$GCC_VERSION/debug/AppDir
mv BitmapViewer-x86_64.AppImage BitmapViewer-gcc-$GCC_VERSION-debug-x86_64.AppImage

cp -r appimage/* bin/gcc-$GCC_VERSION/release/AppDir
appimagetool bin/gcc-$GCC_VERSION/release/AppDir
mv BitmapViewer-x86_64.AppImage BitmapViewer-gcc-$GCC_VERSION-release-x86_64.AppImage

cp -r appimage/* bin/clang-$CLANG_VERSION/debug/AppDir
appimagetool bin/clang-$CLANG_VERSION/debug/AppDir
mv BitmapViewer-x86_64.AppImage BitmapViewer-clang-$CLANG_VERSION-debug-x86_64.AppImage

cp -r appimage/* bin/clang-$CLANG_VERSION/release/AppDir
appimagetool bin/clang-$CLANG_VERSION/release/AppDir
mv BitmapViewer-x86_64.AppImage BitmapViewer-clang-$CLANG_VERSION-release-x86_64.AppImage


# upload created files to Platin server
smbclient //$PLATIN_ADDRESS/Austausch -U "$PLATIN_USER%$PLATIN_PASS" -D //GitLab-Bot -c "mkdir BitmapViewer"
smbclient //$PLATIN_ADDRESS/Austausch -U "$PLATIN_USER%$PLATIN_PASS" -D //GitLab-Bot/BitmapViewer -c "put BitmapViewer-gcc-$GCC_VERSION-debug-x86_64.AppImage"
smbclient //$PLATIN_ADDRESS/Austausch -U "$PLATIN_USER%$PLATIN_PASS" -D //GitLab-Bot/BitmapViewer -c "put BitmapViewer-gcc-$GCC_VERSION-release-x86_64.AppImage"
smbclient //$PLATIN_ADDRESS/Austausch -U "$PLATIN_USER%$PLATIN_PASS" -D //GitLab-Bot/BitmapViewer -c "put BitmapViewer-clang-$CLANG_VERSION-debug-x86_64.AppImage"
smbclient //$PLATIN_ADDRESS/Austausch -U "$PLATIN_USER%$PLATIN_PASS" -D //GitLab-Bot/BitmapViewer -c "put BitmapViewer-clang-$CLANG_VERSION-release-x86_64.AppImage"

# Remove created files
rm BitmapViewer-*.AppImage
