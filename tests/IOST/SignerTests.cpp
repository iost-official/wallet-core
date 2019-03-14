// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

//../trezor-crypto/include
//#include "../trezor-crypto/include/TrezorCrypto/base58.h"
//#include <TrezorCrypto/base58.h>

#include "TrustWalletCore/TWBase58.h"
#include "../../src/proto/IOST.pb.h"

#include "../../src/IOST/Signer.h"
#include <gtest/gtest.h>

#include <nlohmann/json.hpp>
#include <iostream>


namespace TW {
namespace IOST {

TEST(IOSTSigner, Sign) {
    auto tx = Proto::Transaction();
    tx.set_time(1550137587000000000);
    tx.set_expiration(tx.time() + int64_t(1000000000) * 300);
    tx.set_gas_ratio(1);
    tx.set_gas_limit(1000000);
    tx.set_chain_id(1024);

    tx.add_actions();
    auto action = tx.mutable_actions(0);
    action->set_contract("token.iost");
    action->set_action_name("transfer");

    std::string token = "iost";
    std::string src = "myname";
    std::string dst = "admin";
    std::string amount = "10";
    std::string memo = "";

    nlohmann::json j;
    j.push_back(token);
    j.push_back(src);
    j.push_back(dst);
    j.push_back(amount);
    j.push_back(memo);
    std::string data = j.dump();
    ASSERT_EQ(data, std::string("[\"iost\",\"myname\",\"admin\",\"10\",\"\"]"));
    action->set_data(data);

    tx.add_amount_limit();
    auto amountLimit = tx.mutable_amount_limit(0);
    amountLimit->set_token("*");
    amountLimit->set_value("unlimited");

    std::string seckeyB58("Pq1rHY7TESYjt3T6A8C2kiSkvme2sxj9FshPdnn5Yne");
    auto seckey = TWBase58DecodeNoCheck(&seckeyB58);
    ASSERT_TRUE(seckey != nullptr);
    auto keyVec = *(reinterpret_cast<const std::vector<uint8_t>*>(seckey));
    auto key = PrivateKey(keyVec);

    Signer signer;
    signer.sign(key, tx);
    std::string signature = tx.signatures(0).signature();
    auto sig = TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(signature.data()), signature.size());
    auto encodedSig = TWBase58EncodeNoCheck(sig);

    ASSERT_EQ(*(reinterpret_cast<const std::string*>(encodedSig)), "4Nxq3pGFQMuPCFoEmvrr2eTsskeWNNwzJb6TMLtXKWubiCoGmLFTczD16gnCTHpgBTLZWvs6huoVCEeidaSaAXjY");
}
}}

