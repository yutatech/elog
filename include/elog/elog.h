/**
 * @file elog.h
 * @brief elog - Embedded Lightweight Organized Logger
 *
 * A fast and lightweight embedded logging library for C/C++.
 * Single-header, printf-based, compile-time optimized logging system.
 */

#ifndef ELOG_H
#define ELOG_H

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================
 * 1. ログレベル定義
 * ============================================================ */

typedef enum {
  ELOG_LEVEL_OFF = 0,
  ELOG_LEVEL_CRITICAL,
  ELOG_LEVEL_ERROR,
  ELOG_LEVEL_WARN,
  ELOG_LEVEL_INFO,
  ELOG_LEVEL_DEBUG,
  ELOG_LEVEL_TRACE
} elog_level_t;

/* ============================================================
 * 2. コンパイル時設定（デフォルト値）
 * ============================================================ */

/**
 * コンパイル時ログレベル
 * この値より詳細なログはコンパイル時に完全削除される
 */
#ifndef ELOG_COMPILED_LEVEL
#define ELOG_COMPILED_LEVEL ELOG_LEVEL_INFO
#endif

/**
 * 実行時ログレベル機能の有効化
 */
#ifndef ELOG_USE_RUNTIME_LEVEL
#define ELOG_USE_RUNTIME_LEVEL 1
#endif

/**
 * ファイル名:行番号表示の有効化
 */
#ifndef ELOG_USE_FILE_LINE
#define ELOG_USE_FILE_LINE 1
#endif

/**
 * ANSI カラーコード使用の有効化
 */
#ifndef ELOG_USE_COLOR
#define ELOG_USE_COLOR 1
#endif

/* ============================================================
 * 3. 実行時ログレベル変数
 * ============================================================ */

#if ELOG_USE_RUNTIME_LEVEL
/**
 * 実行時ログレベル（ユーザーが定義・変更可能）
 * log_level <= elog_runtime_level の場合に出力される
 */
extern volatile uint8_t elog_runtime_level;
#endif

/* ============================================================
 * 4. ANSI カラーコード定義
 * ============================================================ */

#ifndef ELOG_COLOR_CRITICAL
#define ELOG_COLOR_CRITICAL "\033[1;35m" /* Bright Magenta */
#endif

#ifndef ELOG_COLOR_ERROR
#define ELOG_COLOR_ERROR "\033[1;31m" /* Bright Red */
#endif

#ifndef ELOG_COLOR_WARN
#define ELOG_COLOR_WARN "\033[1;33m" /* Bright Yellow */
#endif

#ifndef ELOG_COLOR_INFO
#define ELOG_COLOR_INFO "\033[1;32m" /* Bright Green */
#endif

#ifndef ELOG_COLOR_DEBUG
#define ELOG_COLOR_DEBUG "\033[1;36m" /* Bright Cyan */
#endif

#ifndef ELOG_COLOR_TRACE
#define ELOG_COLOR_TRACE "\033[0;37m" /* White */
#endif

#ifndef ELOG_COLOR_RESET
#define ELOG_COLOR_RESET "\033[0m" /* Reset */
#endif

/* ============================================================
 * 5. レベル表示フォーマット
 * ============================================================ */

/* CMakeから設定された個別フォーマットを優先 */
#ifndef ELOG_LEVEL_FMT_CRITICAL
#define ELOG_LEVEL_FMT_CRITICAL "[CRITICAL]"
#endif

#ifndef ELOG_LEVEL_FMT_ERROR
#define ELOG_LEVEL_FMT_ERROR "[ERROR]"
#endif

#ifndef ELOG_LEVEL_FMT_WARN
#define ELOG_LEVEL_FMT_WARN "[WARN]"
#endif

#ifndef ELOG_LEVEL_FMT_INFO
#define ELOG_LEVEL_FMT_INFO "[INFO]"
#endif

#ifndef ELOG_LEVEL_FMT_DEBUG
#define ELOG_LEVEL_FMT_DEBUG "[DEBUG]"
#endif

#ifndef ELOG_LEVEL_FMT_TRACE
#define ELOG_LEVEL_FMT_TRACE "[TRACE]"
#endif

/* 後方互換性のため、旧形式のマクロも残す */
#ifndef ELOG_LEVEL_FMT
#define ELOG_LEVEL_FMT(lv) ELOG_LEVEL_FMT_##lv
#endif

/* ============================================================
 * 6. 内部ヘルパーマクロ
 * ============================================================ */

/* __LINE__ を文字列化するためのマクロ */
#define ELOG_STRINGIFY(x) #x
#define ELOG_TOSTRING(x) ELOG_STRINGIFY(x)

/* ファイル名:行番号のフォーマット */
#if ELOG_USE_FILE_LINE
#ifndef ELOG_FILE_LINE_FMT
#define ELOG_FILE_LINE_FMT "[%s: %d]"
#endif
#define ELOG_FILE_LINE_ARGS __FILE_NAME__, __LINE__
#else
#define ELOG_FILE_LINE_FMT
#define ELOG_FILE_LINE_ARGS
#endif

/* カラーコードの開始・終了 */
#if ELOG_USE_COLOR
#define ELOG_COLOR_BEGIN(color) color
#define ELOG_COLOR_END ELOG_COLOR_RESET
#else
#define ELOG_COLOR_BEGIN(color) ""
#define ELOG_COLOR_END ""
#endif

/* ============================================================
 * 7. 実装マクロ（ELOG_IMPL）
 * ============================================================ */

