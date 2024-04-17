#!/usr/bin/env bash

if [ ! -f appimagetool-x86_64.AppImage ]; then
  wget "https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage"
fi
cp cmake-build-release/victor_dmm VictorDMM.AppDir/usr/bin
cp images/Iconset/AppIcon.iconset/icon_512x512.png VictorDMM.AppDir/victor_dmm.png
./appimagetool-x86_64.AppImage VictorDMM.AppDir
