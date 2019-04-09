// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class IOSTTests: XCTestCase {
    func testSign() {
        let tx = IOSTTransaction()
        tx.time = 1550137587000000000
        tx.expiration = tx.time() + Int64(1000000000) * 300
        tx.gasRatio = 1
        tx.gasLimit = 1000000
        tx.chainID = 1024

        let action = IOSTAction()
        action.contract = "token.iost"
        action.actionName = "transfer"

        let token = "iost"
        let src = "myname"
        let dst = "admin"
        let amount = "10"
        let memo = ""
        let params = [token, src, dst, amount, memo]
        var str = ""
        if let data = JSONSerialization.dataWithJSONObject(params, options: nil, error: nil),
            str = String(data: data, encoding: NSUTF8StringEncoding) {
            XCTAssertEqual(str, "[\"iost\",\"myname\",\"admin\",\"10\",\"\"]"))
        }
        action.data = str
        tx.actions = [action]

        let amountLimit = IOSTAmountLimit()
        amountLimit.token = "*"
        amountLimit.value = "unlimited"
        tx.amountLimit = [amountLimit]

        let secKeyData = Base58.decode("Pq1rHY7TESYjt3T6A8C2kiSkvme2sxj9FshPdnn5Yne")
        if let secKeyData = secKeyData {
            XCTAssertEqual(secKeyData.hexString, "05d91171837b6c979f2d812bb8f039a8d802061f3e826aa7e3e0901e592b9f43")
        }

        let key = PrivateKey(data: secKeyData)
        IOSTSigner.sign(key, tx)
        let signature = tx.publisherSigs[0].signature

        // check base58
        let encoded = Base58.encode(signature)
        XCTAssertEqual(encoded, "4Nxq3pGFQMuPCFoEmvrr2eTsskeWNNwzJb6TMLtXKWubiCoGmLFTczD16gnCTHpgBTLZWvs6huoVCEeidaSaAXjY")

        // check hex
        XCTAssertEqual(signature.hexString, "a8fd9cb460f1aa4019b42e6e883e6731b31ad2e40e083c758bcf22cea1740902091a5da0f88a61468d908c39d54f44762029e6ee2d8bccef1eab3129045bca33")
        XCTAssertEqual(1, 2)
    }
}
