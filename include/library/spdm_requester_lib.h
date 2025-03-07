/**
 *  Copyright Notice:
 *  Copyright 2021-2022 DMTF. All rights reserved.
 *  License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/libspdm/blob/main/LICENSE.md
 **/

#ifndef __SPDM_REQUESTER_LIB_H__
#define __SPDM_REQUESTER_LIB_H__

#include "library/spdm_common_lib.h"

/**
 * Send an SPDM or an APP request to a device.
 *
 * @param  spdm_context                  The SPDM context for the device.
 * @param  session_id                    Indicate if the request is a secured message.
 *                                     If session_id is NULL, it is a normal message.
 *                                     If session_id is NOT NULL, it is a secured message.
 * @param  is_app_message                 Indicates if it is an APP message or SPDM message.
 * @param  request_size                  size in bytes of the request data buffer.
 * @param  request                      A pointer to a destination buffer to store the request.
 *                                     The caller is responsible for having
 *                                     either implicit or explicit ownership of the buffer.
 *                                      For normal message, requester pointer point to transport_message + transport header size
 *                                      For secured message, requester pointer will point to the scratch buffer + transport header size in spdm_context.
 *
 * @retval RETURN_SUCCESS               The SPDM request is sent successfully.
 * @retval RETURN_DEVICE_ERROR          A device error occurs when the SPDM request is sent to the device.
 **/
libspdm_return_t libspdm_send_request(void *spdm_context, const uint32_t *session_id,
                                      bool is_app_message,
                                      size_t request_size, const void *request);

/**
 * Receive an SPDM or an APP response from a device.
 *
 * @param  spdm_context                  The SPDM context for the device.
 * @param  session_id                    Indicate if the response is a secured message.
 *                                     If session_id is NULL, it is a normal message.
 *                                     If session_id is NOT NULL, it is a secured message.
 * @param  is_app_message                 Indicates if it is an APP message or SPDM message.
 * @param  response_size                 size in bytes of the response data buffer.
 * @param  response                     A pointer to a destination buffer to store the response.
 *                                     The caller is responsible for having
 *                                     either implicit or explicit ownership of the buffer.
 *                                      For normal message, response pointer still point to original transport_message.
 *                                      For secured message, response pointer will point to the scratch buffer in spdm_context.
 *
 * @retval RETURN_SUCCESS               The SPDM response is received successfully.
 * @retval RETURN_DEVICE_ERROR          A device error occurs when the SPDM response is received from the device.
 **/
libspdm_return_t libspdm_receive_response(void *spdm_context,
                                          const uint32_t *session_id,
                                          bool is_app_message,
                                          size_t *response_size,
                                          void **response);

/**
 * This function sends GET_VERSION, GET_CAPABILITIES, NEGOTIATE_ALGORITHMS
 * to initialize the connection with SPDM responder.
 *
 * Before this function, the requester configuration data can be set via libspdm_set_data.
 * After this function, the negotiated configuration data can be got via libspdm_get_data.
 *
 * @param  spdm_context                  A pointer to the SPDM context.
 * @param  get_version_only               If the requester sends GET_VERSION only or not.
 *
 * @retval RETURN_SUCCESS               The connection is initialized successfully.
 * @retval RETURN_DEVICE_ERROR          A device error occurs when communicates with the device.
 **/
libspdm_return_t libspdm_init_connection(void *spdm_context,
                                         bool get_version_only);

/**
 * This function sends GET_DIGEST
 * to get all digest of the certificate chains from device.
 *
 * If the peer certificate chain is deployed,
 * this function also verifies the digest with the certificate chain.
 *
 * TotalDigestSize = sizeof(digest) * count in slot_mask
 *
 * @param  spdm_context                  A pointer to the SPDM context.
 * @param  slot_mask                     The slots which deploy the CertificateChain.
 * @param  total_digest_buffer            A pointer to a destination buffer to store the digest buffer.
 *
 * @retval RETURN_SUCCESS               The digests are got successfully.
 * @retval RETURN_DEVICE_ERROR          A device error occurs when communicates with the device.
 * @retval RETURN_SECURITY_VIOLATION    Any verification fails.
 **/
