cmake_minimum_required(VERSION 2.8.12)

INCLUDE_DIRECTORIES(${LIBSPDM_DIR}/unit_test/fuzzing/test_responder/test_spdm_responder_algorithms
                    ${LIBSPDM_DIR}/include
                    ${LIBSPDM_DIR}/include/hal/${ARCH}
                    ${LIBSPDM_DIR}/unit_test/include
                    ${LIBSPDM_DIR}/unit_test/fuzzing/spdm_unit_fuzzing_common
                    ${LIBSPDM_DIR}/os_stub/include
)

if(TOOLCHAIN STREQUAL "KLEE")
    INCLUDE_DIRECTORIES($ENV{KLEE_SRC_PATH}/include)
endif()

SET(src_test_spdm_responder_algorithms
    algorithms.c
    ${PROJECT_SOURCE_DIR}/unit_test/fuzzing/spdm_unit_fuzzing_common/common.c
    ${PROJECT_SOURCE_DIR}/unit_test/fuzzing/spdm_unit_fuzzing_common/toolchain_harness.c
    ${PROJECT_SOURCE_DIR}/unit_test/fuzzing/spdm_unit_fuzzing_common/algo.c
)

SET(test_spdm_responder_algorithms_LIBRARY
    memlib
    debuglib
    spdm_responder_lib
    spdm_common_lib
    ${CRYPTO_LIB_PATHS}
    rnglib
    cryptlib_${CRYPTO}
    malloclib
    spdm_crypt_lib
    spdm_secured_message_lib
    spdm_transport_test_lib
    spdm_device_secret_lib_null
    platform_lib
)

if((TOOLCHAIN STREQUAL "KLEE") OR (TOOLCHAIN STREQUAL "CBMC"))
    ADD_EXECUTABLE(test_spdm_responder_algorithms
                   ${src_test_spdm_responder_algorithms}
                   $<TARGET_OBJECTS:memlib>
                   $<TARGET_OBJECTS:debuglib>
                   $<TARGET_OBJECTS:spdm_responder_lib>
                   $<TARGET_OBJECTS:spdm_common_lib>
                   $<TARGET_OBJECTS:${CRYPTO_LIB_PATHS}>
                   $<TARGET_OBJECTS:rnglib>
                   $<TARGET_OBJECTS:cryptlib_${CRYPTO}>
                   $<TARGET_OBJECTS:malloclib>
                   $<TARGET_OBJECTS:spdm_crypt_lib>
                   $<TARGET_OBJECTS:spdm_secured_message_lib>
                   $<TARGET_OBJECTS:spdm_transport_test_lib>
                   $<TARGET_OBJECTS:spdm_device_secret_lib_null>
                   $<TARGET_OBJECTS:platform_lib>
    )
else()
    ADD_EXECUTABLE(test_spdm_responder_algorithms ${src_test_spdm_responder_algorithms})
    TARGET_LINK_LIBRARIES(test_spdm_responder_algorithms ${test_spdm_responder_algorithms_LIBRARY})
endif()
