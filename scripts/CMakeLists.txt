cmake_minimum_required(VERSION 3.5)

if(NOT DEFINED QPC_PLATFORM)
	set(QPC_PLATFORM "WINDOWS")
endif()

if(NOT DEFINED QPC_ARCH)
	set(QPC_ARCH "AMD64")
endif()

if( (CMAKE_BUILD_TYPE STREQUAL Debug) AND (QPC_PLATFORM STREQUAL WINDOWS) AND (QPC_ARCH STREQUAL AMD64) )

	set(
		QPC_SRC_FILES
		"D:/projects/qpc2/src/args.cpp"
		"D:/projects/qpc2/src/base_generator.cpp"
		"D:/projects/qpc2/src/builder.cpp"
		"D:/projects/qpc2/src/conditions.cpp"
		"D:/projects/qpc2/src/config.cpp"
		"D:/projects/qpc2/src/generator_handler.cpp"
		"D:/projects/qpc2/src/lexer.cpp"
		"D:/projects/qpc2/src/logging.cpp"
		"D:/projects/qpc2/src/project.cpp"
		"D:/projects/qpc2/src/project_manager.cpp"
		"D:/projects/qpc2/src/qpc.cpp"
		"D:/projects/qpc2/src/util.cpp"
	)

	set(
		QPC_INC_FILES
		"D:/projects/qpc2/inc/args.h"
		"D:/projects/qpc2/inc/base_generator.h"
		"D:/projects/qpc2/inc/builder.h"
		"D:/projects/qpc2/inc/conditions.h"
		"D:/projects/qpc2/inc/config.h"
		"D:/projects/qpc2/inc/generator_handler.h"
		"D:/projects/qpc2/inc/lexer.h"
		"D:/projects/qpc2/inc/logging.h"
		"D:/projects/qpc2/inc/project.h"
		"D:/projects/qpc2/inc/project_manager.h"
		"D:/projects/qpc2/inc/util.h"
	)

	add_executable( QPC ${QPC_SRC_FILES} ${QPC_INC_FILES} )

	set_target_properties(
		QPC PROPERTIES
		PREFIX ""
		OUTPUT_NAME "qpc"
		CXX_COMPILER "msvc"
		CXX_STANDARD 17
		RUNTIME_OUTPUT_DIRECTORY "D:/projects/qpc2/out/win"
		ARCHIVE_OUTPUT_DIRECTORY "D:/projects/qpc2/obj/qpc_launcher/Debug"
	)

	target_include_directories(
		QPC PRIVATE
		"D:/projects/qpc2/src"
		"D:/projects/qpc2/inc"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/include"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/include"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/include"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/ucrt"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/UnitTest/include"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/um"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/shared"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/winrt"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/cppwinrt"
		"C:/Program Files (x86)/Windows Kits/NETFXSDK/4.8/Include/um"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/mfc"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/mfcm"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/atl"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/crt/src"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/src"
		"C:/Program Files (x86)/Windows Kits/10/Source/10.0.18362.0/ucrt"
	)

	target_compile_options(
		QPC PRIVATE
		/Od
		/MTd
		/ZI
	)