libspdm_return_t libspdm_get_digest(void *spdm_context, uint8_t *slot_mask,
                                    void *total_digest_buffer);

/**
 * This function sends GET_CERTIFICATE
 * to get certificate chain in one slot from device.
 *
 * This function verify the integrity of the certificate chain.
 * root_hash -> Root certificate -> Intermediate certificate -> Leaf certificate.
 *
 * If the peer root certificate hash is deployed,
 * this function also verifies the digest with the root hash in the certificate chain.
 *
 * @param  spdm_context                  A pointer to the SPDM context.
 * @param  slot_id                      The number of slot for the certificate chain.
 * @param  cert_chain_size                On input, indicate the size in bytes of the destination buffer to store the digest buffer.
 *                                     On output, indicate the size in bytes of the certificate chain.
 * @param  cert_chain                    A pointer to a destination buffer to store the certificate chain.
 *
 * @retval RETURN_SUCCESS               The certificate chain is got successfully.
 * @retval RETURN_DEVICE_ERROR          A device error occurs when communicates with the device.
 * @retval RETURN_SECURITY_VIOLATION    Any verification fails.
 **/
libspdm_return_t libspdm_get_certificate(void *spdm_context, uint8_t slot_id,
                                         size_t *cert_chain_size,
                                         void *cert_chain);

/**
 * This function sends GET_CERTIFICATE
 * to get certificate chain in one slot from device.
 *
 * This function verify the integrity of the certificate chain.
 * root_hash -> Root certificate -> Intermediate certificate -> Leaf certificate.
 *
 * If the peer root certificate hash is deployed,
 * this function also verifies the digest with the root hash in the certificate chain.
 *
 * @param  spdm_context                  A pointer to the SPDM context.
 * @param  slot_id                      The number of slot for the certificate chain.
 * @param  cert_chain_size                On input, indicate the size in bytes of the destination buffer to store the digest buffer.
 *                                     On output, indicate the size in bytes of the certificate chain.
 * @param  cert_chain                    A pointer to a destination buffer to store the certificate chain.
 * @param  trust_anchor                  A buffer to hold the trust_anchor which is used to validate the peer certificate, if not NULL.
 * @param  trust_anchor_size             A buffer to hold the trust_anchor_size, if not NULL.
 *
 * @retval RETURN_SUCCESS               The certificate chain is got successfully.
 * @retval RETURN_DEVICE_ERROR          A device error occurs when communicates with the device.
 * @retval RETURN_SECURITY_VIOLATION    Any verification fails.
 **/
libspdm_return_t libspdm_get_certificate_ex(void *context, uint8_t slot_id,
                                            size_t *cert_chain_size,
                                            void *cert_chain,
                                            void **trust_anchor,
                                            size_t *trust_anchor_size);

/**
 * This function sends GET_CERTIFICATE
 * to get certificate chain in one slot from device.
 *
 * This function verify the integrity of the certificate chain.
 * root_hash -> Root certificate -> Intermediate certificate -> Leaf certificate.
 *
 * If the peer root certificate hash is deployed,
 * this function also verifies the digest with the root hash in the certificate chain.
 *
 * @param  spdm_context                  A pointer to the SPDM context.
 * @param  slot_id                      The number of slot for the certificate chain.
 * @param  length                       LIBSPDM_MAX_CERT_CHAIN_BLOCK_LEN.
 * @param  cert_chain_size                On input, indicate the size in bytes of the destination buffer to store the digest buffer.
 *                                     On output, indicate the size in bytes of the certificate chain.
 * @param  cert_chain                    A pointer to a destination buffer to store the certificate chain.
 *
 * @retval RETURN_SUCCESS               The certificate chain is got successfully.
 * @retval RETURN_DEVICE_ERROR          A device error occurs when communicates with the device.
 * @retval RETURN_SECURITY_VIOLATION    Any verification fails.
 **/
libspdm_return_t libspdm_get_certificate_choose_length(void *spdm_context,
                                                       uint8_t slot_id,
                                                       uint16_t length,
                                                       size_t *cert_chain_size,
                                                       void *cert_chain);

