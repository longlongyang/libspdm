/**
 *  Copyright Notice:
 *  Copyright 2021-2022 DMTF. All rights reserved.
 *  License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/libspdm/blob/main/LICENSE.md
 **/

#include "internal/libspdm_responder_lib.h"

#if LIBSPDM_ENABLE_CAPABILITY_CERT_CAP

/**
 * Get the SPDM encapsulated GET_CERTIFICATE request.
 *
 * @param  spdm_context                  A pointer to the SPDM context.
 * @param  encap_request_size             size in bytes of the encapsulated request data.
 *                                     On input, it means the size in bytes of encapsulated request data buffer.
 *                                     On output, it means the size in bytes of copied encapsulated request data buffer if RETURN_SUCCESS is returned,
 *                                     and means the size in bytes of desired encapsulated request data buffer if RETURN_BUFFER_TOO_SMALL is returned.
 * @param  encap_request                 A pointer to the encapsulated request data.
 *
 * @retval RETURN_SUCCESS               The encapsulated request is returned.
 * @retval RETURN_BUFFER_TOO_SMALL      The buffer is too small to hold the data.
 **/
libspdm_return_t
libspdm_get_encap_request_get_certificate(libspdm_context_t *spdm_context,
                                          size_t *encap_request_size,
                                          void *encap_request)
{
    spdm_get_certificate_request_t *spdm_request;
    libspdm_return_t status;

    spdm_context->encap_context.last_encap_request_size = 0;

    if (!libspdm_is_capabilities_flag_supported(
            spdm_context, false,
            SPDM_GET_CAPABILITIES_REQUEST_FLAGS_CERT_CAP, 0)) {
        return LIBSPDM_STATUS_UNSUPPORTED_CAP;
    }

    LIBSPDM_ASSERT(*encap_request_size >= sizeof(spdm_get_certificate_request_t));
    *encap_request_size = sizeof(spdm_get_certificate_request_t);

    spdm_request = encap_request;

    spdm_request->header.spdm_version = libspdm_get_connection_version (spdm_context);
    spdm_request->header.request_response_code = SPDM_GET_CERTIFICATE;
    spdm_request->header.param1 = spdm_context->encap_context.req_slot_id;
    spdm_request->header.param2 = 0;
    spdm_request->offset = (uint16_t)libspdm_get_managed_buffer_size(
        &spdm_context->encap_context.certificate_chain_buffer);
    spdm_request->length = LIBSPDM_MAX_CERT_CHAIN_BLOCK_LEN;
    LIBSPDM_DEBUG((LIBSPDM_DEBUG_INFO, "request (offset 0x%x, size 0x%x):\n",
                   spdm_request->offset, spdm_request->length));

    libspdm_reset_message_buffer_via_request_code(spdm_context, NULL,
                                                  spdm_request->header.request_response_code);


    /* Cache data*/

    status = libspdm_append_message_mut_b(spdm_context, spdm_request,
                                          *encap_request_size);
    if (LIBSPDM_STATUS_IS_ERROR(status)) {
        return LIBSPDM_STATUS_BUFFER_FULL;
    }

    libspdm_copy_mem(&spdm_context->encap_context.last_encap_request_header,
                     sizeof(spdm_context->encap_context.last_encap_request_header),
                     &spdm_request->header, sizeof(spdm_message_header_t));
    spdm_context->encap_context.last_encap_request_size =
        *encap_request_size;

    return LIBSPDM_STATUS_SUCCESS;
}

/**
 * Process the SPDM encapsulated CERTIFICATE response.
 *
 * @param  spdm_context                  A pointer to the SPDM context.
 * @param  encap_response_size            size in bytes of the encapsulated response data.
 * @param  encap_response                A pointer to the encapsulated response data.
 * @param  need_continue                     Indicate if encapsulated communication need continue.
 *
 * @retval RETURN_SUCCESS               The encapsulated response is processed.
 * @retval RETURN_BUFFER_TOO_SMALL      The buffer is too small to hold the data.
 * @retval RETURN_SECURITY_VIOLATION    Any verification fails.
 **/
