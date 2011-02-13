#!/bin/sh

# Clean
rm -rf package

# Create directories
mkdir package
mkdir package/bunnychickendinosaur

# Windows specific directories
mkdir package/bunnychickendinosaur/windows

# Mac OS specific directories
mkdir package/bunnychickendinosaur/macos
mkdir package/bunnychickendinosaur/macos/bunnychickendinosaur.app
mkdir package/bunnychickendinosaur/macos/bunnychickendinosaur.app/Contents
mkdir package/bunnychickendinosaur/macos/bunnychickendinosaur.app/Contents/MacOS
mkdir package/bunnychickendinosaur/macos/bunnychickendinosaur.app/Contents/Resources

# Copy executables
cp /bunnychickendinosaur.exe package/bunnychickendinosaur/windows/
cp glut32.dll package/bunnychickendinosaur/windows/
cp bunnychickendinosaur package/bunnychickendinosaur/macos/bunnychickendinosaur.app/Contents/MacOS

# Copy metadata
cp Info.plist package/bunnychickendinosaur/macos/bunnychickendinosaur.app/Contents
cp icon.icns package/bunnychickendinosaur/macos/bunnychickendinosaur.app/Contents/Resources

# Copy textures and tile layers
cp *.tga package/bunnychickendinosaur/
cp *.dat package/bunnychickendinosaur/

# Copy readme
cp readme.txt package/bunnychickendinosaur/windows/
cp readme.txt package/bunnychickendinosaur/macos/

# Copy the shared data to both directories
cp -r package/bunnychickendinosaur/*.tga package/bunnychickendinosaur/windows/
cp -r package/bunnychickendinosaur/*.dat package/bunnychickendinosaur/windows/
cp -r package/bunnychickendinosaur/*.tga package/bunnychickendinosaur/macos/bunnychickendinosaur.app/Contents/Resources/
cp -r package/bunnychickendinosaur/*.dat package/bunnychickendinosaur/macos/bunnychickendinosaur.app/Contents/Resources/

# Delete the temporary copy of the data
rm -rf package/bunnychickendinosaur/data

# Zip stuff up
cd package/bunnychickendinosaur/macos
zip -r bunnychickendinosaur_macosx.zip bunnychickendinosaur.app readme.txt
cp bunnychickendinosaur_macosx.zip ../
cd ../../../
cd package/bunnychickendinosaur/windows
zip -r bunnychickendinosaur_win32.zip *
cp bunnychickendinosaur_win32.zip ../
cd ../../../

