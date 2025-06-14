# This file makes the expat library and C++ together
#  the only real change here is to export all symbols in MSVC land and combine C++/c
#  There has to be a better way to do this, but I've had a lot of trouble exporting
#  the C implementations of the C++ class in MSVC



include(${CMAKE_CURRENT_SOURCE_DIR}/ConfigureExpatChecks.cmake)

include_directories(${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/expat/lib)
if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -wd4996)
endif(MSVC)

set(_SHARED SHARED)

# expat c lib sources
set(EXPAT_SRC
	expatpp-code-r6-trunk/expat/lib/xmlparse.c
	expatpp-code-r6-trunk/expat/lib/xmlrole.c
	expatpp-code-r6-trunk/expat/lib/xmltok.c
	expatpp-code-r6-trunk/expat/lib/xmltok_impl.c
	expatpp-code-r6-trunk/expat/lib/xmltok_ns.c
	expatpp-code-r6-trunk/src_pp/expatpp.cpp
)

add_library(expat ${EXPAT_SRC})
target_include_directories(expat
     PRIVATE
       ${CMAKE_CURRENT_SOURCE_DIR}/expatpp-code-r6-trunk/src_pp
     PUBLIC
       $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/expatpp-code-r6-trunk/expat/lib>
       $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/expatpp-code-r6-trunk/src_pp>
       $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
       )
       
if(EXPAT_INSTALL_STATIC_LIBS)
  target_include_directories(expat_static
     PRIVATE
       ${CMAKE_CURRENT_SOURCE_DIR}/expatpp-code-r6-trunk/src_pp
     PUBLIC
       $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/expatpp-code-r6-trunk/expat/lib>
       $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/expatpp-code-r6-trunk/src_pp>
       $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
       )
endif(EXPAT_INSTALL_STATIC_LIBS)

if(MSVC)
  # == expat exporting all symbols
  target_compile_definitions(expat PRIVATE "XMLIMPORT=__declspec(dllexport)")
  set_target_properties(expat PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
  
  # turn off warnings as errors, this throws a bunch
  target_compile_options(expat PRIVATE "/W0")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "Emscripten")
  message("== expat turn off warnings as errors when building")
  # turn off warnings as errors, this throws a bunch
  target_compile_options(expat PRIVATE "-Wno-error")
  if(EXPAT_INSTALL_STATIC_LIBS)
    target_compile_options(expat_static PRIVATE "-Wno-error")
  endif(EXPAT_INSTALL_STATIC_LIBS)   
endif()

install(TARGETS expat EXPORT expatTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(EXPORT expatTargets DESTINATION lib/cmake/expat)
