/**
 * @file elog.c
 * @brief elog - Runtime log level variable implementation
 */

#include "elog/elog.h"

#if ELOG_USE_RUNTIME_LEVEL
/**
 * 実行時ログレベル変数の実態
 * デフォルトでコンパイル時レベルと同じ値に初期化
 */
volatile uint8_t elog_runtime_level = ELOG_COMPILED_LEVEL;
#endif
