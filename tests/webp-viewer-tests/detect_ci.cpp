/*
 -------------------------------------------------------------------------------
    This file is part of the test suite for WebP viewer.
    Copyright (C) 2022  Dirk Stolle

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#include "detect_ci.hpp"
#include <cstdlib> // for std::getenv()
#include <string>

bool hasEnvVar(const std::string& name)
{
  return std::getenv(name.c_str()) != nullptr;
}

bool isGitlabCi()
{
  // GitLab defines the environment variable GITLAB_CI, so check that.
  return hasEnvVar("GITLAB_CI");
}

bool isGithubActions()
{
  // GitHub Actions workflow always sets environment variable GITHUB_ACTIONS.
  return hasEnvVar("GITHUB_ACTIONS");
}

bool isPackageBuilder()
{
  // debuild / dpkg-buildpackage usually sets this variable.
  return hasEnvVar("SOURCE_DATE_EPOCH");
}

bool runsInCi()
{
  return isGitlabCi() || isGithubActions() || isPackageBuilder();
}
