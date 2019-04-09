package com.trustwallet.core.app

import com.google.gson.Gson
import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import junit.framework.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.IOSTSigner
import wallet.core.jni.proto.IOST.Action.newBuilder
import java.io.IOException
import java.util.*
import wallet.core.jni.proto.IOST.AmountLimit.newBuilder as amountLimitBuilder
import wallet.core.jni.proto.IOST.Transaction.newBuilder as transactionBuilder
import wallet.core.jni.proto.IOST.SigningInput.newBuilder as signInputBuilder

class TestIOSTSigner {
    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun testIOSTSignerSign() {
        var transactionBuilder = transactionBuilder()
        transactionBuilder.time = 1550137587000000000
        transactionBuilder.expiration = transactionBuilder.time + 1000000000L * 300
        transactionBuilder.gasRatio = 1.0
        transactionBuilder.gasLimit = 1000000.0
        transactionBuilder.chainId = 1024


        var actionBuilder = newBuilder()
        actionBuilder.contract = "token.iost"
        actionBuilder.actionName = "transfer"

        var dataArray = listOf("iost", "myname", "admin", "10", "")
        var data = Gson().toJson(dataArray)
        assertEquals(data, "[\"iost\",\"myname\",\"admin\",\"10\",\"\"]")
        actionBuilder.data = "[\"iost\",\"myname\",\"admin\",\"10\",\"\"]"
        var transferAction = actionBuilder.build()

        transactionBuilder.addActions(transferAction)

        var amountLimitBuilder = amountLimitBuilder()
        amountLimitBuilder.token = "*"
        amountLimitBuilder.value = "unlimited"
        transactionBuilder.addAmountLimit(amountLimitBuilder.build())

        var secKeyBytes = decode("Pq1rHY7TESYjt3T6A8C2kiSkvme2sxj9FshPdnn5Yne")

        assertEquals(bytesToHex(secKeyBytes), "05d91171837b6c979f2d812bb8f039a8d802061f3e826aa7e3e0901e592b9f43")

        var signInputBuilder = signInputBuilder()
        signInputBuilder.transaction = transactionBuilder.build()
        signInputBuilder.privateKey = ByteString.copyFrom(secKeyBytes)

        var outPut = IOSTSigner.sign(signInputBuilder.build())


        assertEquals(outPut.signature, "4Nxq3pGFQMuPCFoEmvrr2eTsskeWNNwzJb6TMLtXKWubiCoGmLFTczD16gnCTHpgBTLZWvs6huoVCEeidaSaAXjY")
    }

    private fun bytesToHex(hashInBytes: ByteArray): String {

        val sb = StringBuilder()
        for (b in hashInBytes) {
            sb.append(String.format("%02x", b))
        }
        return sb.toString()
    }

    //base 58

    val ALPHABET = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz".toCharArray()
    private val ENCODED_ZERO = ALPHABET[0]
    private val INDEXES = IntArray(128)

    init {
        Arrays.fill(INDEXES, -1)
        for (i in 0 until ALPHABET.size) {
            INDEXES[ALPHABET[i].toInt()] = i
        }
    }
    /**
     * Decodes the given base58 string into the original data bytes.
     *
     * @param input the base58-encoded string to decode
     * @return the decoded data bytes
     * @throws java.io.IOException if the given string is not a valid base58 string
     */
    @Throws(IOException::class)
    fun decode(input: String): ByteArray {
        if (input.length == 0) {
            return ByteArray(0)
        }
        // Convert the base58-encoded ASCII chars to a base58 byte sequence (base58 digits).
        val input58 = ByteArray(input.length)
        for (i in 0 until input.length) {
            val c = input[i]
            val digit = if (c.toInt() < 128) INDEXES[c.toInt()] else -1
            if (digit < 0) {
                throw IOException("invalid char")
            }
            input58[i] = digit.toByte()
        }
        // Count leading zeros.
        var zeros = 0
        while (zeros < input58.size && input58[zeros].toInt() == 0) {
            ++zeros
        }
        // Convert base-58 digits to base-256 digits.
        val decoded = ByteArray(input.length)
        var outputStart = decoded.size
        var inputStart = zeros
        while (inputStart < input58.size) {
            decoded[--outputStart] = divmod(input58, inputStart, 58, 256)
            if (input58[inputStart].toInt() == 0) {
                ++inputStart // optimization - skip leading zeros
            }
        }
        // Ignore extra leading zeroes that were added during the calculation.
        while (outputStart < decoded.size && decoded[outputStart].toInt() == 0) {
            ++outputStart
        }
        // Return decoded data (including original number of leading zeros).
        return Arrays.copyOfRange(decoded, outputStart - zeros, decoded.size)
    }

    private fun divmod(number: ByteArray, firstDigit: Int, base: Int, divisor: Int): Byte {
        // this is just long division which accounts for the base of the input digits
        var remainder = 0
        for (i in firstDigit until number.size) {
            val digit = number[i].toInt() and 0xFF
            val temp = remainder * base + digit
            number[i] = (temp / divisor).toByte()
            remainder = temp % divisor
        }
        return remainder.toByte()
    }
}