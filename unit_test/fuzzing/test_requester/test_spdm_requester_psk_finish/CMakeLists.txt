cmake_minimum_required(VERSION 2.8.12)
INCLUDE_DIRECTORIES(${LIBSPDM_DIR}/unit_test/fuzzing/test_requester/test_spdm_requester_psk_finish
                    ${LIBSPDM_DIR}/include
                    ${LIBSPDM_DIR}/include/hal/${ARCH}
                    ${LIBSPDM_DIR}/unit_test/include
                    ${LIBSPDM_DIR}/os_stub/spdm_device_secret_lib_sample
                    ${LIBSPDM_DIR}/unit_test/cmockalib/cmocka/include
                    ${LIBSPDM_DIR}/unit_test/cmockalib/cmocka/include/cmockery
                    ${LIBSPDM_DIR}/unit_test/fuzzing/spdm_unit_fuzzing_common
                    ${LIBSPDM_DIR}/os_stub/include
)

if(TOOLCHAIN STREQUAL "KLEE")
    INCLUDE_DIRECTORIES($ENV{KLEE_SRC_PATH}/include)
endif()

SET(src_test_spdm_requester_psk_finish
    psk_finish.c
    ${PROJECT_SOURCE_DIR}/unit_test/fuzzing/spdm_unit_fuzzing_common/common.c
    ${PROJECT_SOURCE_DIR}/unit_test/fuzzing/spdm_unit_fuzzing_common/toolchain_harness.c
    ${PROJECT_SOURCE_DIR}/unit_test/fuzzing/spdm_unit_fuzzing_common/algo.c
)

SET(test_spdm_requester_psk_finish_LIBRARY
    memlib
    debuglib
    spdm_requester_lib
    spdm_common_lib
    ${CRYPTO_LIB_PATHS}
    rnglib
    platform_lib
    cryptlib_${CRYPTO}
    malloclib
    spdm_crypt_lib
    spdm_secured_message_lib
    spdm_transport_test_lib
    
    spdm_device_secret_lib_sample
    cmockalib
)

if((TOOLCHAIN STREQUAL "KLEE") OR (TOOLCHAIN STREQUAL "CBMC"))
    ADD_EXECUTABLE(test_spdm_requester_psk_finish
                   ${src_test_spdm_requester_psk_finish}
                   $<TARGET_OBJECTS:memlib>
                   $<TARGET_OBJECTS:debuglib>
                   $<TARGET_OBJECTS:spdm_requester_lib>
                   $<TARGET_OBJECTS:spdm_common_lib>
                   $<TARGET_OBJECTS:${CRYPTO_LIB_PATHS}>
                   $<TARGET_OBJECTS:rnglib>
                   $<TARGET_OBJECTS:platform_lib>
                   $<TARGET_OBJECTS:cryptlib_${CRYPTO}>
                   $<TARGET_OBJECTS:malloclib>
                   $<TARGET_OBJECTS:spdm_crypt_lib>
                   $<TARGET_OBJECTS:spdm_secured_message_lib>
                   $<TARGET_OBJECTS:spdm_transport_test_lib>
                   
                   $<TARGET_OBJECTS:spdm_device_secret_lib_sample>
                   $<TARGET_OBJECTS:cmockalib>
    )
else()
    ADD_EXECUTABLE(test_spdm_requester_psk_finish ${src_test_spdm_requester_psk_finish})
    TARGET_LINK_LIBRARIES(test_spdm_requester_psk_finish ${test_spdm_requester_psk_finish_LIBRARY})
endif()
