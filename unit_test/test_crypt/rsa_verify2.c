/**
 *  Copyright Notice:
 *  Copyright 2021-2022 DMTF. All rights reserved.
 *  License: BSD 3-Clause License. For full text see link: https://github.com/DMTF/libspdm/blob/main/LICENSE.md
 **/

#include "test_crypt.h"


/* X509 cert data for RSA public key Retrieving and X509 Verification (Generated by OpenSSL utility).*/

LIBSPDM_GLOBAL_REMOVE_IF_UNREFERENCED uint8_t m_libspdm_test_cert[] = {
    0x30, 0x82, 0x04, 0x96, 0x30, 0x82, 0x02, 0x7E, 0xA0, 0x03, 0x02, 0x01,
    0x02, 0x02, 0x01, 0x01, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86,
    0xF7, 0x0D, 0x01, 0x01, 0x0B, 0x05, 0x00, 0x30, 0x17, 0x31, 0x15, 0x30,
    0x13, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0C, 0x0C, 0x69, 0x6E, 0x74, 0x65,
    0x6C, 0x20, 0x52, 0x53, 0x41, 0x20, 0x43, 0x41, 0x30, 0x1E, 0x17, 0x0D,
    0x32, 0x30, 0x30, 0x38, 0x32, 0x34, 0x30, 0x36, 0x31, 0x38, 0x32, 0x33,
    0x5A, 0x17, 0x0D, 0x33, 0x30, 0x30, 0x38, 0x32, 0x32, 0x30, 0x36, 0x31,
    0x38, 0x32, 0x33, 0x5A, 0x30, 0x26, 0x31, 0x24, 0x30, 0x22, 0x06, 0x03,
    0x55, 0x04, 0x03, 0x0C, 0x1B, 0x69, 0x6E, 0x74, 0x65, 0x6C, 0x20, 0x52,
    0x53, 0x41, 0x20, 0x69, 0x6E, 0x74, 0x65, 0x72, 0x6D, 0x65, 0x64, 0x69,
    0x61, 0x74, 0x65, 0x20, 0x63, 0x65, 0x72, 0x74, 0x30, 0x82, 0x01, 0xA2,
    0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01,
    0x01, 0x05, 0x00, 0x03, 0x82, 0x01, 0x8F, 0x00, 0x30, 0x82, 0x01, 0x8A,
    0x02, 0x82, 0x01, 0x81, 0x00, 0xC9, 0x7D, 0x37, 0xF1, 0x14, 0xF6, 0xEE,
    0x44, 0xE7, 0xD2, 0x5B, 0xAC, 0xA8, 0xAB, 0xF9, 0x15, 0xEE, 0x64, 0x72,
    0x43, 0x6C, 0x31, 0xF1, 0x5D, 0xBD, 0xC8, 0x84, 0xF7, 0x38, 0x5D, 0x46,
    0xE0, 0xDA, 0x86, 0xA7, 0x70, 0x48, 0x70, 0x56, 0x85, 0xD2, 0xC7, 0xD6,
    0xDF, 0x21, 0x90, 0xD0, 0x4A, 0xD5, 0xC5, 0x5C, 0x5B, 0x53, 0x49, 0x71,
    0x82, 0x04, 0x35, 0x5F, 0x2C, 0xDB, 0xAB, 0x35, 0xBC, 0x62, 0x7C, 0x33,
    0xD1, 0x99, 0xBC, 0x40, 0xAF, 0x11, 0xDB, 0xDA, 0x55, 0xB0, 0x7B, 0x02,
    0x14, 0x40, 0x8E, 0x55, 0x89, 0x96, 0xAF, 0x04, 0x75, 0x5E, 0xBC, 0x75,
    0x2A, 0xFC, 0x2C, 0xCE, 0x6B, 0x69, 0x49, 0x2B, 0xF3, 0xC5, 0x71, 0x44,
    0xD1, 0x12, 0x54, 0xA7, 0xCD, 0x8C, 0x26, 0x84, 0x4E, 0x64, 0x72, 0xE1,
    0x17, 0x74, 0x4D, 0x58, 0x9F, 0x93, 0x52, 0x47, 0x87, 0xE0, 0x4C, 0xAD,
    0x3C, 0xAD, 0x62, 0xCB, 0x71, 0x05, 0x92, 0x68, 0xFD, 0x64, 0x43, 0xD4,
    0xB0, 0x86, 0x30, 0xC0, 0xB4, 0xD0, 0x42, 0x96, 0x1E, 0x80, 0x05, 0x5B,
    0xE3, 0x2C, 0xB6, 0xA1, 0xD2, 0xDF, 0x76, 0x7C, 0xAA, 0xFA, 0xC3, 0xDC,
    0x43, 0x1A, 0x57, 0xC8, 0x6D, 0x5D, 0xB9, 0xDF, 0x0E, 0x67, 0xD3, 0x58,
    0x6B, 0x02, 0x8D, 0x84, 0xCD, 0x31, 0xBD, 0xAD, 0x9D, 0x00, 0x45, 0x81,
    0x44, 0xDA, 0xCF, 0x8E, 0xFD, 0xE2, 0x09, 0xA2, 0x68, 0x5A, 0x97, 0x37,
    0x61, 0x05, 0x28, 0x93, 0x77, 0xD5, 0xAB, 0x08, 0x70, 0x09, 0x68, 0x6E,
    0x94, 0x4E, 0x31, 0x5E, 0x56, 0xF3, 0x0F, 0x29, 0x8C, 0x32, 0x3E, 0x43,
    0xA1, 0xB1, 0x98, 0x6B, 0x89, 0xDB, 0xFF, 0xC8, 0x51, 0x26, 0xAD, 0xCB,
    0xA5, 0xFC, 0xF4, 0xE8, 0x5F, 0xA1, 0xCD, 0x2A, 0x3F, 0xDC, 0x64, 0x95,
    0x82, 0x27, 0x6A, 0x8B, 0x3F, 0x0A, 0x4A, 0xEF, 0x26, 0xF7, 0x0D, 0x42,
    0xA2, 0x1A, 0xEE, 0xAD, 0x7C, 0xB0, 0xC3, 0x51, 0x61, 0x73, 0x69, 0xFA,
    0x70, 0xED, 0xD3, 0x04, 0x91, 0xDE, 0x3E, 0x07, 0xCF, 0xC9, 0x38, 0xBD,
    0xF3, 0xE6, 0x66, 0x73, 0x91, 0x22, 0x91, 0x73, 0x84, 0xFE, 0xF2, 0x29,
    0xBA, 0xE5, 0x78, 0x5D, 0xD7, 0x40, 0x47, 0x78, 0x4F, 0x73, 0xB9, 0xE0,
    0xB5, 0x8A, 0x64, 0x5B, 0xA0, 0xD6, 0x32, 0x54, 0x25, 0xF5, 0x5A, 0x86,
    0xEE, 0xBA, 0x0C, 0x29, 0xA9, 0x2A, 0xAB, 0x05, 0x51, 0x79, 0x31, 0xEF,
    0x67, 0x43, 0x21, 0xD8, 0x51, 0x22, 0x60, 0x6B, 0xDD, 0x26, 0x6E, 0xEA,
    0x6E, 0x5A, 0x0F, 0x0F, 0x9F, 0x97, 0x90, 0x8D, 0xB6, 0xFE, 0xDB, 0xA6,
    0xFE, 0xE4, 0xCD, 0xE7, 0x6E, 0x8E, 0x12, 0x94, 0xB9, 0x36, 0x91, 0xFA,
    0xD2, 0x79, 0x09, 0x5D, 0x07, 0xB2, 0x85, 0x23, 0x32, 0xF7, 0x68, 0x98,
    0xFB, 0x9D, 0xFF, 0x01, 0xA9, 0x02, 0x03, 0x01, 0x00, 0x01, 0xA3, 0x5E,
    0x30, 0x5C, 0x30, 0x0C, 0x06, 0x03, 0x55, 0x1D, 0x13, 0x04, 0x05, 0x30,
    0x03, 0x01, 0x01, 0xFF, 0x30, 0x0B, 0x06, 0x03, 0x55, 0x1D, 0x0F, 0x04,
    0x04, 0x03, 0x02, 0x01, 0xFE, 0x30, 0x1D, 0x06, 0x03, 0x55, 0x1D, 0x0E,
    0x04, 0x16, 0x04, 0x14, 0x3E, 0x12, 0xBF, 0xAC, 0x0E, 0x27, 0xD4, 0x07,
    0x83, 0x81, 0xB6, 0x42, 0xD9, 0xC0, 0xE5, 0xF1, 0x32, 0xF7, 0xA3, 0x2B,
    0x30, 0x20, 0x06, 0x03, 0x55, 0x1D, 0x25, 0x01, 0x01, 0xFF, 0x04, 0x16,
    0x30, 0x14, 0x06, 0x08, 0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x03, 0x01,
    0x06, 0x08, 0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x03, 0x02, 0x30, 0x0D,
    0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B, 0x05,
    0x00, 0x03, 0x82, 0x02, 0x01, 0x00, 0x93, 0x02, 0xC1, 0xB8, 0x5E, 0xA0,
    0xDA, 0x59, 0xB6, 0x3A, 0x36, 0x21, 0x94, 0xA0, 0xAA, 0x82, 0x00, 0x74,
    0x5E, 0xEB, 0x8F, 0x3C, 0x2E, 0xD5, 0x4E, 0xB8, 0x3E, 0xA5, 0xD6, 0x2B,
    0x1F, 0x90, 0x77, 0xE8, 0x8B, 0xA4, 0xB0, 0xE2, 0x9F, 0x8A, 0x47, 0x62,
    0xEE, 0x7D, 0xC1, 0x50, 0x97, 0xC1, 0xB8, 0x78, 0x64, 0xB6, 0x81, 0xA0,
    0x6C, 0x0C, 0x67, 0x6C, 0x39, 0xD6, 0x41, 0x1F, 0x44, 0xAD, 0xF5, 0xEA,
    0x59, 0x65, 0x51, 0x4D, 0x60, 0x46, 0xE1, 0x8E, 0xA5, 0x14, 0x44, 0x53,
    0x1E, 0x6F, 0x7D, 0x96, 0x9C, 0x41, 0x4B, 0x2B, 0x3F, 0x7B, 0xB3, 0xCE,
    0xAC, 0x89, 0x66, 0x90, 0xD3, 0xCF, 0xED, 0x73, 0xE4, 0x82, 0x33, 0xFE,
    0x3A, 0xB4, 0x86, 0x51, 0x59, 0x00, 0x76, 0x37, 0x2A, 0x75, 0xE3, 0xA4,
    0x7E, 0xF8, 0xFE, 0x8D, 0x73, 0x02, 0x66, 0x23, 0x51, 0x29, 0x75, 0xCA,
    0xB3, 0x5C, 0xDF, 0xD2, 0x18, 0x70, 0x34, 0x94, 0x3C, 0xB1, 0x18, 0xA9,
    0x82, 0x92, 0x5C, 0xB0, 0x48, 0x11, 0x57, 0xB0, 0xD9, 0x18, 0x5A, 0x6A,
    0x9F, 0x4E, 0x8C, 0x74, 0xD7, 0x67, 0x91, 0xEF, 0x93, 0x81, 0xB1, 0xD2,
    0x26, 0x66, 0xC3, 0x66, 0xFA, 0x4E, 0xDA, 0xD9, 0x46, 0x10, 0xC9, 0xCC,
    0xFE, 0xD6, 0xEF, 0x88, 0x60, 0x0B, 0xFC, 0x42, 0xC9, 0x60, 0xFD, 0x33,
    0xB5, 0xFB, 0xEB, 0x74, 0x1E, 0x5D, 0xA6, 0x0F, 0x28, 0xC3, 0xB2, 0xBC,
    0x8E, 0xB5, 0x47, 0xC2, 0x18, 0xFA, 0x31, 0xE4, 0x7E, 0xB0, 0x28, 0x8E,
    0x6F, 0x90, 0xCC, 0x20, 0xE0, 0x3D, 0xC8, 0xC9, 0x9D, 0x5F, 0x12, 0x3C,
    0x4E, 0x2B, 0x28, 0xD8, 0x4F, 0x3E, 0xC4, 0xAD, 0xD6, 0xD1, 0xF0, 0x30,
    0xA6, 0xF1, 0x10, 0x4E, 0xF1, 0x23, 0x7B, 0xB5, 0xA1, 0xE0, 0x25, 0xA4,
    0x32, 0x49, 0x30, 0x2B, 0x04, 0xB2, 0x1D, 0x31, 0x9C, 0x67, 0x09, 0xBF,
    0xBB, 0xB0, 0x6E, 0xC5, 0x7E, 0xD0, 0xB2, 0xB1, 0x45, 0xEB, 0xAB, 0x95,
    0x05, 0xE8, 0x33, 0x5E, 0x66, 0xBA, 0x93, 0x81, 0x06, 0xED, 0x91, 0x36,
    0x5B, 0x20, 0x49, 0x63, 0xDB, 0x6C, 0xDA, 0xED, 0x9B, 0x9D, 0xAF, 0x07,
    0xD2, 0x6E, 0xAD, 0x11, 0xB0, 0x8F, 0x05, 0xBA, 0x40, 0xD8, 0x58, 0x01,
    0x98, 0x02, 0x91, 0x28, 0x35, 0x2A, 0x7D, 0x9D, 0xEE, 0xEE, 0x34, 0xB4,
    0xF6, 0xCB, 0x7B, 0xDC, 0x5D, 0x34, 0x61, 0x83, 0xA7, 0xC4, 0x54, 0xAE,
    0x25, 0x6D, 0x92, 0x8D, 0xED, 0xE1, 0xA9, 0xC7, 0x53, 0x0F, 0xAA, 0x50,
    0x62, 0x9F, 0x1A, 0xE4, 0x0A, 0x0E, 0x06, 0x5C, 0xC9, 0x97, 0xEB, 0x09,
    0xDB, 0x22, 0xEC, 0x65, 0x79, 0x72, 0xBF, 0xCA, 0x70, 0xB5, 0x4F, 0x32,
    0x9D, 0xCA, 0x91, 0x0E, 0xA8, 0xE3, 0x1B, 0x80, 0x29, 0x50, 0x91, 0x74,
    0x66, 0x58, 0xEC, 0x85, 0xAC, 0xB3, 0x56, 0xA1, 0x1A, 0x6E, 0xA0, 0xA6,
    0xDC, 0xC1, 0xC7, 0xC1, 0x95, 0xEB, 0x44, 0x30, 0x72, 0x70, 0xC0, 0x76,
    0xA0, 0x0F, 0xBD, 0xCB, 0xD7, 0x75, 0xC7, 0xEB, 0x6A, 0xD8, 0x57, 0x49,
    0x3D, 0xF4, 0x61, 0xCB, 0xC1, 0x2B, 0xE1, 0xF2, 0x3E, 0x5F, 0xA2, 0x13,
    0xA5, 0x2C, 0x55, 0x96, 0x88, 0xF5, 0xE8, 0xD0, 0x46, 0xB6, 0x36, 0xD7,
    0x77, 0x22, 0xF6, 0x39, 0xF5, 0xE2, 0x48, 0x9A, 0x72, 0x4A, 0x4D, 0xC5,
    0x39, 0x5B, 0x92, 0xB6, 0x46, 0x33, 0xAF, 0x95, 0x4E, 0x89, 0x73, 0xB3,
    0x73, 0xEF, 0xC6, 0xEC, 0xFC, 0x55, 0xDD, 0x59, 0x2A, 0x7C, 0xE2, 0x59,
    0x1A, 0xA5, 0x5A, 0xA7, 0xC9, 0xF2, 0x63, 0x2F, 0x7C, 0x05, 0x05, 0x0D,
    0x1A, 0xDD, 0x95, 0xCB, 0xCB, 0xCB, 0x77, 0x82, 0x6E, 0x35, 0x69, 0x98,
    0x33, 0xD4, 0x96, 0x58, 0x7D, 0xFF, 0xB0, 0x58, 0x1C, 0xCC, 0x8D, 0x85,
    0x20, 0x3E
};


