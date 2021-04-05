@echo off
if !%TORNADO690_ENV_INIT%==! (
	set /a TORNADO690_ENV_INIT=1
	call Tornado690_Env_Set.bat
)
@echo on

cd %TARGET_APP_DIR%
make -f make.app clean