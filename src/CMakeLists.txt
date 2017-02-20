include(AMXConfig)
include(AddSAMPPlugin)

set(SOURCE_FILES
	callbacks.cpp
	cell.cpp
	core.cpp
	data.cpp
	grid.cpp
	identifier.cpp
	item.cpp
	main.cpp
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
	include/actors.hpp
	include/areas.hpp
	include/checkpoints.hpp
	include/config.hpp
	include/deprecated.hpp
	include/extended.hpp
	include/manipulation.h
	include/manipulation.hpp
	include/map-icons.hpp
	include/miscellaneous.hpp
	include/objects.hpp
	include/pickups.hpp
	include/race-checkpoints.hpp
	include/settings.hpp
	include/streamer.hpp
	include/text-labels.hpp
	include/updates.hpp
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
)

# Plugin
add_samp_plugin(streamer-plugin
  ${SOURCE_FILES}
  ${SOURCE_NATIVES}
  ${HEADER_FILES}
  ${SAMPSDK_PATH}/amxplugin.cpp
  ${SAMPGDK_PATH}/sampgdk.c
  streamer.def
  streamer.rc
)

target_include_directories(streamer-plugin PUBLIC ${SAMPSDK_PATH})
target_include_directories(streamer-plugin PUBLIC ${SAMPSDK_PATH}/amx)
target_include_directories(streamer-plugin PUBLIC ${SAMP_STREAMER_EXTERNAL})

set_target_properties(streamer-plugin PROPERTIES OUTPUT_NAME "streamer")

# Lib
add_library(streamer-plugin-lib
	cell.cpp
	core.cpp
	data.cpp
	grid.cpp
	identifier.cpp
	item.cpp
	player.cpp
	streamer.cpp
	error_code.cpp
	${SOURCE_LIB}
	${SOURCE_LIB_HEADER}
	${SAMPSDK_PATH}/amxplugin.cpp
	${SAMPGDK_PATH}/sampgdk.c
)

target_include_directories(streamer-plugin-lib PRIVATE ${SAMPSDK_PATH})
target_include_directories(streamer-plugin-lib PRIVATE ${SAMPSDK_PATH}/amx)
target_include_directories(streamer-plugin-lib PRIVATE ${SAMP_STREAMER_EXTERNAL})
target_include_directories(streamer-plugin-lib PUBLIC ${CMAKE_CURRENT_DIRECTORY}/include)
