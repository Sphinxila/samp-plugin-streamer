if ("${EIGEN_LIB_PATH}" STREQUAL "")
  message( FATAL_ERROR "Please specifiy an EIGEN_LIB_PATH" )
endif()

if ("${BOOST_LIB_PATH}" STREQUAL "")
  message( FATAL_ERROR "Please specifiy a BOOST_LIB_PATH" )
endif()

if ("${SAMPSDK_DIR}" STREQUAL "")
  message( FATAL_ERROR "Please specifiy a SAMPSDK_DIR" )
endif()