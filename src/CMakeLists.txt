include(AMXConfig)
include(AddSAMPPlugin)

set(SOURCE_NATIVES
	natives/actors.cpp
	natives/areas.cpp
	natives/checkpoints.cpp
	natives/deprecated.cpp
	natives/extended.cpp
	natives/manipulation.cpp
	natives/map-icons.cpp
	natives/miscellaneous.cpp
	natives/objects.cpp
	natives/pickups.cpp
	natives/race-checkpoints.cpp
	natives/settings.cpp
	natives/text-labels.cpp
	natives/updates.cpp
)

set(SOURCE_FILES
	cell.cpp
	core.cpp
	data.cpp
	grid.cpp
	identifier.cpp
	item.cpp
	player.cpp
	streamer.cpp
	utility/amx.cpp
	utility/geometry.cpp
	utility/misc.cpp
	manipulation/array.cpp
	manipulation/float.cpp
	manipulation/int.cpp
	error_code.cpp
)

SET(HEADER_FILES
	cell.h
	common.h
	core.h
	data.h
	grid.h
	identifier.h
	item.h
	main.h
	manipulation.h
	natives.h
	player.h
	streamer.h
	utility.h
	utility/amx.h
	utility/geometry.h
	utility/misc.h
	manipulation/array.h
	manipulation/float.h
	manipulation/int.h
	sampgdk.h
)

SET(PLUGIN_FILES
	callbacks.cpp
	main.cpp
	streamer.def
	streamer.rc
)

# Plugin
if (SAMP_STREAMER_INCLUDE_PLUGIN)
	streamer_add_samp_plugin(streamer
	  ${PLUGIN_FILES}
	  ${SOURCE_FILES}
	  ${SOURCE_NATIVES}
	  ${HEADER_FILES}
	)

	if (SAMP_STREAMER_USE_BUILD_DIR)
		# Target output
		set_target_properties(streamer
			PROPERTIES
			ARCHIVE_OUTPUT_DIRECTORY "${SAMP_STREAMER_BIN_OUT_DIR}/plugins"
			LIBRARY_OUTPUT_DIRECTORY "${SAMP_STREAMER_BIN_OUT_DIR}/plugins"
			RUNTIME_OUTPUT_DIRECTORY "${SAMP_STREAMER_BIN_OUT_DIR}/plugins"
		)
		
		# Streamerr include file handling
		set(STREAMER_INCLUDE_SRC "${SAMP_STREAMER_ROOT}/include/streamer.inc")
		set(STREAMER_INCLUDE_DST "${SAMP_STREAMER_BIN_OUT_DIR}/include/streamer.inc")
		
		# Copy
		add_custom_command(
			TARGET  streamer POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy
					${STREAMER_INCLUDE_SRC}
					${STREAMER_INCLUDE_DST}
			COMMENT "Copy Pawno include file")
	endif()
endif()

set(SOURCE_LIB
	implements/actors_impl.cpp
	implements/areas_impl.cpp
	implements/checkpoints_impl.cpp
	implements/deprecated_impl.cpp
	implements/extended_impl.cpp
	implements/manipulation_impl.cpp
	implements/map-icons_impl.cpp
	implements/miscellaneous_impl.cpp
	implements/objects_impl.cpp
	implements/pickups_impl.cpp
	implements/race-checkpoints_impl.cpp
	implements/settings_impl.cpp
	implements/text-labels_impl.cpp
	implements/updates_impl.cpp
	implements/streamer_impl.cpp
	implements/main_impl.cpp
)

SET(SOURCE_LIB_HEADER
	include/streamer/actors.hpp
	include/streamer/areas.hpp
	include/streamer/checkpoints.hpp
	include/streamer/config.hpp
	include/streamer/deprecated.hpp
	include/streamer/extended.hpp
	include/streamer/manipulation.hpp
	include/streamer/map-icons.hpp
	include/streamer/miscellaneous.hpp
	include/streamer/objects.hpp
	include/streamer/pickups.hpp
	include/streamer/race-checkpoints.hpp
	include/streamer/settings.hpp
	include/streamer/streamer.hpp
	include/streamer/text-labels.hpp
	include/streamer/updates.hpp
)

# Library
streamer_add_library(streamer-lib
  ${SOURCE_FILES}
  ${HEADER_FILES}
  ${SOURCE_LIB}
  ${SOURCE_LIB_HEADER}
)

target_include_directories(streamer-lib PUBLIC include/)

if (SAMP_STREAMER_USE_BUILD_DIR)
	# Target output
	set_target_properties(streamer-lib
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${SAMP_STREAMER_BIN_OUT_DIR}/lib"
		LIBRARY_OUTPUT_DIRECTORY "${SAMP_STREAMER_BIN_OUT_DIR}/lib"
		RUNTIME_OUTPUT_DIRECTORY "${SAMP_STREAMER_BIN_OUT_DIR}/lib"
	)
endif()


# Library with natives
streamer_add_library(streamer-plugin-lib
  ${SOURCE_FILES}
  ${HEADER_FILES}
  ${SOURCE_LIB}
  ${SOURCE_NATIVES}
  ${SOURCE_LIB_HEADER}
)

target_include_directories(streamer-plugin-lib PUBLIC include/)

if (SAMP_STREAMER_USE_BUILD_DIR)
	# Target output
	set_target_properties(streamer-lib
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${SAMP_STREAMER_BIN_OUT_DIR}/lib"
		LIBRARY_OUTPUT_DIRECTORY "${SAMP_STREAMER_BIN_OUT_DIR}/lib"
		RUNTIME_OUTPUT_DIRECTORY "${SAMP_STREAMER_BIN_OUT_DIR}/lib"
	)
endif()
