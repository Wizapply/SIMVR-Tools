@echo off

cd /d %~dp0

setlocal

echo --------------------------------------------------
echo        Wizapply SIMVR�h���C�o�C���X�g�[���[
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

echo SIMVR�h���C�o���C���X�g�[�����Ă��܂�...
if exist "%infpath_var%\wizcdc.inf" echo �h���C�o���X�V���܂��B
copy ".\wizcdc.inf" "%infpath_var%\wizcdc.inf" /b /y
if errorlevel 1 goto ENDINSTALLERROR

rem --- End ----------------------------------------------------

:ENDINSTALL
rem �C���X�g�[������
echo.
echo [����]�S�ẴC���X�g�[�����������܂����I
pause
exit

:ENDINSTALLERROR
rem �C���X�g�[���G���[
echo.
echo [���s]�C���X�g�[���Ɏ��s���܂����I
echo ��Install.bat���E�N���b�N�u�Ǘ��҂Ƃ��Ď��s�v�ɂĎ��s�����������������B
pause
exit