/* Test CA X509 Certificate for X509 Verification Routine (Generated by OpenSSL utility).*/

LIBSPDM_GLOBAL_REMOVE_IF_UNREFERENCED uint8_t m_libspdm_test_ca_cert[] = {
    0x30, 0x82, 0x05, 0x0F, 0x30, 0x82, 0x02, 0xF7, 0xA0, 0x03, 0x02, 0x01,
    0x02, 0x02, 0x14, 0x37, 0xBB, 0xBC, 0x04, 0xE1, 0xAA, 0x4A, 0xB6, 0xB4,
    0x70, 0xAE, 0x36, 0x79, 0x8E, 0xE5, 0xC0, 0x72, 0x5B, 0x54, 0x7B, 0x30,
    0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B,
    0x05, 0x00, 0x30, 0x17, 0x31, 0x15, 0x30, 0x13, 0x06, 0x03, 0x55, 0x04,
    0x03, 0x0C, 0x0C, 0x69, 0x6E, 0x74, 0x65, 0x6C, 0x20, 0x52, 0x53, 0x41,
    0x20, 0x43, 0x41, 0x30, 0x1E, 0x17, 0x0D, 0x32, 0x30, 0x30, 0x38, 0x32,
    0x34, 0x30, 0x36, 0x31, 0x38, 0x32, 0x31, 0x5A, 0x17, 0x0D, 0x33, 0x30,
    0x30, 0x38, 0x32, 0x32, 0x30, 0x36, 0x31, 0x38, 0x32, 0x31, 0x5A, 0x30,
    0x17, 0x31, 0x15, 0x30, 0x13, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0C, 0x0C,
    0x69, 0x6E, 0x74, 0x65, 0x6C, 0x20, 0x52, 0x53, 0x41, 0x20, 0x43, 0x41,
    0x30, 0x82, 0x02, 0x22, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86,
    0xF7, 0x0D, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x02, 0x0F, 0x00,
    0x30, 0x82, 0x02, 0x0A, 0x02, 0x82, 0x02, 0x01, 0x00, 0xA3, 0x1B, 0xD9,
    0xBD, 0xFB, 0xAA, 0x37, 0x97, 0xC7, 0xCD, 0xC4, 0xA2, 0x90, 0x03, 0xDD,
    0xEB, 0xB1, 0x57, 0x42, 0xD4, 0x6D, 0x38, 0xDD, 0xDA, 0x50, 0x25, 0xD1,
    0x19, 0xE4, 0xCB, 0xEE, 0xAB, 0x0C, 0x1D, 0xF0, 0x6E, 0x12, 0x68, 0x44,
    0x93, 0x26, 0x83, 0x08, 0x55, 0x39, 0xE0, 0x01, 0xB0, 0xB6, 0xCA, 0x1D,
    0x05, 0xE0, 0x32, 0x27, 0xD6, 0x2D, 0x89, 0x14, 0x6F, 0x45, 0x29, 0x99,
    0xA5, 0xF1, 0xC2, 0x75, 0xC4, 0x8E, 0x9F, 0x09, 0x0B, 0x84, 0xD3, 0x39,
    0x48, 0x36, 0x18, 0xF8, 0xC2, 0x93, 0x21, 0x43, 0xCB, 0x5D, 0x4D, 0xD2,
    0xC0, 0x2E, 0x53, 0x76, 0x4C, 0x10, 0xD8, 0x52, 0xB1, 0xE6, 0x41, 0xF0,
    0xCB, 0xEE, 0xDC, 0xB7, 0xA9, 0x25, 0x10, 0xCD, 0x8B, 0x53, 0xCF, 0x65,
    0x9A, 0xA2, 0x08, 0x63, 0x89, 0x92, 0x27, 0x3A, 0x6C, 0x9F, 0x35, 0x62,
    0xEC, 0xA8, 0xBD, 0xFB, 0x13, 0x08, 0x53, 0x1E, 0xE1, 0xE4, 0x0E, 0x57,
    0xBB, 0xD1, 0xD3, 0x0F, 0x5B, 0x0A, 0x3E, 0xDE, 0x80, 0xB0, 0x9A, 0xF8,
    0x77, 0x1D, 0x41, 0xBF, 0xE4, 0x48, 0x01, 0xFA, 0x0E, 0xA9, 0xAB, 0xDB,
    0x3A, 0x91, 0x3B, 0x52, 0xA4, 0xA9, 0x6C, 0xDA, 0x3B, 0xF4, 0x78, 0xDC,
    0x10, 0xA7, 0xB3, 0x72, 0x49, 0x11, 0xA8, 0xD6, 0x4C, 0x9E, 0x91, 0xC6,
    0x9D, 0x86, 0xEE, 0xEF, 0x04, 0x7F, 0xD8, 0x89, 0x18, 0xF4, 0xD7, 0x37,
    0x24, 0x78, 0xDF, 0x99, 0x1E, 0x86, 0xCD, 0x97, 0xEF, 0xE9, 0xE5, 0x23,
    0xB4, 0xEF, 0x25, 0x57, 0x8E, 0x7C, 0x53, 0x65, 0xE2, 0xFC, 0xA5, 0xB6,
    0xCD, 0xE8, 0x5B, 0x5E, 0x4A, 0x47, 0xF2, 0x8D, 0xE6, 0x11, 0x52, 0x4B,
    0x52, 0x21, 0x48, 0xDB, 0x9A, 0x2D, 0x87, 0x9C, 0x1B, 0x5A, 0xEC, 0x5C,
    0xED, 0x16, 0x93, 0xB7, 0x40, 0x98, 0x04, 0xC8, 0x07, 0x7F, 0xF9, 0x79,
    0xB2, 0x05, 0xC9, 0x79, 0xE5, 0x49, 0xA9, 0xDF, 0xBA, 0xD0, 0xAC, 0xEA,
    0xA7, 0xB2, 0x27, 0x6B, 0x58, 0xE7, 0xCE, 0x14, 0x03, 0x84, 0xFA, 0x63,
    0xE6, 0x11, 0x56, 0xCD, 0xDB, 0x99, 0x55, 0x98, 0xA7, 0xF1, 0xCC, 0x1F,
    0xE9, 0xF5, 0xAA, 0x63, 0x0F, 0x48, 0x0C, 0xFC, 0xFC, 0x4D, 0xB7, 0x4E,
    0x37, 0x5E, 0x29, 0x1D, 0xD7, 0xEF, 0x07, 0x1D, 0x80, 0x81, 0xC1, 0xA4,
    0x25, 0x5D, 0x47, 0xBF, 0xAD, 0xA9, 0x10, 0xC8, 0x1C, 0x67, 0x3E, 0x7A,
    0x07, 0x43, 0xC0, 0x79, 0x94, 0xC3, 0x29, 0xD8, 0xEF, 0xBD, 0xB4, 0x69,
    0xC6, 0xD0, 0x7F, 0x94, 0x7C, 0x6E, 0xD0, 0xBA, 0x2E, 0xBA, 0x65, 0xA4,
    0xA3, 0x14, 0x11, 0x86, 0xBE, 0xB3, 0xB7, 0xC9, 0x3F, 0x8B, 0xBB, 0xB5,
    0x36, 0x18, 0x51, 0x71, 0xAE, 0x48, 0xB0, 0xE4, 0x8E, 0x5C, 0x76, 0xF5,
    0xD1, 0x2B, 0x40, 0x0C, 0x68, 0x7B, 0x10, 0xB8, 0xD2, 0x3D, 0x20, 0x7D,
    0x61, 0xCB, 0x7A, 0x3A, 0x28, 0xF4, 0xAF, 0x99, 0x8D, 0x99, 0xF8, 0xF0,
    0x4E, 0xBF, 0x03, 0x41, 0x23, 0xDE, 0xF2, 0xF2, 0xA5, 0x80, 0x4A, 0x1A,
    0x54, 0x4A, 0xF3, 0xFB, 0x2C, 0x92, 0x4C, 0x2E, 0xA8, 0x3D, 0x40, 0x85,
    0x34, 0xDF, 0x26, 0xCE, 0xF5, 0xAA, 0xFB, 0x7A, 0x9C, 0xA7, 0xF0, 0xAB,
    0x7D, 0x5F, 0x08, 0x4A, 0x6F, 0x98, 0x0C, 0xBA, 0xC3, 0xEE, 0xA0, 0x3B,
    0x1F, 0x28, 0xC4, 0x12, 0x44, 0xB4, 0xF1, 0xAB, 0xCA, 0xB3, 0xBD, 0x5B,
    0x4B, 0x75, 0x22, 0x39, 0x34, 0xAE, 0x9D, 0x2F, 0x54, 0xEA, 0x00, 0xCD,
    0xAE, 0xFF, 0x26, 0xC5, 0xCF, 0x5F, 0x3D, 0xCB, 0xD2, 0x9B, 0x09, 0x4B,
    0x49, 0x1B, 0x29, 0x68, 0xE4, 0xFE, 0x99, 0x7B, 0x8B, 0xF1, 0xB8, 0xBC,
    0x78, 0x1B, 0x17, 0x07, 0x20, 0xA8, 0x16, 0x50, 0x41, 0xE6, 0xAA, 0x3A,
    0xEC, 0x63, 0x4C, 0x64, 0x15, 0x02, 0x03, 0x01, 0x00, 0x01, 0xA3, 0x53,
    0x30, 0x51, 0x30, 0x1D, 0x06, 0x03, 0x55, 0x1D, 0x0E, 0x04, 0x16, 0x04,
    0x14, 0x9B, 0x58, 0x2B, 0x4E, 0x21, 0x11, 0x31, 0x25, 0xD3, 0x62, 0x53,
    0xF7, 0x4F, 0xC1, 0xD6, 0x47, 0x2A, 0x2D, 0xD5, 0x1B, 0x30, 0x1F, 0x06,
    0x03, 0x55, 0x1D, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0x9B, 0x58,
    0x2B, 0x4E, 0x21, 0x11, 0x31, 0x25, 0xD3, 0x62, 0x53, 0xF7, 0x4F, 0xC1,
    0xD6, 0x47, 0x2A, 0x2D, 0xD5, 0x1B, 0x30, 0x0F, 0x06, 0x03, 0x55, 0x1D,
    0x13, 0x01, 0x01, 0xFF, 0x04, 0x05, 0x30, 0x03, 0x01, 0x01, 0xFF, 0x30,
    0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B,
    0x05, 0x00, 0x03, 0x82, 0x02, 0x01, 0x00, 0x94, 0x09, 0xA4, 0x20, 0x37,
    0x3F, 0x17, 0xAF, 0xB8, 0x02, 0xB8, 0x70, 0x9C, 0xBA, 0x8D, 0xC2, 0x0C,
    0xD6, 0x02, 0xE3, 0xB4, 0xF8, 0xBF, 0x7D, 0xE4, 0x0A, 0xDD, 0x10, 0x39,
    0x0A, 0x3A, 0x22, 0xF4, 0x8B, 0x3A, 0xE2, 0x77, 0xC0, 0xAC, 0xEB, 0xB7,
    0xB4, 0xD3, 0x00, 0x5C, 0x2A, 0xC4, 0xE7, 0x41, 0xEE, 0x4E, 0xE7, 0x79,
    0xC9, 0xD3, 0xC2, 0x58, 0x6E, 0xF4, 0xFC, 0xBF, 0xE6, 0x41, 0xCF, 0xBB,
    0xD5, 0xCE, 0x73, 0x43, 0x64, 0x00, 0xDA, 0x2F, 0xB4, 0x90, 0x1D, 0xBA,
    0xD3, 0x4B, 0xA4, 0x36, 0x7D, 0x69, 0x0A, 0xCC, 0xE1, 0xEC, 0xE1, 0x18,
    0x54, 0x34, 0xC8, 0x7E, 0xF9, 0xDB, 0x43, 0xC3, 0xBB, 0x0E, 0x37, 0xC6,
    0x61, 0xD1, 0x4B, 0xF8, 0x71, 0x93, 0x57, 0x4B, 0x58, 0x8D, 0x49, 0x44,
    0xB1, 0x6E, 0x11, 0x9C, 0xE1, 0xF9, 0xB9, 0x44, 0xA2, 0xB5, 0xB6, 0x73,
    0x1D, 0xB1, 0x99, 0xBD, 0x5A, 0x88, 0xC6, 0x51, 0xB5, 0x4B, 0xFF, 0xB7,
    0x02, 0xBA, 0x97, 0xA3, 0xB9, 0x1B, 0x9B, 0x36, 0x40, 0xC0, 0x94, 0x77,
    0x8C, 0xBD, 0xA6, 0x68, 0x6E, 0x21, 0x18, 0x84, 0xFE, 0xEA, 0xAA, 0x75,
    0x4D, 0x14, 0x41, 0xAD, 0x43, 0x49, 0xAC, 0x91, 0x74, 0xA1, 0xF7, 0xE6,
    0x7B, 0x7E, 0x59, 0xA9, 0xA2, 0xFB, 0xBC, 0x76, 0xA8, 0xDC, 0x22, 0xB8,
    0x86, 0x87, 0x18, 0xB5, 0xC6, 0x3D, 0x00, 0x11, 0x7E, 0x17, 0x10, 0x56,
    0xAF, 0x11, 0x00, 0x78, 0xE0, 0x84, 0x64, 0xE6, 0x5E, 0xE2, 0x47, 0x79,
    0xA5, 0xC7, 0xEF, 0xEA, 0xD4, 0x9D, 0x4C, 0xBE, 0x90, 0x9B, 0x20, 0x0D,
    0xAD, 0xB9, 0x82, 0x4C, 0x47, 0x15, 0x70, 0xE5, 0x7F, 0x2C, 0x6A, 0x39,
    0x93, 0x45, 0xE2, 0xA7, 0xDA, 0x4C, 0xF1, 0x3D, 0xB7, 0x87, 0xD8, 0xD6,
    0x81, 0xFD, 0x8F, 0x31, 0xEE, 0xC7, 0xCC, 0x31, 0x46, 0x37, 0x4F, 0x50,
    0x32, 0x06, 0xB7, 0x18, 0x3F, 0x6B, 0xA2, 0x90, 0x56, 0xD6, 0xA3, 0x4E,
    0x5F, 0x3F, 0x56, 0x66, 0xD0, 0x66, 0xB0, 0x3A, 0x63, 0xD3, 0x09, 0xB7,
    0x30, 0xCE, 0x8F, 0x00, 0x38, 0x97, 0x87, 0x07, 0xA8, 0xA5, 0x99, 0x47,
    0x37, 0x59, 0xA3, 0x7F, 0x67, 0x2D, 0x2F, 0xE9, 0xFF, 0xC9, 0x21, 0x91,
    0x0F, 0xD0, 0x27, 0xB0, 0xE3, 0x86, 0xA1, 0x07, 0x80, 0x18, 0x20, 0xCF,
    0x7E, 0x3F, 0xED, 0x4C, 0xB7, 0x7F, 0xDE, 0x18, 0x0A, 0x02, 0xC9, 0x2B,
    0xAA, 0x52, 0xF2, 0x72, 0x79, 0x6B, 0x62, 0x8A, 0x1B, 0x14, 0x44, 0x03,
    0x7E, 0xF0, 0x7E, 0xF0, 0x6C, 0xB1, 0xC9, 0x60, 0xBC, 0xEA, 0xD3, 0xC6,
    0xCE, 0xB9, 0x5E, 0xBF, 0x51, 0x4A, 0x22, 0xFC, 0x5D, 0xD7, 0x11, 0x58,
    0x82, 0x3A, 0x7D, 0x9C, 0x50, 0xB6, 0x11, 0x6B, 0x62, 0xFE, 0x72, 0x39,
    0x64, 0xA3, 0x0B, 0x97, 0xD4, 0x48, 0x58, 0xE0, 0xD8, 0xAF, 0x9B, 0x45,
    0x2C, 0xCB, 0x28, 0xFC, 0x76, 0x17, 0x37, 0x2F, 0xF9, 0x5D, 0xA4, 0x4A,
    0x83, 0x05, 0xAB, 0x28, 0x0C, 0x8C, 0xDC, 0x12, 0xCA, 0xB5, 0xFA, 0x13,
    0xBF, 0x65, 0x2B, 0x2E, 0x8D, 0xB7, 0xB7, 0x19, 0x82, 0x30, 0x53, 0x54,
    0x83, 0xF2, 0x85, 0xE1, 0x52, 0x5F, 0xEE, 0xD3, 0x38, 0x8D, 0x4C, 0xB9,
    0x0C, 0x2A, 0xDC, 0x36, 0xBF, 0x90, 0xBE, 0x53, 0x46, 0xE2, 0xD9, 0x25,
    0x57, 0x0C, 0x17, 0x85, 0xF1, 0x8F, 0xE9, 0xE1, 0xB7, 0x94, 0x00, 0x0C,
    0x53, 0xBB, 0x29, 0x05, 0xD0, 0x1B, 0x96, 0x5E, 0x4F, 0xAA, 0x20, 0x72,
    0x06, 0x0F, 0x8F, 0x8F, 0xD4, 0xA6, 0xB4, 0x0C, 0xAD, 0xE1, 0x86, 0x7F,
    0xA0, 0xD1, 0x2E, 0xC0, 0x13, 0x08, 0x92, 0x6B, 0xD4, 0x66, 0x16, 0x35,
    0xB6, 0xC9, 0x9E, 0x2C, 0xC7, 0x8A, 0xB2, 0xB5, 0xFF, 0xA5, 0xB4, 0xD0,
    0x28, 0x38, 0x0B
};


