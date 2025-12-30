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
if (NOT DEFINED ELOG_COLOR_CRITICAL)
    set(ELOG_COLOR_CRITICAL "\\033[1;35m" CACHE STRING "ANSI color code for CRITICAL level")
endif()
if (NOT DEFINED ELOG_COLOR_ERROR)
    set(ELOG_COLOR_ERROR "\\033[1;31m" CACHE STRING "ANSI color code for ERROR level")
endif()
if (NOT DEFINED ELOG_COLOR_WARN)
    set(ELOG_COLOR_WARN "\\033[1;33m" CACHE STRING "ANSI color code for WARN level")
endif()
if (NOT DEFINED ELOG_COLOR_INFO)
    set(ELOG_COLOR_INFO "\\033[1;32m" CACHE STRING "ANSI color code for INFO level")
endif()
if (NOT DEFINED ELOG_COLOR_DEBUG)
    set(ELOG_COLOR_DEBUG "\\033[1;36m" CACHE STRING "ANSI color code for DEBUG level")
endif()
if (NOT DEFINED ELOG_COLOR_TRACE)
    set(ELOG_COLOR_TRACE "\\033[0;37m" CACHE STRING "ANSI color code for TRACE level")
endif()
if (NOT DEFINED ELOG_COLOR_RESET)
    set(ELOG_COLOR_RESET "\\033[0m" CACHE STRING "ANSI color code for RESET")
endif()

# オプション: ログレベルフォーマット設定
if (NOT DEFINED ELOG_LEVEL_FMT_CRITICAL)
    set(ELOG_LEVEL_FMT_CRITICAL "[CRITICAL]" CACHE STRING "Format string for CRITICAL level")
endif()
if (NOT DEFINED ELOG_LEVEL_FMT_ERROR)
    set(ELOG_LEVEL_FMT_ERROR "[   ERROR]" CACHE STRING "Format string for ERROR level")
endif()
if (NOT DEFINED ELOG_LEVEL_FMT_WARN)
    set(ELOG_LEVEL_FMT_WARN "[    WARN]" CACHE STRING "Format string for WARN level")
endif()
if (NOT DEFINED ELOG_LEVEL_FMT_INFO)
    set(ELOG_LEVEL_FMT_INFO "[    INFO]" CACHE STRING "Format string for INFO level")
endif()
if (NOT DEFINED ELOG_LEVEL_FMT_DEBUG)
    set(ELOG_LEVEL_FMT_DEBUG "[   DEBUG]" CACHE STRING "Format string for DEBUG level")
endif()
if (NOT DEFINED ELOG_LEVEL_FMT_TRACE)
    set(ELOG_LEVEL_FMT_TRACE "[   TRACE]" CACHE STRING "Format string for TRACE level")
endif()

# オプション: ファイル名:行番号フォーマット設定
if (NOT DEFINED ELOG_FILE_LINE_FMT)
    set(ELOG_FILE_LINE_FMT "[%s: %d]" CACHE STRING "Prefix for file:line display. Must include %s and %d for file name and line number respectively.")
endif()

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
    ELOG_CONFIG_GENERATED
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

# 設定ヘッダーファイルの生成
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/elog_config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/elog_config.h
    @ONLY
)

# 生成された設定ヘッダーのインクルードパスを追加
target_include_directories(elog PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
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
