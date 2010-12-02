############################################################################
##
## This program is free software; you can redistribute it and/or
## modify it under the terms of the GNU General Public License
## as published by the Free Software Foundation; either version 2
## of the License, or (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 51 Franklin Street, Fifth Floor,
## Boston, MA  02110-1301, USA.
##
## ---
## Copyright (C) 2009, gpsbook-team <gpsbook@free.fr>
##
############################################################################

TEMPLATE = subdirs
CONFIG   += ordered

SUBDIRS = GPSBook \
          plugins/displaySRTM \
          #plugins/displayBlogging \
          #plugins/displayEditTrack \
          #plugins/displayGeoPortail \
          plugins/displayGoogleMap \
          plugins/displayGPSBabel \
          #plugins/displayGPSBabel/Gebabbel/Gebabbel.pro \
          plugins/displayGraphic2D \
          plugins/displayGraphic3D \
          #plugins/displayGraphic3D/libQGLViewer/QGLViewer \
          plugins/displayInformation \
          plugins/displayInternetBrowser \
          #plugins/displayMailTo \
          #plugins/displayPathTags \
          #plugins/displayPoI \
          #plugins/displayPictSync \
          #plugins/displayRawEditor \
          #plugins/displayReport \
          #plugins/displayRoadBook \
          plugins/displayStatistic \
          plugins/filterCleanup \
          #plugins/filterKalman \
          #plugins/filterTimeCorrection \
          #plugins/ioCSV \
          #plugins/ioGPSBabel \
          plugins/ioGPX \
          plugins/ioLOC
          #plugins/ioNMEA \
          #plugins/displayTemplate
