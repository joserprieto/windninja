#!/bin/bash

# Set installation directories and versions
PREFIX=/usr/local
POPPLER="poppler-22.02.0"
PROJ="proj-8.2.1"
GDAL="gdal-3.4.1"

# Install necessary dependencies for building Poppler, PROJ, GDAL, and other tools
sudo apt install -y libfontconfig1-dev \
                    libcurl4-gnutls-dev \
                    libnetcdf-dev \
                    libboost-program-options-dev \
                    libboost-date-time-dev \
                    libgeos-dev \
                    libboost-test-dev \
                    libsqlite3-dev \
                    sqlite3 \
                    libopenjp2-7-dev \
                    libtiff-dev
                    

# Install Poppler for PDF support in GDAL
wget http://poppler.freedesktop.org/$POPPLER.tar.xz 
tar -xvf $POPPLER.tar.xz 
cd $POPPLER/
mkdir build                         &&
cd    build                         &&
cmake  -DCMAKE_BUILD_TYPE=Release   \
       -DCMAKE_INSTALL_PREFIX=$PREFIX  \
       -DTESTDATADIR=$PWD/testfiles \
       -DENABLE_UNSTABLE_API_ABI_HEADERS=ON \
       ..                           &&
make -j$(nproc)  # Use all available cores for faster compilation
sudo make install
cd ../..

# Install PROJ (a cartographic projection library)
wget http://download.osgeo.org/proj/$PROJ.tar.gz 
tar xvfz $PROJ.tar.gz 
cd $PROJ
./configure --prefix=$PREFIX
make clean
make -j$(nproc)
sudo make install
cd ..

# Install GDAL with Poppler support
wget http://download.osgeo.org/gdal/${GDAL:5}/$GDAL.tar.gz 
tar -xvf $GDAL.tar.gz 
cd $GDAL/ 
./configure --prefix=$PREFIX --with-poppler=$PREFIX
make -j$(nproc)
sudo make install
cd ..

# Add qt4 libs from ppa
# See here for more info:
# https://ubuntuhandbook.org/index.php/2020/07/install-qt4-ubuntu-20-04/
# !unavailable right now (20240911, ppa:rock-core/qt4)
# used another, ppa:ubuntuhandbook1/ppa

sudo add-apt-repository ppa:ubuntuhandbook1/ppa 
sudo apt install libqt4-dev libqtwebkit-dev 

# Use OpenFOAM 9; OpenFOAM 8 not available for Ubuntu 22.04
# add the dl.openfoam.org repo and install OpenFOAM 9
sudo sh -c "wget -O - https://dl.openfoam.org/gpg.key > /etc/apt/trusted.gpg.d/openfoam.asc"
sudo add-apt-repository http://dl.openfoam.org/ubuntu
sudo apt-get update 
sudo apt-get -y install openfoam9 
echo "source /opt/openfoam9/etc/bashrc" >> ~/.bashrc 
