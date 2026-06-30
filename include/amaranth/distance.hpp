#pragma once

#include "amaranth/types.hpp"

namespace amaranth {

float inner_product(const Vector& lhs, const Vector& rhs);
float l2_distance(const Vector& lhs, const Vector& rhs);
float cosine_similarity(const Vector& lhs, const Vector& rhs);

// Larger score is always better for search results.
float score_by_metric(const Vector& query, const Vector& candidate, MetricType metric);

}  // namespace amaranth
