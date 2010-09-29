#!/bin/bash
ROOT_DIR=../..
SRC_DIR=$ROOT_DIR/src/GPSBook
DELIVERY_DIR=$ROOT_DIR/delivery
PACKAGE_DIR=$DELIVERY_DIR/package
set -e

cd $SRC_DIR

#Define the global variable needed to build the packet
TARGET=$(uname -m)
#VERSION=$(grep DBUILD_VERSION GPSBookConfig.pro | cut -d\" -f2 | sed "s/\\\//g")-$(date +"%Y%m%d%H%M%S")
VERSION=$(grep DBUILD_VERSION GPSBookConfig.pro | cut -d\" -f2 | sed "s/\\\//g")-$(svnversion . | cut -f '2' -d ':')
DATETIME=$(date -R)
DATE=$(date +"%F")

echo gene-deb : Preparation de l\'espace de travail pour gpsbook-$VERSION-$TARGET.

#Delete previous package working area
sudo rm -fr $PACKAGE_DIR/debian/*

#Create package directory
DEBIAN=$PACKAGE_DIR/debian/DEBIAN
mkdir -p $DEBIAN
cp $PACKAGE_DIR/postinst $DEBIAN
cp $PACKAGE_DIR/prerm $DEBIAN
cat $PACKAGE_DIR/control | sed "s/{{VERSION}}/$VERSION/" > $DEBIAN/control

#Create the documentation directories
DOC=$PACKAGE_DIR/debian/usr/share/doc/gpsbook
mkdir -p $DOC
cat $SRC_DIR/changelog | sed "s/{{VERSION}}/$VERSION/" | sed "s/{{DATETIME}}/$DATETIME/" > $DOC/changlog
cp $DOC/changlog $DOC/changlog.Debian
gzip --best $DOC/changlog*
cp $SRC_DIR/copyright $DOC/

#Create the manpage
MAN=$PACKAGE_DIR/debian/usr/share/man/man1
mkdir -p $MAN
cat $SRC_DIR/gpsbook.1 | sed "s/{{VERSION}}/$VERSION/" | sed "s/{{DATE}}/$DATE/" > $MAN/gpsbook.1
gzip --best $MAN/gpsbook.1

#Create application shortcut
APP=$PACKAGE_DIR/debian/usr/share/applications/
mkdir -p $APP
cat $DELIVERY_DIR/bin/gpsbook.desktop | sed "s/{{VERSION}}/$VERSION/" > $APP/gpsbook.desktop

#Create the directories of the software
INSTALL_DIR=$PACKAGE_DIR/debian/usr/local/gpsbook 
mkdir -p $INSTALL_DIR
mkdir -p $INSTALL_DIR/bin
mkdir -p $INSTALL_DIR/lib
mkdir -p $INSTALL_DIR/plugins
for file in $(ls $DELIVERY_DIR/bin/gpsbook* | grep -v exe$) $DELIVERY_DIR/lib/lib* $DELIVERY_DIR/plugins/lib*; do
  DESTINATION=$INSTALL_DIR/$(echo $file | cut -b$(($(echo $DELIVERY_DIR | wc -c) +1))-)
  cp -P $file $DESTINATION 
done;
sudo chown -R root: $PACKAGE_DIR/debian/usr

#Create the package
cd $PACKAGE_DIR
dpkg-deb --build debian
mv debian.deb gpsbook-$VERSION-$TARGET.deb

#Publish into repository
echo gene-deb : Publication de gpsbook-$VERSION-$TARGET.deb dans le depot.
WWW_ROOT=/var/www/
REPOSITORY=$WWW_ROOT/dists/lucid/main/binary-i386
sudo mkdir -p $REPOSITORY 
sudo chown -R $USER: $WWW_ROOT 
rm -f $REPOSITORY/gpsbook* 
mv gpsbook-$VERSION-$TARGET.deb $REPOSITORY 
cd $REPOSITORY/.. 
dpkg-scanpackages binary-i386 /dev/null dists/lucid/main/ | gzip -f9 > binary-i386/Packages.gz 
zcat binary-i386/Packages.gz > binary-i386/Packages 

#Update repository
cd .. 
cat > Release << EOF
Origin: Ubuntu
Label: Ubuntu
Suite: maverick
Version: 10.10
Codename: maverick
Date: $DATE 
Architectures: $TARGET 
Components: main 
Description: Ubuntu Maverick 10.10
EOF
echo MD5Sum: >> Release 
for iloop in $( ls main/binary-i386); do
  size=$(ls -al main/binary-i386/$iloop | cut -d' ' -f5)
  echo -n " " >> Release
  echo $(md5sum main/binary-i386/$iloop) | sed "s/ / $size /g" >> Release
done 
echo SHA1: >> Release 
for iloop in $( ls main/binary-i386); do
  size=$(ls -al main/binary-i386/$iloop | cut -d' ' -f5)
  echo -n " " >> Release
  echo $(sha1sum main/binary-i386/$iloop) | sed "s/ / $size /g" >> Release
done 
gpg -abs --yes -o Release.gpg Release
sudo chown -R root: $WWW_ROOT

# gpg --export -a > gpsbook.key
# apt-key add gpsbook.key