/* password-protected PEM key data for RSA Private key Retrieving (encryption key is "client").
 * (Generated by OpenSSL utility).*/

LIBSPDM_GLOBAL_REMOVE_IF_UNREFERENCED uint8_t m_libspdm_test_key_pem[] = {
    0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x42, 0x45, 0x47, 0x49, 0x4E, 0x20, 0x50,
    0x52, 0x49, 0x56, 0x41, 0x54, 0x45, 0x20, 0x4B, 0x45, 0x59, 0x2D, 0x2D,
    0x2D, 0x2D, 0x2D, 0x0D, 0x0A, 0x4D, 0x49, 0x49, 0x47, 0x2F, 0x51, 0x49,
    0x42, 0x41, 0x44, 0x41, 0x4E, 0x42, 0x67, 0x6B, 0x71, 0x68, 0x6B, 0x69,
    0x47, 0x39, 0x77, 0x30, 0x42, 0x41, 0x51, 0x45, 0x46, 0x41, 0x41, 0x53,
    0x43, 0x42, 0x75, 0x63, 0x77, 0x67, 0x67, 0x62, 0x6A, 0x41, 0x67, 0x45,
    0x41, 0x41, 0x6F, 0x49, 0x42, 0x67, 0x51, 0x44, 0x4A, 0x66, 0x54, 0x66,
    0x78, 0x46, 0x50, 0x62, 0x75, 0x52, 0x4F, 0x66, 0x53, 0x0D, 0x0A, 0x57,
    0x36, 0x79, 0x6F, 0x71, 0x2F, 0x6B, 0x56, 0x37, 0x6D, 0x52, 0x79, 0x51,
    0x32, 0x77, 0x78, 0x38, 0x56, 0x32, 0x39, 0x79, 0x49, 0x54, 0x33, 0x4F,
    0x46, 0x31, 0x47, 0x34, 0x4E, 0x71, 0x47, 0x70, 0x33, 0x42, 0x49, 0x63,
    0x46, 0x61, 0x46, 0x30, 0x73, 0x66, 0x57, 0x33, 0x79, 0x47, 0x51, 0x30,
    0x45, 0x72, 0x56, 0x78, 0x56, 0x78, 0x62, 0x55, 0x30, 0x6C, 0x78, 0x67,
    0x67, 0x51, 0x31, 0x0D, 0x0A, 0x58, 0x79, 0x7A, 0x62, 0x71, 0x7A, 0x57,
    0x38, 0x59, 0x6E, 0x77, 0x7A, 0x30, 0x5A, 0x6D, 0x38, 0x51, 0x4B, 0x38,
    0x52, 0x32, 0x39, 0x70, 0x56, 0x73, 0x48, 0x73, 0x43, 0x46, 0x45, 0x43,
    0x4F, 0x56, 0x59, 0x6D, 0x57, 0x72, 0x77, 0x52, 0x31, 0x58, 0x72, 0x78,
    0x31, 0x4B, 0x76, 0x77, 0x73, 0x7A, 0x6D, 0x74, 0x70, 0x53, 0x53, 0x76,
    0x7A, 0x78, 0x58, 0x46, 0x45, 0x30, 0x52, 0x4A, 0x55, 0x0D, 0x0A, 0x70,
    0x38, 0x32, 0x4D, 0x4A, 0x6F, 0x52, 0x4F, 0x5A, 0x48, 0x4C, 0x68, 0x46,
    0x33, 0x52, 0x4E, 0x57, 0x4A, 0x2B, 0x54, 0x55, 0x6B, 0x65, 0x48, 0x34,
    0x45, 0x79, 0x74, 0x50, 0x4B, 0x31, 0x69, 0x79, 0x33, 0x45, 0x46, 0x6B,
    0x6D, 0x6A, 0x39, 0x5A, 0x45, 0x50, 0x55, 0x73, 0x49, 0x59, 0x77, 0x77,
    0x4C, 0x54, 0x51, 0x51, 0x70, 0x59, 0x65, 0x67, 0x41, 0x56, 0x62, 0x34,
    0x79, 0x79, 0x32, 0x0D, 0x0A, 0x6F, 0x64, 0x4C, 0x66, 0x64, 0x6E, 0x79,
    0x71, 0x2B, 0x73, 0x50, 0x63, 0x51, 0x78, 0x70, 0x58, 0x79, 0x47, 0x31,
    0x64, 0x75, 0x64, 0x38, 0x4F, 0x5A, 0x39, 0x4E, 0x59, 0x61, 0x77, 0x4B,
    0x4E, 0x68, 0x4D, 0x30, 0x78, 0x76, 0x61, 0x32, 0x64, 0x41, 0x45, 0x57,
    0x42, 0x52, 0x4E, 0x72, 0x50, 0x6A, 0x76, 0x33, 0x69, 0x43, 0x61, 0x4A,
    0x6F, 0x57, 0x70, 0x63, 0x33, 0x59, 0x51, 0x55, 0x6F, 0x0D, 0x0A, 0x6B,
    0x33, 0x66, 0x56, 0x71, 0x77, 0x68, 0x77, 0x43, 0x57, 0x68, 0x75, 0x6C,
    0x45, 0x34, 0x78, 0x58, 0x6C, 0x62, 0x7A, 0x44, 0x79, 0x6D, 0x4D, 0x4D,
    0x6A, 0x35, 0x44, 0x6F, 0x62, 0x47, 0x59, 0x61, 0x34, 0x6E, 0x62, 0x2F,
    0x38, 0x68, 0x52, 0x4A, 0x71, 0x33, 0x4C, 0x70, 0x66, 0x7A, 0x30, 0x36,
    0x46, 0x2B, 0x68, 0x7A, 0x53, 0x6F, 0x2F, 0x33, 0x47, 0x53, 0x56, 0x67,
    0x69, 0x64, 0x71, 0x0D, 0x0A, 0x69, 0x7A, 0x38, 0x4B, 0x53, 0x75, 0x38,
    0x6D, 0x39, 0x77, 0x31, 0x43, 0x6F, 0x68, 0x72, 0x75, 0x72, 0x58, 0x79,
    0x77, 0x77, 0x31, 0x46, 0x68, 0x63, 0x32, 0x6E, 0x36, 0x63, 0x4F, 0x33,
    0x54, 0x42, 0x4A, 0x48, 0x65, 0x50, 0x67, 0x66, 0x50, 0x79, 0x54, 0x69,
    0x39, 0x38, 0x2B, 0x5A, 0x6D, 0x63, 0x35, 0x45, 0x69, 0x6B, 0x58, 0x4F,
    0x45, 0x2F, 0x76, 0x49, 0x70, 0x75, 0x75, 0x56, 0x34, 0x0D, 0x0A, 0x58,
    0x64, 0x64, 0x41, 0x52, 0x33, 0x68, 0x50, 0x63, 0x37, 0x6E, 0x67, 0x74,
    0x59, 0x70, 0x6B, 0x57, 0x36, 0x44, 0x57, 0x4D, 0x6C, 0x51, 0x6C, 0x39,
    0x56, 0x71, 0x47, 0x37, 0x72, 0x6F, 0x4D, 0x4B, 0x61, 0x6B, 0x71, 0x71,
    0x77, 0x56, 0x52, 0x65, 0x54, 0x48, 0x76, 0x5A, 0x30, 0x4D, 0x68, 0x32,
    0x46, 0x45, 0x69, 0x59, 0x47, 0x76, 0x64, 0x4A, 0x6D, 0x37, 0x71, 0x62,
    0x6C, 0x6F, 0x50, 0x0D, 0x0A, 0x44, 0x35, 0x2B, 0x58, 0x6B, 0x49, 0x32,
    0x32, 0x2F, 0x74, 0x75, 0x6D, 0x2F, 0x75, 0x54, 0x4E, 0x35, 0x32, 0x36,
    0x4F, 0x45, 0x70, 0x53, 0x35, 0x4E, 0x70, 0x48, 0x36, 0x30, 0x6E, 0x6B,
    0x4A, 0x58, 0x51, 0x65, 0x79, 0x68, 0x53, 0x4D, 0x79, 0x39, 0x32, 0x69,
    0x59, 0x2B, 0x35, 0x33, 0x2F, 0x41, 0x61, 0x6B, 0x43, 0x41, 0x77, 0x45,
    0x41, 0x41, 0x51, 0x4B, 0x43, 0x41, 0x59, 0x42, 0x36, 0x0D, 0x0A, 0x53,
    0x33, 0x69, 0x43, 0x36, 0x58, 0x69, 0x66, 0x55, 0x6B, 0x39, 0x71, 0x7A,
    0x4A, 0x33, 0x56, 0x6D, 0x74, 0x77, 0x4B, 0x78, 0x62, 0x46, 0x4E, 0x38,
    0x55, 0x4A, 0x67, 0x5A, 0x65, 0x48, 0x42, 0x36, 0x55, 0x31, 0x62, 0x53,
    0x47, 0x6A, 0x46, 0x4E, 0x6F, 0x45, 0x73, 0x72, 0x63, 0x42, 0x6E, 0x46,
    0x31, 0x68, 0x4D, 0x7A, 0x59, 0x7A, 0x69, 0x44, 0x48, 0x69, 0x78, 0x44,
    0x74, 0x76, 0x39, 0x0D, 0x0A, 0x4F, 0x46, 0x64, 0x41, 0x54, 0x79, 0x4D,
    0x4B, 0x74, 0x43, 0x47, 0x64, 0x79, 0x33, 0x44, 0x4D, 0x75, 0x2F, 0x52,
    0x58, 0x72, 0x57, 0x63, 0x35, 0x68, 0x49, 0x54, 0x4E, 0x55, 0x47, 0x31,
    0x79, 0x5A, 0x34, 0x44, 0x7A, 0x39, 0x62, 0x51, 0x54, 0x70, 0x50, 0x73,
    0x78, 0x36, 0x52, 0x55, 0x6A, 0x30, 0x6F, 0x4C, 0x67, 0x51, 0x6D, 0x6F,
    0x63, 0x71, 0x38, 0x52, 0x33, 0x35, 0x61, 0x6F, 0x6F, 0x0D, 0x0A, 0x4D,
    0x33, 0x4E, 0x72, 0x2B, 0x48, 0x45, 0x78, 0x6C, 0x62, 0x79, 0x4D, 0x69,
    0x58, 0x55, 0x6E, 0x46, 0x64, 0x48, 0x45, 0x53, 0x77, 0x2B, 0x33, 0x4D,
    0x36, 0x63, 0x78, 0x73, 0x78, 0x4C, 0x59, 0x4E, 0x6C, 0x36, 0x59, 0x53,
    0x44, 0x6F, 0x43, 0x42, 0x41, 0x77, 0x78, 0x4A, 0x4D, 0x34, 0x71, 0x31,
    0x5A, 0x7A, 0x48, 0x2F, 0x2B, 0x31, 0x7A, 0x30, 0x4E, 0x6B, 0x43, 0x43,
    0x6C, 0x55, 0x30, 0x0D, 0x0A, 0x2B, 0x74, 0x56, 0x6E, 0x47, 0x63, 0x6B,
    0x6D, 0x35, 0x70, 0x49, 0x6A, 0x48, 0x38, 0x47, 0x41, 0x52, 0x4D, 0x69,
    0x53, 0x64, 0x72, 0x59, 0x65, 0x6F, 0x4E, 0x51, 0x43, 0x5A, 0x2F, 0x4B,
    0x4A, 0x71, 0x75, 0x47, 0x4B, 0x4F, 0x74, 0x5A, 0x36, 0x6D, 0x69, 0x6A,
    0x6F, 0x67, 0x4B, 0x79, 0x53, 0x2F, 0x6B, 0x2B, 0x38, 0x4E, 0x59, 0x66,
    0x36, 0x57, 0x44, 0x4A, 0x2F, 0x50, 0x49, 0x67, 0x6B, 0x0D, 0x0A, 0x63,
    0x30, 0x38, 0x4A, 0x35, 0x2B, 0x42, 0x50, 0x7A, 0x42, 0x4A, 0x51, 0x59,
    0x67, 0x66, 0x55, 0x64, 0x4E, 0x2B, 0x76, 0x46, 0x6B, 0x54, 0x6E, 0x79,
    0x66, 0x72, 0x6F, 0x69, 0x74, 0x33, 0x42, 0x71, 0x54, 0x36, 0x56, 0x45,
    0x53, 0x55, 0x57, 0x53, 0x46, 0x56, 0x59, 0x71, 0x38, 0x35, 0x57, 0x76,
    0x58, 0x6B, 0x76, 0x50, 0x79, 0x55, 0x6D, 0x79, 0x46, 0x5A, 0x58, 0x61,
    0x2B, 0x74, 0x6A, 0x0D, 0x0A, 0x52, 0x74, 0x56, 0x68, 0x63, 0x68, 0x4E,
    0x49, 0x44, 0x65, 0x71, 0x73, 0x33, 0x44, 0x56, 0x7A, 0x79, 0x36, 0x45,
    0x55, 0x6A, 0x79, 0x30, 0x62, 0x58, 0x35, 0x5A, 0x48, 0x6E, 0x67, 0x58,
    0x42, 0x4B, 0x4D, 0x57, 0x56, 0x68, 0x56, 0x36, 0x52, 0x34, 0x65, 0x65,
    0x44, 0x70, 0x71, 0x71, 0x4B, 0x51, 0x42, 0x4E, 0x74, 0x57, 0x47, 0x32,
    0x75, 0x6C, 0x37, 0x34, 0x73, 0x4A, 0x53, 0x7A, 0x77, 0x0D, 0x0A, 0x52,
    0x67, 0x48, 0x39, 0x56, 0x69, 0x70, 0x73, 0x53, 0x56, 0x4C, 0x6A, 0x34,
    0x73, 0x69, 0x41, 0x38, 0x66, 0x72, 0x52, 0x75, 0x39, 0x33, 0x52, 0x34,
    0x43, 0x72, 0x50, 0x31, 0x4F, 0x32, 0x2B, 0x6A, 0x57, 0x5A, 0x67, 0x51,
    0x36, 0x67, 0x2F, 0x49, 0x6A, 0x4E, 0x4D, 0x77, 0x7A, 0x74, 0x37, 0x4B,
    0x6D, 0x61, 0x44, 0x75, 0x6E, 0x4A, 0x6A, 0x52, 0x4D, 0x55, 0x4B, 0x4F,
    0x53, 0x4F, 0x38, 0x0D, 0x0A, 0x71, 0x77, 0x54, 0x66, 0x68, 0x78, 0x67,
    0x51, 0x79, 0x4F, 0x4D, 0x57, 0x49, 0x73, 0x39, 0x31, 0x69, 0x4D, 0x63,
    0x58, 0x4B, 0x66, 0x59, 0x6D, 0x7A, 0x61, 0x4E, 0x77, 0x66, 0x66, 0x4A,
    0x47, 0x51, 0x65, 0x72, 0x2F, 0x49, 0x48, 0x59, 0x57, 0x79, 0x50, 0x75,
    0x4B, 0x36, 0x57, 0x4D, 0x46, 0x4B, 0x53, 0x68, 0x6B, 0x77, 0x47, 0x2F,
    0x36, 0x5A, 0x4E, 0x42, 0x48, 0x52, 0x41, 0x45, 0x43, 0x0D, 0x0A, 0x67,
    0x63, 0x45, 0x41, 0x37, 0x78, 0x6F, 0x71, 0x5A, 0x75, 0x54, 0x59, 0x66,
    0x7A, 0x6B, 0x62, 0x55, 0x77, 0x55, 0x58, 0x62, 0x30, 0x75, 0x35, 0x4A,
    0x57, 0x69, 0x6E, 0x77, 0x69, 0x76, 0x6D, 0x59, 0x6E, 0x41, 0x75, 0x59,
    0x53, 0x64, 0x55, 0x77, 0x48, 0x69, 0x4D, 0x5A, 0x68, 0x44, 0x36, 0x4E,
    0x6A, 0x51, 0x70, 0x41, 0x41, 0x6C, 0x48, 0x6E, 0x78, 0x31, 0x39, 0x6C,
    0x6A, 0x54, 0x50, 0x0D, 0x0A, 0x4E, 0x72, 0x30, 0x6E, 0x39, 0x52, 0x6A,
    0x76, 0x44, 0x47, 0x4C, 0x62, 0x38, 0x6E, 0x38, 0x42, 0x52, 0x33, 0x4C,
    0x64, 0x79, 0x67, 0x52, 0x63, 0x52, 0x79, 0x75, 0x38, 0x39, 0x59, 0x7A,
    0x58, 0x4F, 0x2F, 0x67, 0x71, 0x70, 0x6B, 0x6F, 0x52, 0x45, 0x35, 0x51,
    0x6D, 0x54, 0x35, 0x51, 0x46, 0x36, 0x30, 0x72, 0x37, 0x55, 0x35, 0x31,
    0x63, 0x48, 0x54, 0x77, 0x65, 0x59, 0x73, 0x46, 0x46, 0x0D, 0x0A, 0x33,
    0x2B, 0x76, 0x33, 0x37, 0x51, 0x59, 0x71, 0x4F, 0x55, 0x41, 0x72, 0x34,
    0x31, 0x74, 0x50, 0x73, 0x50, 0x38, 0x4E, 0x71, 0x64, 0x4E, 0x65, 0x4C,
    0x71, 0x32, 0x79, 0x47, 0x5A, 0x47, 0x50, 0x32, 0x59, 0x50, 0x63, 0x30,
    0x69, 0x30, 0x42, 0x76, 0x71, 0x32, 0x2F, 0x38, 0x32, 0x67, 0x6F, 0x33,
    0x74, 0x6B, 0x41, 0x4E, 0x73, 0x59, 0x4A, 0x56, 0x72, 0x30, 0x48, 0x68,
    0x43, 0x68, 0x58, 0x0D, 0x0A, 0x69, 0x51, 0x69, 0x72, 0x50, 0x4C, 0x30,
    0x43, 0x47, 0x58, 0x33, 0x79, 0x6E, 0x30, 0x79, 0x77, 0x50, 0x58, 0x34,
    0x34, 0x2F, 0x71, 0x4F, 0x64, 0x59, 0x69, 0x69, 0x46, 0x37, 0x78, 0x42,
    0x6C, 0x42, 0x4B, 0x78, 0x34, 0x54, 0x76, 0x75, 0x4C, 0x6F, 0x2F, 0x64,
    0x43, 0x65, 0x76, 0x4F, 0x64, 0x38, 0x54, 0x4E, 0x7A, 0x35, 0x6D, 0x53,
    0x6C, 0x44, 0x46, 0x39, 0x6E, 0x6A, 0x31, 0x51, 0x66, 0x0D, 0x0A, 0x37,
    0x6B, 0x5A, 0x70, 0x41, 0x6F, 0x48, 0x42, 0x41, 0x4E, 0x65, 0x36, 0x6A,
    0x72, 0x33, 0x68, 0x42, 0x75, 0x72, 0x58, 0x56, 0x6D, 0x62, 0x66, 0x57,
    0x58, 0x49, 0x34, 0x37, 0x52, 0x76, 0x44, 0x61, 0x6C, 0x74, 0x30, 0x49,
    0x38, 0x65, 0x48, 0x32, 0x72, 0x68, 0x59, 0x6D, 0x2F, 0x51, 0x61, 0x31,
    0x61, 0x4F, 0x4F, 0x74, 0x66, 0x59, 0x61, 0x43, 0x62, 0x43, 0x36, 0x6E,
    0x4D, 0x61, 0x37, 0x0D, 0x0A, 0x4A, 0x57, 0x2B, 0x53, 0x6F, 0x78, 0x35,
    0x34, 0x6F, 0x6B, 0x73, 0x39, 0x64, 0x6C, 0x66, 0x78, 0x61, 0x65, 0x4C,
    0x56, 0x67, 0x39, 0x6C, 0x5A, 0x58, 0x61, 0x4A, 0x5A, 0x58, 0x45, 0x4A,
    0x74, 0x4A, 0x66, 0x30, 0x35, 0x5A, 0x5A, 0x53, 0x77, 0x72, 0x59, 0x79,
    0x5A, 0x75, 0x4A, 0x58, 0x38, 0x45, 0x65, 0x79, 0x55, 0x31, 0x37, 0x50,
    0x78, 0x4A, 0x78, 0x37, 0x57, 0x6C, 0x31, 0x6E, 0x34, 0x0D, 0x0A, 0x6A,
    0x41, 0x35, 0x53, 0x41, 0x2F, 0x76, 0x62, 0x5A, 0x62, 0x31, 0x6C, 0x59,
    0x4E, 0x51, 0x2B, 0x37, 0x4A, 0x43, 0x44, 0x74, 0x53, 0x62, 0x4E, 0x47,
    0x79, 0x53, 0x54, 0x71, 0x68, 0x49, 0x32, 0x4B, 0x41, 0x58, 0x43, 0x6D,
    0x68, 0x49, 0x70, 0x63, 0x31, 0x64, 0x45, 0x45, 0x47, 0x30, 0x32, 0x6C,
    0x61, 0x44, 0x6F, 0x75, 0x4D, 0x31, 0x4E, 0x75, 0x55, 0x56, 0x6F, 0x2B,
    0x46, 0x59, 0x44, 0x0D, 0x0A, 0x57, 0x42, 0x41, 0x59, 0x66, 0x30, 0x42,
    0x72, 0x2F, 0x42, 0x6B, 0x6F, 0x5A, 0x31, 0x33, 0x65, 0x2F, 0x46, 0x31,
    0x30, 0x54, 0x48, 0x35, 0x2F, 0x45, 0x77, 0x38, 0x38, 0x2F, 0x72, 0x39,
    0x75, 0x68, 0x4B, 0x73, 0x6D, 0x30, 0x72, 0x43, 0x73, 0x38, 0x6B, 0x66,
    0x64, 0x39, 0x2B, 0x53, 0x56, 0x65, 0x68, 0x76, 0x61, 0x51, 0x6A, 0x56,
    0x49, 0x31, 0x47, 0x48, 0x35, 0x6E, 0x47, 0x7A, 0x4B, 0x0D, 0x0A, 0x43,
    0x79, 0x4C, 0x74, 0x76, 0x59, 0x76, 0x35, 0x51, 0x51, 0x4B, 0x42, 0x77,
    0x51, 0x44, 0x48, 0x4E, 0x2B, 0x6A, 0x75, 0x64, 0x47, 0x79, 0x43, 0x6A,
    0x45, 0x56, 0x64, 0x42, 0x64, 0x67, 0x42, 0x7A, 0x6E, 0x6B, 0x45, 0x56,
    0x30, 0x2B, 0x33, 0x74, 0x44, 0x59, 0x76, 0x2F, 0x4D, 0x4A, 0x59, 0x42,
    0x70, 0x70, 0x53, 0x42, 0x39, 0x38, 0x4C, 0x52, 0x38, 0x67, 0x43, 0x6B,
    0x37, 0x45, 0x35, 0x0D, 0x0A, 0x71, 0x34, 0x47, 0x32, 0x62, 0x4B, 0x37,
    0x77, 0x45, 0x53, 0x34, 0x35, 0x73, 0x4A, 0x39, 0x7A, 0x55, 0x77, 0x51,
    0x53, 0x4D, 0x4F, 0x69, 0x4D, 0x41, 0x78, 0x42, 0x41, 0x78, 0x4D, 0x39,
    0x51, 0x2F, 0x42, 0x4D, 0x61, 0x35, 0x62, 0x46, 0x36, 0x63, 0x6F, 0x2F,
    0x2F, 0x45, 0x6F, 0x59, 0x4F, 0x71, 0x5A, 0x58, 0x53, 0x31, 0x31, 0x53,
    0x59, 0x5A, 0x34, 0x41, 0x4C, 0x44, 0x52, 0x32, 0x64, 0x0D, 0x0A, 0x44,
    0x48, 0x62, 0x73, 0x6B, 0x4F, 0x50, 0x4D, 0x61, 0x72, 0x62, 0x61, 0x6E,
    0x4E, 0x53, 0x6B, 0x38, 0x38, 0x63, 0x49, 0x6A, 0x70, 0x52, 0x43, 0x79,
    0x68, 0x6A, 0x37, 0x49, 0x33, 0x66, 0x78, 0x38, 0x50, 0x2F, 0x2F, 0x4B,
    0x70, 0x42, 0x44, 0x52, 0x34, 0x6F, 0x77, 0x74, 0x50, 0x4B, 0x76, 0x56,
    0x2F, 0x71, 0x6C, 0x4D, 0x6B, 0x58, 0x46, 0x2F, 0x42, 0x70, 0x5A, 0x6D,
    0x7A, 0x72, 0x58, 0x0D, 0x0A, 0x61, 0x47, 0x66, 0x74, 0x6B, 0x6E, 0x43,
    0x4A, 0x78, 0x38, 0x63, 0x76, 0x6B, 0x54, 0x36, 0x44, 0x7A, 0x41, 0x50,
    0x4D, 0x30, 0x36, 0x6A, 0x73, 0x32, 0x38, 0x76, 0x59, 0x59, 0x4B, 0x77,
    0x43, 0x56, 0x72, 0x62, 0x53, 0x53, 0x32, 0x74, 0x55, 0x78, 0x6A, 0x63,
    0x31, 0x44, 0x72, 0x4C, 0x72, 0x4E, 0x43, 0x47, 0x6B, 0x53, 0x32, 0x64,
    0x6A, 0x35, 0x45, 0x6E, 0x4C, 0x35, 0x35, 0x41, 0x41, 0x0D, 0x0A, 0x2F,
    0x67, 0x69, 0x61, 0x35, 0x33, 0x37, 0x30, 0x36, 0x43, 0x58, 0x32, 0x74,
    0x55, 0x6B, 0x43, 0x67, 0x63, 0x41, 0x53, 0x4E, 0x4E, 0x6C, 0x31, 0x4A,
    0x76, 0x65, 0x70, 0x66, 0x44, 0x44, 0x37, 0x6A, 0x36, 0x41, 0x42, 0x35,
    0x74, 0x49, 0x45, 0x42, 0x4E, 0x4D, 0x47, 0x45, 0x75, 0x41, 0x75, 0x4A,
    0x38, 0x59, 0x4B, 0x78, 0x74, 0x65, 0x71, 0x6D, 0x31, 0x55, 0x71, 0x62,
    0x35, 0x48, 0x6C, 0x0D, 0x0A, 0x79, 0x6A, 0x37, 0x46, 0x48, 0x6F, 0x71,
    0x36, 0x72, 0x76, 0x4B, 0x79, 0x33, 0x75, 0x4A, 0x4C, 0x59, 0x51, 0x7A,
    0x35, 0x59, 0x70, 0x2B, 0x4A, 0x50, 0x6F, 0x4F, 0x63, 0x4E, 0x44, 0x37,
    0x6A, 0x61, 0x67, 0x70, 0x66, 0x2B, 0x45, 0x43, 0x30, 0x44, 0x6E, 0x67,
    0x6C, 0x54, 0x65, 0x48, 0x48, 0x30, 0x62, 0x31, 0x6A, 0x33, 0x31, 0x6C,
    0x58, 0x74, 0x79, 0x56, 0x67, 0x48, 0x52, 0x4F, 0x7A, 0x0D, 0x0A, 0x53,
    0x51, 0x66, 0x76, 0x4C, 0x31, 0x63, 0x6F, 0x72, 0x62, 0x52, 0x4D, 0x52,
    0x52, 0x6D, 0x32, 0x2B, 0x4D, 0x59, 0x32, 0x58, 0x4D, 0x62, 0x32, 0x51,
    0x72, 0x55, 0x2B, 0x59, 0x51, 0x38, 0x35, 0x7A, 0x68, 0x65, 0x62, 0x56,
    0x36, 0x63, 0x44, 0x30, 0x48, 0x52, 0x43, 0x51, 0x44, 0x45, 0x32, 0x6C,
    0x53, 0x37, 0x35, 0x7A, 0x33, 0x47, 0x2F, 0x64, 0x64, 0x72, 0x63, 0x38,
    0x4B, 0x63, 0x67, 0x0D, 0x0A, 0x46, 0x49, 0x34, 0x74, 0x55, 0x48, 0x57,
    0x63, 0x39, 0x4C, 0x31, 0x4B, 0x35, 0x6E, 0x5A, 0x65, 0x33, 0x6E, 0x34,
    0x78, 0x4E, 0x56, 0x71, 0x37, 0x37, 0x6D, 0x54, 0x49, 0x45, 0x6C, 0x65,
    0x65, 0x57, 0x57, 0x55, 0x6E, 0x5A, 0x52, 0x4E, 0x75, 0x33, 0x6F, 0x6E,
    0x74, 0x30, 0x2B, 0x48, 0x32, 0x4C, 0x54, 0x4A, 0x70, 0x5A, 0x45, 0x39,
    0x57, 0x69, 0x46, 0x4C, 0x38, 0x35, 0x4D, 0x65, 0x76, 0x0D, 0x0A, 0x72,
    0x59, 0x7A, 0x58, 0x45, 0x2F, 0x54, 0x69, 0x72, 0x59, 0x36, 0x52, 0x44,
    0x52, 0x56, 0x78, 0x4B, 0x63, 0x45, 0x43, 0x67, 0x63, 0x42, 0x58, 0x6C,
    0x53, 0x6C, 0x6C, 0x45, 0x6F, 0x54, 0x4F, 0x41, 0x39, 0x77, 0x57, 0x66,
    0x6B, 0x57, 0x77, 0x4A, 0x69, 0x2B, 0x32, 0x56, 0x56, 0x4F, 0x69, 0x7A,
    0x34, 0x75, 0x38, 0x72, 0x6E, 0x37, 0x6C, 0x34, 0x36, 0x4A, 0x39, 0x78,
    0x61, 0x35, 0x51, 0x0D, 0x0A, 0x71, 0x62, 0x59, 0x45, 0x6F, 0x33, 0x66,
    0x5A, 0x30, 0x47, 0x59, 0x41, 0x54, 0x44, 0x67, 0x36, 0x78, 0x52, 0x43,
    0x57, 0x32, 0x75, 0x62, 0x74, 0x2B, 0x6B, 0x39, 0x42, 0x44, 0x46, 0x4D,
    0x46, 0x73, 0x52, 0x34, 0x6B, 0x39, 0x31, 0x45, 0x78, 0x63, 0x42, 0x54,
    0x44, 0x65, 0x2B, 0x6B, 0x36, 0x6B, 0x35, 0x47, 0x54, 0x72, 0x78, 0x5A,
    0x66, 0x4D, 0x75, 0x56, 0x2F, 0x31, 0x58, 0x67, 0x5A, 0x0D, 0x0A, 0x38,
    0x5A, 0x79, 0x37, 0x38, 0x4D, 0x64, 0x36, 0x32, 0x49, 0x4D, 0x78, 0x62,
    0x71, 0x34, 0x6E, 0x56, 0x63, 0x44, 0x52, 0x74, 0x4E, 0x70, 0x70, 0x35,
    0x34, 0x33, 0x59, 0x57, 0x70, 0x71, 0x77, 0x63, 0x67, 0x64, 0x79, 0x6F,
    0x78, 0x4A, 0x6F, 0x4D, 0x6E, 0x37, 0x50, 0x50, 0x54, 0x77, 0x67, 0x4C,
    0x37, 0x43, 0x4B, 0x4B, 0x74, 0x36, 0x58, 0x65, 0x68, 0x76, 0x75, 0x62,
    0x41, 0x49, 0x45, 0x0D, 0x0A, 0x35, 0x65, 0x31, 0x49, 0x34, 0x58, 0x7A,
    0x54, 0x57, 0x6F, 0x53, 0x54, 0x54, 0x61, 0x4C, 0x48, 0x57, 0x56, 0x52,
    0x47, 0x41, 0x41, 0x2F, 0x70, 0x33, 0x56, 0x75, 0x37, 0x53, 0x63, 0x36,
    0x7A, 0x79, 0x54, 0x78, 0x51, 0x6A, 0x4A, 0x73, 0x52, 0x78, 0x42, 0x41,
    0x74, 0x5A, 0x47, 0x47, 0x53, 0x66, 0x76, 0x42, 0x5A, 0x6B, 0x57, 0x4B,
    0x2B, 0x44, 0x75, 0x71, 0x4F, 0x76, 0x6F, 0x36, 0x38, 0x0D, 0x0A, 0x44,
    0x4B, 0x4B, 0x38, 0x51, 0x73, 0x76, 0x43, 0x38, 0x6F, 0x44, 0x47, 0x72,
    0x6B, 0x5A, 0x49, 0x43, 0x52, 0x49, 0x43, 0x7A, 0x37, 0x34, 0x3D, 0x0D,
    0x0A, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x45, 0x4E, 0x44, 0x20, 0x50, 0x52,
    0x49, 0x56, 0x41, 0x54, 0x45, 0x20, 0x4B, 0x45, 0x59, 0x2D, 0x2D, 0x2D,
    0x2D, 0x2D, 0x0D, 0x0A
};


