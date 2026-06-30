#include "amaranth/distance.hpp"
#include "amaranth/errors.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

namespace amaranth {

namespace {
void require_same_dimension(const Vector& lhs, const Vector& rhs) {
    if (lhs.size() != rhs.size()) {
        throw DimensionMismatchError(lhs.size(), rhs.size());
    }
}
}  // namespace

float inner_product(const Vector& lhs, const Vector& rhs) {
    require_same_dimension(lhs, rhs);
    float sum = 0.0F;
    for (std::size_t i = 0; i < lhs.size(); ++i) {
        sum += lhs[i] * rhs[i];
    }
    return sum;
}

float l2_distance(const Vector& lhs, const Vector& rhs) {
    require_same_dimension(lhs, rhs);
    float sum = 0.0F;
    for (std::size_t i = 0; i < lhs.size(); ++i) {
        const float diff = lhs[i] - rhs[i];
        sum += diff * diff;
    }
    return std::sqrt(sum);
}

float cosine_similarity(const Vector& lhs, const Vector& rhs) {
    require_same_dimension(lhs, rhs);
    float dot = 0.0F;
    float lhs_norm = 0.0F;
    float rhs_norm = 0.0F;

    for (std::size_t i = 0; i < lhs.size(); ++i) {
        dot += lhs[i] * rhs[i];
        lhs_norm += lhs[i] * lhs[i];
        rhs_norm += rhs[i] * rhs[i];
    }

    if (lhs_norm <= std::numeric_limits<float>::epsilon() ||
        rhs_norm <= std::numeric_limits<float>::epsilon()) {
        return 0.0F;
    }

    return dot / (std::sqrt(lhs_norm) * std::sqrt(rhs_norm));
}

float score_by_metric(const Vector& query, const Vector& candidate, MetricType metric) {
    switch (metric) {
        case MetricType::Cosine:
            return cosine_similarity(query, candidate);
        case MetricType::L2:
            // Make larger score better, so we negate the distance.
            return -l2_distance(query, candidate);
        case MetricType::InnerProduct:
            return inner_product(query, candidate);
    }
    return cosine_similarity(query, candidate);
}

}  // namespace amaranth
