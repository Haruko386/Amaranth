#include "amaranth/amaranth.hpp"

#include <iostream>

int main() {
    amaranth::Collection collection("papers", 4, amaranth::MetricType::Cosine);

    amaranth::Document d1;
    d1.id = "chunk_001";
    d1.vector = {0.1F, 0.2F, 0.3F, 0.4F};
    d1.metadata["paper_id"] = "paper_001";
    d1.metadata["page"] = "1";

    amaranth::Document d2;
    d2.id = "chunk_002";
    d2.vector = {0.9F, 0.1F, 0.1F, 0.2F};
    d2.metadata["paper_id"] = "paper_002";
    d2.metadata["page"] = "3";

    collection.upsert(d1);
    collection.upsert(d2);

    auto results = collection.search({0.1F, 0.2F, 0.31F, 0.39F}, 2);

    for (const auto& result : results) {
        std::cout << result.rank << ". " << result.id << " score=" << result.score << "\n";
    }

    collection.save("example.amdb");
    auto loaded = amaranth::Collection::load("papers", "example.amdb");
    std::cout << "loaded collection size=" << loaded.size() << "\n";

    return 0;
}
