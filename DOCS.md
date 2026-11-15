# Documentation

## Generic routes

Base path: `/aio/`

### Get information about server

| Property | Value |
| --- | --- |
| Name | `info` |
| Method | GET |

```ts
type GetInfoResponse = {
    version: string // e.g. "1.0.0"
};
```

### Get loaded models

| Property | Value |
| --- | --- |
| Name | `models` |
| Method | GET |

```ts
type SummarizedModel = {
    id: string,
    type: "llm" | "sd" | "tts" | "unknown",
    loaded: boolean
};

type GetModelsResponse = SummarizedModel[];
```

## LLM routes

Base path: `/aio/llm/`

### Load model

| Property | Value |
| --- | --- |
| Name | `load` |
| Method | POST |

```ts
type LoadModelRequest = {
    id: string,
    path: string
};
```

```ts
type LoadModelResponse = {
    success: true
};
```

### Unload model

| Property | Value |
| --- | --- |
| Name | `unload` |
| Method | POST |

```ts
type UnloadModelRequest = {
    id: string
};
```

```ts
type UnloadModelResponse = {
    success: true
};
```