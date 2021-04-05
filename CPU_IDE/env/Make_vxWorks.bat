@echo off
if !%TORNADO690_ENV_INIT%==! (
	set /a TORNADO690_ENV_INIT=1
	call Tornado690_Env_Set.bat
)
@echo on

cd %TARGET_VIP_DIR%\dbgRom_Pjt

make clean
make vxWorks

del D:\tftp\HVDC_CPU.vxWorks
copy D:\HVDC_Simulator\CPU_IDE\vxImgPrj\dbgRom_Pjt\vxWorks D:\tftp\HVDC_CPU.vxWorks

pause