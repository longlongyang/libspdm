/**
 *  Copyright Notice:
 *  Copyright 2021-2022 DMTF. All rights reserved.
 *  License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/libspdm/blob/main/LICENSE.md
 **/

#include "spdm_unit_fuzzing.h"
#include "toolchain_harness.h"
#include "internal/libspdm_responder_lib.h"

size_t libspdm_get_max_buffer_size(void)
{
    return LIBSPDM_MAX_MESSAGE_BUFFER_SIZE;
}

void libspdm_test_responder_version(void **State)
{
    libspdm_test_context_t *spdm_test_context;
    libspdm_context_t *spdm_context;
    size_t response_size;
    uint8_t response[LIBSPDM_MAX_MESSAGE_BUFFER_SIZE];

    spdm_test_context = *State;
    spdm_context = spdm_test_context->spdm_context;

    response_size = sizeof(response);
    libspdm_get_response_version(spdm_context,
                                 spdm_test_context->test_buffer_size,
                                 spdm_test_context->test_buffer,
                                 &response_size, response);
}

libspdm_test_context_t m_libspdm_responder_version_test_context = {
    LIBSPDM_TEST_CONTEXT_VERSION,
    false,
};

void libspdm_run_test_harness(const void *test_buffer, size_t test_buffer_size)
{
    void *State;

    libspdm_setup_test_context(&m_libspdm_responder_version_test_context);

    m_libspdm_responder_version_test_context.test_buffer = (void *)test_buffer;
    m_libspdm_responder_version_test_context.test_buffer_size =
        test_buffer_size;

    libspdm_unit_test_group_setup(&State);

    /* Success Case*/
    libspdm_test_responder_version(&State);

    libspdm_unit_test_group_teardown(&State);
}
