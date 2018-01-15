#!/bin/bash

# This script is not designed to be run manually. It is used when creating a
# libbot debian package with the script called `package` which should be in
# the same directory.
# This script clones LCM's git repository, compiles and packages LCM, and
# compiles and packages libbot.

set -euxo pipefail

pushd /tmp
# LCM is built and package as part of this process as there is no
# official LCM package.
git clone https://github.com/lcm-proj/lcm
mkdir lcm-build
pushd lcm-build
# Forces the package to be installed in /usr/local
# to be consistent with libbot and the fact these packages
# are not "official".
cmake -DBUILD_SHARED_LIBS:BOOL=ON \
      -DCPACK_PACKAGING_INSTALL_PREFIX:PATH=/usr/local \
      ../lcm
make
cpack -G DEB
# Install the package instead of running `make install`. This
# ensures that LCM is installed in the same location when libbot2
# is built and its package is installed. This is necessary because
# some scripts such as `bot-spy` rely on `lcm-spy` and its path
# is hardcoded at compile time.
dpkg -i packages/lcm_1.3.95-1_linux-x86_64.deb
popd
# Configure, compile, and package libbot2
mkdir libbot2-build
cd libbot2-build
cmake -DPACKAGE_LIBBOT:BOOL=ON ../libbot2
make
cpack -G DEB