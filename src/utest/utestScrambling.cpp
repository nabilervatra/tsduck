//----------------------------------------------------------------------------
//
// TSDuck - The MPEG Transport Stream Toolkit
// Copyright (c) 2005-2017, Thierry Lelegard
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
//
//----------------------------------------------------------------------------
//
//  CppUnit test suite for class ts::Scrambling
//
//----------------------------------------------------------------------------

#include "tsScrambling.h"
#include "tsTSPacket.h"
#include "tsNames.h"
#include "utestCppUnitTest.h"
TSDUCK_SOURCE;


//----------------------------------------------------------------------------
// The test fixture
//----------------------------------------------------------------------------

class ScramblingTest: public CppUnit::TestFixture
{
public:
    void setUp();
    void tearDown();
    void testScrambling();

    CPPUNIT_TEST_SUITE(ScramblingTest);
    CPPUNIT_TEST(testScrambling);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(ScramblingTest);


//----------------------------------------------------------------------------
// Initialization.
//----------------------------------------------------------------------------

// Test suite initialization method.
void ScramblingTest::setUp()
{
}

// Test suite cleanup method.
void ScramblingTest::tearDown()
{
}


//----------------------------------------------------------------------------
// Unitary tests.
//----------------------------------------------------------------------------

// Test vectors for DVB-CSA
//
// Extracted from TNT R4, 2007/11/30, service Paris Premiere
// Control Words:
//   CP1: even: C0 B1 F0 61 A6 ED 71 04    odd: B2 92 D3 17 7C CC CE 16
//   CP2: even: A6 34 69 43 D3 EE 85 46    odd: B2 92 D3 17 7C CC CE 16
//   CP3: even: A6 34 69 43 D3 EE 85 46    odd: 03 0B 48 56 54 37 75 00

namespace {
    struct ScramblingTestVector
    {
        uint8_t        cw_even[ts::CW_BYTES];
        uint8_t        cw_odd[ts::CW_BYTES];
        ts::TSPacket plain;
        ts::TSPacket cipher;
    };

