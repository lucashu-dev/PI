@echo off
chcp 65001 >nul
setlocal
cd /d "%~dp0"
title Sistema de Controle de Estoque - Engenharia da Computação 2026

echo ===================================================================
echo   SISTEMA DE CONTROLE DE ESTOQUE - ENGENHARIA DA COMPUTAÇÃO 2026
echo ===================================================================

if exist "sistema.exe" (
    echo [STATUS] Executável encontrado! Iniciando aplicação...
    echo.
    sistema.exe
    goto fim
)

echo [AVISO] O executável sistema.exe não foi encontrado nesta pasta.
echo Verificando se o compilador GCC está instalado...
where gcc >nul 2>nul
if %errorlevel% neq 0 (
    echo.
    echo ===================================================================
    echo  [ERRO] O executável sistema.exe e o GCC não foram encontrados!
    echo ===================================================================
    echo  Como resolver:
    echo   1. Certifique-se de que baixou todos os arquivos do repositório.
    echo   2. No repositório do GitHub, baixe o arquivo 'sistema.exe' e
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
    echo [ERRO] Falha na compilação com GCC. Verifique as dependências.
    pause
    exit /b 1
)

echo Compilação concluída com sucesso! Iniciando sistema...
echo.
sistema.exe

:fim
echo.
pause