elseif( (CMAKE_BUILD_TYPE STREQUAL Debug) AND (QPC_PLATFORM STREQUAL WINDOWS) AND (QPC_ARCH STREQUAL I386) )

	set(
		QPC_SRC_FILES
		"D:/projects/qpc2/src/args.cpp"
		"D:/projects/qpc2/src/base_generator.cpp"
		"D:/projects/qpc2/src/builder.cpp"
		"D:/projects/qpc2/src/conditions.cpp"
		"D:/projects/qpc2/src/config.cpp"
		"D:/projects/qpc2/src/generator_handler.cpp"
		"D:/projects/qpc2/src/lexer.cpp"
		"D:/projects/qpc2/src/logging.cpp"
		"D:/projects/qpc2/src/project.cpp"
		"D:/projects/qpc2/src/project_manager.cpp"
		"D:/projects/qpc2/src/qpc.cpp"
		"D:/projects/qpc2/src/util.cpp"
	)

	set(
		QPC_INC_FILES
		"D:/projects/qpc2/inc/args.h"
		"D:/projects/qpc2/inc/base_generator.h"
		"D:/projects/qpc2/inc/builder.h"
		"D:/projects/qpc2/inc/conditions.h"
		"D:/projects/qpc2/inc/config.h"
		"D:/projects/qpc2/inc/generator_handler.h"
		"D:/projects/qpc2/inc/lexer.h"
		"D:/projects/qpc2/inc/logging.h"
		"D:/projects/qpc2/inc/project.h"
		"D:/projects/qpc2/inc/project_manager.h"
		"D:/projects/qpc2/inc/util.h"
	)

	add_executable( QPC ${QPC_SRC_FILES} ${QPC_INC_FILES} )

	set_target_properties(
		QPC PROPERTIES
		PREFIX ""
		OUTPUT_NAME "qpc"
		CXX_COMPILER "msvc"
		CXX_STANDARD 17
		RUNTIME_OUTPUT_DIRECTORY "D:/projects/qpc2/out/win"
		ARCHIVE_OUTPUT_DIRECTORY "D:/projects/qpc2/obj/qpc_launcher/Debug"
	)

	target_include_directories(
		QPC PRIVATE
		"D:/projects/qpc2/src"
		"D:/projects/qpc2/inc"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/include"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/include"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/include"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/ucrt"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/UnitTest/include"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/um"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/shared"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/winrt"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/cppwinrt"
		"C:/Program Files (x86)/Windows Kits/NETFXSDK/4.8/Include/um"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/mfc"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/mfcm"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/atl"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/crt/src"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/src"
		"C:/Program Files (x86)/Windows Kits/10/Source/10.0.18362.0/ucrt"
	)

	target_compile_options(
		QPC PRIVATE
		/Od
		/MTd
		/ZI
	)

elseif( (CMAKE_BUILD_TYPE STREQUAL Release) AND (QPC_PLATFORM STREQUAL WINDOWS) AND (QPC_ARCH STREQUAL AMD64) )

	set(
		QPC_SRC_FILES
		"D:/projects/qpc2/src/args.cpp"
		"D:/projects/qpc2/src/base_generator.cpp"
		"D:/projects/qpc2/src/builder.cpp"
		"D:/projects/qpc2/src/conditions.cpp"
		"D:/projects/qpc2/src/config.cpp"
		"D:/projects/qpc2/src/generator_handler.cpp"
		"D:/projects/qpc2/src/lexer.cpp"
		"D:/projects/qpc2/src/logging.cpp"
		"D:/projects/qpc2/src/project.cpp"
		"D:/projects/qpc2/src/project_manager.cpp"
		"D:/projects/qpc2/src/qpc.cpp"
		"D:/projects/qpc2/src/util.cpp"
	)

	set(
		QPC_INC_FILES
		"D:/projects/qpc2/inc/args.h"
		"D:/projects/qpc2/inc/base_generator.h"
		"D:/projects/qpc2/inc/builder.h"
		"D:/projects/qpc2/inc/conditions.h"
		"D:/projects/qpc2/inc/config.h"
		"D:/projects/qpc2/inc/generator_handler.h"
		"D:/projects/qpc2/inc/lexer.h"
		"D:/projects/qpc2/inc/logging.h"
		"D:/projects/qpc2/inc/project.h"
		"D:/projects/qpc2/inc/project_manager.h"
		"D:/projects/qpc2/inc/util.h"
	)

	add_executable( QPC ${QPC_SRC_FILES} ${QPC_INC_FILES} )

	set_target_properties(
		QPC PROPERTIES
		PREFIX ""
		OUTPUT_NAME "qpc"
		CXX_COMPILER "msvc"
		CXX_STANDARD 17
		RUNTIME_OUTPUT_DIRECTORY "D:/projects/qpc2/out/win"
		ARCHIVE_OUTPUT_DIRECTORY "D:/projects/qpc2/obj/qpc_launcher/Release"
	)

	target_include_directories(
		QPC PRIVATE
		"D:/projects/qpc2/src"
		"D:/projects/qpc2/inc"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/include"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/include"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/include"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/ucrt"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/UnitTest/include"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/um"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/shared"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/winrt"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/cppwinrt"
		"C:/Program Files (x86)/Windows Kits/NETFXSDK/4.8/Include/um"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/mfc"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/mfcm"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/atl"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/crt/src"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/src"
		"C:/Program Files (x86)/Windows Kits/10/Source/10.0.18362.0/ucrt"
	)

	target_compile_options(
		QPC PRIVATE
		/O2
		/MT
	)

