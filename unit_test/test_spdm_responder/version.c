/**
 *  Copyright Notice:
 *  Copyright 2021-2022 DMTF. All rights reserved.
 *  License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/libspdm/blob/main/LICENSE.md
 **/

#include "spdm_unit_test.h"
#include "internal/libspdm_responder_lib.h"

#define LIBSPDM_DEFAULT_SPDM_VERSION_ENTRY_COUNT SPDM_MAX_VERSION_COUNT

#pragma pack(1)
typedef struct {
    spdm_message_header_t header;
    uint8_t reserved;
    uint8_t version_number_entry_count;
    spdm_version_number_t version_number_entry[LIBSPDM_MAX_VERSION_COUNT];
} libspdm_version_response_mine_t;
#pragma pack()

spdm_get_version_request_t m_libspdm_get_version_request1 = {
    {
        SPDM_MESSAGE_VERSION_10,
        SPDM_GET_VERSION,
    },
};
size_t m_libspdm_get_version_request1_size = sizeof(m_libspdm_get_version_request1);

spdm_get_version_request_t m_libspdm_get_version_request2 = {
    {
        SPDM_MESSAGE_VERSION_10,
        SPDM_GET_VERSION,
    },
};
size_t m_libspdm_get_version_request2_size = LIBSPDM_MAX_MESSAGE_BUFFER_SIZE;

spdm_get_version_request_t m_libspdm_get_version_request3 = {
    {
        SPDM_MESSAGE_VERSION_11,
        SPDM_GET_VERSION,
    },
};
size_t m_libspdm_get_version_request3_size = sizeof(m_libspdm_get_version_request3);

spdm_get_version_request_t m_libspdm_get_version_request4 = {
    {
        SPDM_MESSAGE_VERSION_10,
        SPDM_VERSION,
    },
};
size_t m_libspdm_get_version_request4_size = sizeof(m_libspdm_get_version_request4);

/**
 * Test 1: receiving a correct GET_VERSION from the requester.
 * Expected behavior: the responder accepts the request and produces a valid VERSION
 * response message.
 **/
