function(streamer_add_samp_plugin name)
	# Add plugin
	add_samp_plugin(${name} ${ARGN})

	# Add libraries
	target_link_libraries(${name} sampgdk sampsdk)

	# Directories
	target_include_directories(${name} PRIVATE ${SAMP_STREAMER_EXTERNAL})			# Include externals
	target_include_directories(${name} PRIVATE ${SAMPGDK_DIR}/include)				# Include sampgdk files
	target_include_directories(${name} PRIVATE ${SAMPGDK_GEN_DIR}/include/sampgdk)	# Include sampgdk generated samp includes

	# Includes
	target_include_directories(${name} PRIVATE ${EIGEN_LIB_PATH})
	target_include_directories(${name} PRIVATE ${BOOST_LIB_PATH})
	
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

	set_target_properties(${name} PROPERTIES PREFIX "")

	if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		set_property(TARGET ${name} APPEND_STRING PROPERTY COMPILE_FLAGS " -m32")
		set_property(TARGET ${name} APPEND_STRING PROPERTY LINK_FLAGS    " -m32")
	endif()

	if(CMAKE_COMPILER_IS_GNUCXX)
		set_property(TARGET ${name} APPEND_STRING PROPERTY
				 COMPILE_FLAGS " -Wno-attributes")
	endif()

	if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		set_property(TARGET ${name} APPEND_STRING PROPERTY
				 COMPILE_FLAGS " -Wno-ignored-attributes")
	endif()

	if(WIN32 AND CMAKE_COMPILER_IS_GNUCC)
		set_property(TARGET ${name} APPEND_STRING PROPERTY
				 LINK_FLAGS " -Wl,--enable-stdcall-fixup")
	endif()

	if(CYGWIN)
		set_property(TARGET ${name} APPEND PROPERTY COMPILE_DEFINITIONS "WIN32")
		set_property(TARGET ${name} APPEND_STRING PROPERTY LINK_FLAGS " -Wl,--kill-at")
	elseif(UNIX AND NOT WIN32 AND NOT APPLE)
		set_property(TARGET ${name} APPEND PROPERTY COMPILE_DEFINITIONS "LINUX")
	endif()

	if(MINGW)
		# Work around missing #include <stddef.h> in <SDK>/amx/amx.h.
		set_property(TARGET ${name} APPEND_STRING PROPERTY COMPILE_FLAGS " -include stddef.h")
	endif()
	
	# Add libraries
	target_link_libraries(${name} sampgdk sampsdk)

	# Directories
	target_include_directories(${name} PRIVATE ${SAMP_STREAMER_EXTERNAL})			# Include externals
	target_include_directories(${name} PRIVATE ${SAMPGDK_DIR}/include)				# Include sampgdk files
	target_include_directories(${name} PRIVATE ${SAMPGDK_GEN_DIR}/include/sampgdk)	# Include sampgdk generated samp includes
	
	# Includes
	target_include_directories(${name} PUBLIC ${EIGEN_LIB_PATH})
	target_include_directories(${name} PUBLIC ${BOOST_LIB_PATH})
	
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
	
	# Target
	target_compile_features(${name}
		PUBLIC
			cxx_variadic_templates
			cxx_nullptr
		PRIVATE
			cxx_lambdas
	)
endfunction()