elseif( (CMAKE_BUILD_TYPE STREQUAL Release) AND (QPC_PLATFORM STREQUAL WINDOWS) AND (QPC_ARCH STREQUAL I386) )

	set(
		QPC_SRC_FILES
		"D:/projects/qpc2/src/args.cpp"
		"D:/projects/qpc2/src/base_generator.cpp"
		"D:/projects/qpc2/src/builder.cpp"
		"D:/projects/qpc2/src/conditions.cpp"
		"D:/projects/qpc2/src/config.cpp"
		"D:/projects/qpc2/src/generator_handler.cpp"
		"D:/projects/qpc2/src/lexer.cpp"
		"D:/projects/qpc2/src/logging.cpp"
		"D:/projects/qpc2/src/project.cpp"
		"D:/projects/qpc2/src/project_manager.cpp"
		"D:/projects/qpc2/src/qpc.cpp"
		"D:/projects/qpc2/src/util.cpp"
	)

	set(
		QPC_INC_FILES
		"D:/projects/qpc2/inc/args.h"
		"D:/projects/qpc2/inc/base_generator.h"
		"D:/projects/qpc2/inc/builder.h"
		"D:/projects/qpc2/inc/conditions.h"
		"D:/projects/qpc2/inc/config.h"
		"D:/projects/qpc2/inc/generator_handler.h"
		"D:/projects/qpc2/inc/lexer.h"
		"D:/projects/qpc2/inc/logging.h"
		"D:/projects/qpc2/inc/project.h"
		"D:/projects/qpc2/inc/project_manager.h"
		"D:/projects/qpc2/inc/util.h"
	)

	add_executable( QPC ${QPC_SRC_FILES} ${QPC_INC_FILES} )

	set_target_properties(
		QPC PROPERTIES
		PREFIX ""
		OUTPUT_NAME "qpc"
		CXX_COMPILER "msvc"
		CXX_STANDARD 17
		RUNTIME_OUTPUT_DIRECTORY "D:/projects/qpc2/out/win"
		ARCHIVE_OUTPUT_DIRECTORY "D:/projects/qpc2/obj/qpc_launcher/Release"
	)

	target_include_directories(
		QPC PRIVATE
		"D:/projects/qpc2/src"
		"D:/projects/qpc2/inc"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/include"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/include"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/include"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/ucrt"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/UnitTest/include"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/um"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/shared"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/winrt"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/cppwinrt"
		"C:/Program Files (x86)/Windows Kits/NETFXSDK/4.8/Include/um"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/mfc"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/mfcm"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/atl"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/crt/src"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/src"
		"C:/Program Files (x86)/Windows Kits/10/Source/10.0.18362.0/ucrt"
	)

	target_compile_options(
		QPC PRIVATE
		/O2
		/MT
	)

endif()

