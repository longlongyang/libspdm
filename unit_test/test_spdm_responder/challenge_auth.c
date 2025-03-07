/**
 *  Copyright Notice:
 *  Copyright 2021-2022 DMTF. All rights reserved.
 *  License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/libspdm/blob/main/LICENSE.md
 **/

#include "spdm_unit_test.h"
#include "internal/libspdm_responder_lib.h"

#if LIBSPDM_ENABLE_CAPABILITY_CHAL_CAP

spdm_challenge_request_t m_libspdm_challenge_request1 = {
    { SPDM_MESSAGE_VERSION_11, SPDM_CHALLENGE, 0,
      SPDM_CHALLENGE_REQUEST_NO_MEASUREMENT_SUMMARY_HASH },
};
size_t m_libspdm_challenge_request1_size = sizeof(m_libspdm_challenge_request1);

spdm_challenge_request_t m_libspdm_challenge_request2 = {
    { SPDM_MESSAGE_VERSION_11, SPDM_CHALLENGE, 0,
      SPDM_CHALLENGE_REQUEST_NO_MEASUREMENT_SUMMARY_HASH },
};
size_t m_libspdm_challenge_request2_size = LIBSPDM_MAX_MESSAGE_BUFFER_SIZE;

spdm_challenge_request_t m_libspdm_challenge_request3 = {
    { SPDM_MESSAGE_VERSION_11, SPDM_CHALLENGE, SPDM_MAX_SLOT_COUNT,
      SPDM_CHALLENGE_REQUEST_NO_MEASUREMENT_SUMMARY_HASH },
};
size_t m_libspdm_challenge_request3_size = sizeof(m_libspdm_challenge_request3);

spdm_challenge_request_t m_libspdm_challenge_request4 = {
    { SPDM_MESSAGE_VERSION_11, SPDM_CHALLENGE, 1,
      SPDM_CHALLENGE_REQUEST_NO_MEASUREMENT_SUMMARY_HASH },
};
size_t m_libspdm_challenge_request4_size = sizeof(m_libspdm_challenge_request4);

spdm_challenge_request_t m_libspdm_challenge_request5 = {
    { SPDM_MESSAGE_VERSION_11, SPDM_CHALLENGE, 0,
      SPDM_CHALLENGE_REQUEST_TCB_COMPONENT_MEASUREMENT_HASH },
};
size_t m_libspdm_challenge_request5_size = sizeof(m_libspdm_challenge_request5);

spdm_challenge_request_t m_libspdm_challenge_request6 = {
    { SPDM_MESSAGE_VERSION_11, SPDM_CHALLENGE, 0,
      SPDM_CHALLENGE_REQUEST_ALL_MEASUREMENTS_HASH },
};
size_t m_libspdm_challenge_request6_size = sizeof(m_libspdm_challenge_request6);

uint8_t m_opaque_challenge_auth_rsp[9] = "openspdm";

/**
 * Test 1: receiving a correct CHALLENGE message from the requester with
 * no opaque data, no measurements, and slot number 0.
 * Expected behavior: the responder accepts the request and produces a valid
 * CHALLENGE_AUTH response message.
 **/
void libspdm_test_responder_challenge_auth_case1(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_challenge_auth_response_t *spdm_response;
    void *data1;
    size_t data_size1;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x1;
    spdm_context->connection_info.connection_state =
        LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->local_context.capability.flags = 0;
    spdm_context->local_context.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo =
        m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.base_asym_algo =
        m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.measurement_spec =
        m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;

    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data1,
                                                    &data_size1, NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[0] =
        data_size1;
    spdm_context->local_context.slot_count = 1;
    spdm_context->local_context.opaque_challenge_auth_rsp_size = 0;
    libspdm_reset_message_c(spdm_context);
#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    spdm_context->transcript.message_m.buffer_size =
        spdm_context->transcript.message_m.max_buffer_size;
#endif

    response_size = sizeof(response);
    libspdm_get_random_number(SPDM_NONCE_SIZE,
                              m_libspdm_challenge_request1.nonce);
    status = libspdm_get_response_challenge_auth(
        spdm_context, m_libspdm_challenge_request1_size,
        &m_libspdm_challenge_request1, &response_size, response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(response_size,
                     sizeof(spdm_challenge_auth_response_t) +
                     libspdm_get_hash_size(m_libspdm_use_hash_algo) +
                     SPDM_NONCE_SIZE + 0 + sizeof(uint16_t) + 0 +
                     libspdm_get_asym_signature_size(m_libspdm_use_asym_algo));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_CHALLENGE_AUTH);
    assert_int_equal(spdm_response->header.param1, 0);
    assert_int_equal(spdm_response->header.param2, 1 << 0);
#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    assert_int_equal(spdm_context->transcript.message_m.buffer_size,
                     0);
