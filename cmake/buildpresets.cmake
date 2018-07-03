#  Copyright (C) 2015-2016 JDE Developers Team
#  Authors:
#    Victor Arribas <v.arribas.urjc@gmail.com>
#    Francisco Perez <f.perez475@gmail.com>

cmake_minimum_required(VERSION 2.6)

### Auxiliary functions >>>
## A function to get all user defined variables with a specified prefix
function (getListOfVarsStartingWith _prefix _varResult)
    get_cmake_property(_vars CACHE_VARIABLES)
    string (REGEX MATCHALL "(^|;)${_prefix}[A-Za-z0-9_]*" _matchedVars "${_vars}")
    set (${_varResult} ${_matchedVars} PARENT_SCOPE)
endfunction()


## Helper to override cache variables without specify neither lost type nor description
function(override_cache _var _value)
    get_property(_HelpString CACHE "${_var}" PROPERTY HELPSTRING)
    get_property(_Type CACHE "${_var}" PROPERTY TYPE)
	set(${_var} ${_value} CACHE ${_Type} "${_HelpString}" FORCE)
endfunction()


## Helper to simplify set build_X variables and toggle behavior
# CMakeCache is a bad friend for this feature. So we require override_cache()
function (build_component component value)
	if (DEFINED ${ARGV2})
		if(${ARGV2} STREQUAL "ONLY_ON" AND NOT ${value})
			return()
		endif()
	endif()

	if (NOT DEFINED build_${component})
		set(build_${component} ${value} CACHE BOOL "Build flag for JdeRobot component: ${component} (defined by builtpresets)")
	elseif (NOT build_${component} EQUAL ${value})
		override_cache(build_${component} ${value})
	endif()
endfunction()
### <<<


### buildpresets pre-job >>>
## Mass toggle OFF.
# all of the variables starting with build_ are set to OFF.
# You MUST do it isolatelly. Therefore requires two steps:
#   1) cmake -Dbuild_purge=ON
#   2) cmake -Dbuild_<whatever> (your normal cmake invocation)
option(build_purge "Turn off all build_ variables." OFF)
if (build_purge)
	message(STATUS "Purging build configuration\n\tdisabling any 'build_' (setting to OFF)")
	set(build-default OFF)
	getListOfVarsStartingWith("build_" matchedVars)
	foreach (_var IN LISTS matchedVars)
		override_cache(${_var} OFF)
	endforeach()
	getListOfVarsStartingWith("test_" matchedVars)
	foreach (_var IN LISTS matchedVars)
		override_cache(${_var} OFF)
	endforeach()
	override_cache(build_purge OFF)  # Set itself back to off, as this is a one time thing.
endif()


## A run once macro to add suffix to project name ;)
macro (project_suffix suffix)
	if (NOT _project_suffix_done)
		project(${PROJECT_NAME}-${suffix})
		set(_project_suffix_done ON)
	endif()
endmacro()
set(_project_suffix_done OFF) # allow package rename without clean CACHE


## New alias for build-default
if (build_all)
	message(STATUS "Buildpresets: using old build workflow")
	set(build-default ON)
	getListOfVarsStartingWith("build_" matchedVars)
	foreach (_var IN LISTS matchedVars)
		unset(${_var} CACHE)
	endforeach()
	return()
endif()
### <<<



### Start buildpresets
# Here you can define build groups and aliases to simplify fine grained build
# Notice: definition should be from large to core to allow chainning

## Tests
# test are planned to provide a full testing environment will all dependencies
# it makes sense for fast verify of any improvement, pull request or patch
# related to a component.

if (test_ardrone)
	build_component(ardrone_server ON)
	build_component(introrob_py ON)

	build_component(core ON "ONLY_ON")
endif()

if (test_quadrotor)
	build_component(gazebo ON)
	build_component(quadrotor2 ON)
	build_component(quadrotor ON)
	build_component(introrob_py ON)

	build_component(core ON "ONLY_ON")
endif()

if (test_car)
	build_component(gazebo ON)
	build_component(car ON)
	build_component(introrob_qt ON)

	build_component(core ON "ONLY_ON")
endif()

if (test_flyingKinect)
	build_component(gazebo ON)
	build_component(flyingKinect ON)
	build_component(flyingKinect2 ON)
		build_component(quadrotor2 ON) # dependency
	build_component(navigatorCamera ON)
	build_component(rgbdViz ON)

	build_component(core ON "ONLY_ON")
endif()


## Build aliases (real packages for cpack)
# These alises provides semantic grouping framework's components by layers
# They are mainly planned for:
#   a) reduce built time for custom "tiny" components
#   b) provide multiple package flavors easily

if (build_drivers)
	project_suffix(drivers)
	set(build-default OFF)

	build_component(core ON "ONLY_ON")

	# core drivers
	build_component(ardrone_server ON)
	build_component(basic_server ON)
	build_component(camserver ON)
	build_component(gazebo ON)
		build_component(car ON)
		build_component(flyingKinect ON)
		build_component(kinect ON)
		build_component(nao ON)
		build_component(pioneer ON)
		build_component(quadrotor ON)
		build_component(quadrotor2 ON)
		build_component(turtlebot ON)
	build_component(giraffeServer ON)
	build_component(kinect2server ON)
	build_component(naoserver ON)
	build_component(openni1Server ON)
	build_component(openniServer ON)
	build_component(pclRGBDServer ON)
endif()

if (build_core)
	project_suffix(core)
	set(build-default OFF)

	build_component(msgs ON "ONLY_ON")

	# libs
	build_component(depthLib ON)
	build_component(easyiceconfig_cpp ON)
	build_component(easyiceconfig_py ON)
	build_component(fuzzylib ON)
	build_component(geometry ON)
	build_component(jderobotHandlers ON)
	build_component(jderobotViewer ON)
	build_component(jderobotutil ON)
	build_component(log ON)
	build_component(ns ON)
	build_component(parallelIce ON)
	build_component(pioneer ON)
	build_component(progeo ON)
	build_component(visionlib ON)
	build_component(xmlParser ON)
endif()

if (build_msgs)
	project_suffix(msgs)
	set(build-default OFF)

	build_component(interfaces_cpp ON)
	build_component(interfaces_java ON)
	build_component(interfaces_python ON)
endif()

#message(SEND_ERROR "PROJECT_NAME: ${PROJECT_NAME}")


