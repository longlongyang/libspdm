/**
 *  Copyright Notice:
 *  Copyright 2021-2022 DMTF. All rights reserved.
 *  License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/libspdm/blob/main/LICENSE.md
 **/

#include "spdm_unit_test.h"
#include "internal/libspdm_responder_lib.h"

#if LIBSPDM_ENABLE_CAPABILITY_PSK_EX_CAP

#pragma pack(1)

typedef struct {
    spdm_message_header_t header;
    uint16_t req_session_id;
    uint16_t psk_hint_length;
    uint16_t context_length;
    uint16_t opaque_length;
    uint8_t psk_hint[LIBSPDM_PSK_MAX_HINT_LENGTH];
    uint8_t context[LIBSPDM_PSK_CONTEXT_LENGTH];
    uint8_t opaque_data[SPDM_MAX_OPAQUE_DATA_SIZE];
} libspdm_psk_exchange_request_mine_t;

#pragma pack()

static uint8_t m_libspdm_local_psk_hint[32];

libspdm_psk_exchange_request_mine_t m_libspdm_psk_exchange_request1 = {
    { SPDM_MESSAGE_VERSION_11, SPDM_PSK_EXCHANGE,
      SPDM_CHALLENGE_REQUEST_NO_MEASUREMENT_SUMMARY_HASH, 0 },
};
size_t m_libspdm_psk_exchange_request1_size = sizeof(m_libspdm_psk_exchange_request1);

libspdm_psk_exchange_request_mine_t m_libspdm_psk_exchange_request2 = {
    { SPDM_MESSAGE_VERSION_11, SPDM_PSK_EXCHANGE,
      SPDM_CHALLENGE_REQUEST_NO_MEASUREMENT_SUMMARY_HASH, 0 },
};
size_t m_libspdm_psk_exchange_request2_size = sizeof(spdm_psk_exchange_request_t);

libspdm_psk_exchange_request_mine_t m_libspdm_psk_exchange_request3 = {
    { SPDM_MESSAGE_VERSION_12, SPDM_PSK_EXCHANGE,
      SPDM_CHALLENGE_REQUEST_NO_MEASUREMENT_SUMMARY_HASH, 0 },
};
size_t m_libspdm_psk_exchange_request3_size = sizeof(m_libspdm_psk_exchange_request3);

void libspdm_test_responder_psk_exchange_case1(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_psk_exchange_response_t *spdm_response;
    void *data1;
    size_t data_size1;
    uint8_t *ptr;
    size_t opaque_psk_exchange_req_size;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x1;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    spdm_context->connection_info.connection_state =
        LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.capability.flags |=
        SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PSK_CAP;
    spdm_context->local_context.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo =
        m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.measurement_spec =
        m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;
    spdm_context->connection_info.algorithm.dhe_named_group =
        m_libspdm_use_dhe_algo;
    spdm_context->connection_info.algorithm.aead_cipher_suite =
        m_libspdm_use_aead_algo;
    spdm_context->connection_info.algorithm.key_schedule =
        m_libspdm_use_key_schedule_algo;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data1,
                                                    &data_size1, NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[0] =
        data_size1;
    spdm_context->connection_info.local_used_cert_chain_buffer = data1;
    spdm_context->connection_info.local_used_cert_chain_buffer_size =
        data_size1;
    spdm_context->local_context.slot_count = 1;
    libspdm_reset_message_a(spdm_context);
    libspdm_zero_mem(m_libspdm_local_psk_hint, 32);
    libspdm_copy_mem(&m_libspdm_local_psk_hint[0], sizeof(m_libspdm_local_psk_hint),
                     LIBSPDM_TEST_PSK_HINT_STRING, sizeof(LIBSPDM_TEST_PSK_HINT_STRING));
    spdm_context->local_context.psk_hint_size =
        sizeof(LIBSPDM_TEST_PSK_HINT_STRING);
    spdm_context->local_context.psk_hint = m_libspdm_local_psk_hint;

    m_libspdm_psk_exchange_request1.psk_hint_length =
        (uint16_t)spdm_context->local_context.psk_hint_size;
    m_libspdm_psk_exchange_request1.context_length = LIBSPDM_PSK_CONTEXT_LENGTH;
    opaque_psk_exchange_req_size =
        libspdm_get_opaque_data_supported_version_data_size(spdm_context);
    m_libspdm_psk_exchange_request1.opaque_length =
        (uint16_t)opaque_psk_exchange_req_size;
    m_libspdm_psk_exchange_request1.req_session_id = 0xFFFF;
    ptr = m_libspdm_psk_exchange_request1.psk_hint;
    libspdm_copy_mem(ptr, sizeof(m_libspdm_psk_exchange_request1.psk_hint),
                     spdm_context->local_context.psk_hint,
                     spdm_context->local_context.psk_hint_size);
    ptr += m_libspdm_psk_exchange_request1.psk_hint_length;
    libspdm_get_random_number(LIBSPDM_PSK_CONTEXT_LENGTH, ptr);
    ptr += m_libspdm_psk_exchange_request1.context_length;
    libspdm_build_opaque_data_supported_version_data(
        spdm_context, &opaque_psk_exchange_req_size, ptr);
#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    spdm_context->transcript.message_m.buffer_size =
        spdm_context->transcript.message_m.max_buffer_size;
#endif
    ptr += opaque_psk_exchange_req_size;
    response_size = sizeof(response);
    status = libspdm_get_response_psk_exchange(
        spdm_context, m_libspdm_psk_exchange_request1_size,
        &m_libspdm_psk_exchange_request1, &response_size, response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(
        libspdm_secured_message_get_session_state(
            spdm_context->session_info[0].secured_message_context),
        LIBSPDM_SESSION_STATE_HANDSHAKING);
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_PSK_EXCHANGE_RSP);
    assert_int_equal(spdm_response->rsp_session_id, 0xFFFF);
#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    assert_int_equal(spdm_context->transcript.message_m.buffer_size,
                     0);