/**
 * This function sends GET_CERTIFICATE
 * to get certificate chain in one slot from device.
 *
 * This function verify the integrity of the certificate chain.
 * root_hash -> Root certificate -> Intermediate certificate -> Leaf certificate.
 *
 * If the peer root certificate hash is deployed,
 * this function also verifies the digest with the root hash in the certificate chain.
 *
 * @param  spdm_context                  A pointer to the SPDM context.
 * @param  slot_id                      The number of slot for the certificate chain.
 * @param  length                       LIBSPDM_MAX_CERT_CHAIN_BLOCK_LEN.
 * @param  cert_chain_size                On input, indicate the size in bytes of the destination buffer to store the digest buffer.
 *                                     On output, indicate the size in bytes of the certificate chain.
 * @param  cert_chain                    A pointer to a destination buffer to store the certificate chain.
 * @param  trust_anchor                  A buffer to hold the trust_anchor which is used to validate the peer certificate, if not NULL.
 * @param  trust_anchor_size             A buffer to hold the trust_anchor_size, if not NULL.
 *
 * @retval RETURN_SUCCESS               The certificate chain is got successfully.
 * @retval RETURN_DEVICE_ERROR          A device error occurs when communicates with the device.
 * @retval RETURN_SECURITY_VIOLATION    Any verification fails.
 **/
libspdm_return_t libspdm_get_certificate_choose_length_ex(void *context,
                                                          uint8_t slot_id,
                                                          uint16_t length,
                                                          size_t *cert_chain_size,
                                                          void *cert_chain,
                                                          void **trust_anchor,
                                                          size_t *trust_anchor_size);

/**
 * This function sends CHALLENGE
 * to authenticate the device based upon the key in one slot.
 *
 * This function verifies the signature in the challenge auth.
 *
 * If basic mutual authentication is requested from the responder,
 * this function also perform the basic mutual authentication.
 *
 * @param  spdm_context                  A pointer to the SPDM context.
 * @param  slot_id                      The number of slot for the challenge.
 * @param  measurement_hash_type          The type of the measurement hash.
 * @param  measurement_hash              A pointer to a destination buffer to store the measurement hash.
 * @param  slot_mask                     A pointer to a destination to store the slot mask.
 *
 * @retval RETURN_SUCCESS               The challenge auth is got successfully.
 * @retval RETURN_DEVICE_ERROR          A device error occurs when communicates with the device.
 * @retval RETURN_SECURITY_VIOLATION    Any verification fails.
 **/
libspdm_return_t libspdm_challenge(void *spdm_context, uint8_t slot_id,
                                   uint8_t measurement_hash_type,
                                   void *measurement_hash,
                                   uint8_t *slot_mask);

/**
 * This function sends CHALLENGE
 * to authenticate the device based upon the key in one slot.
 *
 * This function verifies the signature in the challenge auth.
 *
 * If basic mutual authentication is requested from the responder,
 * this function also perform the basic mutual authentication.
 *
 * @param  spdm_context                  A pointer to the SPDM context.
 * @param  slot_id                      The number of slot for the challenge.
 * @param  measurement_hash_type          The type of the measurement hash.
 * @param  measurement_hash              A pointer to a destination buffer to store the measurement hash.
 * @param  slot_mask                     A pointer to a destination to store the slot mask.
 * @param  requester_nonce_in            A buffer to hold the requester nonce (32 bytes) as input, if not NULL.
 * @param  requester_nonce               A buffer to hold the requester nonce (32 bytes), if not NULL.
 * @param  responder_nonce               A buffer to hold the responder nonce (32 bytes), if not NULL.
 *
 * @retval RETURN_SUCCESS               The challenge auth is got successfully.
 * @retval RETURN_DEVICE_ERROR          A device error occurs when communicates with the device.
 * @retval RETURN_SECURITY_VIOLATION    Any verification fails.
 **/
libspdm_return_t libspdm_challenge_ex(void *context, uint8_t slot_id,
                                      uint8_t measurement_hash_type,
                                      void *measurement_hash,
                                      uint8_t *slot_mask,
                                      const void *requester_nonce_in,
                                      void *requester_nonce,
                                      void *responder_nonce);

