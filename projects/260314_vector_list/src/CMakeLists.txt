target_sources(${CONTAINERS_LIB} 
	PRIVATE
		doubly_linked_list.cpp
		vector.cpp
)

target_include_directories(${CONTAINERS_LIB} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)