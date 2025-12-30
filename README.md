# elog

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![C/C++](https://img.shields.io/badge/language-C%2FC%2B%2B-blue.svg)](https://isocpp.org/)

A fast and lightweight embedded logging library for C/C++

組み込みシステム向けの高速・軽量ロギングライブラリ

---

## 📖 Table of Contents / 目次

- [English](#english)
  - [Features](#features)
  - [Design Philosophy](#design-philosophy)
  - [Quick Start](#quick-start)
  - [API Reference](#api-reference)
  - [CMake Configuration](#cmake-configuration)
  - [Advanced Usage](#advanced-usage)
- [日本語](#日本語)
  - [特徴](#特徴)
  - [設計思想](#設計思想)
  - [クイックスタート](#クイックスタート)
  - [API リファレンス](#api-リファレンス)
  - [CMake 設定](#cmake-設定)
  - [高度な使い方](#高度な使い方)

---

# English

## Features

✨ **Compile-Time Optimization**
- Unused log levels are completely eliminated at compile time
- Zero runtime overhead for disabled logs
- No code generation for filtered-out log statements

🎨 **Highly Customizable**
- ANSI color support with customizable colors
- Flexible log format configuration
- File:line display with customizable format

⚡ **Lightweight & Fast**
- printf-based implementation
- Minimal runtime branching
- Static library with small footprint

🔧 **Easy Integration**
- CMake-based configuration
- C99 / C++11 compatible
- No dynamic memory allocation
- No exceptions or RTTI required

📊 **Multiple Log Levels**
- CRITICAL, ERROR, WARN, INFO, DEBUG, TRACE
- Compile-time and runtime level filtering
- Easy level management API

## Design Philosophy

elog follows these core principles:

1. **"Don't create logs" instead of "Don't output logs"**
   - Logs above the compile-time level don't exist in the binary
   - No runtime cost for disabled logs

2. **Predictability**
   - Simple printf-based implementation
   - No hidden allocations or complex formatting
   - Behavior is deterministic and easy to understand

3. **Compile-Time Configuration**
   - Most decisions made at compile time
   - Minimal runtime overhead
   - Optimal code generation

4. **Embedded-First**
   - Works with newlib-nano, picolibc, and other embedded C libraries
   - Suitable for UART, RTT, ITM, and other embedded output methods
   - ISR-safe (depends on printf implementation)

## Quick Start

### Installation

1. **Add as a subdirectory**

```cmake
# In your CMakeLists.txt
add_subdirectory(lib/elog)
target_link_libraries(your_target PRIVATE elog::elog)
```

2. **Configure options (optional)**

```cmake
# Set compile-time log level
set(ELOG_COMPILED_LEVEL ELOG_LEVEL_DEBUG)

# Enable runtime level filtering
set(ELOG_USE_RUNTIME_LEVEL ON)

# Enable ANSI colors
set(ELOG_USE_COLOR ON)

# Enable file:line display
set(ELOG_USE_FILE_LINE ON)

add_subdirectory(lib/elog)
```

### Basic Usage

```c
#include "elog/elog.h"

int main(void) {
    // Basic logging
    ELOG_INFO("System initialized");
    ELOG_WARN("Low battery: %d%%", battery_level);
    ELOG_ERROR("I2C error: %d", error_code);
    
    // With runtime level control
    ELOG_SET_LOG_LEVEL(ELOG_LEVEL_DEBUG);
    ELOG_DEBUG("Debug info: x=%d, y=%d", x, y);
    
    return 0;
}
```

### Output Example

```
[    INFO] [main.c: 5] System initialized
[    WARN] [main.c: 6] Low battery: 25%
[   ERROR] [main.c: 7] I2C error: -1
[   DEBUG] [main.c: 11] Debug info: x=100, y=200
```

## API Reference

### Log Macros

```c
ELOG_CRITICAL(fmt, ...)  // Critical errors
ELOG_ERROR(fmt, ...)     // Errors
ELOG_WARN(fmt, ...)      // Warnings
ELOG_INFO(fmt, ...)      // Information
ELOG_DEBUG(fmt, ...)     // Debug messages
ELOG_TRACE(fmt, ...)     // Trace/verbose messages
```

### Runtime Level Control

```c
// Set runtime log level (only when ELOG_USE_RUNTIME_LEVEL=ON)
ELOG_SET_LOG_LEVEL(ELOG_LEVEL_DEBUG);

// Get current log level
uint8_t level = ELOG_GET_LOG_LEVEL();
```

### Log Levels

```c
typedef enum {
    ELOG_LEVEL_OFF = 0,      // No logging
    ELOG_LEVEL_CRITICAL,     // Critical errors only
    ELOG_LEVEL_ERROR,        // Errors and above
    ELOG_LEVEL_WARN,         // Warnings and above
    ELOG_LEVEL_INFO,         // Info and above (default)
    ELOG_LEVEL_DEBUG,        // Debug and above
    ELOG_LEVEL_TRACE         // All logs
} elog_level_t;
```

## CMake Configuration

### Basic Options

| Option | Default | Description |
|--------|---------|-------------|
| `ELOG_COMPILED_LEVEL` | `ELOG_LEVEL_INFO` | Compile-time log level |
| `ELOG_USE_RUNTIME_LEVEL` | `ON` | Enable runtime level filtering |
| `ELOG_USE_FILE_LINE` | `ON` | Show file:line information |
| `ELOG_USE_COLOR` | `ON` | Enable ANSI colors |

### Color Customization

```cmake
set(ELOG_COLOR_ERROR "\\033[91m")     # Bright red
set(ELOG_COLOR_WARN "\\033[93m")      # Bright yellow
set(ELOG_COLOR_INFO "\\033[94m")      # Bright blue
set(ELOG_COLOR_DEBUG "\\033[96m")     # Bright cyan
set(ELOG_COLOR_TRACE "\\033[37m")     # White
set(ELOG_COLOR_CRITICAL "\\033[95m")  # Bright magenta
set(ELOG_COLOR_RESET "\\033[0m")      # Reset
```

### Format Customization

```cmake
# Level format
set(ELOG_LEVEL_FMT_ERROR "ERR:")
set(ELOG_LEVEL_FMT_WARN "WRN:")
set(ELOG_LEVEL_FMT_INFO "INF:")
set(ELOG_LEVEL_FMT_DEBUG "DBG:")
set(ELOG_LEVEL_FMT_TRACE "TRC:")

# File:line format
set(ELOG_FILE_LINE_FMT "[%10.10s @ %3d]")
```

## Advanced Usage

### Production Build (Minimal Logs)

```cmake
# Only show errors and critical messages
set(ELOG_COMPILED_LEVEL ELOG_LEVEL_ERROR)
set(ELOG_USE_COLOR OFF)
set(ELOG_USE_FILE_LINE OFF)
set(ELOG_USE_RUNTIME_LEVEL OFF)
```

### Development Build (Full Logging)

```cmake
# Show all logs with colors and file info
set(ELOG_COMPILED_LEVEL ELOG_LEVEL_TRACE)
set(ELOG_USE_COLOR ON)
set(ELOG_USE_FILE_LINE ON)
set(ELOG_USE_RUNTIME_LEVEL ON)
```

### Custom Format Example

```cmake
# Output: ERR: [    file.c @  23] message
set(ELOG_LEVEL_FMT_ERROR "ERR:")
set(ELOG_FILE_LINE_FMT "[%10.10s @ %3d]")
```

---

# 日本語

## 特徴

✨ **コンパイル時最適化**
- 使用しないログレベルは完全にコンパイル時に削除
- 無効化されたログの実行時オーバーヘッドはゼロ
- フィルタされたログ文は一切コード生成されない

🎨 **高度なカスタマイズ性**
- ANSIカラー対応（色のカスタマイズ可能）
- 柔軟なログフォーマット設定
- ファイル名:行番号表示（フォーマットカスタマイズ可能）

⚡ **軽量・高速**
- printf ベースの実装
- 最小限の実行時分岐
- 小さなフットプリントの静的ライブラリ

🔧 **簡単な統合**
- CMake ベースの設定
- C99 / C++11 対応
- 動的メモリ確保なし
- 例外・RTTI 不要

📊 **複数のログレベル**
- CRITICAL, ERROR, WARN, INFO, DEBUG, TRACE
- コンパイル時・実行時のレベルフィルタリング
- 簡単なレベル管理API

## 設計思想

elog は以下の原則に従います：

1. **「ログを出力しない」ではなく「ログを作らない」**
   - コンパイル時レベルより上のログはバイナリに存在しない
   - 無効なログの実行時コストはゼロ

2. **予測可能性**
   - シンプルな printf ベースの実装
   - 隠れたメモリ確保や複雑なフォーマット処理なし
   - 動作が決定的で理解しやすい

3. **コンパイル時設定**
   - ほとんどの決定をコンパイル時に実施
   - 最小限の実行時オーバーヘッド
   - 最適なコード生成

4. **組み込み環境ファースト**
   - newlib-nano、picolibc などの組み込み C ライブラリと動作
   - UART、RTT、ITM などの組み込み出力方式に適合
   - ISR セーフ（printf 実装に依存）

## クイックスタート

### インストール

1. **サブディレクトリとして追加**

```cmake
# CMakeLists.txt に追加
add_subdirectory(lib/elog)
target_link_libraries(your_target PRIVATE elog::elog)
```

2. **オプション設定（任意）**

```cmake
# コンパイル時ログレベルを設定
set(ELOG_COMPILED_LEVEL ELOG_LEVEL_DEBUG)

# 実行時レベルフィルタリングを有効化
set(ELOG_USE_RUNTIME_LEVEL ON)

# ANSI カラーを有効化
set(ELOG_USE_COLOR ON)

# ファイル名:行番号表示を有効化
set(ELOG_USE_FILE_LINE ON)

add_subdirectory(lib/elog)
```

### 基本的な使い方

```c
#include "elog/elog.h"

int main(void) {
    // 基本的なログ出力
    ELOG_INFO("システム初期化完了");
    ELOG_WARN("バッテリー低下: %d%%", battery_level);
    ELOG_ERROR("I2Cエラー: %d", error_code);
    
    // 実行時レベル制御
    ELOG_SET_LOG_LEVEL(ELOG_LEVEL_DEBUG);
    ELOG_DEBUG("デバッグ情報: x=%d, y=%d", x, y);
    
    return 0;
}
```

### 出力例

```
[    INFO] [main.c:5] システム初期化完了
[    WARN] [main.c:6] バッテリー低下: 25%
[   ERROR] [main.c:7] I2Cエラー: -1
[   DEBUG] [main.c:11] デバッグ情報: x=100, y=200
```

## API リファレンス

### ログマクロ

```c
ELOG_CRITICAL(fmt, ...)  // クリティカルエラー
ELOG_ERROR(fmt, ...)     // エラー
ELOG_WARN(fmt, ...)      // 警告
ELOG_INFO(fmt, ...)      // 情報
ELOG_DEBUG(fmt, ...)     // デバッグメッセージ
ELOG_TRACE(fmt, ...)     // トレース/詳細メッセージ
```

### 実行時レベル制御

```c
// 実行時ログレベルを設定（ELOG_USE_RUNTIME_LEVEL=ON の場合のみ）
ELOG_SET_LOG_LEVEL(ELOG_LEVEL_DEBUG);

// 現在のログレベルを取得
uint8_t level = ELOG_GET_LOG_LEVEL();
```

### ログレベル

```c
typedef enum {
    ELOG_LEVEL_OFF = 0,      // ログなし
    ELOG_LEVEL_CRITICAL,     // クリティカルエラーのみ
    ELOG_LEVEL_ERROR,        // エラー以上
    ELOG_LEVEL_WARN,         // 警告以上
    ELOG_LEVEL_INFO,         // 情報以上（デフォルト）
    ELOG_LEVEL_DEBUG,        // デバッグ以上
    ELOG_LEVEL_TRACE         // すべてのログ
} elog_level_t;
```

## CMake 設定

### 基本オプション

| オプション | デフォルト | 説明 |
|-----------|----------|------|
| `ELOG_COMPILED_LEVEL` | `ELOG_LEVEL_INFO` | コンパイル時ログレベル |
| `ELOG_USE_RUNTIME_LEVEL` | `ON` | 実行時レベルフィルタリングを有効化 |
| `ELOG_USE_FILE_LINE` | `ON` | ファイル名:行番号情報を表示 |
| `ELOG_USE_COLOR` | `ON` | ANSI カラーを有効化 |

### カラーのカスタマイズ

```cmake
set(ELOG_COLOR_ERROR "\\033[91m")     # 明るい赤
set(ELOG_COLOR_WARN "\\033[93m")      # 明るい黄色
set(ELOG_COLOR_INFO "\\033[94m")      # 明るい青
set(ELOG_COLOR_DEBUG "\\033[96m")     # 明るいシアン
set(ELOG_COLOR_TRACE "\\033[37m")     # 白
set(ELOG_COLOR_CRITICAL "\\033[95m")  # 明るいマゼンタ
set(ELOG_COLOR_RESET "\\033[0m")      # リセット
```

### フォーマットのカスタマイズ

```cmake
# レベルフォーマット
set(ELOG_LEVEL_FMT_ERROR "ERR:")
set(ELOG_LEVEL_FMT_WARN "WRN:")
set(ELOG_LEVEL_FMT_INFO "INF:")
set(ELOG_LEVEL_FMT_DEBUG "DBG:")
set(ELOG_LEVEL_FMT_TRACE "TRC:")

# ファイル名:行番号フォーマット
set(ELOG_FILE_LINE_FMT "[%10.10s @ %3d]")
```

## 高度な使い方

### 本番ビルド（最小限のログ）

```cmake
# エラーとクリティカルメッセージのみ表示
set(ELOG_COMPILED_LEVEL ELOG_LEVEL_ERROR)
set(ELOG_USE_COLOR OFF)
set(ELOG_USE_FILE_LINE OFF)
set(ELOG_USE_RUNTIME_LEVEL OFF)
```

### 開発ビルド（完全なログ）

```cmake
# すべてのログをカラー・ファイル情報付きで表示
set(ELOG_COMPILED_LEVEL ELOG_LEVEL_TRACE)
set(ELOG_USE_COLOR ON)
set(ELOG_USE_FILE_LINE ON)
set(ELOG_USE_RUNTIME_LEVEL ON)
```

### カスタムフォーマット例

```cmake
# Output: ERR: [    file.c @  23] message
set(ELOG_LEVEL_FMT_ERROR "ERR:")
set(ELOG_FILE_LINE_FMT "[%10.10s @ %3d]")
```

---

## License

MIT License - See [LICENSE](LICENSE) file for details

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Author

[yutatech](https://github.com/yutatech)