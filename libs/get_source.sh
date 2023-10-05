#!/bin/bash
set -e

source libs/env_deploy.sh
ENV_NEKORAY=1
source libs/get_source_env.sh
pushd ..

####
if [ ! -d "sing-box-extra" ]; then
  git clone --no-checkout https://github.com/AntiNeko/sing-box-extra.git
fi
pushd sing-box-extra
git fetch https://github.com/AntiNeko/sing-box-extra.git
git checkout "$COMMIT_SING_BOX_EXTRA"

pushd ..
if [ -d "sing-box" ]; then
  pushd sing-box
  git fetch https://github.com/MatsuriDayo/sing-box.git
  popd
fi
if [ -d "libneko" ]; then
  pushd libneko
  git fetch https://github.com/MatsuriDayo/libneko.git
  popd
fi
popd

ENV_SING_BOX_EXTRA=1
source $SRC_ROOT/libs/get_source_env.sh
NO_ENV=1 ./libs/get_source.sh

popd

####
if [ ! -d "Xray-core" ]; then
  git clone --no-checkout https://github.com/MatsuriDayo/Xray-core.git
fi
pushd Xray-core
git fetch https://github.com/MatsuriDayo/Xray-core.git
git checkout "$COMMIT_MATSURI_XRAY"
popd

popd
