
# Uses EXTS, APPLE and MSYS + options and MAIN_LIBRARY

function(add_tool2 name extension)
  add_executable(${name} ${SRCS_ALL} "${name}.${extension}")
  set_target_properties(${name} PROPERTIES 
    OUTPUT_NAME                 "fa_${name}"
    RUNTIME_OUTPUT_DIRECTORY    "../bin"
    )
  if(APPLE)
    target_link_libraries(${name} ${MAIN_LIBRARY})
  else()
    target_link_libraries(${name} ${MAIN_STATIC_LIBRARY})
  endif()
endfunction()

function(add_tool name)
  add_tool2(${name} c)
endfunction()

add_tool(list_audio)
add_tool(list_midi)
add_tool(echo_midi)
add_tool(plug_midi)
add_tool(default_midi)
add_tool(template)
add_tool(print_file_format)
add_tool(plot_file)
add_tool(sine)
add_tool(lift)
add_tool(play)
add_tool(nrt)
add_tool(left)
add_tool(version)
add_tool(stereo)
add_tool(fifo)   
add_tool(custom)
add_tool(test_fs)
add_tool(test_dls)
add_tool(test_midi)
add_tool(test_midi_in)
add_tool(test_core_midi)
