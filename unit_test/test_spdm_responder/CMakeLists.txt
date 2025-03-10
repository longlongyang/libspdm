cmake_minimum_required(VERSION 2.8.12)

INCLUDE_DIRECTORIES(${LIBSPDM_DIR}/unit_test/test_spdm_responder
                    ${LIBSPDM_DIR}/include
                    ${LIBSPDM_DIR}/include/hal/${ARCH}
                    ${LIBSPDM_DIR}/unit_test/include
                    ${LIBSPDM_DIR}/os_stub/spdm_device_secret_lib_sample
                    ${LIBSPDM_DIR}/unit_test/cmockalib/cmocka/include
                    ${LIBSPDM_DIR}/unit_test/cmockalib/cmocka/include/cmockery
                    ${LIBSPDM_DIR}/unit_test/spdm_unit_test_common
)

SET(src_test_spdm_responder
    test_spdm_responder.c
    version.c
    capabilities.c
    algorithms.c
    digests.c
    certificate.c
    challenge_auth.c
    measurements.c
    respond_if_ready.c
    key_exchange.c
    finish.c
    psk_exchange.c
    psk_finish.c
    heartbeat.c
    key_update.c
    end_session.c
    encap_get_certificate.c
    encap_get_digests.c
    encap_key_update.c
    encap_challenge.c
    encap_response.c
    ${LIBSPDM_DIR}/unit_test/spdm_unit_test_common/common.c
    ${LIBSPDM_DIR}/unit_test/spdm_unit_test_common/algo.c
    ${LIBSPDM_DIR}/unit_test/spdm_unit_test_common/support.c
)

SET(test_spdm_responder_LIBRARY
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
    spdm_device_secret_lib_sample
    spdm_transport_test_lib
    cmockalib
    platform_lib
)

if((TOOLCHAIN STREQUAL "KLEE") OR (TOOLCHAIN STREQUAL "CBMC"))
    ADD_EXECUTABLE(test_spdm_responder
                   ${src_test_spdm_responder}
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
                   $<TARGET_OBJECTS:spdm_device_secret_lib_sample>
                   $<TARGET_OBJECTS:spdm_transport_test_lib>
                   $<TARGET_OBJECTS:cmockalib>
                   $<TARGET_OBJECTS:platform_lib>
    )
else()
    ADD_EXECUTABLE(test_spdm_responder ${src_test_spdm_responder})
    TARGET_LINK_LIBRARIES(test_spdm_responder ${test_spdm_responder_LIBRARY})
endif()


