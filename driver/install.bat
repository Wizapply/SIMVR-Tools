@echo off

cd /d %~dp0

setlocal

echo --------------------------------------------------
echo        Wizapply SIMVRドライバインストーラー
echo                          Version 1.0
echo --------------------------------------------------

set infpath_var=%WINDIR%\inf
set dll32path_var=%WINDIR%\SysWOW64
set dll64path_var=%WINDIR%\System32

if "%PROCESSOR_ARCHITECTURE%" == "x86" (
    if "%PROCESSOR_ARCHITEW6432%" == "AMD64" (
        echo 32bit process on 64bit OS - WOW64
    ) else (
        echo 32bit process on 32bit OS
    )
) else (
    echo 64bit process on 64bit OS
)

rem --- Driver install ----------------------------------------

echo SIMVRドライバをインストールしています...
if exist "%infpath_var%\wizcdc.inf" echo ドライバを更新します。
copy ".\wizcdc.inf" "%infpath_var%\wizcdc.inf" /b /y
if errorlevel 1 goto ENDINSTALLERROR

rem --- End ----------------------------------------------------

:ENDINSTALL
rem インストール完了
echo.
echo [成功]全てのインストールが完了しました！
pause
exit

:ENDINSTALLERROR
rem インストールエラー
echo.
echo [失敗]インストールに失敗しました！
echo ※Install.batを右クリック「管理者として実行」にて実行をお試しください。
pause
exit