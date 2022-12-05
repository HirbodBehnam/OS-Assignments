@echo off
for /l %%x in (1, 1, 20) do python thread.py
echo =========
for /l %%x in (1, 1, 20) do python async_programming.py