#! /usr/bin/python3

import sys, os
import re

command_line_pattern = re.compile(r'^\s+"command":')
include_pattern = re.compile(r'^(-I.+)\s*$')
sys_include_pattern = re.compile(r'^\s*-isystem')
define_pattern = re.compile(r'^(-D.+)\s*')

config_file_path = sys.argv[1]

if not os.path.exists( config_file_path ):
    exit()

config_file = open(config_file_path)
lines = config_file.readlines()
config_file.close()

flags = set()
tmp_flag = ""
for line in lines:
    m = command_line_pattern.match(line)
    if m:
        segs = line.split()
        for seg in segs:
            match_inc = include_pattern.match(seg)
            match_sys_inc = sys_include_pattern.match(seg)
            match_define = define_pattern.match(seg)
            if match_inc:
                flags.add(match_inc.group())
            elif match_sys_inc:
                tmp_flag = seg
            elif not tmp_flag == "":
                flag = tmp_flag+" "+seg
                flags.add(flag)
                tmp_flag = ""
            elif match_define:
                flags.add(seg)

print(flags)
print(len(flags))
