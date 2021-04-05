@echo off
if !%TORNADO690_ENV_INIT%==! (
	set /a TORNADO690_ENV_INIT=1
	call Tornado690_Env_Set.bat
)
@echo on

cd %TARGET_VIP_DIR%\stRom_Pjt
make clean
make vxWorks_romCompress.bin

del D:\tftp\HVDC_CPU.rom
copy D:\HVDC_Simulator\CPU_IDE\vxImgPrj\stRom_Pjt\vxWorks_romCompress.bin D:\tftp\HVDC_CPU.rom

pause