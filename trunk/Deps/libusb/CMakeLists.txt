pkg_check_modules(LIBUSB QUIET libusb-1.0)

if (LIBUSB_INCLUDE_DIRS)
	MESSAGE("*** libusb FOUND")
	SET(DEPS "${DEPS} libusb-1.0,")
else()
	MESSAGE("*** libusb NOT FOUND")
endif()


