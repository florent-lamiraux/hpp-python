set(BOOST_PYTHON_SOURCES
    object/class.cpp
    object/stl_iterator.cpp
    object/function.cpp
    object/inheritance.cpp
    object/iterator.cpp
    object/pickle_support.cpp
    object/life_support.cpp
    object/enum.cpp
    object/function_doc_signature.cpp
    exec.cpp
    list.cpp
    dict.cpp
    slice.cpp
    object_operators.cpp
    converter/from_python.cpp
    converter/builtin_converters.cpp
    converter/registry.cpp
    converter/arg_to_python_base.cpp
    converter/type_id.cpp
    wrapper.cpp
    import.cpp
    str.cpp
    tuple.cpp
    long.cpp
    module.cpp
    errors.cpp
    object_protocol.cpp
    numpy/scalars.cpp
    numpy/numpy.cpp
    numpy/matrix.cpp
    numpy/ufunc.cpp
    numpy/ndarray.cpp
    numpy/dtype.cpp
  )

set(TARGET_NAME boost-python)
include_directories(${PYTHON_INCLUDE_DIR} ${NUMPY_INCLUDE_DIRS})
add_library(${TARGET_NAME} SHARED ${BOOST_PYTHON_SOURCES})
set_target_properties(
    ${TARGET_NAME}
    PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${TARGET_NAME} PUBLIC ${PYTHON_LIBRARIES})

install(TARGETS ${TARGET_NAME} DESTINATION lib)