/**
 * This function sends GET_MEASUREMENT
 * to get measurement from the device.
 *
 * If the signature is requested, this function verifies the signature of the measurement.
 *
 * @param  spdm_context                  A pointer to the SPDM context.
 * @param  session_id                    Indicates if it is a secured message protected via SPDM session.
 *                                     If session_id is NULL, it is a normal message.
 *                                     If session_id is NOT NULL, it is a secured message.
 * @param  request_attribute             The request attribute of the request message.
 * @param  measurement_operation         The measurement operation of the request message.
 * @param  slot_id                      The number of slot for the certificate chain.
 * @param  content_changed               The measurement content changed output param.
 * @param  number_of_blocks               The number of blocks of the measurement record.
 * @param  measurement_record_length      On input, indicate the size in bytes of the destination buffer to store the measurement record.
 *                                     On output, indicate the size in bytes of the measurement record.
 * @param  measurement_record            A pointer to a destination buffer to store the measurement record.
 *
 * @retval RETURN_SUCCESS               The measurement is got successfully.
 * @retval RETURN_DEVICE_ERROR          A device error occurs when communicates with the device.
 * @retval RETURN_SECURITY_VIOLATION    Any verification fails.
 **/
libspdm_return_t libspdm_get_measurement(void *spdm_context, const uint32_t *session_id,
                                         uint8_t request_attribute,
                                         uint8_t measurement_operation,
                                         uint8_t slot_id,
                                         uint8_t *content_changed,
                                         uint8_t *number_of_blocks,
                                         uint32_t *measurement_record_length,
                                         void *measurement_record);

/**
 * This function sends GET_MEASUREMENT
 * to get measurement from the device.
 *
 * If the signature is requested, this function verifies the signature of the measurement.
 *
 * @param  spdm_context                  A pointer to the SPDM context.
 * @param  session_id                    Indicates if it is a secured message protected via SPDM session.
 *                                     If session_id is NULL, it is a normal message.
 *                                     If session_id is NOT NULL, it is a secured message.
 * @param  request_attribute             The request attribute of the request message.
 * @param  measurement_operation         The measurement operation of the request message.
 * @param  slot_id                      The number of slot for the certificate chain.
 * @param  content_changed               The measurement content changed output param.
 * @param  number_of_blocks               The number of blocks of the measurement record.
 * @param  measurement_record_length      On input, indicate the size in bytes of the destination buffer to store the measurement record.
 *                                     On output, indicate the size in bytes of the measurement record.
 * @param  measurement_record            A pointer to a destination buffer to store the measurement record.
 * @param  requester_nonce_in            A buffer to hold the requester nonce (32 bytes) as input, if not NULL.
 * @param  requester_nonce               A buffer to hold the requester nonce (32 bytes), if not NULL.
 * @param  responder_nonce               A buffer to hold the responder nonce (32 bytes), if not NULL.
 *
 * @retval RETURN_SUCCESS               The measurement is got successfully.
 * @retval RETURN_DEVICE_ERROR          A device error occurs when communicates with the device.
 * @retval RETURN_SECURITY_VIOLATION    Any verification fails.
 **/
libspdm_return_t libspdm_get_measurement_ex(void *context, const uint32_t *session_id,
                                            uint8_t request_attribute,
                                            uint8_t measurement_operation,
                                            uint8_t slot_id,
                                            uint8_t *content_changed,
                                            uint8_t *number_of_blocks,
                                            uint32_t *measurement_record_length,
                                            void *measurement_record,
                                            const void *requester_nonce_in,
                                            void *requester_nonce,
                                            void *responder_nonce);

