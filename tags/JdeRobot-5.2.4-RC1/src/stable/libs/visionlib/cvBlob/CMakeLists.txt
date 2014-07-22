# Copyright (C) 2007 by Cristóbal Carnero Liñán
# grendel.ccl@gmail.com
#
# This file is part of cvBlob.
#
# cvBlob is free software: you can redistribute it and/or modify
# it under the terms of the Lesser GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# cvBlob is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# Lesser GNU General Public License for more details.
#
# You should have received a copy of the Lesser GNU General Public License
# along with cvBlob.  If not, see <http://www.gnu.org/licenses/>.
#


set(cvBlob_CVBLOB cvblob.cpp
                   cvlabel.cpp
		   cvaux.cpp
		   cvcontour.cpp
		   cvtrack.cpp
		   cvcolor.cpp
)

set_source_files_properties(${cvBlob_SRC}
                            PROPERTIES
                            COMPILE_FLAGS "-O3"
)

add_library(cvblob SHARED ${cvBlob_CVBLOB})

target_link_libraries(cvblob ${OPENGL_LIBRARIES})

#install(FILES cvblob.h DESTINATION include)

#install(TARGETS cvblob
#       RUNTIME DESTINATION bin
#        LIBRARY DESTINATION lib
#        ARCHIVE DESTINATION lib
#)
