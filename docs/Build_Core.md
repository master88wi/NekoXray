## 构建 nekoray_core & nekobox_core

### 目录结构

```
  | nekoray
  |   go/cmd/*
  | Xray-core
  | sing-box-extra
  | sing-box
  | ......
```

### 常规构建

1. `bash libs/get_source.sh` （自动下载目录结构，自动同步最新 commit）
2. `GOOS=windows GOARCH=amd64 bash libs/build_go.sh` or `CGO_ENABLED=1 bash libs/build_go.sh`

编译好的 Core 会在 `development/${目标平台}` 中，如果未指定目标平台，则会位于 `development/local`。

未指定架构和平台时，可以开启 `CGO`。

具体支持的 GOOS 和 GOARCH 请看 `libs/build_go.sh`。

非官方构建无需编译 `updater` `launcher`。

### sing-box tags

具体使用的 tags 请看 `libs/build_go.sh`
