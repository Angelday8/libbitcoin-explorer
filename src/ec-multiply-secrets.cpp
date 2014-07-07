/**
 * Copyright (c) 2011-2014 sx developers (see AUTHORS)
 *
 * This file is part of sx.
 *
 * sx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <iostream>
#include <bitcoin/bitcoin.hpp>
#include <sx/command/ec-multiply-secrets.hpp>
#include <sx/serializer/bytes.hpp>
#include <sx/utility/coin.hpp>
#include <sx/utility/console.hpp>

using namespace bc;
using namespace sx;
using namespace sx::extension;
using namespace sx::serializer;

console_result ec_multiply_secrets::invoke(std::istream& input, 
    std::ostream& output, std::ostream& cerr)
{
    // Bound parameters.
    auto factors = get_secrets_argument();

    // TODO: initialize product with first addend.
    ec_secret product;
    for (auto const& factor: factors)
    {
        // TODO: create deserializable ec_secret
        ec_secret secret;
        if (!set_ec_secret(secret, factor))
        {
            cerr << boost::format(SX_EC_MULTIPLY_SECRETS_INVALID_INTEGER) % 
                factor << std::endl;
            return console_result::failure;
        }

        // Elliptic curve function (INTEGER * INTEGER) % curve-order.
        if (!bc::ec_multiply(product, secret))
        {
            cerr << SX_EC_MULITPLY_SECRETS_OUT_OF_RANGE << std::endl;
            return console_result::failure;
        }
    }

    // TODO: create serializable ec_secret
    bytes hex(product);

    output << hex << std::endl;
    return console_result::okay;
}
