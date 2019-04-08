// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../PublicKey.h"

#include <stdint.h>
#include <string>

namespace TW {
namespace IOST {

class Address {
  public:
    template <typename T>
    static bool isValid(const T& data) {
        auto s = std::string(data);
        if (s.size()<5 || s.size()> 11) { return false; }
        for (char ch : s) {
            if (!((ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || ch == '_')) {
                return false;
            }
        }
        return true;
    }
    Address(const std::string& string) { data = string; }
    Address(const PublicKey& publicKey) {}
    std::string string() const { return data; }
    std::string data;
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.data == rhs.data;
}

} // namespace IOST
} // namespace TW

/// Wrapper for C interface.
struct TWIOSTAddress {
    TW::IOST::Address impl;
};