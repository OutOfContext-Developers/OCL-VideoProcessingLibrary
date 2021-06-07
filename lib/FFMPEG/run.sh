# Remove any pre-existing build files
rm -rf build/
mkdir build/
cd build

# configure
../ffmpeg/configure

# build and install ffmpeg
make
make install

# Remove the build files again
cd ..
rm -rf build
