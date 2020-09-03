#pragma once

/// @file storages/postgres/io/decimal.hpp
/// @brief Decimal64 I/O support

#include <storages/postgres/io/buffer_io.hpp>
#include <storages/postgres/io/buffer_io_base.hpp>
#include <storages/postgres/io/numeric_data.hpp>
#include <storages/postgres/io/type_mapping.hpp>

#include <utils/decimal64.hpp>

namespace storages::postgres::io {

template <int Prec, typename RoundPolicy>
struct BufferFormatter<decimal64::decimal<Prec, RoundPolicy>>
    : detail::BufferFormatterBase<decimal64::decimal<Prec, RoundPolicy>> {
  using BaseType =
      detail::BufferFormatterBase<decimal64::decimal<Prec, RoundPolicy>>;
  using BaseType::BaseType;
  using ValueType = typename BaseType::ValueType;

  template <typename Buffer>
  void operator()(const UserTypes&, Buffer& buffer) const {
    auto bin_str =
        detail::Int64ToNumericBuffer({this->value.getUnbiased(), Prec});
    buffer.reserve(buffer.size() + bin_str.size());
    std::copy(bin_str.begin(), bin_str.end(), std::back_inserter(buffer));
  }
};

template <int Prec, typename RoundPolicy>
struct BufferParser<decimal64::decimal<Prec, RoundPolicy>>
    : detail::BufferParserBase<decimal64::decimal<Prec, RoundPolicy>> {
  using BaseType =
      detail::BufferParserBase<decimal64::decimal<Prec, RoundPolicy>>;
  using BaseType::BaseType;
  using ValueType = typename BaseType::ValueType;

  void operator()(const FieldBuffer& buffer) {
    auto rep = detail::NumericBufferToInt64(buffer);
    this->value =
        ValueType::FromFloatingPoint(rep.value, -rep.fractional_digit_count);
  }
};

template <int Prec, typename RoundPolicy>
struct CppToSystemPg<decimal64::decimal<Prec, RoundPolicy>>
    : PredefinedOid<PredefinedOids::kNumeric> {};
}  // namespace storages::postgres::io
