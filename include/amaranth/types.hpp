#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace amaranth {

using Vector = std::vector<float>;
using Metadata = std::unordered_map<std::string, std::string>;

using CollectionName = std::string;
using VectorID = std::string;

struct Document {
    VectorID id;
    Vector vector;
    Metadata metadata;
};

struct SearchResult {
    VectorID id;
    float score = 0.0F;
    std::uint32_t rank = 0;
    Metadata metadata;
};

enum class MetricType : std::uint8_t {
    Cosine = 0,
    L2 = 1,
    InnerProduct = 2,
};

inline std::string metric_to_string(MetricType metric) {
    switch (metric) {
        case MetricType::Cosine:
            return "cosine";
        case MetricType::L2:
            return "l2";
        case MetricType::InnerProduct:
            return "inner_product";
    }
    return "unknown";
}

inline MetricType metric_from_string(const std::string& value) {
    if (value == "cosine") {
        return MetricType::Cosine;
    }
    if (value == "l2") {
        return MetricType::L2;
    }
    if (value == "inner_product" || value == "ip") {
        return MetricType::InnerProduct;
    }
    return MetricType::Cosine;
}

}  // namespace amaranth
