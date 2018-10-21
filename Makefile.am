# Makefile.am
#
# Copyright (C) 2005 Matthew Flood
# See file AUTHORS for contact information
# 
# This file is part of RudeWebPlatform.
# 
# RudeWebPlatform is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
# 
# RudeWebPlatform is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with RudeWebPlatform; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
# 
#------------------------------------------------------------------------

lib_LTLIBRARIES = librudewebplatform.la

librudewebplatform_la_SOURCES =	src/Command.cc \
	src/Command.h \
	src/CommandError.cc \
	src/CommandError.h \
	src/CommandFactory.cc \
	src/CommandFactory.h \
	src/Decorator.cc \
	src/Decorator.h \
	src/View.cc \
	src/View.h \
	src/ViewError.cc \
	src/ViewError.h \
	src/ViewFactory.cc \
	src/ViewFactory.h \
	src/webplatform.h


librudewebplatform_la_LDFLAGS = -version-info 3:2:1
librudewebplatform_la_LIBADD = -lrudesocket -lrudeconfig -lrudecgi -lrudedatabase -lrudesmtp -lrudesession

pkginclude_HEADERS = src/Command.h src/View.h src/CommandFactory.h src/ViewFactory.h src/Decorator.h src/webplatform.h
pkgincludedir = $(includedir)/rude


AUTOMAKE_OPTIONS = gnits dist-bzip2 dist-shar dist-zip


