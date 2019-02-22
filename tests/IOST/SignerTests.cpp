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


namespace TW {
namespace IOST {

TEST(IOSTSigner, Sign) {
    auto tx = Proto::TransactionRequest();
    tx.set_time(1550137587000000000);
    tx.set_expiration(1550137887000000000);
    tx.set_gas_ratio(1);
    tx.set_gas_limit(1000000);
    tx.set_chain_id(1024);

    tx.add_actions();
    auto action = tx.mutable_actions(0);
    action->set_contract("token.iost");
    action->set_action_name("transfer");
    action->set_data("[\"iost\",\"myname\",\"admin\",\"10\",\"\"]");

    tx.add_amount_limit();
    auto amountLimit = tx.mutable_amount_limit(0);
    amountLimit->set_token("*");
    amountLimit->set_value("unlimited");

    std::string seckeyB58("Pq1rHY7TESYjt3T6A8C2kiSkvme2sxj9FshPdnn5Yne");
    auto seckey = TWBase58DecodeNoCheck(&seckeyB58);
    ASSERT_TRUE(seckey != nullptr);

    Signer signer;
    TWData *_Nonnull sig = signer.sign(tx, seckey);
    auto encodedSig = TWBase58EncodeNoCheck(sig);

    ASSERT_EQ(*(reinterpret_cast<const std::string*>(encodedSig)), "4Nxq3pGFQMuPCFoEmvrr2eTsskeWNNwzJb6TMLtXKWubiCoGmLFTczD16gnCTHpgBTLZWvs6huoVCEeidaSaAXjY");
}
}}

