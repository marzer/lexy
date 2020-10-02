// Copyright (C) 2020 Jonathan Müller <jonathanmueller.dev@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef LEXY_DSL_DIGIT_HPP_INCLUDED
#define LEXY_DSL_DIGIT_HPP_INCLUDED

#include <lexy/dsl/alternative.hpp>
#include <lexy/dsl/base.hpp>
#include <lexy/dsl/failure.hpp>
#include <lexy/dsl/literal.hpp>
#include <lexy/dsl/option.hpp>
#include <lexy/dsl/sequence.hpp>
#include <lexy/dsl/while.hpp>

namespace lexyd
{
struct binary
{
    static constexpr unsigned radix = 2;

    static LEXY_CONSTEVAL auto name()
    {
        return "digit.binary";
    }

    template <typename Encoding, typename IntType>
    LEXY_DSL_FUNC bool match(IntType c)
    {
        return c == Encoding::to_int_type('0') || c == Encoding::to_int_type('1');
    }

    template <typename Encoding, typename IntType>
    LEXY_DSL_FUNC bool match_zero(IntType c)
    {
        return c == Encoding::to_int_type('0');
    }

    template <typename CharT>
    LEXY_DSL_FUNC unsigned value(CharT c)
    {
        return static_cast<unsigned>(c) - '0';
    }
};

struct octal
{
    static constexpr unsigned radix = 8;

    static LEXY_CONSTEVAL auto name()
    {
        return "digit.octal";
    }

    template <typename Encoding, typename IntType>
    LEXY_DSL_FUNC bool match(IntType c)
    {
        return c >= Encoding::to_int_type('0') && c <= Encoding::to_int_type('7');
    }

    template <typename Encoding, typename IntType>
    LEXY_DSL_FUNC bool match_zero(IntType c)
    {
        return c == Encoding::to_int_type('0');
    }

    template <typename CharT>
    LEXY_DSL_FUNC unsigned value(CharT c)
    {
        return static_cast<unsigned>(c) - '0';
    }
};

struct decimal
{
    static constexpr unsigned radix = 10;

    static LEXY_CONSTEVAL auto name()
    {
        return "digit.decimal";
    }

    template <typename Encoding, typename IntType>
    LEXY_DSL_FUNC bool match(IntType c)
    {
        return c >= Encoding::to_int_type('0') && c <= Encoding::to_int_type('9');
    }

    template <typename Encoding, typename IntType>
    LEXY_DSL_FUNC bool match_zero(IntType c)
    {
        return c == Encoding::to_int_type('0');
    }

    template <typename CharT>
    LEXY_DSL_FUNC unsigned value(CharT c)
    {
        return static_cast<unsigned>(c) - '0';
    }
};

struct hex_lower
{
    static constexpr unsigned radix = 16;

    static LEXY_CONSTEVAL auto name()
    {
        return "digit.hex-lower";
    }

    template <typename Encoding, typename IntType>
    LEXY_DSL_FUNC bool match(IntType c)
    {
        return (c >= Encoding::to_int_type('0') && c <= Encoding::to_int_type('9'))
               || (c >= Encoding::to_int_type('a') && c <= Encoding::to_int_type('f'));
    }

    template <typename Encoding, typename IntType>
    LEXY_DSL_FUNC bool match_zero(IntType c)
    {
        return c == Encoding::to_int_type('0');
    }

    template <typename CharT>
    LEXY_DSL_FUNC unsigned value(CharT c)
    {
        if (c >= 'a')
            return static_cast<unsigned>(c) - 'a' + 10;
        else if (c <= '9')
            return static_cast<unsigned>(c) - '0';
        else
            return unsigned(-1);
    }
};

struct hex_upper
{
    static constexpr unsigned radix = 16;

    static LEXY_CONSTEVAL auto name()
    {
        return "digit.hex-upper";
    }

    template <typename Encoding, typename IntType>
    LEXY_DSL_FUNC bool match(IntType c)
    {
        return (c >= Encoding::to_int_type('0') && c <= Encoding::to_int_type('9'))
               || (c >= Encoding::to_int_type('A') && c <= Encoding::to_int_type('F'));
    }

    template <typename Encoding, typename IntType>
    LEXY_DSL_FUNC bool match_zero(IntType c)
    {
        return c == Encoding::to_int_type('0');
    }

    template <typename CharT>
    LEXY_DSL_FUNC unsigned value(CharT c)
    {
        if (c >= 'A')
            return static_cast<unsigned>(c) - 'A' + 10;
        else if (c <= '9')
            return static_cast<unsigned>(c) - '0';
        else
            return unsigned(-1);
    }
};

struct hex
{
    static constexpr unsigned radix = 16;

    static LEXY_CONSTEVAL auto name()
    {
        return "digit.hex";
    }