/**
 * This function sends KEY_EXCHANGE/FINISH or PSK_EXCHANGE/PSK_FINISH
 * to start an SPDM Session.
 *
 * If encapsulated mutual authentication is requested from the responder,
 * this function also perform the encapsulated mutual authentication.
 *
 * @param  spdm_context                  A pointer to the SPDM context.
 * @param  use_psk                       false means to use KEY_EXCHANGE/FINISH to start a session.
 *                                     true means to use PSK_EXCHANGE/PSK_FINISH to start a session.
 * @param  measurement_hash_type          The type of the measurement hash.
 * @param  slot_id                      The number of slot for the certificate chain.
 * @param  session_policy               The policy for the session.
 * @param  session_id                    The session ID of the session.
 * @param  heartbeat_period              The heartbeat period for the session.
 * @param  measurement_hash              A pointer to a destination buffer to store the measurement hash.
 *
 * @retval RETURN_SUCCESS               The SPDM session is started.
 * @retval RETURN_DEVICE_ERROR          A device error occurs when communicates with the device.
 * @retval RETURN_SECURITY_VIOLATION    Any verification fails.
 **/
libspdm_return_t libspdm_start_session(void *spdm_context, bool use_psk,
                                       uint8_t measurement_hash_type,
                                       uint8_t slot_id,
                                       uint8_t session_policy,
                                       uint32_t *session_id,
                                       uint8_t *heartbeat_period,
                                       void *measurement_hash);

/**
 * This function sends KEY_EXCHANGE/FINISH or PSK_EXCHANGE/PSK_FINISH
 * to start an SPDM Session.
 *
 * If encapsulated mutual authentication is requested from the responder,
 * this function also perform the encapsulated mutual authentication.
 *
 * @param  spdm_context                  A pointer to the SPDM context.
 * @param  use_psk                       false means to use KEY_EXCHANGE/FINISH to start a session.
 *                                     true means to use PSK_EXCHANGE/PSK_FINISH to start a session.
 * @param  measurement_hash_type          The type of the measurement hash.
 * @param  slot_id                      The number of slot for the certificate chain.
 * @param  session_policy               The policy for the session.
 * @param  session_id                    The session ID of the session.
 * @param  heartbeat_period              The heartbeat period for the session.
 * @param  measurement_hash              A pointer to a destination buffer to store the measurement hash.
 * @param  requester_random_in           A buffer to hold the requester random as input, if not NULL.
 * @param  requester_random_in_size      The size of requester_random_in.
 *                                      If use_psk is false, it must be 32 bytes.
 *                                      If use_psk is true, it means the PSK context and must be 32 bytes at least,
 *                                      but not exceed LIBSPDM_PSK_CONTEXT_LENGTH.
 * @param  requester_random              A buffer to hold the requester random, if not NULL.
 * @param  requester_random_size         On input, the size of requester_random buffer.
 *                                      On output, the size of data returned in requester_random buffer.
 *                                      If use_psk is false, it must be 32 bytes.
 *                                      If use_psk is true, it means the PSK context and must be 32 bytes at least.
 * @param  responder_random              A buffer to hold the responder random, if not NULL.
 * @param  responder_random_size         On input, the size of requester_random buffer.
 *                                      On output, the size of data returned in requester_random buffer.
 *                                      If use_psk is false, it must be 32 bytes.
 *                                      If use_psk is true, it means the PSK context. It could be 0 if device does not support context.
 *
 * @retval RETURN_SUCCESS               The SPDM session is started.
 * @retval RETURN_DEVICE_ERROR          A device error occurs when communicates with the device.
 * @retval RETURN_SECURITY_VIOLATION    Any verification fails.
 **/
libspdm_return_t libspdm_start_session_ex(void *spdm_context, bool use_psk,
                                          uint8_t measurement_hash_type,
                                          uint8_t slot_id,
                                          uint8_t session_policy,
                                          uint32_t *session_id,
                                          uint8_t *heartbeat_period,
                                          void *measurement_hash,
                                          const void *requester_random_in,
                                          size_t requester_random_in_size,
                                          void *requester_random,
                                          size_t *requester_random_size,
                                          void *responder_random,
                                          size_t *responder_random_size);

/**
 * This function sends END_SESSION
 * to stop an SPDM Session.
 *
 * @param  spdm_context                  A pointer to the SPDM context.
 * @param  session_id                    The session ID of the session.
 * @param  end_session_attributes         The end session attribute for the session.
 *
 * @retval RETURN_SUCCESS               The SPDM session is stopped.
 * @retval RETURN_DEVICE_ERROR          A device error occurs when communicates with the device.
 * @retval RETURN_SECURITY_VIOLATION    Any verification fails.
 **/
