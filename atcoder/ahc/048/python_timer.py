#!/usr/bin/env python3

import subprocess
import time
import os
import glob
from pathlib import Path

def main():
    # 作業ディレクトリ設定
    os.chdir('/home/mumumu/kyopro/atcoder/ahc/048')
    
    print("=== Python実行時間計測スクリプト ===")
    print(f"開始時刻: {time.strftime('%Y-%m-%d %H:%M:%S')}")
    print("=" * 40)
    
    # 実行ファイル確認・コンパイル
    if not os.path.exists('./out/b3'):
        print("b3_execが見つかりません。コンパイルします...")
        result = subprocess.run(['g++', '-O2', '-std=c++20', '-o', 'b3_exec', 'b3.cpp'], 
                              capture_output=True, text=True)
        if result.returncode != 0:
            print("コンパイルエラー:")
            print(result.stderr)
            return
        print("コンパイル完了")
    
    # outputディレクトリ作成
    Path('output').mkdir(exist_ok=True)
    
    # 入力ファイル取得（ソート済み）
    input_files = sorted(glob.glob('input/*.txt'))
    if not input_files:
        print("入力ファイルが見つかりません")
        return
    
    print(f"処理対象ファイル数: {len(input_files)}")
    print()
    
    # 実行時間記録
    execution_times = []
    successful_files = []
    
    for i, input_file in enumerate(input_files, 1):
        filename = Path(input_file).stem  # 拡張子なしファイル名
        output_file = f'output/{filename}.out'
        
        print(f"[{i:2d}/{len(input_files)}] 処理中: {filename} ... ", end='', flush=True)
        
        try:
            # 実行時間計測
            start_time = time.perf_counter()
            
            with open(input_file, 'r') as infile, open(output_file, 'w') as outfile:
                result = subprocess.run(['./out/a2'], 
                                      stdin=infile, 
                                      stdout=outfile, 
                                      stderr=subprocess.PIPE,
                                      timeout=30)  # 30秒タイムアウト
            
            end_time = time.perf_counter()
            exec_time = end_time - start_time
            
            if result.returncode == 0:
                execution_times.append(exec_time)
                successful_files.append(filename)
                print(f"完了 ({exec_time:.6f}s)")
            else:
                print(f"エラー (終了コード: {result.returncode})")
                
        except subprocess.TimeoutExpired:
            print("タイムアウト (30s)")
        except Exception as e:
            print(f"例外発生: {e}")
    
    print()
    print("=" * 40)
    print(f"終了時刻: {time.strftime('%Y-%m-%d %H:%M:%S')}")
    print()
    
    # 統計計算
    if execution_times:
        min_time = min(execution_times)
        max_time = max(execution_times)
        avg_time = sum(execution_times) / len(execution_times)
        total_time = sum(execution_times)
        
        print("== 実行時間統計 ==")
        print(f"処理成功ファイル数: {len(execution_times)}")
        print(f"最小実行時間: {min_time:.6f}s")
        print(f"最大実行時間: {max_time:.6f}s")
        print(f"平均実行時間: {avg_time:.6f}s")
        print(f"総実行時間: {total_time:.6f}s")
        
        # 詳細ログ保存
        with open('detailed_times.txt', 'w') as f:
            f.write("# ファイル名:実行時間(秒)\n")
            for filename, exec_time in zip(successful_files, execution_times):
                f.write(f"{filename}:{exec_time:.6f}\n")
        
        # 統計サマリー保存
        with open('python_stats.txt', 'w') as f:
            f.write("== Python実行時間統計 ==\n")
            f.write(f"処理成功ファイル数: {len(execution_times)}\n")
            f.write(f"最小実行時間: {min_time:.6f}s\n")
            f.write(f"最大実行時間: {max_time:.6f}s\n")
            f.write(f"平均実行時間: {avg_time:.6f}s\n")
            f.write(f"総実行時間: {total_time:.6f}s\n")
            f.write(f"計測日時: {time.strftime('%Y-%m-%d %H:%M:%S')}\n")
        
        print()
        print("詳細ログ: detailed_times.txt")
        print("統計サマリー: python_stats.txt")
        
    else:
        print("実行に成功したファイルがありません")

if __name__ == "__main__":
    main() 