void libspdm_test_responder_version_case1(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_version_response_t *spdm_response;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x1;

    response_size = sizeof(response);
    status = libspdm_get_response_version(spdm_context,
                                          m_libspdm_get_version_request1_size,
                                          &m_libspdm_get_version_request1,
                                          &response_size, response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(response_size,
                     sizeof(spdm_version_response_t) +
                     LIBSPDM_DEFAULT_SPDM_VERSION_ENTRY_COUNT *
                     sizeof(spdm_version_number_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_VERSION);
}

/**
 * Test 2: receiving a GET_VERSION message larger than specified (more parameters than the
 * header), results in a correct VERSION message.
 * Expected behavior: the responder refuses the GET_VERSION message and produces an
 * ERROR message indicating the InvalidRequest.
 **/
void libspdm_test_responder_version_case2(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_version_response_t *spdm_response;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x2;

    response_size = sizeof(response);
    status = libspdm_get_response_version(spdm_context,
                                          m_libspdm_get_version_request2_size,
                                          &m_libspdm_get_version_request2,
                                          &response_size, response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_INVALID_REQUEST);
    assert_int_equal(spdm_response->header.param2, 0);
}

/**
 * Test 3: receiving a correct GET_VERSION from the requester, but the responder is in
 * a Busy state.
 * Expected behavior: the responder accepts the request, but produces an ERROR message
 * indicating the Buse state.
 **/
void libspdm_test_responder_version_case3(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_version_response_t *spdm_response;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x3;
    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_BUSY;

    response_size = sizeof(response);
    status = libspdm_get_response_version(spdm_context,
                                          m_libspdm_get_version_request1_size,
                                          &m_libspdm_get_version_request1,
                                          &response_size, response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1, SPDM_ERROR_CODE_BUSY);
    assert_int_equal(spdm_response->header.param2, 0);
    assert_int_equal(spdm_context->response_state,
                     LIBSPDM_RESPONSE_STATE_BUSY);
}

/**
 * Test 4: receiving a correct GET_VERSION from the requester, but the responder requires
 * resynchronization with the requester.
 * Expected behavior: the requester resets the communication upon receiving the GET_VERSION
 * message, fulfilling the resynchronization. A valid VERSION message is produced.
 **/
void libspdm_test_responder_version_case4(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_version_response_t *spdm_response;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x4;
    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_NEED_RESYNC;

    response_size = sizeof(response);
    status = libspdm_get_response_version(spdm_context,
                                          m_libspdm_get_version_request1_size,
                                          &m_libspdm_get_version_request1,
                                          &response_size, response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(response_size,
                     sizeof(spdm_version_response_t) +
                     LIBSPDM_DEFAULT_SPDM_VERSION_ENTRY_COUNT *
                     sizeof(spdm_version_number_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_VERSION);
    assert_int_equal(spdm_context->response_state,
                     LIBSPDM_RESPONSE_STATE_NORMAL);
}

/**
 * Test 5: receiving a correct GET_VERSION from the requester, but the responder could not
 * produce the response in time.
 * TODO: As from version 1.0.0, a GET_VERSION message should not receive an ERROR message
 * indicating the ResponseNotReady. No timing parameters have been agreed yet.
 **/
void libspdm_test_responder_version_case5(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_version_response_t *spdm_response;
    spdm_error_data_response_not_ready_t *error_data;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x5;
    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_NOT_READY;

    response_size = sizeof(response);
    status = libspdm_get_response_version(spdm_context,
                                          m_libspdm_get_version_request1_size,
                                          &m_libspdm_get_version_request1,
                                          &response_size, response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(response_size,
                     sizeof(spdm_error_response_t) +
                     sizeof(spdm_error_data_response_not_ready_t));
    spdm_response = (void *)response;
    error_data =
        (spdm_error_data_response_not_ready_t *)(&spdm_response
                                                 ->reserved);
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_RESPONSE_NOT_READY);
    assert_int_equal(spdm_response->header.param2, 0);
    assert_int_equal(spdm_context->response_state,
                     LIBSPDM_RESPONSE_STATE_NOT_READY);
    assert_int_equal(error_data->request_code, SPDM_GET_VERSION);
}

/**
 * Test 6: receiving a GET_VERSION message in SPDM version 1.1 (in the header), but correct
 * 1.0-version format.
 * Expected behavior: the responder refuses the GET_VERSION message and produces an
 * ERROR message indicating the InvalidRequest.
 **/
void libspdm_test_responder_version_case6(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_version_response_t *spdm_response;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x6;
    spdm_context->response_state = LIBSPDM_RESPONSE_STATE_NORMAL;

    response_size = sizeof(response);
    status = libspdm_get_response_version(spdm_context,
                                          m_libspdm_get_version_request3_size,
                                          &m_libspdm_get_version_request3,
                                          &response_size, response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_INVALID_REQUEST);
    assert_int_equal(spdm_response->header.param2, 0);
}

/**
 * Test 7: receiving a SPDM message with a VERSION 0x04 request_response_code instead
 * of a GET_VERSION 0x84 one.
 * Expected behavior: the responder refuses the VERSION message and produces an
 * ERROR message indicating the InvalidRequest.
 **/
void libspdm_test_responder_version_case7(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_version_response_t *spdm_response;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x6;

    response_size = sizeof(response);
    status = libspdm_get_response_version(spdm_context,
                                          m_libspdm_get_version_request3_size,
                                          &m_libspdm_get_version_request3,
                                          &response_size, response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_error_response_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code,
                     SPDM_ERROR);
    assert_int_equal(spdm_response->header.param1,
                     SPDM_ERROR_CODE_INVALID_REQUEST);
    assert_int_equal(spdm_response->header.param2, 0);
}

/**
 * Test 8: receiving a correct GET_VERSION from the requester. Buffers A, B and C
 * already have arbitrary data.
 * Expected behavior: the responder accepts the request and produces a valid VERSION
 * response message, buffers A, B and C should be first reset, and then buffer A
 * receives only the exchanged GET_VERSION and VERSION messages.
 **/
void libspdm_test_responder_version_case8(void **state)
{
    libspdm_return_t status;
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];
    spdm_version_response_t *spdm_response;

    spdm_test_context = *state;
    spdm_context = spdm_test_context->spdm_context;
    spdm_test_context->case_id = 0x8;

    /*filling buffers with arbitrary data*/
    libspdm_set_mem(spdm_context->transcript.message_a.buffer, 10, (uint8_t) 0xFF);
    spdm_context->transcript.message_a.buffer_size = 10;
#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    libspdm_set_mem(spdm_context->transcript.message_b.buffer, 8, (uint8_t) 0xEE);
    spdm_context->transcript.message_b.buffer_size = 8;
    libspdm_set_mem(spdm_context->transcript.message_c.buffer, 12, (uint8_t) 0xDD);
    spdm_context->transcript.message_c.buffer_size = 12;
#endif

    response_size = sizeof(response);
    status = libspdm_get_response_version(
        spdm_context, m_libspdm_get_version_request1_size, &m_libspdm_get_version_request1,
        &response_size, response);
    assert_int_equal(status, LIBSPDM_STATUS_SUCCESS);
    assert_int_equal(response_size, sizeof(spdm_version_response_t) +
                     LIBSPDM_DEFAULT_SPDM_VERSION_ENTRY_COUNT * sizeof(spdm_version_number_t));
    spdm_response = (void *)response;
    assert_int_equal(spdm_response->header.request_response_code, SPDM_VERSION);

    assert_int_equal(spdm_context->transcript.message_a.buffer_size,
                     m_libspdm_get_version_request1_size + response_size);
    assert_memory_equal(spdm_context->transcript.message_a.buffer,
                        &m_libspdm_get_version_request1, m_libspdm_get_version_request1_size);
    assert_memory_equal(
        spdm_context->transcript.message_a.buffer + m_libspdm_get_version_request1_size,
        response, response_size);
#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    assert_int_equal(spdm_context->transcript.message_b.buffer_size, 0);
    assert_int_equal(spdm_context->transcript.message_c.buffer_size, 0);
#endif
}

libspdm_test_context_t m_libspdm_responder_version_test_context = {
    LIBSPDM_TEST_CONTEXT_VERSION,
    false,
};

int libspdm_responder_version_test_main(void)
{
    const struct CMUnitTest spdm_responder_version_tests[] = {
        cmocka_unit_test(libspdm_test_responder_version_case1),
        /* Invalid request*/
        cmocka_unit_test(libspdm_test_responder_version_case2),
        /* response_state: SPDM_RESPONSE_STATE_BUSY*/
        cmocka_unit_test(libspdm_test_responder_version_case3),
        /* response_state: SPDM_RESPONSE_STATE_NEED_RESYNC*/
        cmocka_unit_test(libspdm_test_responder_version_case4),
        /* response_state: SPDM_RESPONSE_STATE_NOT_READY*/
        cmocka_unit_test(libspdm_test_responder_version_case5),
        /* Invalid request*/
        cmocka_unit_test(libspdm_test_responder_version_case6),
        /* Buffer verification*/
        cmocka_unit_test(libspdm_test_responder_version_case8),
    };

    libspdm_setup_test_context(&m_libspdm_responder_version_test_context);

    return cmocka_run_group_tests(spdm_responder_version_tests,
                                  libspdm_unit_test_group_setup,
                                  libspdm_unit_test_group_teardown);
}
