//----------------------------------------------------------------------------
//
// TSDuck - The MPEG Transport Stream Toolkit
// Copyright (c) 2005-2024, Thierry Lelegard
// BSD-2-Clause license, see LICENSE.txt file or https://tsduck.io/license
//
//----------------------------------------------------------------------------

#include "tsSHA256.h"
#include "tsSingleton.h"
#include "tsInitCryptoLibrary.h"

#if defined(TS_WINDOWS)

TS_STATIC_INSTANCE(ts::FetchBCryptAlgorithm, (BCRYPT_SHA256_ALGORITHM), Fetch);

void ts::SHA256::getAlgorithm(::BCRYPT_ALG_HANDLE& algo, size_t& length) const
{
    Fetch::Instance().getAlgorithm(algo, length);
}

#else

// The singleton needs to be destroyed no later that OpenSSL cleanup.
TS_STATIC_INSTANCE_ATEXIT(ts::FetchHashAlgorithm, ("SHA256"), Preset, OPENSSL_atexit);

const EVP_MD_CTX* ts::SHA256::referenceContext() const
{
    return Preset::Instance().referenceContext();
}

#endif
