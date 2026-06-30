# Amaranth API Draft

This is a future HTTP API draft. It is not implemented yet.

## Create collection

```http
POST /collections
```

```json
{
  "name": "papers",
  "dimension": 768,
  "metric": "cosine"
}
```

## Insert vectors

```http
POST /collections/papers/vectors
```

```json
{
  "vectors": [
    {
      "id": "chunk_001",
      "vector": [0.1, 0.2, 0.3],
      "metadata": {
        "paper_id": "paper_001",
        "page": "1"
      }
    }
  ]
}
```

## Search

```http
POST /collections/papers/search
```

```json
{
  "vector": [0.1, 0.2, 0.3],
  "top_k": 5
}
```

```json
{
  "results": [
    {
      "id": "chunk_001",
      "score": 0.98,
      "rank": 1,
      "metadata": {
        "paper_id": "paper_001",
        "page": "1"
      }
    }
  ]
}
```
