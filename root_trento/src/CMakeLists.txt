ROOT_GENERATE_DICTIONARY(root_tree_dic event_CLASS.h LINKDEF LinkDef.h)
ROOT_GENERATE_DICTIONARY(events_info_dic events_info_CLASS.h LINKDEF info_LinkDef.h)

add_library(src SHARED 
    event_CLASS.cpp 
    root_tree_dic.cxx
    events_info_CLASS.cpp
    events_info_dic.cxx
    trento_options.cpp
    )

target_link_libraries(src ${ROOT_LIBRARIES})

# include("${ROOT_DIR}/RootMacros.cmake")

# ROOT_GENERATE_DICTIONARY(G__read_root_dict event_CLASS.h MODULE src LINKDEF LinkDef.h)

# # ROOT_GENERATE_DICTIONARY(read_root_dict event_CLASS.h LINKDEF LinkDef.h)

# target_include_directories(src
#           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
#           )

# configure_file(event_CLASS.h event_CLASS.h COPYONLY)

