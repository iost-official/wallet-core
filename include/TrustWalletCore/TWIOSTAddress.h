// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

struct TWPublicKey;

/// Represents an IOST address.
TW_EXPORT_CLASS
struct TWIOSTAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWIOSTAddressEqual(struct TWIOSTAddress *_Nonnull lhs, struct TWIOSTAddress *_Nonnull rhs);

/// Determines if the string is a valid address.
TW_EXPORT_STATIC_METHOD
bool TWIOSTAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWIOSTAddress *_Nullable TWIOSTAddressCreateWithString(TWString *_Nonnull string);

TW_EXPORT_METHOD
void TWIOSTAddressDelete(struct TWIOSTAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWIOSTAddressDescription(struct TWIOSTAddress *_Nonnull address);

TW_EXTERN_C_END
