# Documentation

```ts
type FailureResponse = {
    error: string
};
```

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

Includes both loaded models and unloaded but registered models.

| Property | Value |
| --- | --- |
| Name | `models` |
| Method | GET |

```ts
type SummarizedModel = {
    id: string,
    type: "llm" | "sd" | "tts" | "unknown",
    loaded: boolean // if false, model is registered and not loaded
};

type GetModelsResponse = SummarizedModel[];
```

## LLM routes

Base path: `/aio/llm/`

### Register model

Registering a model won't load it, but it will allow you to use only the ID for loading it.
Pretty much just mapping the ID to a file so you don't have to enter the file path everytime you load the model.

| Property | Value |
| --- | --- |
| Name | `register` |
| Method | POST |

```ts
type RegisterModelRequest = {
    id: string,
    path: string
};
```

```ts
type RegisterModelResponse = {
    success: true
};
```

### Load model

| Property | Value |
| --- | --- |
| Name | `load` |
| Method | POST |

```ts
type LoadModelRequest = {
    id: string,
    path?: string // if not supplied, model must be registered with id
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