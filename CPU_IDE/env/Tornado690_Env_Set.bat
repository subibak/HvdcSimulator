REM	*****************************************************
REM	* WINDRIVER	ENV
REM	*****************************************************

set	WIND_HOME=D:\Tornado690
set	WIND_PLATFORM=vxworks-6.9
set	WIND_BASE=%WIND_HOME%\%WIND_PLATFORM%
set	WIND_HOST_TYPE=x86-win32
set	WIND_PREFERRED_PACKAGES=%WIND_PLATFORM%
set	TGT_DIR=%WIND_BASE%\target

set	COMP_CAFE=cafe-0.1
set	COMP_CAFE_PATH=%WIND_HOME%\components\cafe-0.1
set	COMP_CAN=can-1.5
set	COMP_DCOM=dcom-2.3
set	COMP_IPNET2=ip_net2-6.9
set	COMP_IPNET2_SUBCOMP=ip_net2-6.9/ipmcp/config;ip_net2-6.9/wrnad/config;ip_net2-6.9/vxifproxy/config;ip_net2-6.9/ipcavium/config;ip_net2-6.9/ipfreescale/config;ip_net2-6.9/iphwcrypto/config;ip_net2-6.9/ipipsec2/config;ip_net2-6.9/ipfirewall/config;ip_net2-6.9/ipmpls/config;ip_net2-6.9/ipmip6mn/config;ip_net2-6.9/ipmip6pm/config;ip_net2-6.9/ipmip6ha/config;ip_net2-6.9/ipmip6/config;ip_net2-6.9/ipmipfa/config;ip_net2-6.9/ipmippm/config;ip_net2-6.9/ipmipmn/config;ip_net2-6.9/ipmipha/config;ip_net2-6.9/ipmip4/config;ip_net2-6.9/ipmip/config;ip_net2-6.9/iptcp/config;ip_net2-6.9/ipike/config;ip_net2-6.9/ipradius/config;ip_net2-6.9/ipl2tp/config;ip_net2-6.9/ipppp/config;ip_net2-6.9/iplite2/config;ip_net2-6.9/vxcoreip/config;ip_net2-6.9/vxcompat/config;ip_net2-6.9/ipssl2/config;ip_net2-6.9/ipssh/config;ip_net2-6.9/ipcrypto/config;ip_net2-6.9/ipnet2/config;ip_net2-6.9/ipcom/config;ip_net2-6.9/vxbridge/config;ip_net2-6.9/iprohc/config;ip_net2-6.9/ipdhcpc6/config;ip_net2-6.9/ipdhcpr/config;ip_net2-6.9/ipripng/config;ip_net2-6.9/iprip/config;ip_net2-6.9/ipntp/config;ip_net2-6.9/ipsntp/config;ip_net2-6.9/ipappl/config;ip_net2-6.9/ipsctp/config;ip_net2-6.9/ipdhcps6/config;ip_net2-6.9/ipdhcps/config;ip_net2-6.9/ipdiameter/config;ip_net2-6.9/ipwlan/config;ip_net2-6.9/ipmacsec/config;ip_net2-6.9/ip8021x/config;ip_net2-6.9/vxdot1x/config;ip_net2-6.9/ipwps/config;ip_net2-6.9/ipeap/config
set	COMP_MIPC=mipc-2.1
set	COMP_NETSTACK=netstack-6.9
set	COMP_OPC=opc-3.2
set	COMP_SNMP=wrsnmp-10.5
set	COMP_STACKTRACE=stacktrace-1.0
set	COMP_STACKTRACE_PATH=%WIND_HOME%\components\stacktrace-1.0
set	COMP_SUBCOMP_LIST=COMP_IPNET2_SUBCOMP
set	COMP_TCF_AGENT=tcf_agent-0.3
set	COMP_TCF_AGENT_PATH=%WIND_HOME%\components\tcf_agent-0.3
set	COMP_WEBCLI=webcli-4.9
set	COMP_WEBSERVICES=webservices-1.8
set	COMP_WINDML=windml-5.3
set	COMP_WLAN=wlan-3.3
set	COMP_WRLOAD=wrload-1.0
set	DFW_PLUGIN_PATH=%WIND_HOME%\ocd-3.3
set	DOCS_ROOT=%WIND_HOME%\docs/extentions/eclipse/plugins
set	FLEXLM_NO_CKOUT_INSTALL_LIC=1
set	FPS_BROWSER_APP_PROFILE_STRING=Internet	Explorer
set	FPS_BROWSER_USER_PROFILE_STRING=Default
set	FP_NO_HOST_CHECK=NO
set	IA32ROOT=%WIND_HOME%\intel\20160311-vxworks-6.9
set	LD_LIBRARY_PATH=%WIND_HOME%\vxworks-6.9\host\x86-win32\lib;%WIND_HOME%\workbench-3.3\foundation\x86-win32\lib;%WIND_HOME%\workbench-3.3\wrsysviewer\host\x86-win32\lib;%WIND_HOME%\workbench-3.3\analysis\host\bin\x86-win32;%WIND_HOME%\workbench-3.3\x86-win32\lib;%WIND_HOME%\lmapi-5.0\x86-win32\lib;%WIND_HOME%\intel\20160311-vxworks-6.9\x86-win32\bin\ia32
set	LM_A_APP_DISABLE_CACHE_READ=set
set	LM_LICENSE_FILE=%WIND_HOME%\license\WRSLicense_RTW_04_OCD.lic
set	MANPATH=%WIND_HOME%\vxworks-6.9\man
set	PATH=%WIND_HOME%\components\windml-5.3\host\x86-win32\bin;%WIND_HOME%\components\webservices-1.8\host\x86-win32\bin;%WIND_HOME%\components\wrll-wrsnmp\packages\wrsnmp-10.3\host\x86-win32\bin;%WIND_HOME%\components\webcli-4.9\host\x86-win32\bin;%WIND_HOME%\components\opc-3.2\host\x86-win32\bin;%WIND_HOME%\components\ip_net2-6.9\host\x86-win32\bin;%WIND_HOME%\components\dcom-2.3\host\x86-win32\bin;%WIND_HOME%\vxworks-6.9\host\x86-win32\bin;%WIND_HOME%\vxworks-6.9\vxtest\src\scripts;%WIND_HOME%\vxworks-6.9\host\binutils\x86-win32\bin;%WIND_HOME%;%WIND_HOME%\utilities-1.0\x86-win32\bin;%WIND_HOME%\ocd-3.3\x86-win32\bin;%WIND_HOME%\workbench-3.3\foundation\x86-win32\bin;%WIND_HOME%\workbench-3.3\wrsysviewer\host\x86-win32\bin;%WIND_HOME%\workbench-3.3\wrwb\platform\x86-win32\eclipse;%WIND_HOME%\workbench-3.3\analysis\host\bin\x86-win32;%WIND_HOME%\workbench-3.3\x86-win32\bin;%WIND_HOME%\lmapi-5.0\x86-win32\bin;%WIND_HOME%\gnu\4.3.3-vxworks-6.9\x86-win32\bin;%WIND_HOME%\intel\20160311-vxworks-6.9\x86-win32\bin\ia32;%WIND_HOME%\diab\5.9.1.0\WIN32\bin;%PATH%
set	PATHEXT=.COM;.EXE;.BAT;.CMD;.VBS;.VBE;.JS;.JSE;.WSF;.WSH;.MSC
set	TCLLIBPATH=%WIND_HOME%\vxworks-6.9/host/resource/tcl
set	WIND_ANALYSIS_TARGET_SCRIPT_BASE=%WIND_HOME%\workbench-3.3\analysis\target\scripts
set	WIND_BASE=%WIND_HOME%\vxworks-6.9
set	WIND_COMPONENTS=%WIND_HOME%\components
set	WIND_COMPONENTS_INCLUDES=%WIND_HOME%\components\windml-5.3\h;%WIND_HOME%\components\webservices-1.8\h;%WIND_HOME%\components\webcli-4.9\target\h;%WIND_HOME%\components\tcf_agent-0.3\agent;%WIND_HOME%\components\stacktrace-1.0\include;%WIND_HOME%\components\opc-3.2\h;%WIND_HOME%\components\dcom-2.3\h;%WIND_HOME%\components\cafe-0.1\hpc\include;%WIND_HOME%\components\cafe-0.1\cafe\include;%WIND_HOME%\components\cafe-0.1\vxworks\include
set	WIND_COMPONENTS_LIBNAMES=windml-5.3_dyn;windml-5.3;webservices-1.8;webcli-4.9;tcf_agent-0.3;stacktrace-1.0;opc-3.2;ip_net2-6.9;dcom-2.3;cafe-0.1
set	WIND_COMPONENTS_LIBPATHS=%WIND_HOME%\components\obj\vxworks-6.9\krnl\lib
set	WIND_DFW_PATH=%WIND_HOME%\workbench-3.3\dfw\
set	WIND_DIAB_PATH=%WIND_HOME%\diab\5.9.1.0
set	WIND_DOCS=%WIND_HOME%\docs
set	WIND_EXTENSIONS=%WIND_HOME%\components\windml-5.3\extensions;%WIND_HOME%\components\webservices-1.8\extensions;%WIND_HOME%\components\webcli-4.9\extensions;%WIND_HOME%\components\opc-3.2\extensions;%WIND_HOME%\components\ip_net2-6.9\extensions;%WIND_HOME%\components\dcom-2.3\extensions;%WIND_HOME%\components\extensions;%WIND_HOME%\ocd-3.3;%WIND_HOME%\workbench-3.3\analysis\valgrind;%WIND_HOME%\workbench-3.3\analysis\extensions;%WIND_HOME%\workbench-3.3\wrsysviewer;%WIND_HOME%\workbench-3.3\wrwb\pdt;%WIND_HOME%\workbench-3.3\wrwb\intel;%WIND_HOME%\workbench-3.3\wrwb\webtools;%WIND_HOME%\workbench-3.3\wrwb\wre;%WIND_HOME%\workbench-3.3\wrwb\vspworks;%WIND_HOME%\workbench-3.3\wrwb\tools;%WIND_HOME%\workbench-3.3\wrwb\wrlinux;%WIND_HOME%\workbench-3.3\wrwb\wrhv;%WIND_HOME%\workbench-3.3\wrwb\vthreads;%WIND_HOME%\workbench-3.3\wrwb\vxworksmilshae;%WIND_HOME%\workbench-3.3\wrwb\vxworksmils;%WIND_HOME%\workbench-3.3\wrwb\vxworkscert;%WIND_HOME%\workbench-3.3\wrwb\vxworks653;%WIND_HOME%\workbench-3.3\wrwb\vxworks55;%WIND_HOME%\workbench-3.3\wrwb\vxworks;%WIND_HOME%\workbench-3.3\wrwb\wrworkbench;%WIND_HOME%\wrmscomponents\diagnostics\extensions;%WIND_HOME%\wrlinux-2.0\extensions;%WIND_HOME%\unittester-2.5\extensions;%WIND_HOME%\unittester-2.4\extensions;%WIND_HOME%\studio-2.0\extensions;%WIND_HOME%\components\windml-5.1\extensions;%WIND_HOME%\components\windml-5.0\extensions;%WIND_HOME%\components\windml-4.2\extensions;%WIND_HOME%\components\windml-4.1\extensions;%WIND_HOME%\components\webservices-1.5\extensions;%WIND_HOME%\components\webservices-1.4\extensions;%WIND_HOME%\components\webservices-1.3\extensions;%WIND_HOME%\components\webcli-4.7\extensions;%WIND_HOME%\components\opc-3.1\extensions;%WIND_HOME%\docs\extensions
set	WIND_FOUNDATION_PATH=%WIND_HOME%\workbench-3.3\foundation
set	WIND_GNU_PATH=%WIND_HOME%\gnu\4.3.3-vxworks-6.9
set	WIND_HOME=%WIND_HOME%
set	WIND_HOST_TYPE=x86-win32
set	WIND_ICC_PATH=%WIND_HOME%\intel\20160311-vxworks-6.9
set	WIND_INSTALLER_HOME=%WIND_HOME%\maintenance\wrInstaller
set	WIND_INTRO=%WIND_HOME%\vxworks-6.9\gettingStarted\vxworks.properties;%WIND_HOME%\ocd-3.3\gettingStarted\ocd\ocd.properties;%WIND_HOME%\workbench-3.3\gettingStarted\wrwb\workbench.properties
set	WIND_JRE_HOME=%WIND_HOME%\jre\1.6.0_21\x86-win32
set	WIND_OCD_LIBUSB_PATH=%WIND_HOME%\ocd-3.3\host\x86-win32\lib
set	WIND_OCD_PATH=%WIND_HOME%\ocd-3.3
set	WIND_PLATFORM=vxworks-6.9
set	WIND_PREFERRED_PACKAGES=vxworks-6.9;workbench-3.3
set	WIND_RSS_CHANNELS=http://www.windriver.com/feeds/vxworks_660.xml;http://www.windriver.com/feeds/workbench_ocd.xml;http://www.windriver.com/feeds/workbench_300.xml
set	WIND_SAMPLES=%WIND_HOME%\components\windml-5.3\samples;%WIND_HOME%\components\webservices-1.8\tutorials;%WIND_HOME%\components\webcli-4.9\samples;%WIND_HOME%\components\opc-3.2\samples;%WIND_HOME%\components\ip_net2-6.9\samples;%WIND_HOME%\components\dcom-2.3\samples;%WIND_HOME%\vxworks-6.9\target\usr\apps\samples;%WIND_HOME%\vxworks-6.9\target\src\demo;%WIND_HOME%\workbench-3.3\analysis\target\src\linux;%WIND_HOME%\workbench-3.3\analysis\target\src\vxworks;%WIND_HOME%\workbench-3.3\samples
set	WIND_SCOPETOOLS_BASE=%WIND_HOME%\workbench-3.3\analysis
set	WIND_TOOLCHAINS=gnu;icc;diab
set	WIND_TOOLS=%WIND_HOME%\workbench-3.3
set	WIND_USERMODE_AGENT=%WIND_HOME%\workbench-3.3\usermode-agent\bin\usermode-agent.sh
set	WIND_USERMODE_AGENT_PATH=%WIND_HOME%\workbench-3.3\usermode-agent
set	WIND_USR=%WIND_HOME%\vxworks-6.9\target\usr
set	WIND_UTILITIES=%WIND_HOME%\utilities-1.0
set	WIND_VXCONFIG=%WIND_HOME%\components\windml-5.3\osconfig\vxworks;%WIND_HOME%\components\tcf_agent-0.3\osconfig\vxworks;%WIND_HOME%\components\stacktrace-1.0\osconfig\vxworks;%WIND_HOME%\components\ip_net2-6.9\osconfig\vxworks;%WIND_HOME%\components\cafe-0.1\osconfig\vxworks
set	WIND_WB_SCRIPTS=%WIND_HOME%\workbench-3.3\scripts\
set	WIND_WRSV_PATH=%WIND_HOME%\workbench-3.3\wrsysviewer
set	WIND_WRWB_PATH=%WIND_HOME%\workbench-3.3\wrwb\platform\x86-win32\eclipse
REM set	WRSD_LICENSE_FILE=%WIND_HOME%\license
set WRSD_LICENSE_FILE=%WIND_HOME%\license;%WIND_HOME%\license\zwrsLicense.lic
set	WRVX_COMPBASE=%WIND_HOME%\components
set	__COMPAT_LAYER=ElevateCreateProcess	DetectorsWin8

set	WIND_COMP_IPCOM=%WIND_COMPONENTS%\ip_net2-6.9\ipcom\config

REM	*****************************************************
REM	* LKV-120 Application ENV
REM	*****************************************************
set	USR_APP_HOME=D:\HVDC_Simulator

set	TARGET_OS_DIR=%WIND_HOME%\vxworks-6.9\target\config\lk11_lkv_120
set	TARGET_APP_DIR=%USR_APP_HOME%\CPU_IDE\make
set	TARGET_ENV_DIR=%USR_APP_HOME%\CPU_IDE\env
set	TARGET_VIP_DIR=%USR_APP_HOME%\CPU_IDE\vxImgPrj
