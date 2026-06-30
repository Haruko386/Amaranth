#include "amaranth/amaranth.hpp"

#include <cassert>
#include <filesystem>
#include <iostream>

namespace {

amaranth::Document doc(std::string id, std::vector<float> vector) {
    amaranth::Document d;
    d.id = std::move(id);
    d.vector = std::move(vector);
    d.metadata["kind"] = "test";
    return d;
}

void test_insert_and_search() {
    amaranth::Collection collection("test", 3, amaranth::MetricType::Cosine);
    collection.upsert(doc("a", {1.0F, 0.0F, 0.0F}));
    collection.upsert(doc("b", {0.0F, 1.0F, 0.0F}));
    collection.upsert(doc("c", {0.0F, 0.0F, 1.0F}));

    auto results = collection.search({1.0F, 0.1F, 0.0F}, 2);
    assert(results.size() == 2);
    assert(results[0].id == "a");
    assert(results[0].rank == 1);
    assert(collection.size() == 3);
}

void test_remove() {
    amaranth::Collection collection("test", 2, amaranth::MetricType::InnerProduct);
    collection.upsert(doc("a", {1.0F, 2.0F}));
    assert(collection.contains("a"));
    assert(collection.remove("a"));
    assert(!collection.contains("a"));
    assert(collection.size() == 0);
}

void test_save_and_load() {
    const std::filesystem::path path = "test_collection.amdb";

    amaranth::Collection collection("test", 2, amaranth::MetricType::L2);
    collection.upsert(doc("near", {1.0F, 1.0F}));
    collection.upsert(doc("far", {9.0F, 9.0F}));
    collection.save(path);

    auto loaded = amaranth::Collection::load("test", path);
    assert(loaded.size() == 2);
    assert(loaded.dimension() == 2);
    assert(loaded.metric() == amaranth::MetricType::L2);

    auto results = loaded.search({1.1F, 1.1F}, 1);
    assert(results.size() == 1);
    assert(results[0].id == "near");

    std::filesystem::remove(path);
}

}  // namespace

int main() {
    test_insert_and_search();
    test_remove();
    test_save_and_load();
    std::cout << "all tests passed\n";
    return 0;
}
