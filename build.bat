@echo off
echo ==============================
echo Building Compiler Project...
echo ==============================

:: Run Yacc/Bison -> parser
yacc -d -v syd.y
if errorlevel 1 goto :error

:: Run Flex -> lexer
flex lex
if errorlevel 1 goto :error

:: Compile everything
gcc lex.yy.c y.tab.c zyywrap.c syndax_tree.c log.c symbol_table.c mixal_generation.c -o compiler.exe
if errorlevel 1 goto :error

:: Run compiler
compiler.exe < tests/test3/loops


echo ==============================
echo Build successful! :)
echo ==============================
goto :eof

:error
echo ==============================
echo Build failed! Check errors above.
echo ==============================
exit /b 1