    template <typename Encoding, typename IntType>
    LEXY_DSL_FUNC bool match(IntType c)
    {
        return (c >= Encoding::to_int_type('0') && c <= Encoding::to_int_type('9'))
               || (c >= Encoding::to_int_type('A') && c <= Encoding::to_int_type('F'))
               || (c >= Encoding::to_int_type('a') && c <= Encoding::to_int_type('f'));
    }

    template <typename Encoding, typename IntType>
    LEXY_DSL_FUNC bool match_zero(IntType c)
    {
        return c == Encoding::to_int_type('0');
    }

    template <typename CharT>
    LEXY_DSL_FUNC unsigned value(CharT c)
    {
        if (c >= 'a')
            return static_cast<unsigned>(c) - 'a' + 10;
        else if (c >= 'A')
            return static_cast<unsigned>(c) - 'A' + 10;
        else if (c <= '9')
            return static_cast<unsigned>(c) - '0';
        else
            return unsigned(-1);
    }
};
} // namespace lexyd

namespace lexyd
{
template <typename Base>
struct _zero : atom_base<_zero<Base>>
{
    template <typename Reader>
    LEXY_DSL_FUNC bool match(Reader& reader)
    {
        if (!Base::template match<typename Reader::encoding>(reader.peek()))
            return false;
        else if (!Base::template match_zero<typename Reader::encoding>(reader.peek()))
            return false;

        reader.bump();
        return true;
    }

    template <typename Reader>
    LEXY_DSL_FUNC auto error(const Reader&, typename Reader::iterator pos)
    {
        return lexy::error<Reader, lexy::expected_char_class>(pos, "digit.zero");
    }
};

template <typename Base>
struct _nzero : atom_base<_nzero<Base>>
{
    template <typename Reader>
    LEXY_DSL_FUNC bool match(Reader& reader)
    {
        if (!Base::template match<typename Reader::encoding>(reader.peek()))
            return false;
        else if (Base::template match_zero<typename Reader::encoding>(reader.peek()))
            return false;

        reader.bump();
        return true;
    }

    template <typename Reader>
    LEXY_DSL_FUNC auto error(const Reader& reader, typename Reader::iterator pos)
    {
        if (Base::template match_zero<typename Reader::encoding>(reader.peek()))
            return lexy::error<Reader, lexy::expected_char_class>(pos, "digit.non-zero");
        else
            return lexy::error<Reader, lexy::expected_char_class>(pos, Base::name());
    }
};

template <typename Base>
struct _digit : atom_base<_digit<Base>>
{
    template <typename Reader>
    LEXY_DSL_FUNC bool match(Reader& reader)
    {
        if (!Base::template match<typename Reader::encoding>(reader.peek()))
            return false;

        reader.bump();
        return true;
    }

    template <typename Reader>
    LEXY_DSL_FUNC auto error(const Reader&, typename Reader::iterator pos)
    {
        return lexy::error<Reader, lexy::expected_char_class>(pos, Base::name());
    }

    //=== dsl ===//
    LEXY_CONSTEVAL auto zero() const
    {
        return _zero<Base>{};
    }

    LEXY_CONSTEVAL auto non_zero() const
    {
        return _nzero<Base>{};
    }
};

/// Matches a single digit.
template <typename Base = decimal>
constexpr auto digit = _digit<Base>{};
} // namespace lexyd

namespace lexy
{
struct forbidden_leading_zero
{};
} // namespace lexy

namespace lexyd
{
template <typename Base, typename Sep, bool LeadingZero>
LEXY_CONSTEVAL auto _make_digits()
{
    auto d = digit<Base>;
    if constexpr (LeadingZero)
    {
        if constexpr (std::is_same_v<Sep, void>)
            return d + while_(d);
        else
            return d + while_(opt(Sep{}) + d);
    }
    else
    {
        if constexpr (std::is_same_v<Sep, void>)
            return (d.zero() + prevent<lexy::forbidden_leading_zero>(d))
                   / (d.non_zero() + while_(d));
        else
            return (d.zero() + prevent<lexy::forbidden_leading_zero>(d))
                   / (d.non_zero() + while_(opt(Sep{}) + d));
    }
}

template <typename Base, typename Sep, bool LeadingZero>
struct _digits : decltype(_make_digits<Base, Sep, LeadingZero>())
{
    template <typename Pattern>
    LEXY_CONSTEVAL auto sep(Pattern) const
    {
        static_assert(lexy::is_pattern<Pattern>);
        return _digits<Base, Pattern, LeadingZero>{};
    }

    LEXY_CONSTEVAL auto no_leading_zero() const
    {
        return _digits<Base, Sep, false>{};
    }
};

/// Matches a non-empty list of digits.
template <typename Base = decimal>
constexpr auto digits = _digits<Base, void, true>{};

constexpr auto digit_sep_underscore = LEXY_LIT("_");
constexpr auto digit_sep_tick       = LEXY_LIT("'");
} // namespace lexyd

#endif // LEXY_DSL_DIGIT_HPP_INCLUDED

