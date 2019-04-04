// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../IOST/Address.h"

#include <TrustWalletCore/TWIOSTAddress.h>

using namespace TW;
using namespace TW::IOST;

bool TWIOSTAddressEqual(struct TWIOSTAddress* _Nonnull lhs, struct TWIOSTAddress* _Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWIOSTAddressIsValidString(TWString* _Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWIOSTAddress* _Nullable TWIOSTAddressCreateWithString(TWString* _Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);

    try {
        const auto address = Address(*s);
        return new TWIOSTAddress{std::move(address)};
    } catch (...) {
        return nullptr;
    }
}

void TWIOSTAddressDelete(struct TWIOSTAddress* _Nonnull address) {
    delete address;
}

TWString* _Nonnull TWIOSTAddressDescription(struct TWIOSTAddress* _Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}