libspdm_return_t libspdm_stop_session(void *spdm_context, uint32_t session_id,
                                      uint8_t end_session_attributes);

/**
 * Send and receive an SPDM or APP message.
 *
 * The SPDM message can be a normal message or a secured message in SPDM session.
 *
 * The APP message is encoded to a secured message directly in SPDM session.
 * The APP message format is defined by the transport layer.
 * Take MCTP as example: APP message == MCTP header (MCTP_MESSAGE_TYPE_SPDM) + SPDM message
 *
 * @param  spdm_context                  A pointer to the SPDM context.
 * @param  session_id                    Indicates if it is a secured message protected via SPDM session.
 *                                     If session_id is NULL, it is a normal message.
 *                                     If session_id is NOT NULL, it is a secured message.
 * @param  is_app_message                 Indicates if it is an APP message or SPDM message.
 * @param  request                      A pointer to the request data.
 * @param  request_size                  size in bytes of the request data.
 * @param  response                     A pointer to the response data.
 * @param  response_size                 size in bytes of the response data.
 *                                     On input, it means the size in bytes of response data buffer.
 *                                     On output, it means the size in bytes of copied response data buffer if RETURN_SUCCESS is returned,
 *                                     and means the size in bytes of desired response data buffer if RETURN_BUFFER_TOO_SMALL is returned.
 *
 * @retval RETURN_SUCCESS               The SPDM request is set successfully.
 * @retval RETURN_BUFFER_TOO_SMALL      The buffer is too small to hold the data.
 * @retval RETURN_DEVICE_ERROR          A device error occurs when communicates with the device.
 * @retval RETURN_SECURITY_VIOLATION    Any verification fails.
 **/
libspdm_return_t libspdm_send_receive_data(void *spdm_context,
                                           const uint32_t *session_id,
                                           bool is_app_message,
                                           const void *request, size_t request_size,
                                           void *response,
                                           size_t *response_size);

/**
 * This function sends HEARTBEAT
 * to an SPDM Session.
 *
 * @param  spdm_context                  A pointer to the SPDM context.
 * @param  session_id                    The session ID of the session.
 *
 * @retval RETURN_SUCCESS               The heartbeat is sent and received.
 * @retval RETURN_DEVICE_ERROR          A device error occurs when communicates with the device.
 * @retval RETURN_SECURITY_VIOLATION    Any verification fails.
 **/
libspdm_return_t libspdm_heartbeat(void *spdm_context, uint32_t session_id);

/**
 * This function sends KEY_UPDATE
 * to update keys for an SPDM Session.
 *
 * After keys are updated, this function also uses VERIFY_NEW_KEY to verify the key.
 *
 * @param  spdm_context                  A pointer to the SPDM context.
 * @param  session_id                    The session ID of the session.
 * @param  single_direction              true means the operation is UPDATE_KEY.
 *                                     false means the operation is UPDATE_ALL_KEYS.
 *
 * @retval RETURN_SUCCESS               The keys of the session are updated.
 * @retval RETURN_DEVICE_ERROR          A device error occurs when communicates with the device.
 * @retval RETURN_SECURITY_VIOLATION    Any verification fails.
 **/
libspdm_return_t libspdm_key_update(void *spdm_context, uint32_t session_id,
                                    bool single_direction);

/**
 * This function executes a series of SPDM encapsulated requests and receives SPDM encapsulated responses.
 *
 * This function starts with the first encapsulated request (such as GET_ENCAPSULATED_REQUEST)
 * and ends with last encapsulated response (such as RESPONSE_PAYLOAD_TYPE_ABSENT or RESPONSE_PAYLOAD_TYPE_SLOT_NUMBER).
 *
 * @param  spdm_context                  A pointer to the SPDM context.
 * @param  session_id                    Indicate if the encapsulated request is a secured message.
 *                                     If session_id is NULL, it is a normal message.
 *                                     If session_id is NOT NULL, it is a secured message.
 *
 * @retval RETURN_SUCCESS               The SPDM Encapsulated requests are sent and the responses are received.
 * @retval RETURN_DEVICE_ERROR          A device error occurs when communicates with the device.
 **/
