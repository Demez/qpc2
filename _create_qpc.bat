@echo off

@REM force build right now because wildcard hashing is somehow broken
py qpc1\qpc.py -f -a "scripts/qpc.qpc" "scripts/vstudio.qpc" "scripts/cmake.qpc" -g visual_studio cmake -mf qpc

pause

