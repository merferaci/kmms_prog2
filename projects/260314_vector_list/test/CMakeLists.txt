target_sources(${CONTAINERS_LIB} 
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/templates_doubly_linked_list.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/templates_vector.cpp
)

# doubly linked list test
add_executable(doubly_linked_list_tests 
	test_doubly_linked_list.cpp
)

target_link_libraries(doubly_linked_list_tests 
	gtest 
	${CONTAINERS_LIB}
)

target_include_directories(doubly_linked_list_tests PRIVATE 
	${CMAKE_SOURCE_DIR}/src
)

add_test(doubly_linked_list_tests doubly_linked_list_tests)

# vector test
add_executable(vector_tests 
	test_vector.cpp
)

target_link_libraries(vector_tests 
	gtest 
	${CONTAINERS_LIB}
)

target_include_directories(vector_tests PRIVATE 
	${CMAKE_SOURCE_DIR}/src
)

add_test(vector_tests vector_tests)