libspdm_return_t libspdm_send_receive_encap_request(void *spdm_context,
                                                    const uint32_t *session_id);

/**
 * Process the encapsulated request and return the encapsulated response.
 *
 * @param  spdm_context                  A pointer to the SPDM context.
 * @param  spdm_request_size              size in bytes of the request data.
 * @param  spdm_request                  A pointer to the request data.
 * @param  spdm_response_size             size in bytes of the response data.
 *                                     On input, it means the size in bytes of response data buffer.
 *                                     On output, it means the size in bytes of copied response data buffer if RETURN_SUCCESS is returned,
 *                                     and means the size in bytes of desired response data buffer if RETURN_BUFFER_TOO_SMALL is returned.
 * @param  spdm_response                 A pointer to the response data.
 *
 * @retval RETURN_SUCCESS               The request is processed and the response is returned.
 * @retval RETURN_BUFFER_TOO_SMALL      The buffer is too small to hold the data.
 * @retval RETURN_DEVICE_ERROR          A device error occurs when communicates with the device.
 * @retval RETURN_SECURITY_VIOLATION    Any verification fails.
 **/
typedef libspdm_return_t (*libspdm_get_encap_response_func)(
    void *spdm_context, size_t spdm_request_size,
    void *spdm_request, size_t *spdm_response_size,
    void *spdm_response);

/**
 * Register an SPDM encapsulated message process function.
 *
 * If the default encapsulated message process function cannot handle the encapsulated message,
 * this function will be invoked.
 *
 * @param  spdm_context                  A pointer to the SPDM context.
 * @param  get_encap_response_func         The function to process the encapsuled message.
 **/
void libspdm_register_get_encap_response_func(void *spdm_context,
                                              const libspdm_get_encap_response_func
                                              get_encap_response_func);

/**
 * Generate encapsulated ERROR message.
 *
 * This function can be called in libspdm_get_encap_response_func.
 *
 * @param  spdm_context                  A pointer to the SPDM context.
 * @param  error_code                    The error code of the message.
 * @param  error_data                    The error data of the message.
 * @param  spdm_response_size             size in bytes of the response data.
 *                                     On input, it means the size in bytes of data buffer.
 *                                     On output, it means the size in bytes of copied data buffer if RETURN_SUCCESS is returned,
 *                                     and means the size in bytes of desired data buffer if RETURN_BUFFER_TOO_SMALL is returned.
 * @param  spdm_response                 A pointer to the response data.
 *
 * @retval RETURN_SUCCESS               The error message is generated.
 * @retval RETURN_BUFFER_TOO_SMALL      The buffer is too small to hold the data.
 **/
libspdm_return_t libspdm_generate_encap_error_response(
    const void *spdm_context, uint8_t error_code, uint8_t error_data,
    size_t *spdm_response_size, void *spdm_response);

/**
 * Generate encapsulated ERROR message with extended error data.
 *
 * This function can be called in libspdm_get_encap_response_func.
 *
 * @param  spdm_context                  A pointer to the SPDM context.
 * @param  error_code                    The error code of the message.
 * @param  error_data                    The error data of the message.
 * @param  extended_error_data_size        The size in bytes of the extended error data.
 * @param  extended_error_data            A pointer to the extended error data.
 * @param  spdm_response_size             size in bytes of the response data.
 *                                     On input, it means the size in bytes of response data buffer.
 *                                     On output, it means the size in bytes of copied response data buffer if RETURN_SUCCESS is returned,
 *                                     and means the size in bytes of desired response data buffer if RETURN_BUFFER_TOO_SMALL is returned.
 * @param  spdm_response                 A pointer to the response data.
 *
 * @retval RETURN_SUCCESS               The error message is generated.
 * @retval RETURN_BUFFER_TOO_SMALL      The buffer is too small to hold the data.
 **/
libspdm_return_t libspdm_generate_encap_extended_error_response(
    const void *spdm_context, uint8_t error_code, uint8_t error_data,
    size_t extended_error_data_size, const uint8_t *extended_error_data,
    size_t *spdm_response_size, void *spdm_response);

#endif