    const ScramblingTestVector scrambling_test_vectors[] = {
        // Payload size: 184 bytes, no residue
        {
            // cw_even
            {0xC0, 0xB1, 0xF0, 0x61, 0xA6, 0xED, 0x71, 0x04},
            // cw_odd
            {0xB2, 0x92, 0xD3, 0x17, 0x7C, 0xCC, 0xCE, 0x16},
            // plain
            {{
                0x47, 0x01, 0xA4, 0x10, 0x81, 0xDB, 0xEF, 0xC2, 0x7A, 0xAE, 0x3F, 0xB0, 0x41, 0x48, 0x1E, 0x0D,
                0xA8, 0xC0, 0x76, 0x4A, 0xB3, 0x0F, 0xCB, 0x33, 0x94, 0xA5, 0x79, 0x9C, 0xAD, 0x9D, 0xA2, 0x79,
                0xA9, 0x25, 0x89, 0x71, 0xB2, 0xE8, 0xE5, 0x06, 0x1F, 0xDA, 0x3B, 0xBF, 0x32, 0xD2, 0x02, 0xDF,
                0xC0, 0x2D, 0xD3, 0x9A, 0xE3, 0x29, 0xE7, 0x7E, 0xDF, 0x21, 0x74, 0x36, 0x35, 0x33, 0x1D, 0x5A,
                0xC6, 0xB1, 0xB8, 0x4C, 0x88, 0x5D, 0x71, 0x9B, 0xBE, 0x77, 0x7C, 0x29, 0xBF, 0x44, 0xA9, 0xF0,
                0xFA, 0xBA, 0x07, 0x69, 0xAD, 0xC7, 0x0E, 0xF8, 0x51, 0x71, 0xA0, 0x57, 0xE5, 0x73, 0xE6, 0x39,
                0xCC, 0xC0, 0xE8, 0x9F, 0x7C, 0xF1, 0x23, 0x06, 0xEC, 0x5D, 0x9B, 0x4B, 0xF3, 0x53, 0xB7, 0x92,
                0xD4, 0x71, 0xC9, 0xEB, 0xFF, 0xAE, 0x47, 0xF3, 0x4D, 0x63, 0x28, 0x6A, 0xD6, 0x39, 0x11, 0x97,
                0x94, 0xA5, 0x5C, 0x21, 0x65, 0x19, 0x32, 0x24, 0xFA, 0x41, 0xEB, 0x42, 0xCA, 0x44, 0x93, 0xA1,
                0xD5, 0xE9, 0xE0, 0x37, 0x02, 0x91, 0xC6, 0xE0, 0x5E, 0x0B, 0xA9, 0xDC, 0x0E, 0xFE, 0xA5, 0xF9,
                0x47, 0x5B, 0x5A, 0x45, 0x7F, 0x4B, 0x79, 0x31, 0xAE, 0xB6, 0xDE, 0x9C, 0x0C, 0x40, 0x4B, 0x93,
                0x50, 0x3A, 0xD5, 0x7B, 0x8A, 0x00, 0x74, 0xEB, 0xB6, 0x43, 0x12, 0xF2,
            }},
            // cipher
            {{
                0x47, 0x01, 0xA4, 0x90, 0xE3, 0x4B, 0x70, 0x1E, 0x54, 0x16, 0x5B, 0x14, 0x07, 0xC5, 0x81, 0xAA,
                0x37, 0xC7, 0x00, 0x9E, 0xC9, 0x85, 0x83, 0x79, 0xF1, 0x85, 0x3B, 0x4E, 0x9C, 0xFA, 0x0D, 0x50,
                0xA7, 0x0E, 0xF6, 0x95, 0x7B, 0x1C, 0xFB, 0xD1, 0x31, 0xF1, 0xA9, 0x99, 0x54, 0x91, 0x55, 0xBA,
                0x41, 0x23, 0x0E, 0xFE, 0x02, 0x91, 0x90, 0x58, 0x38, 0x50, 0x7A, 0x40, 0x8A, 0xA9, 0x79, 0x00,
                0x3C, 0x55, 0xB6, 0x07, 0xFD, 0x4E, 0xED, 0xFE, 0xA3, 0x93, 0x22, 0x6A, 0x2A, 0xB1, 0xA2, 0x43,
                0x6F, 0x32, 0xE2, 0x97, 0x29, 0xE7, 0xF5, 0x6A, 0x43, 0xF4, 0x7A, 0x6C, 0x64, 0x55, 0x64, 0x04,
                0xA3, 0x77, 0x28, 0x98, 0xB1, 0x6F, 0xC5, 0x07, 0xF7, 0xD5, 0x97, 0xBE, 0x6B, 0x73, 0xF5, 0x15,
                0x8A, 0xBA, 0x80, 0xB7, 0x14, 0xC8, 0x2E, 0x34, 0x69, 0x47, 0x09, 0x9B, 0x53, 0xB3, 0x0E, 0xFD,
                0x28, 0x81, 0x2E, 0xFF, 0xD6, 0x6E, 0xAE, 0x5C, 0xBC, 0x91, 0xCE, 0x5B, 0x98, 0xA2, 0xED, 0x6B,
                0x05, 0x53, 0x61, 0x30, 0xD1, 0xAD, 0x73, 0x25, 0xB0, 0x9A, 0x4F, 0xFC, 0x5E, 0x70, 0xDD, 0x51,
                0x7B, 0x2D, 0xC2, 0x7D, 0xAF, 0x70, 0xE1, 0x58, 0x98, 0xF6, 0x96, 0x66, 0x84, 0x06, 0x00, 0x6C,
                0x4D, 0x8D, 0x65, 0xFE, 0x6F, 0x6A, 0x83, 0xBC, 0xF5, 0xCF, 0xF0, 0x5A,
            }},
        },
        // Payload size: 108 bytes, residue: 8 bytes
        {
            // cw_even
            {0xC0, 0xB1, 0xF0, 0x61, 0xA6, 0xED, 0x71, 0x04},
            // cw_odd
            {0xB2, 0x92, 0xD3, 0x17, 0x7C, 0xCC, 0xCE, 0x16},
            // plain
            {{
                0x47, 0x01, 0xA4, 0x31, 0x4B, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0x62, 0x34, 0xAC, 0x56, 0xDB, 0x0E, 0x38, 0x37, 0xE5, 0x7B, 0x7E, 0x6A, 0xDB, 0xC1, 0x6F, 0x44,
                0xF3, 0xDB, 0xF6, 0xFE, 0x0E, 0xD7, 0x2F, 0xF8, 0xEA, 0xCF, 0x67, 0x55, 0x8A, 0xAF, 0x3F, 0x12,
                0x54, 0x5C, 0x87, 0x4F, 0x0D, 0xA9, 0x47, 0x6E, 0xDC, 0xC2, 0x60, 0x12, 0xC7, 0x45, 0xAA, 0xA8,
                0xFD, 0x14, 0xEB, 0x63, 0x6A, 0x87, 0x4E, 0xC3, 0xFA, 0x99, 0xA3, 0x04, 0x71, 0x3B, 0x98, 0x22,
                0x90, 0xC1, 0xFC, 0x24, 0x99, 0x97, 0xA3, 0xD8, 0x15, 0x7E, 0x9F, 0x6F, 0x2A, 0x10, 0x62, 0x61,
                0x65, 0x32, 0x01, 0xC7, 0xEA, 0x48, 0xC0, 0xC9, 0xA1, 0xB8, 0x8F, 0x8B, 0xDE, 0x4F, 0xC9, 0x49,
                0x8A, 0x32, 0xB2, 0x9A, 0xC5, 0xAB, 0x63, 0x22, 0x6F, 0x82, 0xD2, 0x77,
            }},
            // cipher
            {{
                0x47, 0x01, 0xA4, 0xB1, 0x4B, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xA1, 0x30, 0x42, 0x32, 0x1C, 0xD1, 0x98, 0x38, 0x1C, 0x80, 0x71, 0xF3, 0xFC, 0x60, 0xB3, 0x28,
                0xB9, 0xDD, 0x2C, 0x79, 0x0C, 0x61, 0x00, 0xFD, 0xBF, 0x94, 0x5A, 0xEC, 0xED, 0x00, 0xCD, 0x82,
                0x33, 0xCC, 0x86, 0xEC, 0xD6, 0x1C, 0x92, 0xF6, 0xEE, 0xE3, 0xD2, 0x85, 0xFD, 0xB7, 0xD4, 0xA3,
                0x37, 0xE3, 0x1C, 0xFA, 0x3A, 0x9B, 0x6A, 0xCE, 0xCD, 0x93, 0x18, 0x28, 0xEF, 0xCE, 0x22, 0x5B,
                0x64, 0x18, 0xE4, 0x72, 0xBF, 0x4F, 0xF6, 0xE5, 0xD6, 0x49, 0x51, 0xB2, 0xFB, 0xF4, 0x06, 0x1C,
                0xA8, 0x49, 0xFB, 0x9C, 0x18, 0x5F, 0x8B, 0xE2, 0x3B, 0x07, 0xED, 0x0A, 0x16, 0xA6, 0x50, 0x91,
                0x0F, 0xB8, 0xA1, 0x42, 0x95, 0x86, 0x65, 0x25, 0xDF, 0xB0, 0x07, 0xA5,
            }},
        },
    };
}

void ScramblingTest::testScrambling()
{
    const ScramblingTestVector* vec = scrambling_test_vectors;
    size_t count = sizeof(scrambling_test_vectors) / sizeof(ScramblingTestVector);
    ts::Scrambling scrambler;
    ts::TSPacket pkt;

    utest::Out() << "ScramblingTest: " << count << " test vectors" << std::endl;

    for (size_t ti = 0; ti < count; ++ti, ++vec) {

        const size_t header_size = vec->plain.getHeaderSize();
        const size_t payload_size = vec->plain.getPayloadSize();
        const uint8_t scv = vec->cipher.getScrambling();

        utest::Out() << "ScramblingTest: " << ti << ", header: " << header_size <<
              " byte, payload: " << payload_size <<
              " bytes, PID: " << vec->plain.getPID() <<
              ", scrambling: " << ts::names::ScramblingControl (scv) << std::endl;

        CPPUNIT_ASSERT(header_size == vec->cipher.getHeaderSize());
        CPPUNIT_ASSERT(payload_size == vec->cipher.getPayloadSize());
        CPPUNIT_ASSERT(header_size + payload_size == ts::PKT_SIZE);
        CPPUNIT_ASSERT(scv == ts::SC_EVEN_KEY || scv == ts::SC_ODD_KEY);
        CPPUNIT_ASSERT(vec->plain.getScrambling() == ts::SC_CLEAR);

        scrambler.init (scv == ts::SC_EVEN_KEY ? vec->cw_even : vec->cw_odd, ts::Scrambling::REDUCE_ENTROPY);

        // Descrambling test
        pkt = vec->cipher;
        scrambler.decrypt(pkt.b + header_size, payload_size);
        CPPUNIT_ASSERT(::memcmp (pkt.b + header_size, vec->plain.b + header_size, payload_size) == 0);

        // Scrambling test
        pkt = vec->plain;
        scrambler.encrypt(pkt.b + header_size, payload_size);
        CPPUNIT_ASSERT(::memcmp(pkt.b + header_size, vec->cipher.b + header_size, payload_size) == 0);
    }
}
