#!/bin/sh

# Clean
rm -rf package

# Create directories
mkdir package
mkdir package/astrovax

# Windows specific directories
mkdir package/astrovax/windows

# Mac OS specific directories
mkdir package/astrovax/macos
mkdir package/astrovax/macos/astrovax.app
mkdir package/astrovax/macos/astrovax.app/Contents
mkdir package/astrovax/macos/astrovax.app/Contents/MacOS
mkdir package/astrovax/macos/astrovax.app/Contents/Resources

# Copy executables
cp vs2008/Release/astrovax.exe package/astrovax/windows/
cp vs2008/Release/glut32.dll package/astrovax/windows/
cp build/Release/testgame package/astrovax/macos/astrovax.app/Contents/MacOS

# Copy metadata
cp Info.plist package/astrovax/macos/astrovax.app/Contents
cp icon.icns package/astrovax/macos/astrovax.app/Contents/Resources

# Copy textures
cp *.tga package/astrovax/

# Copy readme
cp readme.txt package/astrovax/windows/
cp readme.txt package/astrovax/macos/

# Copy the shared data to both directories
cp -r package/astrovax/*.tga package/astrovax/windows/
cp -r package/astrovax/*.tga package/astrovax/macos/astrovax.app/Contents/Resources/

# Delete the temporary copy of the data
rm -rf package/astrovax/data

# Zip stuff up
cd package/astrovax/macos
zip -r astrovax_macosx.zip astrovax.app ReadMe.txt
cp astrovax_macosx.zip ../
cd ../../../
cd package/astrovax/windows
zip -r astrovax_win32.zip *
cp astrovax_win32.zip ../
cd ../../../

