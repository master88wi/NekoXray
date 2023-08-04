#!/bin/bash

VERSION=$1

cp ../libs/nekoXray.desktop nekoray/nekoXray.desktop
cp ../libs/PKGBUILD nekoray/PKGBUILD
sed "2s/.*/pkgver=${VERSION}/" nekoray/PKGBUILD > temp.txt && mv temp.txt nekoray/PKGBUILD

pushd nekoray/
unzip -o nekoray.zip && rm nekoray.zip
rm -rf nekoray/usr && rm nekoray/launcher
zip -r nekoray.zip nekoray && rm -rf nekoray
popd


docker pull archlinux
docker run --volume ${PWD}/nekoray/:/src archlinux \
    bash -c "pacman -Sy --noconfirm sudo && \
    useradd -m user && echo 'user ALL=(ALL) NOPASSWD: ALL' >> /etc/sudoers && \
    chown -R user:user /src && \
    sudo -u user bash -c 'cd /src && \
    sudo pacman -Syu --noconfirm base-devel qt5-base qt5-svg qt5-tools qt5-x11extras protobuf yaml-cpp zxing-cpp unzip && \
    makepkg -s --noconfirm && \
    find . -type f ! -name 'nekoXray-*.pkg.tar.zst' -delete && \
    mv nekoXray-*.pkg.tar.zst nekoray.pkg.tar.zst'"
