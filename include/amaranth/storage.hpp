#pragma once

#include "amaranth/types.hpp"

#include <filesystem>
#include <vector>

namespace amaranth {

struct PersistedCollection {
    std::size_t dimension = 0;
    MetricType metric = MetricType::Cosine;
    std::vector<Document> documents;
};

void save_collection_to_file(const std::filesystem::path& path,
                             const PersistedCollection& collection);

PersistedCollection load_collection_from_file(const std::filesystem::path& path);

}  // namespace amaranth
