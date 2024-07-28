# Flux Browser JS API Docs

## Tab Management

```ts
window.tabs.requestSplitTab(url: string): null
window.tabs.requestSplitTab(): null
window.tabs.requestFlipTabs(): null
window.tabs.requestNewGroup(url: string): null
```
## File API

```ts
window.fs.listDir(path: string, subdir?: boolean) : Array<{path: string, isDir: boolean, dirs: Array<{path: string, isDir: boolean}>}>
window.fs.getFileUrl(): string
window.fs.getFolderUrl(): string
```