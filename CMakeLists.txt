  #设置 CMake 版本如下
  cmake_minimum_required(VERSION 3.17.2)
  
  
  set(objname test)
  
  #设置项目名称
  project(${objname})
  
  set(GCM_CACHE_PATH "/temp")
  # 设置编译器选项，将gcm.cache路径传递给g++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -B${GCM_CACHE_PATH}")
  
    #添加到输出目录
   set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  
  #设置c++标准，11
  
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED True)
  
  
   
  add_subdirectory(zlib-1.3.1)
   add_subdirectory(unzip)
  add_subdirectory(SDL-release-2.30.11)
 
 #add_subdirectory(SDL-release-2.0.20)
 
  add_subdirectory(SDL_image-release-2.8.4)
  
  add_subdirectory(SDL_ttf-release-2.24.0)
  add_subdirectory(SDL_mixer-release-2.8.0)
   
   add_subdirectory(orange)
   
    
    
    
     include_directories( 
 ${CMAKE_CURENT_SOURCE_DIR}
   
   "${CMAKE_CURENT_SOURCE_DIR}/zlib-1.3.1"
   "${CMAKE_CURENT_SOURCE_DIR}/unzip"
 
   "${CMAKE_CURENT_SOURCE_DIR}/orange"
  )

 add_executable(${objname} 

 test/GameBody.h 
 test/GameBody.cpp 
 test/HelloWorld.h 
 test/HelloWorld.cpp 
 test/main.cpp 
 )

  target_link_libraries(${objname} 
   
  
  unzip
  zlibstatic 
  
  
 	SDL2
 	SDL2main
 	SDL2_image
 	SDL2_mixer
 	SDL2_ttf
	
	orange2d
  )
  target_include_directories(
    	${objname} 
    	 PRIVATE 
    	 "${CMAKE_CURENT_SOURCE_DIR}/unzip/"
 
    	"${CMAKE_CURENT_SOURCE_DIR}/orange/"
    	)
  
  # cmake ..

  
 include_directories( 
    	"${CMAKE_CURENT_SOURCE_DIR}/"
    	"${CMAKE_CURENT_SOURCE_DIR}/SDL-release-2.30.11/include/"
 
	 "${CMAKE_CURENT_SOURCE_DIR}/SDL_image-release-2.8.4/include/"
	 "${CMAKE_CURENT_SOURCE_DIR}/SDL_mixer-release-2.8.0/include/"
	 "${CMAKE_CURENT_SOURCE_DIR}/SDL_ttf-release-2.24.0/include/"
	 
	  "${CMAKE_CURENT_SOURCE_DIR}/unzip/"
 
    	)
    	
     	target_include_directories(
      	test
   PUBLIC 
     	
	
	"${CMAKE_CURENT_SOURCE_DIR}/SDL-release-2.30.11/include"
 
	 "${CMAKE_CURENT_SOURCE_DIR}/SDL_image-release-2.8.4/include/"
	 "${CMAKE_CURENT_SOURCE_DIR}/SDL_mixer-release-2.8.0/include/"
	 "${CMAKE_CURENT_SOURCE_DIR}/SDL_ttf-release-2.24.0/include/"
	 
	  "${CMAKE_CURENT_SOURCE_DIR}/unzip/"
 "${CMAKE_CURENT_SOURCE_DIR}/"
 
    	 )
   
if (NOT VS)  
  file(COPY "test/Resources" DESTINATION "${CMAKE_BINARY_DIR}/bin/")
else()
   #------------------------------------------
   
   function(og_mark_resources)
    set(oneValueArgs BASEDIR RESOURCEBASE)
    set(multiValueArgs FILES)
    cmake_parse_arguments(opt "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT opt_RESOURCEBASE)
        set(opt_RESOURCEBASE Resources)
    endif()

    get_filename_component(BASEDIR_ABS ${opt_BASEDIR} ABSOLUTE)
    foreach(RES_FILE ${opt_FILES} ${opt_UNPARSED_ARGUMENTS})
        get_filename_component(RES_FILE_ABS ${RES_FILE} ABSOLUTE)
        file(RELATIVE_PATH RES ${BASEDIR_ABS} ${RES_FILE_ABS})
        get_filename_component(RES_LOC ${RES} PATH)
        set_source_files_properties(${RES_FILE} PROPERTIES
                                    MACOSX_PACKAGE_LOCATION "${opt_RESOURCEBASE}/${RES_LOC}"
                                    HEADER_FILE_ONLY 1
                                    )

      #  if(XCODE OR VS)
            string(REPLACE "/" "\\" ide_source_group "${opt_RESOURCEBASE}/${RES_LOC}")
            source_group("${ide_source_group}" FILES ${RES_FILE})
      #  endif()
    endforeach()
endfunction() 

	#------------------------------------------
	


   	#------------------------------------------
	function(og_mark_multi_resources res_out)
    set(oneValueArgs RES_TO)
    set(multiValueArgs FILES FOLDERS)
    cmake_parse_arguments(opt "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(tmp_file_list)
    foreach(cc_file ${opt_FILES})
        get_filename_component(cc_file_abs ${cc_file} ABSOLUTE)
        get_filename_component(file_dir ${cc_file_abs} DIRECTORY)
        og_mark_resources(FILES ${cc_file_abs} BASEDIR ${file_dir} RESOURCEBASE ${opt_RES_TO})
    endforeach()
    list(APPEND tmp_file_list ${opt_FILES})

    foreach(cc_folder ${opt_FOLDERS})
        file(GLOB_RECURSE folder_files "${cc_folder}/*")
        list(APPEND tmp_file_list ${folder_files})
        og_mark_resources(FILES ${folder_files} BASEDIR ${cc_folder} RESOURCEBASE ${opt_RES_TO})
    endforeach()
    set(${res_out} ${tmp_file_list} PARENT_SCOPE)
endfunction()
  
  set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
    )

	og_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
	
	
	endif()