#endif
    free(data1);
}

/**
 * Test 2: receiving a CHALLENGE message larger than specified.
 * Expected behavior: the responder refuses the CHALLENGE message and produces an
 * ERROR message indicating the InvalidRequest.
 **/
void libspdm_test_responder_challenge_auth_case2(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_challenge_auth_response_t *spdm_response;
    void *data1;
    size_t data_size1;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x2;
    spdm_context->connection_info.connection_state =
        LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->local_context.capability.flags = 0;
    spdm_context->local_context.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo =
        m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.base_asym_algo =
        m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.measurement_spec =
        m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;

    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data1,
                                                    &data_size1, NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[0] =
        data_size1;
    spdm_context->local_context.slot_count = 1;
    spdm_context->local_context.opaque_challenge_auth_rsp_size = 0;
    libspdm_reset_message_c(spdm_context);

    response_size = sizeof(response);
    libspdm_get_random_number(SPDM_NONCE_SIZE,
                              m_libspdm_challenge_request2.nonce);
    status = libspdm_get_response_challenge_auth(
        spdm_context, m_libspdm_challenge_request2_size,
        &m_libspdm_challenge_request2, &response_size, response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_INVALID_REQUEST);
    assert_int_equal(spdm_response->header.param2, 0);
    free(data1);
}

/**
 * Test 3: receiving a correct CHALLENGE from the requester, but the responder is in
 * a Busy state.
 * Expected behavior: the responder accepts the request, but produces an ERROR message
 * indicating the Busy state.
 **/
void libspdm_test_responder_challenge_auth_case3(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_challenge_auth_response_t *spdm_response;
    void *data1;
    size_t data_size1;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x3;
    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_BUSY;
    spdm_context->connection_info.connection_state =
        LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->local_context.capability.flags = 0;
    spdm_context->local_context.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo =
        m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.base_asym_algo =
        m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.measurement_spec =
        m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;

    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data1,
                                                    &data_size1, NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[0] =
        data_size1;
    spdm_context->local_context.slot_count = 1;
    spdm_context->local_context.opaque_challenge_auth_rsp_size = 0;
    libspdm_reset_message_c(spdm_context);

    response_size = sizeof(response);
    libspdm_get_random_number(SPDM_NONCE_SIZE,
                              m_libspdm_challenge_request1.nonce);
    status = libspdm_get_response_challenge_auth(
        spdm_context, m_libspdm_challenge_request1_size,
        &m_libspdm_challenge_request1, &response_size, response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1, SPDM_ERROR_CODE_BUSY);
    assert_int_equal(spdm_response->header.param2, 0);
    assert_int_equal(spdm_context->response_state,
                     LIBSPDM_RESPONSE_STATE_BUSY);
    free(data1);
}

/**
 * Test 4: receiving a correct CHALLENGE from the requester, but the responder requires
 * resynchronization with the requester.
 * Expected behavior: the responder accepts the request, but produces an ERROR message
 * indicating the NeedResynch state.
 **/