/* password for private key retrieving from encrypted PEM ("m_libspdm_test_key_pem").*/

LIBSPDM_GLOBAL_REMOVE_IF_UNREFERENCED const char *m_libspdm_pem_pass = "client";


/* message hash for Signing & Verification Validation.*/

LIBSPDM_GLOBAL_REMOVE_IF_UNREFERENCED uint8_t m_libspdm_msg_hash[] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0x01,
    0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0x01
};


/* payload for PKCS#7 Signing & Verification Validation.*/

LIBSPDM_GLOBAL_REMOVE_IF_UNREFERENCED const char *m_libspdm_payload =
    "payload data for PKCS#7 Signing";

/**
 * Validate Crypto RSA key Retrieving & signature Interfaces.
 *
 * @retval  true  Validation succeeded.
 * @retval  false  Validation failed.
 *
 **/
bool libspdm_validate_crypt_rsa_2(void)
{
    bool status;
    void *rsa_priv_key;
    void *rsa_pub_key;
    uint8_t *signature;
    size_t sig_size;

    libspdm_my_print("\nCrypto RSA key Retrieving Testing: ");


    /* Retrieve RSA private key from encrypted PEM data.*/

    libspdm_my_print("\n- Retrieve RSA Private key for PEM ...");
    status = libspdm_rsa_get_private_key_from_pem(m_libspdm_test_key_pem,
                                                  sizeof(m_libspdm_test_key_pem),
                                                  m_libspdm_pem_pass, &rsa_priv_key);
    if (!status) {
        libspdm_my_print("[Fail]");
        return false;
    } else {
        libspdm_my_print("[Pass]");
    }


    /* Retrieve RSA public key from X509 Certificate.*/

    libspdm_my_print("\n- Retrieve RSA public key from X509 ... ");
    rsa_pub_key = NULL;
    status = libspdm_rsa_get_public_key_from_x509(m_libspdm_test_cert, sizeof(m_libspdm_test_cert),
                                                  &rsa_pub_key);
    if (!status) {
        libspdm_my_print("[Fail]");
        libspdm_rsa_free(rsa_priv_key);
        return false;
    } else {
        libspdm_my_print("[Pass]");
    }


    /* Generate RSA PKCS#1 signature.*/

    libspdm_my_print("\n- PKCS#1 signature ... ");
    sig_size = 0;
    status = libspdm_rsa_pkcs1_sign_with_nid(rsa_priv_key, LIBSPDM_CRYPTO_NID_SHA256,
                                             m_libspdm_msg_hash, LIBSPDM_SHA256_DIGEST_SIZE, NULL,
                                             &sig_size);
    if (status || sig_size == 0) {
        libspdm_my_print("[Fail]");
        libspdm_rsa_free(rsa_pub_key);
        libspdm_rsa_free(rsa_priv_key);
        return false;
    }

    signature = allocate_pool(sig_size);
    status = libspdm_rsa_pkcs1_sign_with_nid(rsa_priv_key, LIBSPDM_CRYPTO_NID_SHA256,
                                             m_libspdm_msg_hash, LIBSPDM_SHA256_DIGEST_SIZE,
                                             signature, &sig_size);
    if (!status) {
        libspdm_my_print("[Fail]");
        free_pool(signature);
        libspdm_rsa_free(rsa_pub_key);
        libspdm_rsa_free(rsa_priv_key);
        return false;
    } else {
        libspdm_my_print("[Pass]");
    }


    /* Verify RSA PKCS#1-encoded signature.*/

    libspdm_my_print("\n- PKCS#1 signature Verification ... ");
    status = libspdm_rsa_pkcs1_verify_with_nid(rsa_pub_key, LIBSPDM_CRYPTO_NID_SHA256,
                                               m_libspdm_msg_hash, LIBSPDM_SHA256_DIGEST_SIZE,
                                               signature, sig_size);
    if (!status) {
        libspdm_my_print("[Fail]");
        free_pool(signature);
        libspdm_rsa_free(rsa_pub_key);
        libspdm_rsa_free(rsa_priv_key);
        return false;
    } else {
        libspdm_my_print("[Pass]");
    }

    free_pool(signature);


    /* X509 Certificate subject Retrieving.*/

    /* libspdm_my_print ("\n- X509 Certificate subject Bytes Retrieving ... ");
     * subject_size = 0;
     * status  = libspdm_x509_get_subject_name (m_libspdm_test_cert, sizeof (m_libspdm_test_cert), NULL, &subject_size);
     * subject = (uint8_t *)allocate_pool (subject_size);
     * status  = libspdm_x509_get_subject_name (m_libspdm_test_cert, sizeof (m_libspdm_test_cert), subject, &subject_size);
     * if (!status) {
     *   libspdm_my_print ("[Fail]");
     *   libspdm_rsa_free  (rsa_pub_key);
     *   libspdm_rsa_free  (rsa_priv_key);
     *   return false;
     * } else {
     *   libspdm_my_print ("[Pass]");
     * }*/


    /* Get common_name from X509 Certificate subject*/

    /* common_name_size = 64;
     * libspdm_zero_mem (common_name, common_name_size);
     * status = libspdm_x509_get_common_name (m_libspdm_test_cert, sizeof (m_libspdm_test_cert), common_name, &common_name_size);
     * if (!status) {
     *   libspdm_my_print ("\n  - Retrieving Common name - [Fail]");
     *   libspdm_rsa_free  (rsa_pub_key);
     *   libspdm_rsa_free  (rsa_priv_key);
     *   return false;
     * } else {
     *   libspdm_my_print ("\n  - Retrieving Common name = \"%a\" (size = %d)", common_name, common_name_size);
     * }*/


    /* Release Resources.*/

    libspdm_rsa_free(rsa_pub_key);
    libspdm_rsa_free(rsa_priv_key);
    /* free_pool (subject);*/

    return true;
}
