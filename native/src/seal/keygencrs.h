//
// Created by lpsun on 23.05.19.
//

#pragma once

#include "seal/randomgen.h"
#include "seal/plaintext.h"
#include "seal/memorymanager.h"
#include "seal/util/common.h"
#include "ciphertext.h"
#include "publickey.h"
#include <iostream>
#include <cstdint>
#include <cstddef>
#include <memory>
#include <random>

namespace seal
{
    /**
    Class to store a CRS value which is used for public key generation. Normally this
    value is randomly generated inside the pub key generation procedure. But for some
    more sophisticated usages (e.g. to construct a distributed key generation protocol)
    we may want to make this value configurable. Actually it is named as 'a' in the
    SEAL manual, but here, for readability purpose, we will call it CRS value, because
    we are going to use it as a Common Reference String for our distributed key generation
    protocol.


    @par Thread Safety
    In general, reading from KeyGenCRS is thread-safe as long as no other thread
    is concurrently mutating it. This is due to the underlying data structure
    storing the data not being thread-safe.


    @see KeyGenerator for the class that generates the KeyGenCRS.
    @see PublicKey for the class that uses KeyGenCRS.
    */
    class KeyGenCRS : public PublicKey
    {
    public:

        /**
         * Creates an empty CRS value for key generation
         */
         KeyGenCRS() = default;

        /**
        Creates a new KeyGenCRS by copying an old one.

        @param[in] copy The KeyGenCRS to copy from
        */
        KeyGenCRS(const KeyGenCRS &copy) = default;

        /**
        Creates a new KeyGenCRS by moving an old one.

        @param[in] source The KeyGenCRS to move from
        */
        KeyGenCRS(KeyGenCRS &&source) = default;

        /**
        Copies an old KeyGenCRS to the current one.

        @param[in] assign The KeyGenCRS to copy from
        */
        KeyGenCRS &operator =(const KeyGenCRS &assign) = default;

        /**
        Moves an old KeyGenCRS to the current one.

        @param[in] assign The KeyGenCRS to move from
        */
        KeyGenCRS &operator =(KeyGenCRS &&assign) = default;
    };
}