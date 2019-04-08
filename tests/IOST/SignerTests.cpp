// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Base58.h"
#include "proto/IOST.pb.h"
#include "IOST/Signer.h"
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

    auto secKeyBytes = Base58::bitcoin.decode("Pq1rHY7TESYjt3T6A8C2kiSkvme2sxj9FshPdnn5Yne");
    ASSERT_EQ(hex(secKeyBytes), "05d91171837b6c979f2d812bb8f039a8d802061f3e826aa7e3e0901e592b9f43");
    auto key = PrivateKey(secKeyBytes);

    Signer signer;
    signer.sign(key, tx);
    std::string signature = tx.publisher_sigs(0).signature();

    // check base58
    auto ptr = reinterpret_cast<byte*>(signature.data());
    auto encoded = Base58::bitcoin.encode(ptr, ptr + signature.size());
    ASSERT_EQ(encoded, "4Nxq3pGFQMuPCFoEmvrr2eTsskeWNNwzJb6TMLtXKWubiCoGmLFTczD16gnCTHpgBTLZWvs6huoVCEeidaSaAXjY");
    // check hex
    ASSERT_EQ(hex(signature), "a8fd9cb460f1aa4019b42e6e883e6731b31ad2e40e083c758bcf22cea1740902091a5da0f88a61468d908c39d54f44762029e6ee2d8bccef1eab3129045bca33");
}
}}

