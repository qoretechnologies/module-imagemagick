#!/bin/bash

set -e
set -x

ENV_FILE=/tmp/env.sh

. ${ENV_FILE}

# Save original working directory before building ImageMagick
orig_dir=`pwd`

# Install ImageMagick 7 development libraries (Ubuntu repos only have IM6)
apt-get update -qq
apt-get install -y -qq pkg-config build-essential libjpeg-dev libpng-dev libtiff-dev libwebp-dev wget

# Build and install ImageMagick 7 from source
IM_VERSION="7.1.1-43"
wget -q "https://github.com/ImageMagick/ImageMagick/archive/refs/tags/${IM_VERSION}.tar.gz" -O /tmp/im.tar.gz
cd /tmp && tar xf im.tar.gz
cd /tmp/ImageMagick-${IM_VERSION}
./configure --prefix=/usr --disable-docs --without-x --disable-openmp --with-quantum-depth=16 --with-magick-plus-plus=no --quiet
make -j4 --quiet
make install --quiet
ldconfig
cd ${orig_dir}

# setup MODULE_SRC_DIR env var
cwd=`pwd`
if [ -z "${MODULE_SRC_DIR}" ]; then
    if [ -e "$cwd/src/QoreMagickImage.cpp" ]; then
        MODULE_SRC_DIR=$cwd
    else
        MODULE_SRC_DIR=$WORKDIR/module-imagemagick
    fi
fi
echo "export MODULE_SRC_DIR=${MODULE_SRC_DIR}" >> ${ENV_FILE}

echo "export QORE_UID=999" >> ${ENV_FILE}
echo "export QORE_GID=999" >> ${ENV_FILE}

. ${ENV_FILE}

export MAKE_JOBS=4

# build module and install
echo && echo "-- building module --"
mkdir -p ${MODULE_SRC_DIR}/build
cd ${MODULE_SRC_DIR}/build
cmake -S .. -DCMAKE_BUILD_TYPE=debug -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}
make -j${MAKE_JOBS}
make install

# add Qore user and group
groupadd -o -g ${QORE_GID} qore
useradd -o -m -d /home/qore -u ${QORE_UID} -g ${QORE_GID} qore

# own everything by the qore user
chown -R qore:qore ${MODULE_SRC_DIR}

# run the tests
export QORE_MODULE_DIR=${MODULE_SRC_DIR}/qlib:${QORE_MODULE_DIR}
cd ${MODULE_SRC_DIR}
for test in test/*.qtest; do
    gosu qore:qore qore $test -vv
done
