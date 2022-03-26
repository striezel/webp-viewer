#!/bin/bash

#  create-package.sh - script to create *.deb package from latest Git commit
#
#  Copyright (C) 2019, 2022  Dirk Stolle
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Gets the latest tag.
TAG="${CI_COMMIT_TAG:-$(git describe | cut -d '-' -f 1)}"
# Version is the same as the tag, just without the leading 'v'.
VERSION=$(echo $TAG | cut -c '2-')
ARCH=$(dpkg --print-architecture)
COMMIT_HASH=$(git rev-parse HEAD)

echo "Info: Tag is $TAG."
echo "Info: Version is $VERSION."
echo "Info: Architecture is $ARCH."
echo "Info: Commit hash is $COMMIT_HASH."

wget -O webp-viewer_${VERSION}.orig.tar.bz2 https://gitlab.com/striezel/webp-viewer/-/archive/${COMMIT_HASH}/webp-viewer-${COMMIT_HASH}.tar.bz2
tar -x --bzip2 -f webp-viewer_${VERSION}.orig.tar.bz2
if [[ $? -ne 0 ]]
then
  echo "ERROR: Archive extraction failed!"
  exit 1
fi

mv webp-viewer-${COMMIT_HASH} webp-viewer-${VERSION}
if [[ $? -ne 0 ]]
then
  echo "ERROR: Could not move extracted files!"
  exit 1
fi

cd webp-viewer-${VERSION}
if [[ $? -ne 0 ]]
then
  echo "ERROR: Could not change to extracted directory"
  exit 1
fi

# build the package
debuild -uc -us
if [[ $? -ne 0 ]]
then
  echo "ERROR: Package build failed!"
  exit 1
fi

cd ..
RELEASE=$(lsb_release -cs)
mv webp-viewer*.deb webp-viewer_${VERSION}-${RELEASE}_${ARCH}.deb
