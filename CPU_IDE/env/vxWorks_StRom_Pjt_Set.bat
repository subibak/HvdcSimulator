@echo off
if !%TORNADO690_ENV_INIT%==! (
	set /a TORNADO690_ENV_INIT=1
	call Tornado690_Env_Set.bat
)
@echo on

set	VIP_NAME=stRom_Pjt

cd /D %TARGET_OS_DIR%
copy /y Makefile.0x1000_lk Makefile
cd /D %TARGET_VIP_DIR%
del /s %VIP_NAME% /q
call vxprj create -profile PROFILE_DEVELOPMENT lk11_lkv_120 e500v2diab ./%VIP_NAME%
cd  ./%VIP_NAME%
call vxprj component add INCLUDE_STANDALONE_SYM_TBL
call vxprj component remove INCLUDE_NET_SYM_TBL
call vxprj component remove INCLUDE_SHELL

call vxprj component add INCLUDE_IPNET_SYSCTL
call vxprj component add INCLUDE_IPWRAP_GETSERVBYPORT
call vxprj component add INCLUDE_NET_SYSCTL
call vxprj component add INCLUDE_POSIX_TIMERS
call vxprj component add INCLUDE_SIGEVENT
call vxprj component add INCLUDE_SPY
call vxprj component add INCLUDE_TIMER_OPEN
call vxprj component add INCLUDE_NET_BOOT_CONFIG
call vxprj component add INCLUDE_NET_INIT
call vxprj component add INCLUDE_WDB_BP_SYNC
call vxprj component remove INCLUDE_LOADER
call vxprj component remove INCLUDE_LOADER_CODE
call vxprj component remove INCLUDE_LOADER_HOOKS
call vxprj component remove INCLUDE_UNLOADER
call vxprj component remove INCLUDE_VXDBG_MODULE
call vxprj component remove INCLUDE_WDB_BANNER
call vxprj component remove INCLUDE_WDB_MDL_SYM_SYNC
call vxprj component remove INCLUDE_WDB_SYS
call vxprj component remove INCLUDE_WDB_SYS_COPROC_REG
call vxprj component remove INCLUDE_WDB_SYS_REG
call vxprj component remove ORTHROS
call vxprj component remove INCLUDE_WDB_PRE_KERNEL_INIT
call vxprj build set %VIP_NAME%.wpj default_romCompress
cd /D %TARGET_ENV_DIR%
copy usrAppInit.c /y %TARGET_VIP_DIR%\%VIP_NAME%\
pause