#if ELOG_USE_RUNTIME_LEVEL
/* 実行時レベル判定あり */
#define ELOG_IMPL(level, level_str, color, fmt, ...)                  \
  do {                                                                \
    if ((level) <= elog_runtime_level) {                              \
      printf("%s%s " ELOG_FILE_LINE_FMT " " fmt "%s\n",               \
             ELOG_COLOR_BEGIN(color), level_str, ELOG_FILE_LINE_ARGS, \
             ELOG_COLOR_RESET, ##__VA_ARGS__);                        \
    }                                                                 \
  } while (0)
#else
/* 実行時レベル判定なし */
#define ELOG_IMPL(level, level_str, color, fmt, ...)                           \
  do {                                                                         \
    printf("%s%s " ELOG_FILE_LINE_FMT " " fmt "%s\n", ELOG_COLOR_BEGIN(color), \
           level_str, ELOG_FILE_LINE_ARGS, ELOG_COLOR_RESET, ##__VA_ARGS__);   \
  } while (0)
#endif

/* CRITICAL */
#if ELOG_COMPILED_LEVEL >= ELOG_LEVEL_CRITICAL
#define ELOG_CRITICAL(fmt, ...)                                                \
  ELOG_IMPL(ELOG_LEVEL_CRITICAL, ELOG_LEVEL_FMT_CRITICAL, ELOG_COLOR_CRITICAL, \
            fmt, ##__VA_ARGS__)
#else
#define ELOG_CRITICAL(fmt, ...) ((void)0)
#endif

/* ERROR */
#if ELOG_COMPILED_LEVEL >= ELOG_LEVEL_ERROR
#define ELOG_ERROR(fmt, ...)                                               \
  ELOG_IMPL(ELOG_LEVEL_ERROR, ELOG_LEVEL_FMT_ERROR, ELOG_COLOR_ERROR, fmt, \
            ##__VA_ARGS__)
#else
#define ELOG_ERROR(fmt, ...) ((void)0)
#endif

/* WARN */
#if ELOG_COMPILED_LEVEL >= ELOG_LEVEL_WARN
#define ELOG_WARN(fmt, ...)                                             \
  ELOG_IMPL(ELOG_LEVEL_WARN, ELOG_LEVEL_FMT_WARN, ELOG_COLOR_WARN, fmt, \
            ##__VA_ARGS__)
#else
#define ELOG_WARN(fmt, ...) ((void)0)
#endif

/* INFO */
#if ELOG_COMPILED_LEVEL >= ELOG_LEVEL_INFO
#define ELOG_INFO(fmt, ...)                                             \
  ELOG_IMPL(ELOG_LEVEL_INFO, ELOG_LEVEL_FMT_INFO, ELOG_COLOR_INFO, fmt, \
            ##__VA_ARGS__)
#else
#define ELOG_INFO(fmt, ...) ((void)0)
#endif

/* DEBUG */
#if ELOG_COMPILED_LEVEL >= ELOG_LEVEL_DEBUG
#define ELOG_DEBUG(fmt, ...)                                               \
  ELOG_IMPL(ELOG_LEVEL_DEBUG, ELOG_LEVEL_FMT_DEBUG, ELOG_COLOR_DEBUG, fmt, \
            ##__VA_ARGS__)
#else
#define ELOG_DEBUG(fmt, ...) ((void)0)
#endif

/* TRACE */
#if ELOG_COMPILED_LEVEL >= ELOG_LEVEL_TRACE
#define ELOG_TRACE(fmt, ...)                                               \
  ELOG_IMPL(ELOG_LEVEL_TRACE, ELOG_LEVEL_FMT_TRACE, ELOG_COLOR_TRACE, fmt, \
            ##__VA_ARGS__)
#else
#define ELOG_TRACE(fmt, ...) ((void)0)
#endif

/* WARN */
#if ELOG_COMPILED_LEVEL >= ELOG_LEVEL_WARN
#define ELOG_WARN(fmt, ...)                                             \
  ELOG_IMPL(ELOG_LEVEL_WARN, ELOG_LEVEL_FMT_WARN, ELOG_COLOR_WARN, fmt, \
            ##__VA_ARGS__)
#else
#define ELOG_WARN(fmt, ...) ((void)0)
#endif

/* INFO */
#if ELOG_COMPILED_LEVEL >= ELOG_LEVEL_INFO
#define ELOG_INFO(fmt, ...)                                             \
  ELOG_IMPL(ELOG_LEVEL_INFO, ELOG_LEVEL_FMT_INFO, ELOG_COLOR_INFO, fmt, \
            ##__VA_ARGS__)
#else
#define ELOG_INFO(fmt, ...) ((void)0)
#endif

/* DEBUG */
#if ELOG_COMPILED_LEVEL >= ELOG_LEVEL_DEBUG
#define ELOG_DEBUG(fmt, ...)                                               \
  ELOG_IMPL(ELOG_LEVEL_DEBUG, ELOG_LEVEL_FMT_DEBUG, ELOG_COLOR_DEBUG, fmt, \
            ##__VA_ARGS__)
#else
#define ELOG_DEBUG(fmt, ...) ((void)0)
#endif

/* TRACE */
#if ELOG_COMPILED_LEVEL >= ELOG_LEVEL_TRACE
#define ELOG_TRACE(fmt, ...)                                               \
  ELOG_IMPL(ELOG_LEVEL_TRACE, ELOG_LEVEL_FMT_TRACE, ELOG_COLOR_TRACE, fmt, \
            ##__VA_ARGS__)
#else
#define ELOG_TRACE(fmt, ...) ((void)0)
#endif

#ifdef __cplusplus
}
#endif

#endif /* ELOG_H */
