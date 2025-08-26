#pragma once

template <typename T, typename TagT>
class StrongType
{
public:
    constexpr explicit StrongType(T value) : mValue{value} {}

    constexpr StrongType(StrongType const&) = default;
    constexpr StrongType(StrongType&&) = default;
    constexpr StrongType& operator=(StrongType const&) = default;
    constexpr StrongType& operator=(StrongType&&) = default;

    constexpr T const& raw() const noexcept
    {
        return mValue;
    }


private:
    T mValue;
};

template <typename T, typename TagT>
constexpr bool operator<(StrongType<T, TagT> const& one, StrongType<T, TagT> const& other)
{
    return one.raw() < other.raw();
}

template <typename T, typename TagT>
constexpr bool operator==(StrongType<T, TagT> const& one, StrongType<T, TagT> const& other)
{
    return one.raw() == other.raw();
}