libspdm_return_t libspdm_process_encap_response_certificate(
    libspdm_context_t *spdm_context, size_t encap_response_size,
    const void *encap_response, bool *need_continue)
{
    const spdm_certificate_response_t *spdm_response;
    size_t spdm_response_size;
    bool result;
    libspdm_return_t status;
    uint16_t request_offset;

    spdm_response = encap_response;
    spdm_response_size = encap_response_size;

    if (spdm_response_size < sizeof(spdm_message_header_t)) {
        return LIBSPDM_STATUS_INVALID_MSG_SIZE;
    }
    if (spdm_response->header.spdm_version != libspdm_get_connection_version (spdm_context)) {
        return LIBSPDM_STATUS_INVALID_MSG_FIELD;
    }
    if (spdm_response->header.request_response_code == SPDM_ERROR) {
        status = libspdm_handle_encap_error_response_main(
            spdm_context,
            spdm_response->header.param1);
        if (LIBSPDM_STATUS_IS_ERROR(status)) {
            return status;
        }
    } else if (spdm_response->header.request_response_code !=
               SPDM_CERTIFICATE) {
        return LIBSPDM_STATUS_INVALID_MSG_FIELD;
    }
    if (encap_response_size < sizeof(spdm_certificate_response_t)) {
        return LIBSPDM_STATUS_INVALID_MSG_SIZE;
    }
    if ((spdm_response->portion_length > LIBSPDM_MAX_CERT_CHAIN_BLOCK_LEN) ||
        (spdm_response->portion_length == 0)) {
        return LIBSPDM_STATUS_INVALID_MSG_FIELD;
    }
    request_offset = (uint16_t)libspdm_get_managed_buffer_size(
        &spdm_context->encap_context.certificate_chain_buffer);
    if (request_offset == 0) {
        spdm_context->encap_context.cert_chain_total_len = spdm_response->portion_length +
                                                           spdm_response->remainder_length;
    } else if (spdm_context->encap_context.cert_chain_total_len !=
               request_offset + spdm_response->portion_length + spdm_response->remainder_length) {
        return LIBSPDM_STATUS_INVALID_MSG_FIELD;
    }
    if (spdm_response->header.param1 !=
        spdm_context->encap_context.req_slot_id) {
        return LIBSPDM_STATUS_INVALID_MSG_FIELD;
    }
    if (spdm_response_size < sizeof(spdm_certificate_response_t) +
        spdm_response->portion_length) {
        return LIBSPDM_STATUS_INVALID_MSG_SIZE;
    }
    spdm_response_size = sizeof(spdm_certificate_response_t) +
                         spdm_response->portion_length;

    /* Cache data*/

    status = libspdm_append_message_mut_b(spdm_context, spdm_response,
                                          spdm_response_size);
    if (LIBSPDM_STATUS_IS_ERROR(status)) {
        return LIBSPDM_STATUS_BUFFER_FULL;
    }

    LIBSPDM_DEBUG((LIBSPDM_DEBUG_INFO, "Certificate (offset 0x%x, size 0x%x):\n",
                   libspdm_get_managed_buffer_size(
                       &spdm_context->encap_context.certificate_chain_buffer),
                   spdm_response->portion_length));
    libspdm_internal_dump_hex((void *)(spdm_response + 1),
                              spdm_response->portion_length);

    status = libspdm_append_managed_buffer(
        &spdm_context->encap_context.certificate_chain_buffer,
        (void *)(spdm_response + 1), spdm_response->portion_length);
    if (LIBSPDM_STATUS_IS_ERROR(status)) {
        return LIBSPDM_STATUS_BUFFER_FULL;
    }

    if (spdm_response->remainder_length != 0) {
        *need_continue = true;
        return LIBSPDM_STATUS_SUCCESS;
    }

    *need_continue = false;

    if (spdm_context->local_context.verify_peer_spdm_cert_chain != NULL) {
        result = spdm_context->local_context.verify_peer_spdm_cert_chain (
            spdm_context, spdm_context->encap_context.req_slot_id,
            libspdm_get_managed_buffer_size(
                &spdm_context->encap_context.certificate_chain_buffer),
            libspdm_get_managed_buffer(
                &spdm_context->encap_context.certificate_chain_buffer),
            NULL, NULL);
        if (!result) {
            return LIBSPDM_STATUS_VERIF_FAIL;
        }
    } else {
        result = libspdm_verify_peer_cert_chain_buffer(
            spdm_context,
            libspdm_get_managed_buffer(
                &spdm_context->encap_context.certificate_chain_buffer),
            libspdm_get_managed_buffer_size(
                &spdm_context->encap_context.certificate_chain_buffer),
            NULL, NULL, false);
        if (!result) {
            return LIBSPDM_STATUS_VERIF_FAIL;
        }
    }

#if LIBSPDM_RECORD_TRANSCRIPT_DATA_SUPPORT
    spdm_context->connection_info.peer_used_cert_chain_buffer_size =
        libspdm_get_managed_buffer_size(
            &spdm_context->encap_context.certificate_chain_buffer);
    libspdm_copy_mem(spdm_context->connection_info.peer_used_cert_chain_buffer,
                     sizeof(spdm_context->connection_info.peer_used_cert_chain_buffer),
                     libspdm_get_managed_buffer(
                         &spdm_context->encap_context.certificate_chain_buffer),
                     libspdm_get_managed_buffer_size(
                         &spdm_context->encap_context.certificate_chain_buffer));
#else
    result = libspdm_hash_all(
        spdm_context->connection_info.algorithm.base_hash_algo,
        libspdm_get_managed_buffer(
            &spdm_context->encap_context.certificate_chain_buffer),
        libspdm_get_managed_buffer_size(
            &spdm_context->encap_context.certificate_chain_buffer),
        spdm_context->connection_info.peer_used_cert_chain_buffer_hash);
    if (!result) {
        return LIBSPDM_STATUS_CRYPTO_ERROR;
    }
    spdm_context->connection_info.peer_used_cert_chain_buffer_hash_size =
        libspdm_get_hash_size(spdm_context->connection_info.algorithm.base_hash_algo);

    result = libspdm_get_leaf_cert_public_key_from_cert_chain(
        spdm_context->connection_info.algorithm.base_hash_algo,
        spdm_context->connection_info.algorithm.req_base_asym_alg,
        libspdm_get_managed_buffer(
            &spdm_context->encap_context.certificate_chain_buffer),
        libspdm_get_managed_buffer_size(
            &spdm_context->encap_context.certificate_chain_buffer),
        &spdm_context->connection_info.peer_used_leaf_cert_public_key);
    if (!result) {
        return LIBSPDM_STATUS_INVALID_CERT;
    }
#endif

    return LIBSPDM_STATUS_SUCCESS;
}

#endif /* LIBSPDM_ENABLE_CAPABILITY_CERT_CAP*/
