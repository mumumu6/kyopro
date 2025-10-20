#!/usr/bin/env bash
# ビルド＆実行スクリプト
# 使い方: run_cpp.sh <source_file>
# VSCode の Code Runner から呼び出されることを想定しています。

set -euo pipefail


SRC="$1"
DIR="$(dirname "$SRC")"
BASE="$(basename "$SRC")"
NAME="${BASE%.*}"
OUT_DIR="$DIR/out"
mkdir -p "$OUT_DIR"

# PCH を RAM ディスク上に展開すると読み込みが高速化する
PCH_DIR="/dev/shm/kyopro_pch"
mkdir -p "$PCH_DIR"

# --- 同じ PCH を毎回利用 ------------------------------------
#  * ソースヘッダ : ~/kyopro/pch/pch.hpp
#  * RAM ディスク : /dev/shm/kyopro_pch/pch.hpp(.gch)

SRC_PCH_HEADER="$HOME/kyopro/pch/pch.hpp"
PCH_HEADER="$PCH_DIR/pch.hpp"
PCH_FILE="$PCH_DIR/pch.hpp.gch"

# --------------------------------------------------------------------------

# ヘッダが存在しなければコピー（更新されたら毎回コピーし直す）
if [ ! -f "$PCH_HEADER" ] || [ "$SRC_PCH_HEADER" -nt "$PCH_HEADER" ]; then
  echo "[run_cpp.sh] Copying precompiled header..."
  cp -u --preserve=timestamps "$SRC_PCH_HEADER" "$PCH_HEADER"
fi

# PCH が無い、もしくはヘッダが更新されていたら再生成
if [ ! -f "$PCH_FILE" ] || [ "$PCH_HEADER" -nt "$PCH_FILE" ]; then
  echo "[run_cpp.sh] Generating precompiled header..."
  ccache g++ -std=gnu++2b -O0 -g  -march=native -pipe -I "$PCH_DIR" \
         -x c++-header "$PCH_HEADER" -o "$PCH_FILE"
fi

# 一時オブジェクトは RAM ディスクへ
OBJ="/dev/shm/${NAME}.o"

echo "[run_cpp.sh] Compiling $SRC ..."
ccache g++  -std=gnu++2b -O0 -g  -march=native -pipe -I "$PCH_DIR" -include pch.hpp -c "$SRC" -o "$OBJ"

g++ -fuse-ld=mold "$OBJ" "$HOME/kyopro/lib/libacl.a"  -o "$OUT_DIR/$NAME"

# 実行
"$OUT_DIR/$NAME" 