#pragma once

#include <stdexcept>
#include <string>

namespace amaranth {

class AmaranthError : public std::runtime_error {
public:
    explicit AmaranthError(const std::string& message) : std::runtime_error(message) {}
};

class DimensionMismatchError : public AmaranthError {
public:
    DimensionMismatchError(std::size_t expected, std::size_t actual)
        : AmaranthError("dimension mismatch: expected " + std::to_string(expected) +
                        ", got " + std::to_string(actual)) {}
};

class NotFoundError : public AmaranthError {
public:
    explicit NotFoundError(const std::string& message) : AmaranthError(message) {}
};

}  // namespace amaranth
