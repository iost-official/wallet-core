
#include "Signer.h"
#include "../Hash.h"
#include "../PrivateKey.h"
#include <sstream>
#include <boost/endian/conversion.hpp>

#include <iostream>

using namespace TW;
using namespace TW::IOST;

class IOSTEncoder {
public:    
    IOSTEncoder(){}
    void WriteByte(uint8_t b) {
        buffer << b;
    }
    void WriteInt32(uint32_t i) {
        uint32_t r = boost::endian::endian_reverse(i);
        buffer << std::string(reinterpret_cast<const char*>(&r), sizeof(r));
    }

    void WriteInt64(uint64_t i) {
        uint64_t r = boost::endian::endian_reverse(i);
        buffer << std::string(reinterpret_cast<const char*>(&r), sizeof(r));
    }

    void WriteString(std::string s) {
        WriteInt32(s.size());
        buffer << s;
    }

    void WriteStringSlice(const std::vector<std::string> v) {
        WriteInt32(v.size());
        for (std::string s : v) {
            WriteString(s);
        }
    }

    std::string AsString() {
        return buffer.str();
    }

private:
    std::stringstream buffer;
};

TWData *_Nonnull Signer::sign(Proto::TransactionRequest t, TWData *_Nonnull keyStr) const {
    IOSTEncoder se;
	se.WriteInt64(t.time());
    std::cout << "t.time" << t.time() << std::endl;
	se.WriteInt64(t.expiration());
	se.WriteInt64(int64_t(t.gas_ratio() * 100));
	se.WriteInt64(int64_t(t.gas_limit() * 100));
	se.WriteInt64(t.delay());
	se.WriteInt32(int32_t(t.chain_id()));
	se.WriteString("");

    std::vector<std::string> svec;
    for (auto item : t.signers()) {
        svec.push_back(item);
    }
	se.WriteStringSlice(svec);

    se.WriteInt32(t.actions_size());
    for (auto a : t.actions()) {
        IOSTEncoder s;
        s.WriteString(a.contract());
        s.WriteString(a.action_name());
        s.WriteString(a.data());
        se.WriteString(s.AsString());
    }

    se.WriteInt32(t.amount_limit_size());
    for (auto a : t.amount_limit()) {
        IOSTEncoder s;
        s.WriteString(a.token());
        s.WriteString(a.value());
        se.WriteString(s.AsString());
    }

    se.WriteInt32(t.signatures_size());
    for (auto sig : t.signatures()) {
        IOSTEncoder s;
        s.WriteByte(sig.algorithm());
        s.WriteString(sig.signature());
        s.WriteString(sig.public_key());
        se.WriteString(s.AsString());
    }

    std::string txRaw = se.AsString();

    std::cout << "txRaw size" << txRaw.size() << std::endl;
    for (char c : txRaw) {
        std::cout << (uint32_t)(uint8_t)c << " ";
    }

    //auto hash = Hash::sha256(txRaw);
    auto keyVec = *(reinterpret_cast<const std::vector<uint8_t>*>(keyStr));
    std::cout << "keyVec " << keyVec.size() << std::endl;
    auto key = PrivateKey(keyVec);
    auto hash = Hash::sha3_256(txRaw);
    auto signature = key.sign(hash);
    //TWData *_Nonnull
    //auto sig = std::vector<uint8_t>(signature.begin(), signature.end() - 1);
    return TWDataCreateWithBytes(signature.begin(), signature.size() - 1U);
}