void libspdm_test_responder_challenge_auth_case4(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_challenge_auth_response_t *spdm_response;
    void *data1;
    size_t data_size1;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x4;
    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_NEED_RESYNC;
    spdm_context->connection_info.connection_state =
        LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->local_context.capability.flags = 0;
    spdm_context->local_context.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo =
        m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.base_asym_algo =
        m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.measurement_spec =
        m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;

    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data1,
                                                    &data_size1, NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[0] =
        data_size1;
    spdm_context->local_context.slot_count = 1;
    spdm_context->local_context.opaque_challenge_auth_rsp_size = 0;
    libspdm_reset_message_c(spdm_context);

    response_size = sizeof(response);
    libspdm_get_random_number(SPDM_NONCE_SIZE,
                              m_libspdm_challenge_request1.nonce);
    status = libspdm_get_response_challenge_auth(
        spdm_context, m_libspdm_challenge_request1_size,
        &m_libspdm_challenge_request1, &response_size, response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_REQUEST_RESYNCH);
    assert_int_equal(spdm_response->header.param2, 0);
    assert_int_equal(spdm_context->response_state,
                     LIBSPDM_RESPONSE_STATE_NEED_RESYNC);
    free(data1);
}

/**
 * Test 5: receiving a correct CHALLENGE from the requester, but the responder could not
 * produce the response in time.
 * Expected behavior: the responder accepts the request, but produces an ERROR message
 * indicating the ResponseNotReady state.
 **/
void libspdm_test_responder_challenge_auth_case5(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_challenge_auth_response_t *spdm_response;
    void *data1;
    size_t data_size1;
    spdm_error_data_response_not_ready_t *error_data;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x5;
    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_NOT_READY;
    spdm_context->connection_info.connection_state =
        LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->local_context.capability.flags = 0;
    spdm_context->local_context.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo =
        m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.base_asym_algo =
        m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.measurement_spec =
        m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;

    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data1,
                                                    &data_size1, NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[0] =
        data_size1;
    spdm_context->local_context.slot_count = 1;
    spdm_context->local_context.opaque_challenge_auth_rsp_size = 0;
    libspdm_reset_message_c(spdm_context);

    response_size = sizeof(response);
    libspdm_get_random_number(SPDM_NONCE_SIZE,
                              m_libspdm_challenge_request1.nonce);
    status = libspdm_get_response_challenge_auth(
        spdm_context, m_libspdm_challenge_request1_size,
        &m_libspdm_challenge_request1, &response_size, response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(response_size,
                     sizeof(spdm_error_response_t) +
                     sizeof(spdm_error_data_response_not_ready_t));
    spdm_response = (void *)response;
    error_data =
        (spdm_error_data_response_not_ready_t *)(spdm_response + 1);
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_RESPONSE_NOT_READY);
    assert_int_equal(spdm_response->header.param2, 0);
    assert_int_equal(spdm_context->response_state,
                     LIBSPDM_RESPONSE_STATE_NOT_READY);
    assert_int_equal(error_data->request_code, SPDM_CHALLENGE);
    free(data1);
}

/**
 * Test 6: receiving a correct CHALLENGE from the requester, but the responder is not set
 * no receive a CHALLENGE message because previous messages (namely, GET_CAPABILITIES,
 * NEGOTIATE_ALGORITHMS or GET_DIGESTS) have not been received.
 * Expected behavior: the responder rejects the request, and produces an ERROR message
 * indicating the UnexpectedRequest.
 **/
void libspdm_test_responder_challenge_auth_case6(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_challenge_auth_response_t *spdm_response;
    void *data1;
    size_t data_size1;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x6;
    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_NORMAL;
    spdm_context->connection_info.connection_state =
        LIBSPDM_CONNECTION_STATE_NOT_STARTED;
    spdm_context->local_context.capability.flags = 0;
    spdm_context->local_context.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo =
        m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.base_asym_algo =
        m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.measurement_spec =
        m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;

    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data1,
                                                    &data_size1, NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[0] =
        data_size1;
    spdm_context->local_context.slot_count = 1;
    spdm_context->local_context.opaque_challenge_auth_rsp_size = 0;
    libspdm_reset_message_c(spdm_context);

    response_size = sizeof(response);
    libspdm_get_random_number(SPDM_NONCE_SIZE,
                              m_libspdm_challenge_request1.nonce);
    status = libspdm_get_response_challenge_auth(
        spdm_context, m_libspdm_challenge_request1_size,
        &m_libspdm_challenge_request1, &response_size, response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_UNEXPECTED_REQUEST);
    assert_int_equal(spdm_response->header.param2, 0);
    free(data1);
}

