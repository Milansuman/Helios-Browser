# Flux Browser JS API Docs

## Tab Management

```ts
window.tabs.requestSplitTab(url: string): null
window.tabs.requestSplitTab(): null
window.tabs.requestFlipTabs(): null
window.tabs.requestNewGroup(url: string): null
window.tabs.addTabs(tabs: Array<Array<string>>): boolean //array of array of urls
window.tabs.load(group: number, tab: number, url: string): null
```
## File API

```ts
window.fs.listDir(path: string, subdir?: boolean) : Array<{path: string, isDir: boolean, dirs: Array<{path: string, isDir: boolean}>}>
window.fs.getFileUrl(): string
window.fs.getFolderUrl(): string
```
## Dialog API
```ts
window.dialog.closeDialog()
```

## Ollama API
```ts
window.ai.generate(model: string, prompt: string): {
  model: string;
  created_at: string;
  response: string;
  done: boolean;
  context: number[];
  total_duration: number;
  load_duration: number;
  prompt_eval_count: number;
  prompt_eval_duration: number;
  eval_count: number;
  eval_duration: number;
};
```