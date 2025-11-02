#!/usr/bin/env python3
import sys
import io
import os
import re

# Walk repository and remove comments and debug cout/cerr prints
# This script edits files in-place. It keeps a .bak copy for safety.
# It handles C/C++ style comments and avoids removing comment-like sequences inside strings.

EXTS = ('.c', '.cpp', '.cc', '.h', '.hpp', '.hh')

def strip_comments_and_debug(src):
    out = []
    i = 0
    n = len(src)
    state = 'code'  # code, squote, dquote, block, line
    while i < n:
        ch = src[i]
        next_ch = src[i+1] if i+1 < n else ''
        if state == 'code':
            if ch == '"':
                out.append(ch)
                state = 'dquote'
                i += 1
            elif ch == "'":
                out.append(ch)
                state = 'squote'
                i += 1
            elif ch == '/' and next_ch == '*':
                state = 'block'
                i += 2
            elif ch == '/' and next_ch == '/':
                state = 'line'
                i += 2
            else:
                out.append(ch)
                i += 1
        elif state == 'dquote':
            out.append(ch)
            if ch == '\\':
                # escape next char
                if i+1 < n:
                    out.append(src[i+1]); i += 2
                else:
                    i += 1
            elif ch == '"':
                state = 'code'; i += 1
            else:
                i += 1
        elif state == 'squote':
            out.append(ch)
            if ch == '\\':
                if i+1 < n:
                    out.append(src[i+1]); i += 2
                else:
                    i += 1
            elif ch == "'":
                state = 'code'; i += 1
            else:
                i += 1
        elif state == 'block':
            if ch == '*' and next_ch == '/':
                i += 2
                state = 'code'
            else:
                i += 1
        elif state == 'line':
            if ch == '\n':
                out.append(ch)
                i += 1
                state = 'code'
            else:
                i += 1
    result = ''.join(out)

    # Now remove lines that contain debugging prints: std::cout, std::cerr, cout<<, cerr<<, printf(
    lines = result.splitlines(True)
    cleaned = []
    debug_patterns = re.compile(r'\b(std::cout|std::cerr|cout\s*<<|cerr\s*<<|printf\s*\()')
    for line in lines:
        if debug_patterns.search(line):
            # skip line entirely
            continue
        cleaned.append(line)
    return ''.join(cleaned)


def process_file(path):
    with io.open(path, 'r', encoding='utf-8', errors='ignore') as f:
        src = f.read()
    new = strip_comments_and_debug(src)
    if new != src:
        bak = path + '.bak'
        if not os.path.exists(bak):
            with io.open(bak, 'w', encoding='utf-8') as bf:
                bf.write(src)
        with io.open(path, 'w', encoding='utf-8') as f:
            f.write(new)
        print('modified:', path)


def main(root):
    for dirpath, dirnames, filenames in os.walk(root):
        # skip .git and build dirs
        if '.git' in dirpath.split(os.sep):
            continue
        if 'build' in dirpath.split(os.sep):
            continue
        for fn in filenames:
            if fn.endswith(EXTS):
                path = os.path.join(dirpath, fn)
                try:
                    process_file(path)
                except Exception as e:
                    print('error processing', path, e)

if __name__ == '__main__':
    root = sys.argv[1] if len(sys.argv) > 1 else '.'
    main(root)
