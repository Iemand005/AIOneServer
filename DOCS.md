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
    modelId: string,
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
    modelId: string,
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
    modelId: string,
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
    modelId: string
};
```

```ts
type UnloadModelResponse = {
    success: true
};
```

### Create new managed context

Creates a new context or multiple, used for prompt caching.
When generating using a managed context, the prompt caching will be stored in that context and can be reused.
Reusing it removes the tokens which do not match what was catched (so it only keeps the last generated prompt as cache).

| Property | Value |
| --- | --- |
| Name | `context` |
| Method | POST |

```ts
type CreateContextRequest = {
    modelId: string,
    amount?: number // amount of contexts to create, default 1
};
```

```ts
type CreateContextResponse = {
    success: true,
    ctxIds: string[] // uuids
};
```