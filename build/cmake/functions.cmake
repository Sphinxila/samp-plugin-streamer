function(streamer_add_samp_plugin name)
	# Add plugin
	add_samp_plugin(${name} ${ARGN})

	# Add libraries
	target_link_libraries(${name} sampgdk sampsdk)

	# Directories
	target_include_directories(streamer PUBLIC ${SAMP_STREAMER_EXTERNAL})			# Include externals
	target_include_directories(streamer PRIVATE ${SAMPGDK_DIR}/include)				# Include sampgdk files
	target_include_directories(streamer PRIVATE ${SAMPGDK_GEN_DIR}/include/sampgdk)	# Include sampgdk generated samp includes

	# Set gdk stuff
	target_compile_definitions(${name} PRIVATE SAMPGDK_AMALGAMATION SAMPGDK_CPP_WRAPPERS)
endfunction()

function(streamer_add_library name)
	# Add plugin
	add_library(${name} ${ARGN})

	# Add libraries
	target_link_libraries(${name} sampgdk sampsdk)

	# Directories
	target_include_directories(${name} PUBLIC ${SAMP_STREAMER_EXTERNAL})			# Include externals
	target_include_directories(${name} PRIVATE ${SAMPGDK_DIR}/include)				# Include sampgdk files
	target_include_directories(${name} PRIVATE ${SAMPGDK_GEN_DIR}/include/sampgdk)	# Include sampgdk generated samp includes

	# Set gdk stuff
	target_compile_definitions(${name} PRIVATE SAMPGDK_AMALGAMATION SAMPGDK_CPP_WRAPPERS)
endfunction()

