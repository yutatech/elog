cmake_minimum_required(VERSION 3.15)

project(elog
    VERSION 1.0.0
    DESCRIPTION "Embedded Lightweight Organized Logger"
    LANGUAGES C CXX
)

# オプション: コンパイル時ログレベルの設定
set(ELOG_COMPILED_LEVEL "ELOG_LEVEL_INFO" CACHE STRING 
    "Compile-time log level (ELOG_LEVEL_OFF, ELOG_LEVEL_CRITICAL, ELOG_LEVEL_ERROR, ELOG_LEVEL_WARN, ELOG_LEVEL_INFO, ELOG_LEVEL_DEBUG, ELOG_LEVEL_TRACE)")

# オプション: 実行時ログレベル機能の有効化
option(ELOG_USE_RUNTIME_LEVEL "Enable runtime log level filtering" ON)

# オプション: ファイル名:行番号表示の有効化
option(ELOG_USE_FILE_LINE "Enable file name and line number display in logs" ON)

# オプション: ANSIカラーコードの有効化
option(ELOG_USE_COLOR "Enable ANSI color codes in logs" ON)

# オプション: ANSIカラーコード設定
set(ELOG_COLOR_CRITICAL "\\033[1;35m" CACHE STRING "ANSI color code for CRITICAL level")
set(ELOG_COLOR_ERROR "\\033[1;31m" CACHE STRING "ANSI color code for ERROR level")
set(ELOG_COLOR_WARN "\\033[1;33m" CACHE STRING "ANSI color code for WARN level")
set(ELOG_COLOR_INFO "\\033[1;32m" CACHE STRING "ANSI color code for INFO level")
set(ELOG_COLOR_DEBUG "\\033[1;36m" CACHE STRING "ANSI color code for DEBUG level")
set(ELOG_COLOR_TRACE "\\033[0;37m" CACHE STRING "ANSI color code for TRACE level")
set(ELOG_COLOR_RESET "\\033[0m" CACHE STRING "ANSI color code for RESET")

# オプション: ログレベルフォーマット設定
set(ELOG_LEVEL_FMT_CRITICAL "[CRITICAL]" CACHE STRING "Format string for CRITICAL level")
set(ELOG_LEVEL_FMT_ERROR    "[   ERROR]" CACHE STRING "Format string for ERROR level")
set(ELOG_LEVEL_FMT_WARN     "[    WARN]" CACHE STRING "Format string for WARN level")
set(ELOG_LEVEL_FMT_INFO     "[    INFO]" CACHE STRING "Format string for INFO level")
set(ELOG_LEVEL_FMT_DEBUG    "[   DEBUG]" CACHE STRING "Format string for DEBUG level")
set(ELOG_LEVEL_FMT_TRACE    "[   TRACE]" CACHE STRING "Format string for TRACE level")

# オプション: ファイル名:行番号フォーマット設定
set(ELOG_FILE_LINE_FMT "[%s: %d]" CACHE STRING "Prefix for file:line display. Must include %s and %d for file name and line number respectively.")

# 静的ライブラリとして定義
add_library(elog STATIC src/elog.c)
add_library(elog::elog ALIAS elog)

# インクルードディレクトリの設定
target_include_directories(elog PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# コンパイル定義の追加
target_compile_definitions(elog PUBLIC
    ELOG_COMPILED_LEVEL=${ELOG_COMPILED_LEVEL}
)

# 実行時ログレベルの設定
if(ELOG_USE_RUNTIME_LEVEL)
    target_compile_definitions(elog PUBLIC ELOG_USE_RUNTIME_LEVEL=1)
endif()

# ファイル名:行番号表示の設定
if(ELOG_USE_FILE_LINE)
    target_compile_definitions(elog PUBLIC ELOG_USE_FILE_LINE=1)
else()
    target_compile_definitions(elog PUBLIC ELOG_USE_FILE_LINE=0)
endif()

# ANSIカラーの設定
if(ELOG_USE_COLOR)
    target_compile_definitions(elog PUBLIC ELOG_USE_COLOR=1)
else()
    target_compile_definitions(elog PUBLIC ELOG_USE_COLOR=0)
endif()

# ソースコードのパスをコンパイルオプションから除外（デバッグ情報の軽量化）
add_compile_options(-fmacro-prefix-map=${CMAKE_SOURCE_DIR}=)

# カラーコードの定義
target_compile_definitions(elog PUBLIC
    ELOG_COLOR_CRITICAL="${ELOG_COLOR_CRITICAL}"
    ELOG_COLOR_ERROR="${ELOG_COLOR_ERROR}"
    ELOG_COLOR_WARN="${ELOG_COLOR_WARN}"
    ELOG_COLOR_INFO="${ELOG_COLOR_INFO}"
    ELOG_COLOR_DEBUG="${ELOG_COLOR_DEBUG}"
    ELOG_COLOR_TRACE="${ELOG_COLOR_TRACE}"
    ELOG_COLOR_RESET="${ELOG_COLOR_RESET}"
)

# ログレベルフォーマットの定義
target_compile_definitions(elog PUBLIC
    ELOG_LEVEL_FMT_CRITICAL="${ELOG_LEVEL_FMT_CRITICAL}"
    ELOG_LEVEL_FMT_ERROR="${ELOG_LEVEL_FMT_ERROR}"
    ELOG_LEVEL_FMT_WARN="${ELOG_LEVEL_FMT_WARN}"
    ELOG_LEVEL_FMT_INFO="${ELOG_LEVEL_FMT_INFO}"
    ELOG_LEVEL_FMT_DEBUG="${ELOG_LEVEL_FMT_DEBUG}"
    ELOG_LEVEL_FMT_TRACE="${ELOG_LEVEL_FMT_TRACE}"
)

# ファイル名:行番号フォーマットの定義
target_compile_definitions(elog PUBLIC
    ELOG_FILE_LINE_FMT="${ELOG_FILE_LINE_FMT}"
)

# インストール設定（オプション）
if(PROJECT_IS_TOP_LEVEL)
    include(GNUInstallDirs)
    
    install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h"
    )
    
    install(TARGETS elog
        EXPORT elogTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
    
    install(EXPORT elogTargets
        FILE elogTargets.cmake
        NAMESPACE elog::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/elog
    )
endif()