/**
 * Test 7: receiving a correct CHALLENGE from the requester, but the responder does not
 * have the challenge capability set.
 * Expected behavior: the responder accepts the request and produces a valid
 * CHALLENGE_AUTH response message.
 **/
void libspdm_test_responder_challenge_auth_case7(void **state) {
    libspdm_return_t status;
    libspdm_test_context_t    *spdm_test_context;
    libspdm_context_t  *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_challenge_auth_response_t *spdm_response;
    void                 *data1;
    size_t data_size1;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x7;
    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->local_context.capability.flags = 0;
    /* spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP;*/
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.base_asym_algo = m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.measurement_spec = m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;

    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain (m_libspdm_use_hash_algo,
                                                     m_libspdm_use_asym_algo,
                                                     &data1, &data_size1,
                                                     NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[0] = data_size1;
    spdm_context->local_context.slot_count = 1;
    spdm_context->local_context.opaque_challenge_auth_rsp_size = 0;
    libspdm_reset_message_c(spdm_context);

    response_size = sizeof(response);
    libspdm_get_random_number (SPDM_NONCE_SIZE, m_libspdm_challenge_request1.nonce);
    status = libspdm_get_response_challenge_auth (spdm_context, m_libspdm_challenge_request1_size,
                                                  &m_libspdm_challenge_request1, &response_size,
                                                  response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal (response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal (spdm_response->header.request_response_code, SPDM_ERROR);
    assert_int_equal (spdm_response->header.param1, SPDM_ERROR_CODE_UNSUPPORTED_REQUEST);
    assert_int_equal (spdm_response->header.param2, SPDM_CHALLENGE);
    free(data1);
}

/**
 * Test 8: receiving an incorrect CHALLENGE from the requester, with the slot number
 * larger than the specification limit.
 * Expected behavior: the responder rejects the request, and produces an ERROR message
 * indicating the UnexpectedRequest.
 **/
void libspdm_test_responder_challenge_auth_case8(void **state) {
    libspdm_return_t status;
    libspdm_test_context_t    *spdm_test_context;
    libspdm_context_t  *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_challenge_auth_response_t *spdm_response;
    void                 *data1;
    size_t data_size1;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x8;
    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->local_context.capability.flags = 0;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.base_asym_algo = m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.measurement_spec = m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;

    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain (m_libspdm_use_hash_algo,
                                                     m_libspdm_use_asym_algo,
                                                     &data1, &data_size1,
                                                     NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[0] = data_size1;
    spdm_context->local_context.slot_count = 1;
    spdm_context->local_context.opaque_challenge_auth_rsp_size = 0;
    libspdm_reset_message_c(spdm_context);

    response_size = sizeof(response);
    libspdm_get_random_number (SPDM_NONCE_SIZE, m_libspdm_challenge_request1.nonce);
    status = libspdm_get_response_challenge_auth (spdm_context, m_libspdm_challenge_request3_size,
                                                  &m_libspdm_challenge_request3, &response_size,
                                                  response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal (response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal (spdm_response->header.request_response_code, SPDM_ERROR);
    assert_int_equal (spdm_response->header.param1, SPDM_ERROR_CODE_INVALID_REQUEST);
    assert_int_equal (spdm_response->header.param2, 0);
    free(data1);
}

/**
 * Test 9: eceiving a correct CHALLENGE message from the requester with
 * no opaque data, no measurements, and slot number 1.
 * Expected behavior: the responder accepts the request and produces a valid
 * CHALLENGE_AUTH response message.
 **/
void libspdm_test_responder_challenge_auth_case9(void **state) {
    libspdm_return_t status;
    libspdm_test_context_t    *spdm_test_context;
    libspdm_context_t  *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_challenge_auth_response_t *spdm_response;
    void                 *data1;
    size_t data_size1;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x9;
    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->local_context.capability.flags = 0;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.base_asym_algo = m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.measurement_spec = m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;

    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain (m_libspdm_use_hash_algo,
                                                     m_libspdm_use_asym_algo,
                                                     &data1, &data_size1,
                                                     NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[1] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[1] = data_size1;
    spdm_context->local_context.slot_count = 2;
    spdm_context->local_context.opaque_challenge_auth_rsp_size = 0;
    libspdm_reset_message_c(spdm_context);

    response_size = sizeof(response);
    libspdm_get_random_number (SPDM_NONCE_SIZE, m_libspdm_challenge_request1.nonce);
    status = libspdm_get_response_challenge_auth (spdm_context, m_libspdm_challenge_request4_size,
                                                  &m_libspdm_challenge_request4, &response_size,
                                                  response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal (response_size, sizeof(spdm_challenge_auth_response_t) + libspdm_get_hash_size (
                          m_libspdm_use_hash_algo) + SPDM_NONCE_SIZE + 0 + sizeof(uint16_t) + 0 + libspdm_get_asym_signature_size (
                          m_libspdm_use_asym_algo));
    spdm_response = (void *)response;
    assert_int_equal (spdm_response->header.request_response_code, SPDM_CHALLENGE_AUTH);
    assert_int_equal (spdm_response->header.param1, 1);
    assert_int_equal (spdm_response->header.param2, 1 << 1);
    free(data1);
}

/**
 * Test 10: receiving a correct CHALLENGE from the requester, but with certificate
 * unavailable at the requested slot number (1).
 * Expected behavior: the responder rejects the request, and produces an ERROR message
 * indicating the UnexpectedRequest.
 **/
void libspdm_test_responder_challenge_auth_case10(void **state) {
    libspdm_return_t status;
    libspdm_test_context_t    *spdm_test_context;
    libspdm_context_t  *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_challenge_auth_response_t *spdm_response;
    void                 *data1;
    size_t data_size1;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0xA;
    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->local_context.capability.flags = 0;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.base_asym_algo = m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.measurement_spec = m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;

    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain (m_libspdm_use_hash_algo,
                                                     m_libspdm_use_asym_algo,
                                                     &data1, &data_size1,
                                                     NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[0] = data_size1;
    spdm_context->local_context.slot_count = 1;
    spdm_context->local_context.opaque_challenge_auth_rsp_size = 0;
    libspdm_reset_message_c(spdm_context);

    response_size = sizeof(response);
    libspdm_get_random_number (SPDM_NONCE_SIZE, m_libspdm_challenge_request1.nonce);
    status = libspdm_get_response_challenge_auth (spdm_context, m_libspdm_challenge_request3_size,
                                                  &m_libspdm_challenge_request3, &response_size,
                                                  response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal (response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal (spdm_response->header.request_response_code, SPDM_ERROR);
    assert_int_equal (spdm_response->header.param1, SPDM_ERROR_CODE_INVALID_REQUEST);
    assert_int_equal (spdm_response->header.param2, 0);
    free(data1);
}

/**
 * Test 11: receiving a correct CHALLENGE message from the requester with opaque
 * data as the bytes of the string "openspdm", no measurements, and slot number 0.
 * Expected behavior: the responder accepts the request and produces a valid
 * CHALLENGE_AUTH response message.
 **/
void libspdm_test_responder_challenge_auth_case11(void **state) {
    libspdm_return_t status;
    libspdm_test_context_t    *spdm_test_context;
    libspdm_context_t  *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_challenge_auth_response_t *spdm_response;
    void                 *data1;
    size_t data_size1;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0xB;
    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->local_context.capability.flags = 0;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.base_asym_algo = m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.measurement_spec = m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;

    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain (m_libspdm_use_hash_algo,
                                                     m_libspdm_use_asym_algo,
                                                     &data1, &data_size1,
                                                     NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[0] = data_size1;
    spdm_context->local_context.slot_count = 1;
    spdm_context->local_context.opaque_challenge_auth_rsp_size = 8;
    spdm_context->local_context.opaque_challenge_auth_rsp = m_opaque_challenge_auth_rsp;
    libspdm_reset_message_c(spdm_context);

    response_size = sizeof(response);
    libspdm_get_random_number (SPDM_NONCE_SIZE, m_libspdm_challenge_request1.nonce);
    status = libspdm_get_response_challenge_auth (spdm_context, m_libspdm_challenge_request1_size,
                                                  &m_libspdm_challenge_request1, &response_size,
                                                  response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal (response_size, sizeof(spdm_challenge_auth_response_t) + libspdm_get_hash_size (
                          m_libspdm_use_hash_algo) + SPDM_NONCE_SIZE + 0 + sizeof(uint16_t) + 8 + libspdm_get_asym_signature_size (
                          m_libspdm_use_asym_algo));
    spdm_response = (void *)response;
    assert_int_equal (spdm_response->header.request_response_code, SPDM_CHALLENGE_AUTH);
    assert_int_equal (spdm_response->header.param1, 0);
    assert_int_equal (spdm_response->header.param2, 1 << 0);
    free(data1);
}

/**
 * Test 12: receiving a correct CHALLENGE message from the requester with
 * no opaque data, TCB measurement hash, and slot number 0.
 * Expected behavior: the responder accepts the request and produces a valid
 * CHALLENGE_AUTH response message.
 **/
void libspdm_test_responder_challenge_auth_case12(void **state) {
    libspdm_return_t status;
    libspdm_test_context_t    *spdm_test_context;
    libspdm_context_t  *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_challenge_auth_response_t *spdm_response;
    void                 *data1;
    size_t data_size1;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0xC;
    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->local_context.capability.flags = 0;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_CAP; /*additional measurement capability*/
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.base_asym_algo = m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.measurement_spec = m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;

    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain (m_libspdm_use_hash_algo,
                                                     m_libspdm_use_asym_algo,
                                                     &data1, &data_size1,
                                                     NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[0] = data_size1;
    spdm_context->local_context.slot_count = 1;
    spdm_context->local_context.opaque_challenge_auth_rsp_size = 0;
    libspdm_reset_message_c(spdm_context);

    response_size = sizeof(response);
    libspdm_get_random_number (SPDM_NONCE_SIZE, m_libspdm_challenge_request1.nonce);
    status = libspdm_get_response_challenge_auth (spdm_context, m_libspdm_challenge_request5_size,
                                                  &m_libspdm_challenge_request5, &response_size,
                                                  response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal (response_size, sizeof(spdm_challenge_auth_response_t) + libspdm_get_hash_size (
                          m_libspdm_use_hash_algo) + SPDM_NONCE_SIZE +
                      libspdm_get_hash_size (m_libspdm_use_hash_algo) +
                      sizeof(uint16_t) + 0 +
                      libspdm_get_asym_signature_size (m_libspdm_use_asym_algo));
    spdm_response = (void *)response;
    assert_int_equal (spdm_response->header.request_response_code, SPDM_CHALLENGE_AUTH);
    assert_int_equal (spdm_response->header.param1, 0);
    assert_int_equal (spdm_response->header.param2, 1 << 0);
    free(data1);
}

/**
 * Test 13: receiving a correct CHALLENGE message from the requester with
 * no opaque data, all measurement hashes, and slot number 0.
 * Expected behavior: the responder accepts the request and produces a valid
 * CHALLENGE_AUTH response message.
 **/
void libspdm_test_responder_challenge_auth_case13(void **state) {
    libspdm_return_t status;
    libspdm_test_context_t    *spdm_test_context;
    libspdm_context_t  *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_challenge_auth_response_t *spdm_response;
    void                 *data1;
    size_t data_size1;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0xD;
    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->local_context.capability.flags = 0;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_CAP; /*additional measurement capability*/
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.base_asym_algo = m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.measurement_spec = m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;

    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain (m_libspdm_use_hash_algo,
                                                     m_libspdm_use_asym_algo,
                                                     &data1, &data_size1,
                                                     NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[0] = data_size1;
    spdm_context->local_context.slot_count = 1;
    spdm_context->local_context.opaque_challenge_auth_rsp_size = 0;
    libspdm_reset_message_c(spdm_context);

    response_size = sizeof(response);
    libspdm_get_random_number (SPDM_NONCE_SIZE, m_libspdm_challenge_request1.nonce);
    status = libspdm_get_response_challenge_auth (spdm_context, m_libspdm_challenge_request6_size,
                                                  &m_libspdm_challenge_request6, &response_size,
                                                  response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal (response_size, sizeof(spdm_challenge_auth_response_t) + libspdm_get_hash_size (
                          m_libspdm_use_hash_algo) + SPDM_NONCE_SIZE +
                      libspdm_get_hash_size (m_libspdm_use_hash_algo) +
                      sizeof(uint16_t) + 0 +
                      libspdm_get_asym_signature_size (m_libspdm_use_asym_algo));
    spdm_response = (void *)response;
    assert_int_equal (spdm_response->header.request_response_code, SPDM_CHALLENGE_AUTH);
    assert_int_equal (spdm_response->header.param1, 0);
    assert_int_equal (spdm_response->header.param2, 1 << 0);
    free(data1);
}

/**
 * Test 14: the responder does not have measurements capabilities, but
 * receives a correct CHALLENGE message from the requester with
 * no opaque data, all measurement hashes, and slot number 0.
 * Expected behavior: the responder refuses the CHALLENGE message and produces an
 * ERROR message indicating the UnsupportedRequest.
 **/
void libspdm_test_responder_challenge_auth_case14(void **state) {
    libspdm_return_t status;
    libspdm_test_context_t    *spdm_test_context;
    libspdm_context_t  *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_challenge_auth_response_t *spdm_response;
    void                 *data1;
    size_t data_size1;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0xE;
    spdm_context->connection_info.connection_state = LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->local_context.capability.flags = 0;
    spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP;
    /* spdm_context->local_context.capability.flags |= SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_MEAS_CAP;   no measurement capability*/
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.base_asym_algo = m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.measurement_spec = m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;

    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain (m_libspdm_use_hash_algo,
                                                     m_libspdm_use_asym_algo,
                                                     &data1, &data_size1,
                                                     NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[0] = data_size1;
    spdm_context->local_context.slot_count = 1;
    spdm_context->local_context.opaque_challenge_auth_rsp_size = 0;
    libspdm_reset_message_c(spdm_context);

    response_size = sizeof(response);
    libspdm_get_random_number (SPDM_NONCE_SIZE, m_libspdm_challenge_request1.nonce);
    status = libspdm_get_response_challenge_auth (spdm_context, m_libspdm_challenge_request6_size,
                                                  &m_libspdm_challenge_request6, &response_size,
                                                  response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal (response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal (spdm_response->header.request_response_code, SPDM_ERROR);
    assert_int_equal (spdm_response->header.param1, SPDM_ERROR_CODE_INVALID_REQUEST);
    assert_int_equal (spdm_response->header.param2, SPDM_CHALLENGE);
    free(data1);
}

/**
 * Test 15: receiving a correct CHALLENGE from the requester. Buffers A, B and
 * C already have arbitrary data.
 * Expected behavior: the responder accepts the request and produces a valid
 * CHALLENGE_AUTH response message, and buffer C receives the exchanged CHALLENGE
 * and CHALLENGE_AUTH (without signature) messages.
 **/
void libspdm_test_responder_challenge_auth_case15(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_challenge_auth_response_t *spdm_response;
    void *data1;
    size_t data_size1;
#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    size_t signature_size;
#endif

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0xF;
    spdm_context->connection_info.connection_state =
        LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->local_context.capability.flags = 0;
    spdm_context->local_context.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_CHAL_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.base_asym_algo = m_libspdm_use_asym_algo;
    spdm_context->connection_info.algorithm.measurement_spec = m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;

    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data1,
                                                    &data_size1, NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[0] = data_size1;
    spdm_context->local_context.slot_count = 1;
    spdm_context->local_context.opaque_challenge_auth_rsp_size = 0;
    libspdm_reset_message_c(spdm_context);

#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    /*filling buffers with arbitrary data*/
    libspdm_set_mem(spdm_context->transcript.message_a.buffer, 10, (uint8_t) 0xFF);
    spdm_context->transcript.message_a.buffer_size = 10;
    libspdm_set_mem(spdm_context->transcript.message_b.buffer, 8, (uint8_t) 0xEE);
    spdm_context->transcript.message_b.buffer_size = 8;
    libspdm_set_mem(spdm_context->transcript.message_c.buffer, 12, (uint8_t) 0xDD);
    spdm_context->transcript.message_c.buffer_size = 12;

    signature_size = libspdm_get_asym_signature_size(
        spdm_context->connection_info.algorithm.base_asym_algo);
#endif

    response_size = sizeof(response);
    libspdm_get_random_number(SPDM_NONCE_SIZE, m_libspdm_challenge_request1.nonce);
    status = libspdm_get_response_challenge_auth(
        spdm_context, m_libspdm_challenge_request1_size,
        &m_libspdm_challenge_request1, &response_size, response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(response_size,
                     sizeof(spdm_challenge_auth_response_t) +
                     libspdm_get_hash_size(m_libspdm_use_hash_algo) +
                     SPDM_NONCE_SIZE + 0 + sizeof(uint16_t) + 0 +
                     libspdm_get_asym_signature_size(m_libspdm_use_asym_algo));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code, SPDM_CHALLENGE_AUTH);
    assert_int_equal(spdm_response->header.param1, 0);
    assert_int_equal(spdm_response->header.param2, 1 << 0);

#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    assert_int_equal(spdm_context->transcript.message_a.buffer_size, 10);
    assert_int_equal(spdm_context->transcript.message_b.buffer_size, 8);
    assert_int_equal(spdm_context->transcript.message_c.buffer_size, 12 +
                     m_libspdm_challenge_request1_size + response_size - signature_size);

    assert_memory_equal(spdm_context->transcript.message_c.buffer + 12,
                        &m_libspdm_challenge_request1, m_libspdm_challenge_request1_size);
    assert_memory_equal(spdm_context->transcript.message_c.buffer + 12 +
                        m_libspdm_challenge_request1_size,
                        response, response_size - signature_size);
#endif
    free(data1);
}

libspdm_test_context_t m_libspdm_responder_challenge_auth_test_context = {
    LIBSPDM_TEST_CONTEXT_VERSION,
    false,
};

int libspdm_responder_challenge_auth_test_main(void)
{
    const struct CMUnitTest spdm_responder_challenge_auth_tests[] = {
        /* Success Case*/
        cmocka_unit_test(libspdm_test_responder_challenge_auth_case1),
        /* Bad request size*/
        cmocka_unit_test(libspdm_test_responder_challenge_auth_case2),
        /* response_state: LIBSPDM_RESPONSE_STATE_BUSY*/
        cmocka_unit_test(libspdm_test_responder_challenge_auth_case3),
        /* response_state: LIBSPDM_RESPONSE_STATE_NEED_RESYNC*/
        cmocka_unit_test(libspdm_test_responder_challenge_auth_case4),
        /* response_state: LIBSPDM_RESPONSE_STATE_NOT_READY*/
        cmocka_unit_test(libspdm_test_responder_challenge_auth_case5),
        /* connection_state Check*/
        cmocka_unit_test(libspdm_test_responder_challenge_auth_case6),
        cmocka_unit_test(libspdm_test_responder_challenge_auth_case7),
        cmocka_unit_test(libspdm_test_responder_challenge_auth_case8),
        cmocka_unit_test(libspdm_test_responder_challenge_auth_case9),
        cmocka_unit_test(libspdm_test_responder_challenge_auth_case10),
        cmocka_unit_test(libspdm_test_responder_challenge_auth_case11),
        cmocka_unit_test(libspdm_test_responder_challenge_auth_case12),
        cmocka_unit_test(libspdm_test_responder_challenge_auth_case13),
        cmocka_unit_test(libspdm_test_responder_challenge_auth_case14),
        /* Buffer verification*/
        cmocka_unit_test(libspdm_test_responder_challenge_auth_case15),
    };

    libspdm_setup_test_context(&m_libspdm_responder_challenge_auth_test_context);

    return cmocka_run_group_tests(spdm_responder_challenge_auth_tests,
                                  libspdm_unit_test_group_setup,
                                  libspdm_unit_test_group_teardown);
}

#endif /* LIBSPDM_ENABLE_CAPABILITY_CHAL_CAP*/
