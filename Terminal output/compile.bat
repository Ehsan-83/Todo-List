@echo off

if not exist "Users" (
    mkdir "Users"
    attrib +h +s "Users"
)

if not exist "Accounts" (
    mkdir "Accounts"
    attrib +h +s "Accounts"
)

g++ lib.cpp ToDo.cpp -o ToDo.exe

ToDo.exe