if( (CMAKE_BUILD_TYPE STREQUAL Debug) AND (QPC_PLATFORM STREQUAL WINDOWS) AND (QPC_ARCH STREQUAL AMD64) )

	set(
		VSTUDIO_SRC_FILES
		"D:/projects/qpc2/src/gen/vstudio.cpp"
	)

	set(
		VSTUDIO_INC_FILES
		"D:/projects/qpc2/inc/args.h"
		"D:/projects/qpc2/inc/base_generator.h"
		"D:/projects/qpc2/inc/builder.h"
		"D:/projects/qpc2/inc/conditions.h"
		"D:/projects/qpc2/inc/config.h"
		"D:/projects/qpc2/inc/generator_handler.h"
		"D:/projects/qpc2/inc/lexer.h"
		"D:/projects/qpc2/inc/logging.h"
		"D:/projects/qpc2/inc/project.h"
		"D:/projects/qpc2/inc/project_manager.h"
		"D:/projects/qpc2/inc/util.h"
	)

	add_library( VSTUDIO SHARED ${VSTUDIO_SRC_FILES} ${VSTUDIO_INC_FILES} )

	set_target_properties(
		VSTUDIO PROPERTIES
		PREFIX ""
		OUTPUT_NAME "vstudio"
		CXX_COMPILER "msvc"
		CXX_STANDARD 17
		RUNTIME_OUTPUT_DIRECTORY "D:/projects/qpc2/out/win"
		ARCHIVE_OUTPUT_DIRECTORY "D:/projects/qpc2/obj/vstudio/Debug"
	)

	target_include_directories(
		VSTUDIO PRIVATE
		"D:/projects/qpc2/src"
		"D:/projects/qpc2/inc"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/include"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/include"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/include"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/ucrt"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/UnitTest/include"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/um"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/shared"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/winrt"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/cppwinrt"
		"C:/Program Files (x86)/Windows Kits/NETFXSDK/4.8/Include/um"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/mfc"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/mfcm"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/atl"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/crt/src"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/src"
		"C:/Program Files (x86)/Windows Kits/10/Source/10.0.18362.0/ucrt"
	)

	target_compile_options(
		VSTUDIO PRIVATE
		/Od
		/MTd
		/ZI
	)

elseif( (CMAKE_BUILD_TYPE STREQUAL Debug) AND (QPC_PLATFORM STREQUAL WINDOWS) AND (QPC_ARCH STREQUAL I386) )

	set(
		VSTUDIO_SRC_FILES
		"D:/projects/qpc2/src/gen/vstudio.cpp"
	)

	set(
		VSTUDIO_INC_FILES
		"D:/projects/qpc2/inc/args.h"
		"D:/projects/qpc2/inc/base_generator.h"
		"D:/projects/qpc2/inc/builder.h"
		"D:/projects/qpc2/inc/conditions.h"
		"D:/projects/qpc2/inc/config.h"
		"D:/projects/qpc2/inc/generator_handler.h"
		"D:/projects/qpc2/inc/lexer.h"
		"D:/projects/qpc2/inc/logging.h"
		"D:/projects/qpc2/inc/project.h"
		"D:/projects/qpc2/inc/project_manager.h"
		"D:/projects/qpc2/inc/util.h"
	)

	add_library( VSTUDIO SHARED ${VSTUDIO_SRC_FILES} ${VSTUDIO_INC_FILES} )

	set_target_properties(
		VSTUDIO PROPERTIES
		PREFIX ""
		OUTPUT_NAME "vstudio"
		CXX_COMPILER "msvc"
		CXX_STANDARD 17
		RUNTIME_OUTPUT_DIRECTORY "D:/projects/qpc2/out/win"
		ARCHIVE_OUTPUT_DIRECTORY "D:/projects/qpc2/obj/vstudio/Debug"
	)

	target_include_directories(
		VSTUDIO PRIVATE
		"D:/projects/qpc2/src"
		"D:/projects/qpc2/inc"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/include"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/include"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/include"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/ucrt"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/UnitTest/include"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/um"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/shared"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/winrt"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/cppwinrt"
		"C:/Program Files (x86)/Windows Kits/NETFXSDK/4.8/Include/um"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/mfc"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/mfcm"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/atl"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/crt/src"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/src"
		"C:/Program Files (x86)/Windows Kits/10/Source/10.0.18362.0/ucrt"
	)

	target_compile_options(
		VSTUDIO PRIVATE
		/Od
		/MTd
		/ZI
	)

