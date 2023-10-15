import os
import re

# 除外するディレクトリのリスト
EXCLUDED_DIRS = ['bin', '.github', 'build', 'Data', 'Document', 'ExternalLibrary', 'Image', 'Scripts']

def is_excluded_dir(dirpath):
    for excluded in EXCLUDED_DIRS:
        if excluded in dirpath.split(os.sep):
            return True
    return False

def count_code_lines(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()

    code_line_count = 0
    multi_line_comment = False
    for line in lines:
        line = line.strip()
        if line.startswith('/*'):
            multi_line_comment = True

        if not multi_line_comment and not line.startswith('//') and line != '':
            code_line_count += 1

        if line.endswith('*/'):
            multi_line_comment = False

    return code_line_count

def count_directory_code_lines(directory):
    total_lines = 0
    for root, dirs, files in os.walk(directory):
        if is_excluded_dir(root):
            continue
        for file in files:
            if file.endswith(('.cpp', '.hpp')):
                file_path = os.path.join(root, file)
                total_lines += count_code_lines(file_path)

    return total_lines

# 使い方
# 探索するディレクトリのパスを入力する
directory = input("Directory:")
print(f"Total code lines: {count_directory_code_lines(directory)}")
