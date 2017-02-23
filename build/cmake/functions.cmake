function(streamer_add_samp_plugin name)
	# Add plugin
	add_samp_plugin(${name} ${ARGN})

	# Add libraries
	target_link_libraries(${name} sampgdk sampsdk)

	# Directories
	target_include_directories(streamer PRIVATE ${SAMP_STREAMER_EXTERNAL})			# Include externals
	target_include_directories(streamer PRIVATE ${SAMPGDK_DIR}/include)				# Include sampgdk files
	target_include_directories(streamer PRIVATE ${SAMPGDK_GEN_DIR}/include/sampgdk)	# Include sampgdk generated samp includes

	# Set gdk stuff
	target_compile_definitions(${name} PRIVATE SAMPGDK_AMALGAMATION SAMPGDK_CPP_WRAPPERS)
	
	# Plattform specified definitions
	if (WIN32)
		target_compile_definitions(
			${name}
			PRIVATE
			BOOST_ALL_NO_LIB
			BOOST_CHRONO_HEADER_ONLY
			NOMINMAX
			WIN32
		)
	else()
		target_compile_definitions(
			${name}
			PRIVATE
			BOOST_CHRONO_HEADER_ONLY
			LINUX
			NDEBUG
		)
	endif()	
endfunction()

function(streamer_add_library name)
	# Add plugin
	add_library(${name} ${ARGN})

	# Add libraries
	target_link_libraries(${name} sampgdk sampsdk)

	# Directories
	target_include_directories(${name} PRIVATE ${SAMP_STREAMER_EXTERNAL})			# Include externals
	target_include_directories(${name} PRIVATE ${SAMPGDK_DIR}/include)				# Include sampgdk files
	target_include_directories(${name} PRIVATE ${SAMPGDK_GEN_DIR}/include/sampgdk)	# Include sampgdk generated samp includes

	# Set gdk stuff
	target_compile_definitions(${name} PRIVATE SAMPGDK_AMALGAMATION SAMPGDK_CPP_WRAPPERS)

	# Plattform specified definitions
	if (WIN32)
		target_compile_definitions(
			${name}
			PRIVATE
			BOOST_ALL_NO_LIB
			BOOST_CHRONO_HEADER_ONLY
			NOMINMAX
			WIN32
		)
	else()
		target_compile_definitions(
			${name}
			PRIVATE
			BOOST_CHRONO_HEADER_ONLY
			LINUX
			NDEBUG
		)
	endif()	
endfunction()

