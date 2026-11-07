@echo off
setlocal
cd /d "%~dp0"
title Sistema de Controle de Estoque - Engenharia da Computação 2026

echo ===================================================================
echo   SISTEMA DE CONTROLE DE ESTOQUE - ENGENHARIA DA COMPUTACAO 2026
echo ===================================================================

if exist "sistema.exe" (
    echo [STATUS] Executavel encontrado! Iniciando aplicacao...
    echo.
    sistema.exe
    goto fim
)

echo [AVISO] O executavel sistema.exe nao foi encontrado nesta pasta.
echo Verificando se o compilador GCC esta instalado...
where gcc >nul 2>nul
if %errorlevel% neq 0 (
    echo.
    echo ===================================================================
    echo  [ERRO] O executavel sistema.exe e o GCC nao foram encontrados!
    echo ===================================================================
    echo  Como resolver:
    echo   1. Certifique-se de que baixou todos os arquivos do repositorio.
    echo   2. No repositorio do GitHub, baixe o arquivo 'sistema.exe' e
    echo      coloque-o dentro desta mesma pasta.
    echo   3. Ou instale o MinGW/GCC e compile usando o comando: make
    echo ===================================================================
    echo.
    pause
    exit /b 1
)

echo Compilando sistema.exe automaticamente...
gcc -std=c11 -Wall -Wextra -g -Iinclude main.c src/sistema.c src/algoritmos.c src/banco.c src/util.c -lsqlite3 -o sistema.exe
if %errorlevel% neq 0 (
    echo [ERRO] Falha na compilacao com GCC. Verifique as dependencias.
    pause
    exit /b 1
)

echo Compilacao concluida com sucesso! Iniciando sistema...
echo.
sistema.exe

:fim
echo.
pause
