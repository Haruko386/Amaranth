# Amaranth

Amaranth is a lightweight C++ vector search engine skeleton designed for RAG experiments.

The current version is intentionally small: it gives you a clean framework that you can extend later into a real vector database.

## Current status

Version: `0.1.0`

Implemented:

- Collection abstraction
- Flat vector index
- Cosine similarity
- L2 distance
- Inner product
- Upsert vectors
- Delete vectors
- TopK search
- Simple metadata map
- Binary save/load
- In-memory database container
- CLI demo
- Basic tests

Not implemented yet:

- HTTP/gRPC server
- HNSW / IVF / PQ
- WAL
- Segment compaction
- Concurrent writes
- Metadata filter pushdown
- Distributed storage

## Project structure

```text
amaranth/
├── apps/
│   └── amaranth_cli/          # small CLI for demo/search
├── examples/                  # usage examples
├── include/amaranth/          # public headers
├── src/                       # implementation
├── tests/                     # basic unit tests
├── CMakeLists.txt
└── README.md
```

## Build

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Run demo

```bash
./build/amaranth_cli demo
```

Expected output is similar to:

```text
#1 id=chunk_001 score=...
#2 id=chunk_002 score=...
#3 id=chunk_003 score=...
```

## Create and search a sample collection

```bash
./build/amaranth_cli make-sample ./papers.amdb
./build/amaranth_cli search ./papers.amdb 3 0.1,0.2,0.3
```

## Basic usage

```cpp
#include "amaranth/amaranth.hpp"

int main() {
    amaranth::Collection collection("papers", 3, amaranth::MetricType::Cosine);

    amaranth::Document doc;
    doc.id = "chunk_001";
    doc.vector = {0.1F, 0.2F, 0.3F};
    doc.metadata["paper_id"] = "paper_001";
    doc.metadata["page"] = "1";

    collection.upsert(doc);

    auto results = collection.search({0.1F, 0.2F, 0.31F}, 5);
    collection.save("papers.amdb");

    return 0;
}
```

## Design notes

The current search implementation is `FlatIndex`:

```text
query vector
  ↓
scan all documents
  ↓
compute score by metric
  ↓
sort by score
  ↓
return topK
```

This is slow for very large datasets, but it is exact and simple. It is a good baseline before implementing HNSW or IVF.

## Suggested roadmap

### v0.2: Service layer

- Add HTTP server
- Add JSON request/response
- Add endpoints:
  - `POST /collections`
  - `POST /collections/{name}/vectors`
  - `POST /collections/{name}/search`
  - `DELETE /collections/{name}/vectors/{id}`
  - `POST /collections/{name}/save`
  - `POST /collections/{name}/load`

### v0.3: RAG integration

- Use `chunk_id` as vector id
- Store full chunk text in Go/PostgreSQL
- Store embeddings in Amaranth
- Let Go call Amaranth as an optional vector backend

### v0.4: Performance

- Add SIMD distance calculation
- Add batch insert
- Add benchmark against FlatIndex

### v0.5: ANN index

- Add HNSW index interface
- Keep FlatIndex as exact baseline
- Compare recall and latency

## Role in the paper RAG system

Recommended split:

```text
MinIO       -> original PDFs, page images, OCR JSON, parsed artifacts
PostgreSQL -> paper metadata, task status, chunks, page information
ES          -> BM25 / keyword search
Amaranth    -> vector search
Go API      -> orchestration, hybrid retrieval, RAG answer generation
```

Amaranth should store vector-oriented data only:

```text
chunk_id
embedding vector
small metadata fields, such as paper_id/page_no
```

The full chunk text should remain in the Go RAG system database.
