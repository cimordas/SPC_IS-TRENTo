ROOT_GENERATE_DICTIONARY(root_tree_dic event_CLASS.h LINKDEF LinkDef.h)
ROOT_GENERATE_DICTIONARY(functions_dic functions.h LINKDEF func_LinkDef.h)
ROOT_GENERATE_DICTIONARY(events_info_dic events_info_CLASS.h LINKDEF info_LinkDef.h)

add_library(src SHARED 
    event_CLASS.cpp 
    events_info_CLASS.h
    root_tree_dic.cxx
    events_info_CLASS.cpp
    events_info_dic.cxx
    functions.cpp
    functions_dic.cxx
    )

# =========== Include openMP ==========
# set_target_properties(src PROPERTIES COMPILE_FLAGS "-fopenmp") # IMPORTANT: This line is added to compile the openMP for the libraries as well.
# ====================================

target_link_libraries(src ${ROOT_LIBRARIES})


