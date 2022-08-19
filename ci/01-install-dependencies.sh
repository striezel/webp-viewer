#!/bin/bash

#  install-depencencies.sh - script to install package build dependencies
#
#  Copyright (C) 2022  Dirk Stolle
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

apt-get update
if [ $? -ne 0 ]
then
  echo "ERROR: APT package lists could not be updated!"
  exit 1
fi

apt-get -y install --no-install-recommends build-essential bzip2 ca-certificates debhelper devscripts fakeroot g++ git lsb-release tar wget
if [ $? -ne 0 ]
then
  echo "ERROR: Could not install packages required for *.deb creation!"
  exit 1
fi

apt-get -y install --no-install-recommends catch cmake g++ libgl-dev libglfw3-dev libwebp-dev pkg-config
if [ $? -ne 0 ]
then
  echo "ERROR: Could not install build dependencies!"
  exit 1
fi