elseif( (CMAKE_BUILD_TYPE STREQUAL Release) AND (QPC_PLATFORM STREQUAL WINDOWS) AND (QPC_ARCH STREQUAL AMD64) )

	set(
		VSTUDIO_SRC_FILES
		"D:/projects/qpc2/src/gen/vstudio.cpp"
	)

	set(
		VSTUDIO_INC_FILES
		"D:/projects/qpc2/inc/args.h"
		"D:/projects/qpc2/inc/base_generator.h"
		"D:/projects/qpc2/inc/builder.h"
		"D:/projects/qpc2/inc/conditions.h"
		"D:/projects/qpc2/inc/config.h"
		"D:/projects/qpc2/inc/generator_handler.h"
		"D:/projects/qpc2/inc/lexer.h"
		"D:/projects/qpc2/inc/logging.h"
		"D:/projects/qpc2/inc/project.h"
		"D:/projects/qpc2/inc/project_manager.h"
		"D:/projects/qpc2/inc/util.h"
	)

	add_library( VSTUDIO SHARED ${VSTUDIO_SRC_FILES} ${VSTUDIO_INC_FILES} )

	set_target_properties(
		VSTUDIO PROPERTIES
		PREFIX ""
		OUTPUT_NAME "vstudio"
		CXX_COMPILER "msvc"
		CXX_STANDARD 17
		RUNTIME_OUTPUT_DIRECTORY "D:/projects/qpc2/out/win"
		ARCHIVE_OUTPUT_DIRECTORY "D:/projects/qpc2/obj/vstudio/Release"
	)

	target_include_directories(
		VSTUDIO PRIVATE
		"D:/projects/qpc2/src"
		"D:/projects/qpc2/inc"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/include"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/include"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/include"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/ucrt"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/UnitTest/include"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/um"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/shared"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/winrt"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/cppwinrt"
		"C:/Program Files (x86)/Windows Kits/NETFXSDK/4.8/Include/um"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/mfc"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/mfcm"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/atl"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/crt/src"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/src"
		"C:/Program Files (x86)/Windows Kits/10/Source/10.0.18362.0/ucrt"
	)

	target_compile_options(
		VSTUDIO PRIVATE
		/O2
		/MT
	)

elseif( (CMAKE_BUILD_TYPE STREQUAL Release) AND (QPC_PLATFORM STREQUAL WINDOWS) AND (QPC_ARCH STREQUAL I386) )

	set(
		VSTUDIO_SRC_FILES
		"D:/projects/qpc2/src/gen/vstudio.cpp"
	)

	set(
		VSTUDIO_INC_FILES
		"D:/projects/qpc2/inc/args.h"
		"D:/projects/qpc2/inc/base_generator.h"
		"D:/projects/qpc2/inc/builder.h"
		"D:/projects/qpc2/inc/conditions.h"
		"D:/projects/qpc2/inc/config.h"
		"D:/projects/qpc2/inc/generator_handler.h"
		"D:/projects/qpc2/inc/lexer.h"
		"D:/projects/qpc2/inc/logging.h"
		"D:/projects/qpc2/inc/project.h"
		"D:/projects/qpc2/inc/project_manager.h"
		"D:/projects/qpc2/inc/util.h"
	)

	add_library( VSTUDIO SHARED ${VSTUDIO_SRC_FILES} ${VSTUDIO_INC_FILES} )

	set_target_properties(
		VSTUDIO PROPERTIES
		PREFIX ""
		OUTPUT_NAME "vstudio"
		CXX_COMPILER "msvc"
		CXX_STANDARD 17
		RUNTIME_OUTPUT_DIRECTORY "D:/projects/qpc2/out/win"
		ARCHIVE_OUTPUT_DIRECTORY "D:/projects/qpc2/obj/vstudio/Release"
	)

	target_include_directories(
		VSTUDIO PRIVATE
		"D:/projects/qpc2/src"
		"D:/projects/qpc2/inc"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/include"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/include"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/include"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/ucrt"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/UnitTest/include"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/um"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/shared"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/winrt"
		"C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/cppwinrt"
		"C:/Program Files (x86)/Windows Kits/NETFXSDK/4.8/Include/um"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/mfc"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/mfcm"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/atlmfc/src/atl"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/crt/src"
		"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/VS/src"
		"C:/Program Files (x86)/Windows Kits/10/Source/10.0.18362.0/ucrt"
	)

	target_compile_options(
		VSTUDIO PRIVATE
		/O2
		/MT
	)

endif()

