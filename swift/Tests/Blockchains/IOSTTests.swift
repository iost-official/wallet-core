// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class IOSTTests: XCTestCase {
    func testSign() {
        var tx = IOSTTransaction()
        tx.time = 1550137587000000000
        tx.expiration = tx.time + Int64(1000000000) * 300
        tx.gasRatio = 1
        tx.gasLimit = 1000000
        tx.chainID = 1024

        var action = IOSTAction()
        action.contract = "token.iost"
        action.actionName = "transfer"

        let token = "iost"
        let src = "myname"
        let dst = "admin"
        let amount = "10"
        let memo = ""
        let params = [token, src, dst, amount, memo]
        let data = try! JSONSerialization.data(withJSONObject: params)
        let str = String(data: data, encoding: String.Encoding.utf8)!
        XCTAssertEqual(str, "[\"iost\",\"myname\",\"admin\",\"10\",\"\"]")

        action.data = str
        tx.actions = [action]

        var amountLimit = IOSTAmountLimit()
        amountLimit.token = "*"
        amountLimit.value = "unlimited"
        tx.amountLimit = [amountLimit]

        let secKeyData = Base58.decodeNoCheck(string: "Pq1rHY7TESYjt3T6A8C2kiSkvme2sxj9FshPdnn5Yne")!
        XCTAssertEqual(secKeyData.hexString, "05d91171837b6c979f2d812bb8f039a8d802061f3e826aa7e3e0901e592b9f43")
        
        let input = IOSTSigningInput.with {
            $0.privateKey = secKeyData
            $0.transaction = tx
        }
        let output = IOSTSigner.sign(input: input)
        XCTAssertEqual(output.signature, "4Nxq3pGFQMuPCFoEmvrr2eTsskeWNNwzJb6TMLtXKWubiCoGmLFTczD16gnCTHpgBTLZWvs6huoVCEeidaSaAXjY")
    }
}
