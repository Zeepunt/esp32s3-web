# 目录结构

```shell
.
|- esp32 # esp32s3 ESP-IDF 项目
|- web   # Web 项目
```

esp32 依赖项：

- ESP-IDF >= 5.5.0

web 依赖项：

- Node.js >= 24.13.0

# 运行

esp32：

```shell
# 编译整个项目
idf.py build

# 仅编译 app
idf.py app

# 清理
idf.py clean

# 通过串口 5 烧录整个项目
idf.py flash -p COM5

# 通过串口 5 烧录 app
idf.py app-flash -p COM5
```

web：

```shell
# 调试运行
npm run dev

# 预览生产版本
npm run preview

# 构建生产版本
npm run build
```

