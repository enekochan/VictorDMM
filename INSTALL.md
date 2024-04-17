# Install needed tools and libraries

## Install build tools

* Ubuntu

```
sudo apt-get install build-essential libgl1-mesa-dev
```

* macOS

```
xcode-select –-install
```

##  Install ninja build

Download release builds from:

https://github.com/ninja-build/ninja/releases

Install it on `/usr/local/bin`.

## Install QT6

* Ubuntu

```
wget https://mirrors.20i.com/pub/qt.io/archive/qt/6.6/6.6.3/single/qt-everywhere-src-6.6.3.tar.xz
tar xf qt-everywhere-src-6.6.3.tar.xz
cd qt-everywhere-src-6.6.3
mkdir build
cd build
../configure -prefix /usr/local/Qt6
cmake --build . --parallel 4
sudo cmake --install .
```

Add this to your `.profile` or `.bash_profile` file for example:

```
export PATH=$PATH:/usr/local/Qt6/bin
```

* macOS

```
wget https://mirrors.20i.com/pub/qt.io/archive/qt/6.6/6.6.3/single/qt-everywhere-src-6.6.3.tar.xz
tar xf qt-everywhere-src-6.6.3.tar.xz
cd qt-everywhere-src-6.6.3
mkdir build
cd build
../configure -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" -prefix /usr/local/Qt6
cmake --build . --parallel 4
sudo cmake --install .
```

Add this to your `.bash_profile` or `.zprofile` file for example:

```
export PATH=$PATH:/usr/local/Qt6/bin
```

# Prepare build structure

* Debug

```
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=`which ninja` -G Ninja -S ./ -B ./cmake-build-debug
```

* Release

```
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=`which ninja` -G Ninja -S ./ -B ./cmake-build-release
```

Those commands will create 2 folders: `cmake-build-debug` and `cmake-build-release`.

# Build the app

* Debug

```
cmake --build ./cmake-build-debug --target all -j 3
```

* Release

```
cmake --build ./cmake-build-release --target all -j 3
```

Those commands will build the app inside their respective build folder (`cmake-build-debug` and `cmake-build-release`).

## Create AppImage for Linux

```
$ mkdir -P VictorDMM.AppDir/usr/bin
$ cp cmake-build-release/victor_dmm VictorDMM.AppDir/usr/bin
$ cp images/Iconset/AppIcon.iconset/icon_512x512.png VictorDMM.AppDir/victor_dmm.png
$ wget "https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage"
$ chmod a+x appimagetool-x86_64.AppImage
$ ./appimagetool-x86_64.AppImage VictorDMM.AppDir
```
