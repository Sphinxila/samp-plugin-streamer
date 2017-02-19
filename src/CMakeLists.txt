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
	manipulation/array.cpp
	manipulation/float.cpp
	manipulation/int.cpp
)

add_samp_plugin(streamer-plugin
  ${SOURCE_FILES}
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