#endif
    free(data1);
}

void libspdm_test_responder_psk_exchange_case2(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_psk_exchange_response_t *spdm_response;
    void *data1;
    size_t data_size1;
    uint8_t *ptr;
    size_t opaque_psk_exchange_req_size;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x2;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    spdm_context->connection_info.connection_state =
        LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.capability.flags |=
        SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PSK_CAP;
    spdm_context->local_context.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo =
        m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.measurement_spec =
        m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;
    spdm_context->connection_info.algorithm.dhe_named_group =
        m_libspdm_use_dhe_algo;
    spdm_context->connection_info.algorithm.aead_cipher_suite =
        m_libspdm_use_aead_algo;
    spdm_context->connection_info.algorithm.key_schedule =
        m_libspdm_use_key_schedule_algo;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data1,
                                                    &data_size1, NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[0] =
        data_size1;
    spdm_context->connection_info.local_used_cert_chain_buffer = data1;
    spdm_context->connection_info.local_used_cert_chain_buffer_size =
        data_size1;
    spdm_context->local_context.slot_count = 1;
    libspdm_reset_message_a(spdm_context);
    libspdm_zero_mem(m_libspdm_local_psk_hint, 32);
    libspdm_copy_mem(&m_libspdm_local_psk_hint[0], sizeof(m_libspdm_local_psk_hint),
                     LIBSPDM_TEST_PSK_HINT_STRING, sizeof(LIBSPDM_TEST_PSK_HINT_STRING));
    spdm_context->local_context.psk_hint_size =
        sizeof(LIBSPDM_TEST_PSK_HINT_STRING);
    spdm_context->local_context.psk_hint = m_libspdm_local_psk_hint;

    m_libspdm_psk_exchange_request2.psk_hint_length =
        (uint16_t)spdm_context->local_context.psk_hint_size;
    m_libspdm_psk_exchange_request2.context_length = LIBSPDM_PSK_CONTEXT_LENGTH;
    opaque_psk_exchange_req_size =
        libspdm_get_opaque_data_supported_version_data_size(spdm_context);
    m_libspdm_psk_exchange_request2.opaque_length =
        (uint16_t)opaque_psk_exchange_req_size;
    m_libspdm_psk_exchange_request2.req_session_id = 0xFFFF;
    ptr = m_libspdm_psk_exchange_request2.psk_hint;
    libspdm_copy_mem(ptr, sizeof(m_libspdm_psk_exchange_request2.psk_hint),
                     spdm_context->local_context.psk_hint,
                     spdm_context->local_context.psk_hint_size);
    ptr += m_libspdm_psk_exchange_request2.psk_hint_length;
    libspdm_get_random_number(LIBSPDM_PSK_CONTEXT_LENGTH, ptr);
    ptr += m_libspdm_psk_exchange_request2.context_length;
    libspdm_build_opaque_data_supported_version_data(
        spdm_context, &opaque_psk_exchange_req_size, ptr);
    ptr += opaque_psk_exchange_req_size;
    response_size = sizeof(response);
    status = libspdm_get_response_psk_exchange(
        spdm_context, m_libspdm_psk_exchange_request2_size,
        &m_libspdm_psk_exchange_request2, &response_size, response);
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

void libspdm_test_responder_psk_exchange_case3(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_psk_exchange_response_t *spdm_response;
    void *data1;
    size_t data_size1;
    uint8_t *ptr;
    size_t opaque_psk_exchange_req_size;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x3;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_BUSY;
    spdm_context->connection_info.connection_state =
        LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.capability.flags |=
        SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PSK_CAP;
    spdm_context->local_context.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo =
        m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.measurement_spec =
        m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;
    spdm_context->connection_info.algorithm.dhe_named_group =
        m_libspdm_use_dhe_algo;
    spdm_context->connection_info.algorithm.aead_cipher_suite =
        m_libspdm_use_aead_algo;
    spdm_context->connection_info.algorithm.key_schedule =
        m_libspdm_use_key_schedule_algo;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data1,
                                                    &data_size1, NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[0] =
        data_size1;
    spdm_context->connection_info.local_used_cert_chain_buffer = data1;
    spdm_context->connection_info.local_used_cert_chain_buffer_size =
        data_size1;
    spdm_context->local_context.slot_count = 1;
    libspdm_reset_message_a(spdm_context);
    libspdm_zero_mem(m_libspdm_local_psk_hint, 32);
    libspdm_copy_mem(&m_libspdm_local_psk_hint[0], sizeof(m_libspdm_local_psk_hint),
                     LIBSPDM_TEST_PSK_HINT_STRING, sizeof(LIBSPDM_TEST_PSK_HINT_STRING));
    spdm_context->local_context.psk_hint_size =
        sizeof(LIBSPDM_TEST_PSK_HINT_STRING);
    spdm_context->local_context.psk_hint = m_libspdm_local_psk_hint;

    m_libspdm_psk_exchange_request1.psk_hint_length =
        (uint16_t)spdm_context->local_context.psk_hint_size;
    m_libspdm_psk_exchange_request1.context_length = LIBSPDM_PSK_CONTEXT_LENGTH;
    opaque_psk_exchange_req_size =
        libspdm_get_opaque_data_supported_version_data_size(spdm_context);
    m_libspdm_psk_exchange_request1.opaque_length =
        (uint16_t)opaque_psk_exchange_req_size;
    m_libspdm_psk_exchange_request1.req_session_id = 0xFFFF;
    ptr = m_libspdm_psk_exchange_request1.psk_hint;
    libspdm_copy_mem(ptr, sizeof(m_libspdm_psk_exchange_request1.psk_hint),
                     spdm_context->local_context.psk_hint,
                     spdm_context->local_context.psk_hint_size);
    ptr += m_libspdm_psk_exchange_request1.psk_hint_length;
    libspdm_get_random_number(LIBSPDM_PSK_CONTEXT_LENGTH, ptr);
    ptr += m_libspdm_psk_exchange_request1.context_length;
    libspdm_build_opaque_data_supported_version_data(
        spdm_context, &opaque_psk_exchange_req_size, ptr);
    ptr += opaque_psk_exchange_req_size;
    response_size = sizeof(response);
    status = libspdm_get_response_psk_exchange(
        spdm_context, m_libspdm_psk_exchange_request1_size,
        &m_libspdm_psk_exchange_request1, &response_size, response);
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

void libspdm_test_responder_psk_exchange_case4(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_psk_exchange_response_t *spdm_response;
    void *data1;
    size_t data_size1;
    uint8_t *ptr;
    size_t opaque_psk_exchange_req_size;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x4;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_NEED_RESYNC;
    spdm_context->connection_info.connection_state =
        LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.capability.flags |=
        SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PSK_CAP;
    spdm_context->local_context.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo =
        m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.measurement_spec =
        m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;
    spdm_context->connection_info.algorithm.dhe_named_group =
        m_libspdm_use_dhe_algo;
    spdm_context->connection_info.algorithm.aead_cipher_suite =
        m_libspdm_use_aead_algo;
    spdm_context->connection_info.algorithm.key_schedule =
        m_libspdm_use_key_schedule_algo;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data1,
                                                    &data_size1, NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[0] =
        data_size1;
    spdm_context->connection_info.local_used_cert_chain_buffer = data1;
    spdm_context->connection_info.local_used_cert_chain_buffer_size =
        data_size1;
    spdm_context->local_context.slot_count = 1;
    libspdm_reset_message_a(spdm_context);
    libspdm_zero_mem(m_libspdm_local_psk_hint, 32);
    libspdm_copy_mem(&m_libspdm_local_psk_hint[0], sizeof(m_libspdm_local_psk_hint),
                     LIBSPDM_TEST_PSK_HINT_STRING, sizeof(LIBSPDM_TEST_PSK_HINT_STRING));
    spdm_context->local_context.psk_hint_size =
        sizeof(LIBSPDM_TEST_PSK_HINT_STRING);
    spdm_context->local_context.psk_hint = m_libspdm_local_psk_hint;

    m_libspdm_psk_exchange_request1.psk_hint_length =
        (uint16_t)spdm_context->local_context.psk_hint_size;
    m_libspdm_psk_exchange_request1.context_length = LIBSPDM_PSK_CONTEXT_LENGTH;
    opaque_psk_exchange_req_size =
        libspdm_get_opaque_data_supported_version_data_size(spdm_context);
    m_libspdm_psk_exchange_request1.opaque_length =
        (uint16_t)opaque_psk_exchange_req_size;
    m_libspdm_psk_exchange_request1.req_session_id = 0xFFFF;
    ptr = m_libspdm_psk_exchange_request1.psk_hint;
    libspdm_copy_mem(ptr, sizeof(m_libspdm_psk_exchange_request1.psk_hint),
                     spdm_context->local_context.psk_hint,
                     spdm_context->local_context.psk_hint_size);
    ptr += m_libspdm_psk_exchange_request1.psk_hint_length;
    libspdm_get_random_number(LIBSPDM_PSK_CONTEXT_LENGTH, ptr);
    ptr += m_libspdm_psk_exchange_request1.context_length;
    libspdm_build_opaque_data_supported_version_data(
        spdm_context, &opaque_psk_exchange_req_size, ptr);
    ptr += opaque_psk_exchange_req_size;
    response_size = sizeof(response);
    status = libspdm_get_response_psk_exchange(
        spdm_context, m_libspdm_psk_exchange_request1_size,
        &m_libspdm_psk_exchange_request1, &response_size, response);
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

void libspdm_test_responder_psk_exchange_case5(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_psk_exchange_response_t *spdm_response;
    void *data1;
    size_t data_size1;
    uint8_t *ptr;
    size_t opaque_psk_exchange_req_size;
    spdm_error_data_response_not_ready_t *error_data;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x5;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_NOT_READY;
    spdm_context->connection_info.connection_state =
        LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.capability.flags |=
        SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PSK_CAP;
    spdm_context->local_context.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo =
        m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.measurement_spec =
        m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;
    spdm_context->connection_info.algorithm.dhe_named_group =
        m_libspdm_use_dhe_algo;
    spdm_context->connection_info.algorithm.aead_cipher_suite =
        m_libspdm_use_aead_algo;
    spdm_context->connection_info.algorithm.key_schedule =
        m_libspdm_use_key_schedule_algo;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data1,
                                                    &data_size1, NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[0] =
        data_size1;
    spdm_context->connection_info.local_used_cert_chain_buffer = data1;
    spdm_context->connection_info.local_used_cert_chain_buffer_size =
        data_size1;
    spdm_context->local_context.slot_count = 1;
    libspdm_reset_message_a(spdm_context);
    libspdm_zero_mem(m_libspdm_local_psk_hint, 32);
    libspdm_copy_mem(&m_libspdm_local_psk_hint[0], sizeof(m_libspdm_local_psk_hint),
                     LIBSPDM_TEST_PSK_HINT_STRING, sizeof(LIBSPDM_TEST_PSK_HINT_STRING));
    spdm_context->local_context.psk_hint_size =
        sizeof(LIBSPDM_TEST_PSK_HINT_STRING);
    spdm_context->local_context.psk_hint = m_libspdm_local_psk_hint;

    m_libspdm_psk_exchange_request1.psk_hint_length =
        (uint16_t)spdm_context->local_context.psk_hint_size;
    m_libspdm_psk_exchange_request1.context_length = LIBSPDM_PSK_CONTEXT_LENGTH;
    opaque_psk_exchange_req_size =
        libspdm_get_opaque_data_supported_version_data_size(spdm_context);
    m_libspdm_psk_exchange_request1.opaque_length =
        (uint16_t)opaque_psk_exchange_req_size;
    m_libspdm_psk_exchange_request1.req_session_id = 0xFFFF;
    ptr = m_libspdm_psk_exchange_request1.psk_hint;
    libspdm_copy_mem(ptr, sizeof(m_libspdm_psk_exchange_request1.psk_hint),
                     spdm_context->local_context.psk_hint,
                     spdm_context->local_context.psk_hint_size);
    ptr += m_libspdm_psk_exchange_request1.psk_hint_length;
    libspdm_get_random_number(LIBSPDM_PSK_CONTEXT_LENGTH, ptr);
    ptr += m_libspdm_psk_exchange_request1.context_length;
    libspdm_build_opaque_data_supported_version_data(
        spdm_context, &opaque_psk_exchange_req_size, ptr);
    ptr += opaque_psk_exchange_req_size;
    response_size = sizeof(response);
    status = libspdm_get_response_psk_exchange(
        spdm_context, m_libspdm_psk_exchange_request1_size,
        &m_libspdm_psk_exchange_request1, &response_size, response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(response_size,
                     sizeof(spdm_error_response_t) +
                     sizeof(spdm_error_data_response_not_ready_t));
    spdm_response = (void *)response;
    error_data = (spdm_error_data_response_not_ready_t
                  *)(&spdm_response->rsp_session_id);
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_RESPONSE_NOT_READY);
    assert_int_equal(spdm_response->header.param2, 0);
    assert_int_equal(spdm_context->response_state,
                     LIBSPDM_RESPONSE_STATE_NOT_READY);
    assert_int_equal(error_data->request_code, SPDM_PSK_EXCHANGE);
    free(data1);
}

void libspdm_test_responder_psk_exchange_case6(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_psk_exchange_response_t *spdm_response;
    void *data1;
    size_t data_size1;
    uint8_t *ptr;
    size_t opaque_psk_exchange_req_size;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x6;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_NORMAL;
    spdm_context->connection_info.connection_state =
        LIBSPDM_CONNECTION_STATE_NOT_STARTED;
    spdm_context->connection_info.capability.flags |=
        SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PSK_CAP;
    spdm_context->local_context.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo =
        m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.measurement_spec =
        m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;
    spdm_context->connection_info.algorithm.dhe_named_group =
        m_libspdm_use_dhe_algo;
    spdm_context->connection_info.algorithm.aead_cipher_suite =
        m_libspdm_use_aead_algo;
    spdm_context->connection_info.algorithm.key_schedule =
        m_libspdm_use_key_schedule_algo;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data1,
                                                    &data_size1, NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[0] =
        data_size1;
    spdm_context->connection_info.local_used_cert_chain_buffer = data1;
    spdm_context->connection_info.local_used_cert_chain_buffer_size =
        data_size1;
    spdm_context->local_context.slot_count = 1;
    libspdm_reset_message_a(spdm_context);
    libspdm_zero_mem(m_libspdm_local_psk_hint, 32);
    libspdm_copy_mem(&m_libspdm_local_psk_hint[0], sizeof(m_libspdm_local_psk_hint),
                     LIBSPDM_TEST_PSK_HINT_STRING, sizeof(LIBSPDM_TEST_PSK_HINT_STRING));
    spdm_context->local_context.psk_hint_size =
        sizeof(LIBSPDM_TEST_PSK_HINT_STRING);
    spdm_context->local_context.psk_hint = m_libspdm_local_psk_hint;

    m_libspdm_psk_exchange_request1.psk_hint_length =
        (uint16_t)spdm_context->local_context.psk_hint_size;
    m_libspdm_psk_exchange_request1.context_length = LIBSPDM_PSK_CONTEXT_LENGTH;
    opaque_psk_exchange_req_size =
        libspdm_get_opaque_data_supported_version_data_size(spdm_context);
    m_libspdm_psk_exchange_request1.opaque_length =
        (uint16_t)opaque_psk_exchange_req_size;
    m_libspdm_psk_exchange_request1.req_session_id = 0xFFFF;
    ptr = m_libspdm_psk_exchange_request1.psk_hint;
    libspdm_copy_mem(ptr, sizeof(m_libspdm_psk_exchange_request1.psk_hint),
                     spdm_context->local_context.psk_hint,
                     spdm_context->local_context.psk_hint_size);
    ptr += m_libspdm_psk_exchange_request1.psk_hint_length;
    libspdm_get_random_number(LIBSPDM_PSK_CONTEXT_LENGTH, ptr);
    ptr += m_libspdm_psk_exchange_request1.context_length;
    libspdm_build_opaque_data_supported_version_data(
        spdm_context, &opaque_psk_exchange_req_size, ptr);
    ptr += opaque_psk_exchange_req_size;
    response_size = sizeof(response);
    status = libspdm_get_response_psk_exchange(
        spdm_context, m_libspdm_psk_exchange_request1_size,
        &m_libspdm_psk_exchange_request1, &response_size, response);
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

void libspdm_test_responder_psk_exchange_case7(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_psk_exchange_response_t *spdm_response;
    void *data1;
    size_t data_size1;
    uint8_t *ptr;
    size_t opaque_psk_exchange_req_size;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x7;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    spdm_context->connection_info.connection_state =
        LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.capability.flags |=
        SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PSK_CAP;
    spdm_context->local_context.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo =
        m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.measurement_spec =
        m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;
    spdm_context->connection_info.algorithm.dhe_named_group =
        m_libspdm_use_dhe_algo;
    spdm_context->connection_info.algorithm.aead_cipher_suite =
        m_libspdm_use_aead_algo;
    spdm_context->connection_info.algorithm.key_schedule =
        m_libspdm_use_key_schedule_algo;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data1,
                                                    &data_size1, NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[0] =
        data_size1;
    spdm_context->connection_info.local_used_cert_chain_buffer = data1;
    spdm_context->connection_info.local_used_cert_chain_buffer_size =
        data_size1;
    spdm_context->local_context.slot_count = 1;
    libspdm_reset_message_a(spdm_context);
    libspdm_zero_mem(m_libspdm_local_psk_hint, 32);
    libspdm_copy_mem(&m_libspdm_local_psk_hint[0], sizeof(m_libspdm_local_psk_hint),
                     LIBSPDM_TEST_PSK_HINT_STRING, sizeof(LIBSPDM_TEST_PSK_HINT_STRING));
    spdm_context->local_context.psk_hint_size =
        sizeof(LIBSPDM_TEST_PSK_HINT_STRING);
    spdm_context->local_context.psk_hint = m_libspdm_local_psk_hint;

    m_libspdm_psk_exchange_request1.psk_hint_length =
        (uint16_t)spdm_context->local_context.psk_hint_size;
    m_libspdm_psk_exchange_request1.context_length = LIBSPDM_PSK_CONTEXT_LENGTH;
    opaque_psk_exchange_req_size =
        libspdm_get_opaque_data_supported_version_data_size(spdm_context);
    m_libspdm_psk_exchange_request1.opaque_length =
        (uint16_t)opaque_psk_exchange_req_size;
    m_libspdm_psk_exchange_request1.req_session_id = 0xFFFF;
    ptr = m_libspdm_psk_exchange_request1.psk_hint;
    libspdm_copy_mem(ptr, sizeof(m_libspdm_psk_exchange_request1.psk_hint),
                     spdm_context->local_context.psk_hint,
                     spdm_context->local_context.psk_hint_size);
    ptr += m_libspdm_psk_exchange_request1.psk_hint_length;
    libspdm_get_random_number(LIBSPDM_PSK_CONTEXT_LENGTH, ptr);
    ptr += m_libspdm_psk_exchange_request1.context_length;
    libspdm_build_opaque_data_supported_version_data(
        spdm_context, &opaque_psk_exchange_req_size, ptr);
    ptr += opaque_psk_exchange_req_size;

#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    spdm_context->transcript.message_m.buffer_size =
        spdm_context->transcript.message_m.max_buffer_size;
    spdm_context->transcript.message_b.buffer_size =
        spdm_context->transcript.message_b.max_buffer_size;
    spdm_context->transcript.message_c.buffer_size =
        spdm_context->transcript.message_c.max_buffer_size;
    spdm_context->transcript.message_mut_b.buffer_size =
        spdm_context->transcript.message_mut_b.max_buffer_size;
    spdm_context->transcript.message_mut_c.buffer_size =
        spdm_context->transcript.message_mut_c.max_buffer_size;
#endif

    response_size = sizeof(response);
    status = libspdm_get_response_psk_exchange(
        spdm_context, m_libspdm_psk_exchange_request1_size,
        &m_libspdm_psk_exchange_request1, &response_size, response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(
        libspdm_secured_message_get_session_state(
            spdm_context->session_info[0].secured_message_context),
        LIBSPDM_SESSION_STATE_HANDSHAKING);
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_PSK_EXCHANGE_RSP);
#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    assert_int_equal(spdm_context->transcript.message_m.buffer_size, 0);
    assert_int_equal(spdm_context->transcript.message_b.buffer_size, 0);
    assert_int_equal(spdm_context->transcript.message_c.buffer_size, 0);
    assert_int_equal(spdm_context->transcript.message_mut_b.buffer_size, 0);
    assert_int_equal(spdm_context->transcript.message_mut_c.buffer_size, 0);
#endif

    free(data1);
}

void libspdm_test_responder_psk_exchange_case8(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t current_request_size;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_psk_exchange_response_t *spdm_response;
    void *data1;
    size_t data_size1;
    uint8_t *ptr;
    size_t opaque_psk_exchange_req_size;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;

    if(spdm_context->session_info[0].session_id != INVALID_SESSION_ID) {
        libspdm_free_session_id(spdm_context,0xFFFFFFFF);
    }

    spdm_test_context->case_id = 0x8;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_11 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    spdm_context->connection_info.connection_state =
        LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.capability.flags |=
        SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PSK_CAP;
    spdm_context->local_context.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo = m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.measurement_spec = m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;
    spdm_context->connection_info.algorithm.dhe_named_group = m_libspdm_use_dhe_algo;
    spdm_context->connection_info.algorithm.aead_cipher_suite = m_libspdm_use_aead_algo;
    spdm_context->connection_info.algorithm.key_schedule = m_libspdm_use_key_schedule_algo;
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data1,
                                                    &data_size1, NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[0] = data_size1;
    spdm_context->connection_info.local_used_cert_chain_buffer = data1;
    spdm_context->connection_info.local_used_cert_chain_buffer_size = data_size1;
    spdm_context->local_context.slot_count = 1;
    libspdm_reset_message_a(spdm_context);
    libspdm_zero_mem(m_libspdm_local_psk_hint, 32);
    libspdm_copy_mem(&m_libspdm_local_psk_hint[0], sizeof(m_libspdm_local_psk_hint),
                     LIBSPDM_TEST_PSK_HINT_STRING, sizeof(LIBSPDM_TEST_PSK_HINT_STRING));
    spdm_context->local_context.psk_hint_size = sizeof(LIBSPDM_TEST_PSK_HINT_STRING);
    spdm_context->local_context.psk_hint = m_libspdm_local_psk_hint;

    m_libspdm_psk_exchange_request1.psk_hint_length =
        (uint16_t)spdm_context->local_context.psk_hint_size;
    m_libspdm_psk_exchange_request1.context_length = LIBSPDM_PSK_CONTEXT_LENGTH;
    opaque_psk_exchange_req_size =
        libspdm_get_opaque_data_supported_version_data_size(spdm_context);
    m_libspdm_psk_exchange_request1.opaque_length =
        (uint16_t)opaque_psk_exchange_req_size;
    m_libspdm_psk_exchange_request1.req_session_id = 0xFFFF;
    ptr = m_libspdm_psk_exchange_request1.psk_hint;
    libspdm_copy_mem(ptr, sizeof(m_libspdm_psk_exchange_request1.psk_hint),
                     spdm_context->local_context.psk_hint,
                     spdm_context->local_context.psk_hint_size);
    ptr += m_libspdm_psk_exchange_request1.psk_hint_length;
    libspdm_get_random_number(LIBSPDM_PSK_CONTEXT_LENGTH, ptr);
    ptr += m_libspdm_psk_exchange_request1.context_length;
    libspdm_build_opaque_data_supported_version_data(
        spdm_context, &opaque_psk_exchange_req_size, ptr);
    ptr += opaque_psk_exchange_req_size;

    current_request_size = sizeof(spdm_psk_exchange_request_t) +
                           m_libspdm_psk_exchange_request1.psk_hint_length +
                           m_libspdm_psk_exchange_request1.context_length +
                           opaque_psk_exchange_req_size;
    response_size = sizeof(response);
    status = libspdm_get_response_psk_exchange(
        spdm_context, current_request_size, &m_libspdm_psk_exchange_request1,
        &response_size, response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(libspdm_secured_message_get_session_state(
                         spdm_context->session_info[0].secured_message_context),
                     LIBSPDM_SESSION_STATE_HANDSHAKING);
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code, SPDM_PSK_EXCHANGE_RSP);
    assert_int_equal(spdm_response->rsp_session_id, 0xFFFF);

#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    assert_int_equal(spdm_context->session_info[0].session_transcript.message_k.buffer_size,
                     current_request_size + response_size);
    assert_memory_equal(spdm_context->session_info[0].session_transcript.message_k.buffer,
                        &m_libspdm_psk_exchange_request1, current_request_size);
    assert_memory_equal(spdm_context->session_info[0].session_transcript.message_k.buffer +
                        current_request_size, response, response_size);
#endif
    free(data1);
}

void libspdm_test_responder_psk_exchange_case9(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_psk_exchange_response_t *spdm_response;
    void *data1;
    size_t data_size1;
    uint8_t *ptr;
    size_t opaque_psk_exchange_req_size;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x9;
    spdm_context->connection_info.version = SPDM_MESSAGE_VERSION_12 <<
                                            SPDM_VERSION_NUMBER_SHIFT_BIT;
    spdm_context->connection_info.connection_state =
        LIBSPDM_CONNECTION_STATE_NEGOTIATED;
    spdm_context->connection_info.capability.flags |=
        SPDM_GET_CAPABILITIES_REQUEST_FLAGS_PSK_CAP;
    spdm_context->local_context.capability.flags |=
        SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP;
    spdm_context->connection_info.algorithm.base_hash_algo =
        m_libspdm_use_hash_algo;
    spdm_context->connection_info.algorithm.measurement_spec =
        m_libspdm_use_measurement_spec;
    spdm_context->connection_info.algorithm.measurement_hash_algo =
        m_libspdm_use_measurement_hash_algo;
    spdm_context->connection_info.algorithm.dhe_named_group =
        m_libspdm_use_dhe_algo;
    spdm_context->connection_info.algorithm.aead_cipher_suite =
        m_libspdm_use_aead_algo;
    spdm_context->connection_info.algorithm.key_schedule =
        m_libspdm_use_key_schedule_algo;
    libspdm_session_info_init(spdm_context,
                              spdm_context->session_info,
                              INVALID_SESSION_ID, false);
    libspdm_read_responder_public_certificate_chain(m_libspdm_use_hash_algo,
                                                    m_libspdm_use_asym_algo, &data1,
                                                    &data_size1, NULL, NULL);
    spdm_context->local_context.local_cert_chain_provision[0] = data1;
    spdm_context->local_context.local_cert_chain_provision_size[0] =
        data_size1;
    spdm_context->connection_info.local_used_cert_chain_buffer = data1;
    spdm_context->connection_info.local_used_cert_chain_buffer_size =
        data_size1;
    spdm_context->local_context.slot_count = 1;
    libspdm_reset_message_a(spdm_context);
    libspdm_zero_mem(m_libspdm_local_psk_hint, 32);
    libspdm_copy_mem(&m_libspdm_local_psk_hint[0], sizeof(m_libspdm_local_psk_hint),
                     LIBSPDM_TEST_PSK_HINT_STRING, sizeof(LIBSPDM_TEST_PSK_HINT_STRING));
    spdm_context->local_context.psk_hint_size =
        sizeof(LIBSPDM_TEST_PSK_HINT_STRING);
    spdm_context->local_context.psk_hint = m_libspdm_local_psk_hint;

    m_libspdm_psk_exchange_request3.psk_hint_length =
        (uint16_t)spdm_context->local_context.psk_hint_size;
    m_libspdm_psk_exchange_request3.context_length = LIBSPDM_PSK_CONTEXT_LENGTH;
    opaque_psk_exchange_req_size =
        libspdm_get_opaque_data_supported_version_data_size(spdm_context);
    m_libspdm_psk_exchange_request3.opaque_length =
        (uint16_t)opaque_psk_exchange_req_size;
    m_libspdm_psk_exchange_request3.req_session_id = 0xFFFF;
    ptr = m_libspdm_psk_exchange_request3.psk_hint;
    libspdm_copy_mem(ptr, sizeof(m_libspdm_psk_exchange_request3.psk_hint),
                     spdm_context->local_context.psk_hint,
                     spdm_context->local_context.psk_hint_size);
    ptr += m_libspdm_psk_exchange_request3.psk_hint_length;
    libspdm_get_random_number(LIBSPDM_PSK_CONTEXT_LENGTH, ptr);
    ptr += m_libspdm_psk_exchange_request3.context_length;
    libspdm_build_opaque_data_supported_version_data(
        spdm_context, &opaque_psk_exchange_req_size, ptr);
#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    spdm_context->transcript.message_m.buffer_size =
        spdm_context->transcript.message_m.max_buffer_size;
#endif
    ptr += opaque_psk_exchange_req_size;
    response_size = sizeof(response);
    status = libspdm_get_response_psk_exchange(
        spdm_context, m_libspdm_psk_exchange_request3_size,
        &m_libspdm_psk_exchange_request3, &response_size, response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(
        libspdm_secured_message_get_session_state(
            spdm_context->session_info[0].secured_message_context),
        LIBSPDM_SESSION_STATE_HANDSHAKING);
    assert_int_equal(spdm_context->session_info[0].session_policy, 0);
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.spdm_version,
                     SPDM_MESSAGE_VERSION_12);
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_PSK_EXCHANGE_RSP);
    assert_int_equal(spdm_response->rsp_session_id, 0xFFFF);
#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    assert_int_equal(spdm_context->transcript.message_m.buffer_size,
                     0);
#endif
    free(data1);
}

libspdm_test_context_t m_libspdm_responder_psk_exchange_test_context = {
    LIBSPDM_TEST_CONTEXT_VERSION,
    false,
};

int libspdm_responder_psk_exchange_test_main(void)
{
    const struct CMUnitTest spdm_responder_psk_exchange_tests[] = {
        /* Success Case*/
        cmocka_unit_test(libspdm_test_responder_psk_exchange_case1),
        /* Bad request size*/
        cmocka_unit_test(libspdm_test_responder_psk_exchange_case2),
        /* response_state: SPDM_RESPONSE_STATE_BUSY*/
        cmocka_unit_test(libspdm_test_responder_psk_exchange_case3),
        /* response_state: SPDM_RESPONSE_STATE_NEED_RESYNC*/
        cmocka_unit_test(libspdm_test_responder_psk_exchange_case4),
        /* response_state: SPDM_RESPONSE_STATE_NOT_READY*/
        cmocka_unit_test(libspdm_test_responder_psk_exchange_case5),
        /* connection_state Check*/
        cmocka_unit_test(libspdm_test_responder_psk_exchange_case6),
        /* Buffer reset*/
        cmocka_unit_test(libspdm_test_responder_psk_exchange_case7),
        /* Buffer verification*/
        cmocka_unit_test(libspdm_test_responder_psk_exchange_case8),
        /* Successful response V1.2*/
        cmocka_unit_test(libspdm_test_responder_psk_exchange_case9),
    };

    libspdm_setup_test_context(&m_libspdm_responder_psk_exchange_test_context);

    return cmocka_run_group_tests(spdm_responder_psk_exchange_tests,
                                  libspdm_unit_test_group_setup,
                                  libspdm_unit_test_group_teardown);
}

#endif /* LIBSPDM_ENABLE_CAPABILITY_PSK_EX_CAP*/
