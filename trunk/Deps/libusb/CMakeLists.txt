pkg_check_modules(LIBUSB QUIET libusb-1.0)
include_directories(${LIBUSB_INCLUDE_DIRS})
link_directories(${LIBUSB_LIBRARY_DIRS})

if (LIBUSB_INCLUDE_DIRS)
	MESSAGE("*** libusb FOUND")
else()
	MESSAGE("*** libusb NOT FOUND")
endif()
