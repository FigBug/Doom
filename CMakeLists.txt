cmake_minimum_required (VERSION 3.16.0 FATAL_ERROR)

#
# Set for each plugin
#
file (READ "${CMAKE_CURRENT_LIST_DIR}/VERSION" PLUGIN_VERSION)
string (STRIP "${PLUGIN_VERSION}" PLUGIN_VERSION)
set (PLUGIN_NAME Doom)
set (BUNDLE_ID com.SocaLabs.Doom)
set (AU_ID DoomAU)
set (LV2_URI "https://socalabs.com/Doom/")
set (PLUGIN_CODE SLor)

set (CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set (CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set (CMAKE_SUPPRESS_REGENERATION true)
set (CMAKE_SKIP_INSTALL_RULES YES)
set (CMAKE_POSITION_INDEPENDENT_CODE ON)
set_property (GLOBAL PROPERTY DEBUG_CONFIGURATIONS "Debug")

set (CMAKE_C_FLAGS_DEVELOPMENT ${CMAKE_C_FLAGS_RELEASE})
set (CMAKE_CXX_FLAGS_DEVELOPMENT ${CMAKE_CXX_FLAGS_RELEASE})

project (${PLUGIN_NAME} VERSION ${PLUGIN_VERSION} LANGUAGES CXX C HOMEPAGE_URL "https://socalabs.com/")

include (CMakeDependentOption)

set_property (DIRECTORY APPEND PROPERTY LABELS ${PLUGIN_NAME})

set_property (DIRECTORY APPEND PROPERTY LABELS SocaLabs)
set_property (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PLUGIN_NAME}_Standalone)

set (CMAKE_OSX_DEPLOYMENT_TARGET 10.13)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION ON)
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_OBJCXX_STANDARD 20)
set (CMAKE_OBJCXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_VISIBILITY_PRESET hidden)
set (CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set (CMAKE_MINSIZEREL_POSTFIX -rm)
set (CMAKE_RELWITHDEBINFO_POSTFIX -rd)
set (CMAKE_OPTIMIZE_DEPENDENCIES OFF)

set (BUILD_SHARED_LIBS OFF)

if (APPLE)
	set (CMAKE_OSX_ARCHITECTURES arm64 x86_64)
endif()

if (WIN32)
	set (FORMATS Standalone VST3)
elseif (APPLE)
	set (FORMATS Standalone VST3 AU)
else()
	set (FORMATS Standalone LV2)
endif()

set_property (GLOBAL PROPERTY USE_FOLDERS YES)
set_property (GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER utility)
set_property (GLOBAL PROPERTY REPORT_UNDEFINED_PROPERTIES "${CMAKE_BINARY_DIR}/undefined_properties.log")
set_property (GLOBAL PROPERTY JUCE_COPY_PLUGIN_AFTER_BUILD NO)

set_property (DIRECTORY APPEND PROPERTY LABELS External)

# JUCE

set (JUCE_MODULES_ONLY OFF)
set (JUCE_ENABLE_MODULE_SOURCE_GROUPS ON)
set (JUCE_BUILD_EXTRAS OFF)
set (JUCE_BUILD_EXAMPLES OFF)

add_subdirectory (modules/JUCE)

set_property (DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/modules/JUCE" APPEND PROPERTY LABELS JUCE)

#

# Gin modules

foreach(module_name IN ITEMS gin gin_dsp gin_graphics gin_gui gin_metadata gin_network gin_plugin gin_simd gin_standaloneplugin)
	juce_add_module (
		"${CMAKE_CURRENT_LIST_DIR}/modules/Gin/modules/${module_name}"
		)

	set_property (TARGET "${module_name}" APPEND PROPERTY LABELS Gin)
endforeach()

# Gin Doom
juce_add_module ("${CMAKE_CURRENT_LIST_DIR}/modules/Gin_Doom/modules/gin_doom")
set_property (TARGET gin_doom APPEND PROPERTY LABELS Gin)

# Binary Data

set_property (DIRECTORY APPEND PROPERTY LABELS Assets)

juce_add_binary_data (${PLUGIN_NAME}_Assets SOURCES
	"${CMAKE_CURRENT_LIST_DIR}/plugin/Assets/DOOM1.WAD")

juce_add_plugin (${PLUGIN_NAME}
				 PRODUCT_NAME ${PLUGIN_NAME}
				 VERSION ${PLUGIN_VERSION}
				 COMPANY_NAME SocaLabs
				 COMPANY_WEBSITE "https://socalabs.com/"
				 BUNDLE_ID ${BUNDLE_ID}
				 FORMATS ${FORMATS}
				 PLUGIN_MANUFACTURER_CODE Soca
				 PLUGIN_CODE ${PLUGIN_CODE}
				 IS_SYNTH ON
				 NEEDS_MIDI_INPUT ON
				 EDITOR_WANTS_KEYBOARD_FOCUS ON
				 VST3_CATEGORIES Instrument
				 AU_MAIN_TYPE kAudioUnitType_MusicDevice
				 AU_EXPORT_PREFIX ${AU_ID}
				 AU_SANDBOX_SAFE FALSE
				 LV2URI ${LV2_URI})

file (GLOB_RECURSE source_files CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin/Source/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin/Source/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin/Source/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin/Source/*.h)

target_sources (${PLUGIN_NAME} PRIVATE ${source_files})
source_group (TREE ${CMAKE_CURRENT_SOURCE_DIR}/plugin/Source PREFIX Source FILES ${source_files})

target_link_libraries (${PLUGIN_NAME} PRIVATE
						${PLUGIN_NAME}_Assets

						gin
						gin_dsp
						gin_graphics
						gin_gui
						gin_plugin
						gin_simd
						gin_standaloneplugin

						gin_doom

						juce::juce_audio_basics
						juce::juce_audio_devices
						juce::juce_audio_formats
						juce::juce_audio_plugin_client
						juce::juce_audio_processors
						juce::juce_audio_utils
						juce::juce_core
						juce::juce_cryptography
						juce::juce_data_structures
						juce::juce_events
						juce::juce_graphics
						juce::juce_gui_basics
						juce::juce_gui_extra

    					juce::juce_recommended_config_flags
					)

juce_generate_juce_header (${PLUGIN_NAME})

target_compile_definitions (${PLUGIN_NAME} PRIVATE
								JUCE_DISPLAY_SPLASH_SCREEN=0
								JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP=1
								JUCE_COREGRAPHICS_DRAW_ASYNC=1
								JUCE_MODAL_LOOPS_PERMITTED=1
								JUCE_STRICT_REFCOUNTEDPOINTER=1
								JUCE_VST3_CAN_REPLACE_VST2=0
								JUCE_WEB_BROWSER=0
								JUCE_USE_FLAC=0
								JUCE_USE_CURL=1
								JUCE_USE_MP3AUDIOFORMAT=0
								JUCE_USE_LAME_AUDIO_FORMAT=0
								JUCE_USE_WINDOWS_MEDIA_FORMAT=0
								JUCE_SILENCE_XCODE_15_LINKER_WARNING=1
								JUCE_IGNORE_VST3_MISMATCHED_PARAMETER_ID_WARNING=1
								JucePlugin_PreferredChannelConfigurations={0,2}
								_CRT_SECURE_NO_WARNINGS
							)

if (APPLE)
    foreach(t ${FORMATS} "Assets" "All" "")
        set(tgt ${CMAKE_PROJECT_NAME})
        if (NOT t STREQUAL "")
            set(tgt ${tgt}_${t})
        endif()
        if (TARGET ${tgt})
			set_target_properties(${tgt} PROPERTIES
				XCODE_ATTRIBUTE_CLANG_LINK_OBJC_RUNTIME NO
				XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH[variant=Debug] "YES"
				XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT[variant=Release] "dwarf-with-dsym"
				)
			if (NOT t STREQUAL "All")
				target_compile_options(${tgt} PRIVATE
					-Wall -Wstrict-aliasing -Wunused-parameter -Wconditional-uninitialized -Woverloaded-virtual -Wreorder -Wconstant-conversion -Wbool-conversion -Wextra-semi
					-Wunreachable-code -Winconsistent-missing-destructor-override -Wshift-sign-overflow -Wnullable-to-nonnull-conversion -Wuninitialized -Wno-missing-field-initializers
					-Wno-ignored-qualifiers -Wno-missing-braces -Wno-char-subscripts -Wno-unused-private-field -fno-aligned-allocation -Wunused-private-field -Wunreachable-code
					-Wenum-compare -Wshadow -Wfloat-conversion -Wshadow-uncaptured-local -Wshadow-field -Wsign-compare -Wdeprecated-this-capture -Wimplicit-float-conversion
					-ffast-math -Wno-implicit-int-float-conversion -fno-finite-math-only)
			endif()
       	endif()
    endforeach()
endif()

if (WIN32)
    foreach(t ${FORMATS} "Assets" "All" "")
        set(tgt ${CMAKE_PROJECT_NAME})
        if (NOT t STREQUAL "")
            set(tgt ${tgt}_${t})
        endif()
        if (TARGET ${tgt})
			set_property(TARGET ${tgt} APPEND_STRING PROPERTY LINK_FLAGS_DEBUG " /INCREMENTAL:NO")
			set_target_properties(${tgt} PROPERTIES LINK_FLAGS "/ignore:4099")
		endif()
	endforeach()

	# Generate debug symbols (PDBs) for Release builds so crashes can be symbolicated.
	# /Zi emits full debug info; /DEBUG produces the .pdb; /OPT:REF and /OPT:ICF restore
	# the size optimisations that /DEBUG otherwise disables.
	foreach(t ${FORMATS} "")
        set(tgt ${CMAKE_PROJECT_NAME})
        if (NOT t STREQUAL "")
            set(tgt ${tgt}_${t})
        endif()
        if (TARGET ${tgt})
			target_compile_options(${tgt} PRIVATE "$<$<CONFIG:Release>:/Zi>")
			target_link_options(${tgt} PRIVATE "$<$<CONFIG:Release>:/DEBUG>")
			target_link_options(${tgt} PRIVATE "$<$<CONFIG:Release>:/OPT:REF>")
			target_link_options(${tgt} PRIVATE "$<$<CONFIG:Release>:/OPT:ICF>")
		endif()
	endforeach()
endif()

if (UNIX AND NOT APPLE)
	target_link_libraries (${PLUGIN_NAME} PRIVATE curl)

	# Emit debug info in Release so symbols can be split out for symbolication.
	foreach(t ${FORMATS} "Assets" "")
		set(tgt ${CMAKE_PROJECT_NAME})
		if (NOT t STREQUAL "")
			set(tgt ${tgt}_${t})
		endif()
		if (TARGET ${tgt})
			target_compile_options(${tgt} PRIVATE "$<$<CONFIG:Release>:-g>")
		endif()
	endforeach()
endif()
