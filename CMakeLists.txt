# CMakeLists.txt
cmake_minimum_required(VERSION 3.14)
project(MyLibrary
    VERSION 1.0.6
    LANGUAGES CXX C
)

# Let user choose STATIC vs SHARED (default to SHARED)
option(BUILD_SHARED_LIBS "Build ChemDraw as a shared library" ON)

include(TestBigEndian)
test_big_endian(WORDS_BIGENDIAN)
#/* 1234 = LIL_ENDIAN, 4321 = BIGENDIAN */

if(WORDS_BIGENDIAN)
    add_definitions("-DPLATFORM_BIGENDIAN")	
else(WORDS_BIGENDIAN)
    add_definitions("-DPLATFORM_LITTLEENDIAN")
endif(WORDS_BIGENDIAN)


# Build expat
add_subdirectory(expatpp)

set(HEADERS
    chemdraw/Base64Converter.h
    chemdraw/BasicTypes.h
    chemdraw/CDMap.h
    chemdraw/CDXAttachedDataHelper.h
    chemdraw/CDXBasicTypes.h
    chemdraw/CDXColoredMolecularArea.h
    chemdraw/CDXConstants.h
    chemdraw/CDXDocumentProperty.h
    chemdraw/CDXDocumentPropertyCollection.h
    chemdraw/CDXFontTable.h
    chemdraw/CDXIO.h
    chemdraw/CDXMLNames.h
    chemdraw/CDXMLParser.h
    chemdraw/CDXObject.h
    chemdraw/CDXStdObjects.h
    chemdraw/CDXUnicode.h
    chemdraw/CDXUtils_Win.h
    chemdraw/CDXUtils.h
    chemdraw/CoreChemistryAPI.h
    chemdraw/cs_assert.h
    chemdraw/cs_auto_buffer.h
    chemdraw/cs_charUtils.h
    chemdraw/cs_lock.h
    chemdraw/cs_mapUtils.h
    chemdraw/cs_math.h
    chemdraw/cs_specialchardefs.h
    chemdraw/cs_stringUtils.h
    chemdraw/cs_swapBytes.h
    chemdraw/cs_univDefs.h
    chemdraw/ffMDL.h
    chemdraw/ffMIMETypes.h
    chemdraw/UTF8Iterator.h
    chemdraw/UTF8String.h
    chemdraw/XMLDoc.h
    chemdraw/XMLParser.h
    chemdraw/XMLPutEnum.h)

add_library(ChemDraw
    chemdraw/Base64Converter.cpp
    chemdraw/CDXAnnotation.cpp
    chemdraw/CDXArrow.cpp
    chemdraw/CDXAttachedDataHelper.cpp
    chemdraw/CDXBandMarker.cpp
    chemdraw/CDXBioDraw.cpp
    chemdraw/CDXBond.cpp
    chemdraw/CDXBorder.cpp
    chemdraw/CDXBracketedGroup.cpp
    chemdraw/CDXChemicalProperty.cpp
    chemdraw/CDXColoredMolecularArea.cpp
    chemdraw/CDXColorTable.cpp
    chemdraw/CDXConstraint.cpp
    chemdraw/CDXCurve.cpp
    chemdraw/CDXDocument.cpp
    chemdraw/CDXDocumentProperty.cpp
    chemdraw/CDXDocumentPropertyCollection.cpp
    chemdraw/CDXEmbeddedObject.cpp
    chemdraw/CDXFontTable.cpp
    chemdraw/CDXFragment.cpp
    chemdraw/CDXGeometry.cpp
    chemdraw/CDXGEPBand.cpp
    chemdraw/CDXGEPPlate.cpp
    chemdraw/CDXGraphic.cpp
    chemdraw/CDXGraphicObject.cpp
    chemdraw/CDXGroup.cpp
    chemdraw/CDXGroupOrFragment.cpp
    chemdraw/CDXIndicatorText.cpp
    chemdraw/CDXIO.cpp
    chemdraw/CDXMLParser.cpp
    chemdraw/CDXNamedAlternativeGroup.cpp
    chemdraw/CDXNode.cpp
    chemdraw/CDXObject.cpp
    chemdraw/CDXObjectTag.cpp
    chemdraw/CDXPage.cpp
    chemdraw/CDXPlasmidMap.cpp
    chemdraw/CDXPlateBase.cpp
    chemdraw/CDXPlateItemBase.cpp
    chemdraw/CDXReactionStep.cpp
    chemdraw/CDXRLogic.cpp
    chemdraw/CDXSpectrum.cpp
    chemdraw/CDXSplitter.cpp
    chemdraw/CDXStdObjects.cpp
    chemdraw/CDXStoichiometryGrid.cpp
    chemdraw/CDXTable.cpp
    chemdraw/CDXTemplateGrid.cpp
    chemdraw/CDXTest.cpp
    chemdraw/CDXText.cpp
    chemdraw/CDXTLCLane.cpp
    chemdraw/CDXTLCPlate.cpp
    chemdraw/CDXTLCSpot.cpp
    chemdraw/CDXUnicode.cpp
    chemdraw/CDXUtils_Win.cpp
    chemdraw/CDXUtils.cpp
    chemdraw/XMLDoc.cpp
    chemdraw/XMLParser.cpp
)

set(BOOST_VERSION "1.81.0")
find_package(Boost ${BOOST_VERSION} REQUIRED CONFIG)

target_include_directories(ChemDraw PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(ChemDraw INTERFACE Boost::boost PUBLIC expat)

# Install headers (optional)
target_include_directories(ChemDraw
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/chemdraw>
        $<INSTALL_INTERFACE:include>
)

# On Windows, define MYLIB_EXPORTS when building the DLL
if (WIN32)
    target_compile_definitions(ChemDraw
        PRIVATE CHEMDRAW_EXPORTS
    )
endif()

# On Linux/macOS, hide all symbols by default and expose only our API
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "Emscripten")
    # Require -fvisibility=hidden
    set_target_properties(ChemDraw PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
    )
endif()


# export all the symbols for ChemDraw on MSVC                                                 
if((MSVC AND BUILD_SHARED_LIBS) OR ((NOT MSVC) AND WIN32))                                
    set_target_properties(ChemDraw PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)                
endif()                                                                                       
                                                                                              
if(MSVC)                                                                                      
  ADD_DEFINITIONS("-DTARGET_API_LIB -D_WINDOWS -DTARGET_OS_WIN32 -DHAVE_EXPAT_CONFIG_H")      
  # we don't really control chemdraw source code, so suppress warnings                        
  target_compile_options(ChemDraw PRIVATE "/W0")                                              
else()                                                                                        
  ADD_DEFINITIONS("-DTARGET_API_LIB -D__linux -DHAVE_EXPAT_CONFIG_H")                         
  # we don't really control chemdraw source code, so suppress warnings                        
  target_compile_options(ChemDraw PRIVATE -w -Wno-unknown-pragmas -Wno-error)                 
endif()                                                                                       

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-comment -Wno-parentheses -Wno-logical-op-parentheses -Wno-pointer-bool-conversion -Wno-unused-value -Wno-unsequenced -Wno-constant-logical-operand")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Gnu")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wformat-overflow=0 -Wformat=0 -Wno-format-security")
endif()


# Example: install rules
install(TARGETS ChemDraw
    EXPORT ChemDrawTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${HEADERS} DESTINATION include)


# Export config for downstream projects
install(EXPORT ChemDrawTargets
    FILE ChemDrawConfig.cmake
    NAMESPACE ChemDraw::
    DESTINATION lib/cmake/ChemDraw
)
