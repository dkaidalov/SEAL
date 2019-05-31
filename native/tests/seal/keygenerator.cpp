// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#include "gtest/gtest.h"
#include "seal/context.h"
#include "seal/keygenerator.h"
#include "seal/util/polycore.h"
#include "seal/defaultparams.h"
#include "seal/encryptor.h"
#include "seal/decryptor.h"
#include "seal/evaluator.h"

using namespace seal;
using namespace seal::util;
using namespace std;

namespace SEALTest
{
    TEST(KeyGeneratorTest, FVKeyGeneration)
    {
        EncryptionParameters parms(scheme_type::BFV);
        {
            parms.set_noise_standard_deviation(3.20);
            parms.set_poly_modulus_degree(64);
            parms.set_plain_modulus(1 << 6);
            parms.set_coeff_modulus({ DefaultParams::small_mods_60bit(0) });
            auto context = SEALContext::Create(parms);
            KeyGenerator keygen(context);

            RelinKeys evk = keygen.relin_keys(60);
            ASSERT_TRUE(evk.parms_id() == parms.parms_id());
            ASSERT_EQ(2ULL, evk.key(2)[0].size());
            for (size_t j = 0; j < evk.size(); j++)
            {
                for (size_t i = 0; i < evk.key(j + 2).size(); i++)
                {
                    for (size_t k = 0; k < evk.key(j + 2)[i].size(); k++)
                    {
                        ASSERT_FALSE(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                    }
                }
            }

            evk = keygen.relin_keys(30, 1);
            ASSERT_TRUE(evk.parms_id() == parms.parms_id());
            ASSERT_EQ(4ULL, evk.key(2)[0].size());
            for (size_t j = 0; j < evk.size(); j++)
            {
                for (size_t i = 0; i < evk.key(j + 2).size(); i++)
                {
                    for (size_t k = 0; k < evk.key(j + 2)[i].size(); k++)
                    {
                        ASSERT_FALSE(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                    }
                }
            }

            evk = keygen.relin_keys(2, 2);
            ASSERT_TRUE(evk.parms_id() == parms.parms_id());
            ASSERT_EQ(60ULL, evk.key(2)[0].size());
            for (size_t j = 0; j < evk.size(); j++)
            {
                for (size_t i = 0; i < evk.key(j + 2).size(); i++)
                {
                    for (size_t k = 0; k < evk.key(j + 2)[i].size(); k++)
                    {
                        ASSERT_FALSE(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                    }
                }
            }

            GaloisKeys galks = keygen.galois_keys(60);
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_EQ(2ULL, galks.key(3)[0].size());
            ASSERT_EQ(10ULL, galks.size());

            galks = keygen.galois_keys(30);
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_EQ(4ULL, galks.key(3)[0].size());
            ASSERT_EQ(10ULL, galks.size());

            galks = keygen.galois_keys(2);
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_EQ(60ULL, galks.key(3)[0].size());
            ASSERT_EQ(10ULL, galks.size());

            galks = keygen.galois_keys(60, vector<uint64_t>{ 1, 3, 5, 7 });
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_TRUE(galks.has_key(1));
            ASSERT_TRUE(galks.has_key(3));
            ASSERT_TRUE(galks.has_key(5));
            ASSERT_TRUE(galks.has_key(7));
            ASSERT_FALSE(galks.has_key(9));
            ASSERT_FALSE(galks.has_key(127));
            ASSERT_EQ(2ULL, galks.key(1)[0].size());
            ASSERT_EQ(2ULL, galks.key(3)[0].size());
            ASSERT_EQ(2ULL, galks.key(5)[0].size());
            ASSERT_EQ(2ULL, galks.key(7)[0].size());
            ASSERT_EQ(4ULL, galks.size());

            galks = keygen.galois_keys(30, vector<uint64_t>{ 1, 3, 5, 7 });
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_TRUE(galks.has_key(1));
            ASSERT_TRUE(galks.has_key(3));
            ASSERT_TRUE(galks.has_key(5));
            ASSERT_TRUE(galks.has_key(7));
            ASSERT_FALSE(galks.has_key(9));
            ASSERT_FALSE(galks.has_key(127));
            ASSERT_EQ(4ULL, galks.key(1)[0].size());
            ASSERT_EQ(4ULL, galks.key(3)[0].size());
            ASSERT_EQ(4ULL, galks.key(5)[0].size());
            ASSERT_EQ(4ULL, galks.key(7)[0].size());
            ASSERT_EQ(4ULL, galks.size());

            galks = keygen.galois_keys(2, vector<uint64_t>{ 1, 3, 5, 7 });
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_TRUE(galks.has_key(1));
            ASSERT_TRUE(galks.has_key(3));
            ASSERT_TRUE(galks.has_key(5));
            ASSERT_TRUE(galks.has_key(7));
            ASSERT_FALSE(galks.has_key(9));
            ASSERT_FALSE(galks.has_key(127));
            ASSERT_EQ(60ULL, galks.key(1)[0].size());
            ASSERT_EQ(60ULL, galks.key(3)[0].size());
            ASSERT_EQ(60ULL, galks.key(5)[0].size());
            ASSERT_EQ(60ULL, galks.key(7)[0].size());
            ASSERT_EQ(4ULL, galks.size());

            galks = keygen.galois_keys(30, vector<uint64_t>{ 1 });
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_TRUE(galks.has_key(1));
            ASSERT_FALSE(galks.has_key(3));
            ASSERT_FALSE(galks.has_key(127));
            ASSERT_EQ(4ULL, galks.key(1)[0].size());
            ASSERT_EQ(1ULL, galks.size());

            galks = keygen.galois_keys(30, vector<uint64_t>{ 127 });
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_FALSE(galks.has_key(1));
            ASSERT_TRUE(galks.has_key(127));
            ASSERT_EQ(4ULL, galks.key(127)[0].size());
            ASSERT_EQ(1ULL, galks.size());
        }
        {
            parms.set_noise_standard_deviation(3.20);
            parms.set_poly_modulus_degree(256);
            parms.set_plain_modulus(1 << 6);
            parms.set_coeff_modulus({ DefaultParams::small_mods_60bit(0), DefaultParams::small_mods_30bit(0), DefaultParams::small_mods_30bit(1) });
            auto context = SEALContext::Create(parms);
            KeyGenerator keygen(context);

            RelinKeys evk = keygen.relin_keys(60, 2);
            ASSERT_TRUE(evk.parms_id() == parms.parms_id());
            ASSERT_EQ(2ULL, evk.key(2)[0].size());
            for (size_t j = 0; j < evk.size(); j++)
            {
                for (size_t i = 0; i < evk.key(j + 2).size(); i++)
                {
                    for (size_t k = 0; k < evk.key(j + 2)[i].size(); k++)
                    {
                        ASSERT_FALSE(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                    }
                }
            }

            evk = keygen.relin_keys(30, 2);
            ASSERT_TRUE(evk.parms_id() == parms.parms_id());
            ASSERT_EQ(4ULL, evk.key(2)[0].size());
            for (size_t j = 0; j < evk.size(); j++)
            {
                for (size_t i = 0; i < evk.key(j + 2).size(); i++)
                {
                    for (size_t k = 0; k < evk.key(j + 2)[i].size(); k++)
                    {
                        ASSERT_FALSE(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                    }
                }
            }

            evk = keygen.relin_keys(4, 1);
            ASSERT_TRUE(evk.parms_id() == parms.parms_id());
            ASSERT_EQ(30ULL, evk.key(2)[0].size());
            for (size_t j = 0; j < evk.size(); j++)
            {
                for (size_t i = 0; i < evk.key(j + 2).size(); i++)
                {
                    for (size_t k = 0; k < evk.key(j + 2)[i].size(); k++)
                    {
                        ASSERT_FALSE(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                    }
                }
            }

            GaloisKeys galks = keygen.galois_keys(60);
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_EQ(2ULL, galks.key(3)[0].size());
            ASSERT_EQ(14ULL, galks.size());

            galks = keygen.galois_keys(30);
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_EQ(4ULL, galks.key(3)[0].size());
            ASSERT_EQ(14ULL, galks.size());

            galks = keygen.galois_keys(2);
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_EQ(60ULL, galks.key(3)[0].size());
            ASSERT_EQ(14ULL, galks.size());

            galks = keygen.galois_keys(60, vector<uint64_t>{ 1, 3, 5, 7 });
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_TRUE(galks.has_key(1));
            ASSERT_TRUE(galks.has_key(3));
            ASSERT_TRUE(galks.has_key(5));
            ASSERT_TRUE(galks.has_key(7));
            ASSERT_FALSE(galks.has_key(9));
            ASSERT_FALSE(galks.has_key(511));
            ASSERT_EQ(2ULL, galks.key(1)[0].size());
            ASSERT_EQ(2ULL, galks.key(3)[0].size());
            ASSERT_EQ(2ULL, galks.key(5)[0].size());
            ASSERT_EQ(2ULL, galks.key(7)[0].size());
            ASSERT_EQ(4ULL, galks.size());

            galks = keygen.galois_keys(30, vector<uint64_t>{ 1, 3, 5, 7 });
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_TRUE(galks.has_key(1));
            ASSERT_TRUE(galks.has_key(3));
            ASSERT_TRUE(galks.has_key(5));
            ASSERT_TRUE(galks.has_key(7));
            ASSERT_FALSE(galks.has_key(9));
            ASSERT_FALSE(galks.has_key(511));
            ASSERT_EQ(4ULL, galks.key(1)[0].size());
            ASSERT_EQ(4ULL, galks.key(3)[0].size());
            ASSERT_EQ(4ULL, galks.key(5)[0].size());
            ASSERT_EQ(4ULL, galks.key(7)[0].size());
            ASSERT_EQ(4ULL, galks.size());

            galks = keygen.galois_keys(2, vector<uint64_t>{ 1, 3, 5, 7 });
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_TRUE(galks.has_key(1));
            ASSERT_TRUE(galks.has_key(3));
            ASSERT_TRUE(galks.has_key(5));
            ASSERT_TRUE(galks.has_key(7));
            ASSERT_FALSE(galks.has_key(9));
            ASSERT_FALSE(galks.has_key(511));
            ASSERT_EQ(60ULL, galks.key(1)[0].size());
            ASSERT_EQ(60ULL, galks.key(3)[0].size());
            ASSERT_EQ(60ULL, galks.key(5)[0].size());
            ASSERT_EQ(60ULL, galks.key(7)[0].size());
            ASSERT_EQ(4ULL, galks.size());

            galks = keygen.galois_keys(30, vector<uint64_t>{ 1 });
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_TRUE(galks.has_key(1));
            ASSERT_FALSE(galks.has_key(3));
            ASSERT_FALSE(galks.has_key(511));
            ASSERT_EQ(4ULL, galks.key(1)[0].size());
            ASSERT_EQ(1ULL, galks.size());

            galks = keygen.galois_keys(30, vector<uint64_t>{ 511 });
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_FALSE(galks.has_key(1));
            ASSERT_TRUE(galks.has_key(511));
            ASSERT_EQ(4ULL, galks.key(511)[0].size());
            ASSERT_EQ(1ULL, galks.size());
        }
    }

    TEST(KeyGeneratorTest, CKKSKeyGeneration)
    {
        EncryptionParameters parms(scheme_type::CKKS);
        {
            parms.set_noise_standard_deviation(3.20);
            parms.set_poly_modulus_degree(64);
            parms.set_coeff_modulus({ DefaultParams::small_mods_60bit(0) });
            auto context = SEALContext::Create(parms);
            KeyGenerator keygen(context);

            RelinKeys evk = keygen.relin_keys(60);
            ASSERT_TRUE(evk.parms_id() == parms.parms_id());
            ASSERT_EQ(2ULL, evk.key(2)[0].size());
            for (size_t j = 0; j < evk.size(); j++)
            {
                for (size_t i = 0; i < evk.key(j + 2).size(); i++)
                {
                    for (size_t k = 0; k < evk.key(j + 2)[i].size(); k++)
                    {
                        ASSERT_FALSE(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                    }
                }
            }

            evk = keygen.relin_keys(30, 1);
            ASSERT_TRUE(evk.parms_id() == parms.parms_id());
            ASSERT_EQ(4ULL, evk.key(2)[0].size());
            for (size_t j = 0; j < evk.size(); j++)
            {
                for (size_t i = 0; i < evk.key(j + 2).size(); i++)
                {
                    for (size_t k = 0; k < evk.key(j + 2)[i].size(); k++)
                    {
                        ASSERT_FALSE(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                    }
                }
            }

            evk = keygen.relin_keys(2, 2);
            ASSERT_TRUE(evk.parms_id() == parms.parms_id());
            ASSERT_EQ(60ULL, evk.key(2)[0].size());
            for (size_t j = 0; j < evk.size(); j++)
            {
                for (size_t i = 0; i < evk.key(j + 2).size(); i++)
                {
                    for (size_t k = 0; k < evk.key(j + 2)[i].size(); k++)
                    {
                        ASSERT_FALSE(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                    }
                }
            }

            GaloisKeys galks = keygen.galois_keys(60);
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_EQ(2ULL, galks.key(3)[0].size());
            ASSERT_EQ(10ULL, galks.size());

            galks = keygen.galois_keys(30);
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_EQ(4ULL, galks.key(3)[0].size());
            ASSERT_EQ(10ULL, galks.size());

            galks = keygen.galois_keys(2);
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_EQ(60ULL, galks.key(3)[0].size());
            ASSERT_EQ(10ULL, galks.size());

            galks = keygen.galois_keys(60, vector<uint64_t>{ 1, 3, 5, 7 });
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_TRUE(galks.has_key(1));
            ASSERT_TRUE(galks.has_key(3));
            ASSERT_TRUE(galks.has_key(5));
            ASSERT_TRUE(galks.has_key(7));
            ASSERT_FALSE(galks.has_key(9));
            ASSERT_FALSE(galks.has_key(127));
            ASSERT_EQ(2ULL, galks.key(1)[0].size());
            ASSERT_EQ(2ULL, galks.key(3)[0].size());
            ASSERT_EQ(2ULL, galks.key(5)[0].size());
            ASSERT_EQ(2ULL, galks.key(7)[0].size());
            ASSERT_EQ(4ULL, galks.size());

            galks = keygen.galois_keys(30, vector<uint64_t>{ 1, 3, 5, 7 });
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_TRUE(galks.has_key(1));
            ASSERT_TRUE(galks.has_key(3));
            ASSERT_TRUE(galks.has_key(5));
            ASSERT_TRUE(galks.has_key(7));
            ASSERT_FALSE(galks.has_key(9));
            ASSERT_FALSE(galks.has_key(127));
            ASSERT_EQ(4ULL, galks.key(1)[0].size());
            ASSERT_EQ(4ULL, galks.key(3)[0].size());
            ASSERT_EQ(4ULL, galks.key(5)[0].size());
            ASSERT_EQ(4ULL, galks.key(7)[0].size());
            ASSERT_EQ(4ULL, galks.size());

            galks = keygen.galois_keys(2, vector<uint64_t>{ 1, 3, 5, 7 });
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_TRUE(galks.has_key(1));
            ASSERT_TRUE(galks.has_key(3));
            ASSERT_TRUE(galks.has_key(5));
            ASSERT_TRUE(galks.has_key(7));
            ASSERT_FALSE(galks.has_key(9));
            ASSERT_FALSE(galks.has_key(127));
            ASSERT_EQ(60ULL, galks.key(1)[0].size());
            ASSERT_EQ(60ULL, galks.key(3)[0].size());
            ASSERT_EQ(60ULL, galks.key(5)[0].size());
            ASSERT_EQ(60ULL, galks.key(7)[0].size());
            ASSERT_EQ(4ULL, galks.size());

            galks = keygen.galois_keys(30, vector<uint64_t>{ 1 });
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_TRUE(galks.has_key(1));
            ASSERT_FALSE(galks.has_key(3));
            ASSERT_FALSE(galks.has_key(127));
            ASSERT_EQ(4ULL, galks.key(1)[0].size());
            ASSERT_EQ(1ULL, galks.size());

            galks = keygen.galois_keys(30, vector<uint64_t>{ 127 });
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_FALSE(galks.has_key(1));
            ASSERT_TRUE(galks.has_key(127));
            ASSERT_EQ(4ULL, galks.key(127)[0].size());
            ASSERT_EQ(1ULL, galks.size());
        }
        {
            parms.set_noise_standard_deviation(3.20);
            parms.set_poly_modulus_degree(256);
            parms.set_coeff_modulus({ DefaultParams::small_mods_60bit(0), DefaultParams::small_mods_30bit(0), DefaultParams::small_mods_30bit(1) });
            auto context = SEALContext::Create(parms);
            KeyGenerator keygen(context);

            RelinKeys evk = keygen.relin_keys(60, 2);
            ASSERT_TRUE(evk.parms_id() == parms.parms_id());
            ASSERT_EQ(2ULL, evk.key(2)[0].size());
            for (size_t j = 0; j < evk.size(); j++)
            {
                for (size_t i = 0; i < evk.key(j + 2).size(); i++)
                {
                    for (size_t k = 0; k < evk.key(j + 2)[i].size(); k++)
                    {
                        ASSERT_FALSE(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                    }
                }
            }

            evk = keygen.relin_keys(30, 2);
            ASSERT_TRUE(evk.parms_id() == parms.parms_id());
            ASSERT_EQ(4ULL, evk.key(2)[0].size());
            for (size_t j = 0; j < evk.size(); j++)
            {
                for (size_t i = 0; i < evk.key(j + 2).size(); i++)
                {
                    for (size_t k = 0; k < evk.key(j + 2)[i].size(); k++)
                    {
                        ASSERT_FALSE(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                    }
                }
            }

            evk = keygen.relin_keys(4, 1);
            ASSERT_TRUE(evk.parms_id() == parms.parms_id());
            ASSERT_EQ(30ULL, evk.key(2)[0].size());
            for (size_t j = 0; j < evk.size(); j++)
            {
                for (size_t i = 0; i < evk.key(j + 2).size(); i++)
                {
                    for (size_t k = 0; k < evk.key(j + 2)[i].size(); k++)
                    {
                        ASSERT_FALSE(is_zero_poly(evk.key(j + 2)[i].data(k), evk.key(j + 2)[i].poly_modulus_degree(), evk.key(j + 2)[i].coeff_mod_count()));
                    }
                }
            }

            GaloisKeys galks = keygen.galois_keys(60);
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_EQ(2ULL, galks.key(3)[0].size());
            ASSERT_EQ(14ULL, galks.size());

            galks = keygen.galois_keys(30);
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_EQ(4ULL, galks.key(3)[0].size());
            ASSERT_EQ(14ULL, galks.size());

            galks = keygen.galois_keys(2);
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_EQ(60ULL, galks.key(3)[0].size());
            ASSERT_EQ(14ULL, galks.size());

            galks = keygen.galois_keys(60, vector<uint64_t>{ 1, 3, 5, 7 });
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_TRUE(galks.has_key(1));
            ASSERT_TRUE(galks.has_key(3));
            ASSERT_TRUE(galks.has_key(5));
            ASSERT_TRUE(galks.has_key(7));
            ASSERT_FALSE(galks.has_key(9));
            ASSERT_FALSE(galks.has_key(511));
            ASSERT_EQ(2ULL, galks.key(1)[0].size());
            ASSERT_EQ(2ULL, galks.key(3)[0].size());
            ASSERT_EQ(2ULL, galks.key(5)[0].size());
            ASSERT_EQ(2ULL, galks.key(7)[0].size());
            ASSERT_EQ(4ULL, galks.size());

            galks = keygen.galois_keys(30, vector<uint64_t>{ 1, 3, 5, 7 });
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_TRUE(galks.has_key(1));
            ASSERT_TRUE(galks.has_key(3));
            ASSERT_TRUE(galks.has_key(5));
            ASSERT_TRUE(galks.has_key(7));
            ASSERT_FALSE(galks.has_key(9));
            ASSERT_FALSE(galks.has_key(511));
            ASSERT_EQ(4ULL, galks.key(1)[0].size());
            ASSERT_EQ(4ULL, galks.key(3)[0].size());
            ASSERT_EQ(4ULL, galks.key(5)[0].size());
            ASSERT_EQ(4ULL, galks.key(7)[0].size());
            ASSERT_EQ(4ULL, galks.size());

            galks = keygen.galois_keys(2, vector<uint64_t>{ 1, 3, 5, 7 });
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_TRUE(galks.has_key(1));
            ASSERT_TRUE(galks.has_key(3));
            ASSERT_TRUE(galks.has_key(5));
            ASSERT_TRUE(galks.has_key(7));
            ASSERT_FALSE(galks.has_key(9));
            ASSERT_FALSE(galks.has_key(511));
            ASSERT_EQ(60ULL, galks.key(1)[0].size());
            ASSERT_EQ(60ULL, galks.key(3)[0].size());
            ASSERT_EQ(60ULL, galks.key(5)[0].size());
            ASSERT_EQ(60ULL, galks.key(7)[0].size());
            ASSERT_EQ(4ULL, galks.size());

            galks = keygen.galois_keys(30, vector<uint64_t>{ 1 });
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_TRUE(galks.has_key(1));
            ASSERT_FALSE(galks.has_key(3));
            ASSERT_FALSE(galks.has_key(511));
            ASSERT_EQ(4ULL, galks.key(1)[0].size());
            ASSERT_EQ(1ULL, galks.size());

            galks = keygen.galois_keys(30, vector<uint64_t>{ 511 });
            ASSERT_TRUE(galks.parms_id() == parms.parms_id());
            ASSERT_FALSE(galks.has_key(1));
            ASSERT_TRUE(galks.has_key(511));
            ASSERT_EQ(4ULL, galks.key(511)[0].size());
            ASSERT_EQ(1ULL, galks.size());
        }
    }

    TEST(KeyGeneratorTest, FVSecretKeyGeneration)
    {
        EncryptionParameters parms(scheme_type::BFV);
        SmallModulus plain_modulus(1 << 6);
        parms.set_poly_modulus_degree(128);
        parms.set_plain_modulus(plain_modulus);
        parms.set_coeff_modulus({ DefaultParams::small_mods_40bit(0), DefaultParams::small_mods_40bit(1), DefaultParams::small_mods_40bit(2) });
        parms.set_noise_standard_deviation(3.20);
        auto context = SEALContext::Create(parms);
        {
            KeyGenerator keygen(context);
            auto pk = keygen.public_key();
            auto sk = keygen.secret_key();

            KeyGenerator keygen2(context, sk);
            auto sk2 = keygen2.secret_key();
            auto pk2 = keygen2.public_key();

            ASSERT_EQ(sk.data().coeff_count(), sk2.data().coeff_count());
            for (size_t i = 0; i < sk.data().coeff_count(); i++)
            {
                ASSERT_EQ(sk.data()[i], sk2.data()[i]);
            }

            ASSERT_EQ(pk.data().uint64_count(), pk2.data().uint64_count());
            for (size_t i = 0; i < pk.data().uint64_count(); i++)
            {
                ASSERT_NE(pk.data()[i], pk2.data()[i]);
            }

            Encryptor encryptor(context, pk2);
            Decryptor decryptor(context, sk);
            Ciphertext ctxt;
            Plaintext pt1("1x^63 + 2x^33 + 3x^23 + 4x^13 + 5x^1 + 6");
            Plaintext pt2;
            encryptor.encrypt(pt1, ctxt);
            decryptor.decrypt(ctxt, pt2);
            ASSERT_TRUE(pt1 == pt2);
        }
        {
            KeyGenerator keygen(context);
            auto pk = keygen.public_key();
            auto sk = keygen.secret_key();

            KeyGenerator keygen2(context, sk);
            auto sk2 = keygen2.secret_key();
            auto pk2 = keygen2.public_key();
            auto rlk2 = keygen2.relin_keys(60, 1);

            Evaluator evaluator(context);
            Encryptor encryptor(context, pk2);
            Decryptor decryptor(context, sk);
            Ciphertext ctxt;
            Plaintext pt1("1x^10 + 2");
            Plaintext pt2;
            encryptor.encrypt(pt1, ctxt);
            evaluator.square_inplace(ctxt);
            evaluator.relinearize_inplace(ctxt, rlk2);
            decryptor.decrypt(ctxt, pt2);
            ASSERT_TRUE(pt2.to_string() == "1x^20 + 4x^10 + 4");
        }
        {
            KeyGenerator keygen(context);
            auto pk = keygen.public_key();
            auto sk = keygen.secret_key();

            KeyGenerator keygen2(context, sk, pk);
            auto sk2 = keygen2.secret_key();
            auto pk2 = keygen2.public_key();
            auto rlk2 = keygen2.relin_keys(60, 1);

            ASSERT_EQ(sk.data().coeff_count(), sk2.data().coeff_count());
            for (size_t i = 0; i < sk.data().coeff_count(); i++)
            {
                ASSERT_EQ(sk.data()[i], sk2.data()[i]);
            }

            ASSERT_EQ(pk.data().uint64_count(), pk2.data().uint64_count());
            for (size_t i = 0; i < pk.data().uint64_count(); i++)
            {
                ASSERT_EQ(pk.data()[i], pk2.data()[i]);
            }

            Evaluator evaluator(context);
            Encryptor encryptor(context, pk2);
            Decryptor decryptor(context, sk);
            Ciphertext ctxt;
            Plaintext pt1("1x^10 + 2");
            Plaintext pt2;
            encryptor.encrypt(pt1, ctxt);
            evaluator.square_inplace(ctxt);
            evaluator.relinearize_inplace(ctxt, rlk2);
            decryptor.decrypt(ctxt, pt2);
            ASSERT_TRUE(pt2.to_string() == "1x^20 + 4x^10 + 4");
        }
    }
    TEST(KeyGeneratorTest, FVKeyGenCRSGeneration)
    {
        EncryptionParameters parms(scheme_type::BFV);
        parms.set_noise_standard_deviation(3.20);
        parms.set_poly_modulus_degree(128);
        parms.set_plain_modulus(1 << 6);
        parms.set_coeff_modulus({ DefaultParams::small_mods_40bit(0), DefaultParams::small_mods_40bit(1), DefaultParams::small_mods_40bit(2) });
        auto context = SEALContext::Create(parms);
        {
            KeyGenerator keygen(context);
            auto sk = keygen.secret_key();
            auto pk = keygen.public_key();
            auto crs = keygen.keygen_crs();

            KeyGenerator keygen2(context, sk, crs);
            auto sk2 = keygen2.secret_key();
            auto pk2 = keygen2.public_key();
            auto crs2 = keygen2.keygen_crs();

            ASSERT_EQ(sk.data().coeff_count(), sk2.data().coeff_count());
            for (size_t i = 0; i < sk.data().coeff_count(); i++)
            {
                ASSERT_EQ(sk.data()[i], sk2.data()[i]);
            }

            ASSERT_EQ(crs.data().uint64_count(), crs2.data().uint64_count());
            for (size_t i = 0; i < crs.data().uint64_count(); i++)
            {
                ASSERT_EQ(crs.data()[i], crs2.data()[i]);
            }

            Encryptor encryptor(context, pk2);
            Decryptor decryptor(context, sk);
            Ciphertext ctxt;
            Plaintext pt1("1x^63 + 2x^33 + 3x^23 + 4x^13 + 5x^1 + 6");
            Plaintext pt2;
            encryptor.encrypt(pt1, ctxt);
            decryptor.decrypt(ctxt, pt2);
            ASSERT_TRUE(pt1 == pt2);
        }
        {
            KeyGenerator keygen(context);
            auto crs = keygen.keygen_crs();

            KeyGenerator keygen2(context, crs);
            auto sk2 = keygen2.secret_key();
            auto pk2 = keygen2.public_key();
            auto crs2 = keygen2.keygen_crs();

            ASSERT_EQ(crs.data().uint64_count(), crs2.data().uint64_count());
            for (size_t i = 0; i < crs.data().uint64_count(); i++)
            {
                ASSERT_EQ(crs.data()[i], crs2.data()[i]);
            }

            Encryptor encryptor(context, pk2);
            Decryptor decryptor(context, sk2);
            Ciphertext ctxt;
            Plaintext pt1("1x^63 + 2x^33 + 3x^23 + 4x^13 + 5x^1 + 6");
            Plaintext pt2;
            encryptor.encrypt(pt1, ctxt);
            decryptor.decrypt(ctxt, pt2);
            ASSERT_TRUE(pt1 == pt2);
        }
        {
            KeyGenerator keygen(context);
            auto sk = keygen.secret_key();
            auto pk = keygen.public_key();
            auto rlk = keygen.relin_keys(60, 3, true);

            Evaluator evaluator(context);
            Encryptor encryptor(context, pk);
            Decryptor decryptor(context, sk);
            Ciphertext ctxt(context);
            Plaintext pt1("1x^10 + 2");
            Plaintext pt2;
            encryptor.encrypt(pt1, ctxt);
            evaluator.square_inplace(ctxt);
            evaluator.relinearize_inplace(ctxt, rlk);
            decryptor.decrypt(ctxt, pt2);
            ASSERT_TRUE(pt2.to_string() == "1x^20 + 4x^10 + 4");
        }
        {
            KeyGenerator keygen(context);
            auto sk = keygen.secret_key();
            auto pk = keygen.public_key();
            auto relin_keys = keygen.relin_keys(60, 1, true);
            auto crs = keygen.keygen_crs();

            KeyGenerator keygen2(context, crs);
            auto sk2 = keygen2.secret_key();
            auto pk2 = keygen2.public_key();
            auto relin_keys2 = keygen2.relin_keys(60, 1, true);

            Evaluator evaluator(context);
            Encryptor encryptor(context, pk);
            Encryptor encryptor2(context, pk2);
            Decryptor decryptor(context, sk);
            Decryptor decryptor2(context, sk2);
            Ciphertext ctxt;
            Ciphertext ctxt2;
            Plaintext pt1("1x^10 + 2");
            Plaintext pt2;
            Plaintext pt22;
            encryptor.encrypt(pt1, ctxt);
            encryptor2.encrypt(pt1, ctxt2);
            evaluator.square_inplace(ctxt);
            evaluator.square_inplace(ctxt2);
            evaluator.relinearize_inplace(ctxt, relin_keys);
            evaluator.relinearize_inplace(ctxt2, relin_keys2);
            decryptor.decrypt(ctxt, pt2);
            decryptor2.decrypt(ctxt2, pt22);
            ASSERT_TRUE(pt2.to_string() == "1x^20 + 4x^10 + 4");
            ASSERT_TRUE(pt2 == pt22);
        }
        {
            KeyGenerator keygen(context);
            auto sk = keygen.secret_key();
            auto pk = keygen.public_key();
            auto relin_keys = keygen.relin_keys(60, 1, true);
            auto crs = keygen.keygen_crs();

            KeyGenerator keygen2(context, sk, crs);
            auto sk2 = keygen2.secret_key();
            auto pk2 = keygen2.public_key();
            auto relin_keys2 = keygen2.relin_keys(60, 1, true);
            auto crs2 = keygen2.keygen_crs();

            ASSERT_EQ(crs.data().uint64_count(), crs2.data().uint64_count());
            for (size_t i = 0; i < crs.data().uint64_count(); i++)
            {
                ASSERT_EQ(crs.data()[i], crs2.data()[i]);
            }

            Evaluator evaluator(context);
            Evaluator evaluator2(context);
            Encryptor encryptor(context, pk);
            Encryptor encryptor2(context, pk2);
            Decryptor decryptor(context, sk);
            Decryptor decryptor2(context, sk2);
            Ciphertext ctxt(context);
            Ciphertext ctxt2(context);
            Plaintext pt1("1x^10 + 2");
            Plaintext pt2;
            Plaintext pt22;
            encryptor.encrypt(pt1, ctxt);
            encryptor2.encrypt(pt1, ctxt2);
            evaluator.square_inplace(ctxt);
            evaluator2.square_inplace(ctxt2);
            evaluator.relinearize_inplace(ctxt, relin_keys);
            evaluator2.relinearize_inplace(ctxt2, relin_keys2);
            decryptor.decrypt(ctxt, pt2);
            decryptor2.decrypt(ctxt2, pt22);
            ASSERT_TRUE(pt2.to_string() == "1x^20 + 4x^10 + 4");
            ASSERT_TRUE(pt2 == pt22);
        }
    }
}
