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

#ifndef WEBPVIEWER_TESTS_DETECT_CI_HPP
#define WEBPVIEWER_TESTS_DETECT_CI_HPP

/** \brief Determines whether the application runs within a CI environment.
 *
 * \return Returns true, if the program runs in a CI environment.
 *         Returns false otherwise.
 */
bool runsInCi();

#endif // WEBPVIEWER_TESTS_DETECT_CI_HPP
