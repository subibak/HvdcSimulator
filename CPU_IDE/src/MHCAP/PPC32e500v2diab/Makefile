# Wind River Workbench generated Makefile.
# Do not edit!!!
#
# The file ".wrmakefile" is the template used by the Wind River Workbench to
# generate the makefiles of this project. Add user-specific build targets and
# make rules only(!) in this project's ".wrmakefile" file. These will then be
# automatically dumped into the makefiles.

WIND_HOME := $(subst \,/,$(WIND_HOME))
WIND_BASE := $(subst \,/,$(WIND_BASE))
WIND_USR := $(subst \,/,$(WIND_USR))
WRVX_COMPBASE := $(subst \,/,$(WRVX_COMPBASE))

all : pre_build main_all post_build

_clean ::
	@echo "make: removing targets and objects of `pwd`"

TRACE=0
TRACEON=$(TRACE:0=@)
TRACE_FLAG=$(TRACEON:1=)

JOBS?=1
TARGET_JOBS?=$(JOBS)

MAKEFILE := Makefile

FLEXIBLE_BUILD := 1

BUILD_SPEC = PPC32e500v2diab
DEBUG_MODE = 1
ifeq ($(DEBUG_MODE),1)
MODE_DIR := Debug
else
MODE_DIR := NonDebug
endif
OBJ_DIR := .
WS_ROOT_DIR := C:/WindRiver6.9/workspace
PRJ_ROOT_DIR := $(WS_ROOT_DIR)/MHCAP-Test



#Global Build Macros
PROJECT_TYPE = DKM
DEFINES = 
EXPAND_DBG = 0


#BuildSpec specific Build Macros
VX_CPU_FAMILY = ppc
CPU = PPC32
TOOL_FAMILY = diab
TOOL = e500v2diab
TOOL_PATH = 
CC_ARCH_SPEC = -tPPCE500V2FH:vxworks69
VSB_DIR = $(WIND_BASE)/target/lib
VSB_CONFIG_FILE = $(VSB_DIR)/h/config/vsbConfig.h
LIBPATH = 
LIBS = 

IDE_INCLUDES = -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/wrn/coreip 

IDE_LIBRARIES = 

IDE_DEFINES = -DCPU=_VX_$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D_VSB_CONFIG_FILE=\"$(VSB_DIR)/h/config/vsbConfig.h\"   -DIP_PORT_VXWORKS=69 



#BuildTool flags
ifeq ($(DEBUG_MODE),1)
DEBUGFLAGS_C-Compiler = -g
DEBUGFLAGS_C++-Compiler = -g
DEBUGFLAGS_Linker = -g
DEBUGFLAGS_Partial-Image-Linker = 
DEBUGFLAGS_Librarian = 
DEBUGFLAGS_Assembler = -g
else
DEBUGFLAGS_C-Compiler =  -XO -Xsize-opt 
DEBUGFLAGS_C++-Compiler =  -XO -Xsize-opt 
DEBUGFLAGS_Linker =  -XO -Xsize-opt 
DEBUGFLAGS_Partial-Image-Linker = 
DEBUGFLAGS_Librarian = 
DEBUGFLAGS_Assembler =  -XO -Xsize-opt 
endif


#Project Targets
PROJECT_TARGETS = MHCAP-Test/$(MODE_DIR)/MHCAP-Test.out \
	 MHCAP-Test_partialImage/$(MODE_DIR)/MHCAP-Test_partialImage.o


#Rules

# MHCAP-Test
ifeq ($(DEBUG_MODE),1)
MHCAP-Test/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
MHCAP-Test/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
MHCAP-Test/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
MHCAP-Test/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
MHCAP-Test/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
MHCAP-Test/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
MHCAP-Test/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler =  -XO -Xsize-opt 
MHCAP-Test/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler =  -XO -Xsize-opt 
MHCAP-Test/$(MODE_DIR)/% : DEBUGFLAGS_Linker =  -XO -Xsize-opt 
MHCAP-Test/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
MHCAP-Test/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
MHCAP-Test/$(MODE_DIR)/% : DEBUGFLAGS_Assembler =  -XO -Xsize-opt 
endif
MHCAP-Test/$(MODE_DIR)/% : IDE_INCLUDES = -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/wrn/coreip 
MHCAP-Test/$(MODE_DIR)/% : IDE_LIBRARIES = 
MHCAP-Test/$(MODE_DIR)/% : IDE_DEFINES = -DCPU=_VX_$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D_VSB_CONFIG_FILE=\"$(VSB_DIR)/h/config/vsbConfig.h\"   -DIP_PORT_VXWORKS=69 
MHCAP-Test/$(MODE_DIR)/% : PROJECT_TYPE = DKM
MHCAP-Test/$(MODE_DIR)/% : DEFINES = 
MHCAP-Test/$(MODE_DIR)/% : EXPAND_DBG = 0
MHCAP-Test/$(MODE_DIR)/% : VX_CPU_FAMILY = ppc
MHCAP-Test/$(MODE_DIR)/% : CPU = PPC32
MHCAP-Test/$(MODE_DIR)/% : TOOL_FAMILY = diab
MHCAP-Test/$(MODE_DIR)/% : TOOL = e500v2diab
MHCAP-Test/$(MODE_DIR)/% : TOOL_PATH = 
MHCAP-Test/$(MODE_DIR)/% : CC_ARCH_SPEC = -tPPCE500V2FH:vxworks69
MHCAP-Test/$(MODE_DIR)/% : VSB_DIR = $(WIND_BASE)/target/lib
MHCAP-Test/$(MODE_DIR)/% : VSB_CONFIG_FILE = $(VSB_DIR)/h/config/vsbConfig.h
MHCAP-Test/$(MODE_DIR)/% : LIBPATH = 
MHCAP-Test/$(MODE_DIR)/% : LIBS = 
MHCAP-Test/$(MODE_DIR)/% : OBJ_DIR := MHCAP-Test/$(MODE_DIR)

OBJECTS_MHCAP-Test = MHCAP-Test_partialImage/$(MODE_DIR)/MHCAP-Test_partialImage.o

ifeq ($(TARGET_JOBS),1)
MHCAP-Test/$(MODE_DIR)/MHCAP-Test.out : $(OBJECTS_MHCAP-Test)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@";rm -f "$@";ddump -Ng $(OBJECTS_MHCAP-Test) | tclsh $(WIND_BASE)/host/resource/hutils/tcl/munch.tcl -c ppc -tags $(VSB_DIR)/tags/ppc/PPC32/e500v2common/dkm.tags > $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)dcc $(DEBUGFLAGS_Linker) $(CC_ARCH_SPEC) -Xdollar-in-ident -ei1518,4177,4223,4301,4550,5409,1606 -ei4193,4826,4381,4237,1573,4007,4082,4177,4223,4260,4550,5361,5828,2273,5387,5388 -ei1522,4092,4111,4144,4152,4167,4171,4174,4186,4188,4191,4192,4223,4231,4236,4284,4375,4494,4513,5152,5457 -Xforce-declarations  $(ADDED_CFLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES)  $(IDE_DEFINES) $(DEFINES) -o $(OBJ_DIR)/ctdt.o -c $(OBJ_DIR)/ctdt.c; $(TOOL_PATH)dld -tPPCE500V2FH:vxworks69 -X -r5 -r4 -o "$@" $(OBJ_DIR)/ctdt.o $(OBJECTS_MHCAP-Test) $(IDE_LIBRARIES) $(LIBPATH) $(LIBS) $(ADDED_LIBPATH) $(ADDED_LIBS) && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi

else
MHCAP-Test/$(MODE_DIR)/MHCAP-Test.out : MHCAP-Test/$(MODE_DIR)/MHCAP-Test.out_jobs

endif
MHCAP-Test/$(MODE_DIR)/MHCAP-Test_compile_file : $(FILE) ;

_clean :: MHCAP-Test/$(MODE_DIR)/MHCAP-Test_clean

MHCAP-Test/$(MODE_DIR)/MHCAP-Test_clean : 
	$(TRACE_FLAG)if [ -d "MHCAP-Test" ]; then cd "MHCAP-Test"; rm -rf $(MODE_DIR); fi


# MHCAP-Test_partialImage
ifeq ($(DEBUG_MODE),1)
MHCAP-Test_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
MHCAP-Test_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
MHCAP-Test_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
MHCAP-Test_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
MHCAP-Test_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
MHCAP-Test_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
MHCAP-Test_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler =  -XO -Xsize-opt 
MHCAP-Test_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler =  -XO -Xsize-opt 
MHCAP-Test_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Linker =  -XO -Xsize-opt 
MHCAP-Test_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
MHCAP-Test_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
MHCAP-Test_partialImage/$(MODE_DIR)/% : DEBUGFLAGS_Assembler =  -XO -Xsize-opt 
endif
MHCAP-Test_partialImage/$(MODE_DIR)/% : IDE_INCLUDES = -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/wrn/coreip 
MHCAP-Test_partialImage/$(MODE_DIR)/% : IDE_LIBRARIES = 
MHCAP-Test_partialImage/$(MODE_DIR)/% : IDE_DEFINES = -DCPU=_VX_$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D_VSB_CONFIG_FILE=\"$(VSB_DIR)/h/config/vsbConfig.h\"   -DIP_PORT_VXWORKS=69 
MHCAP-Test_partialImage/$(MODE_DIR)/% : PROJECT_TYPE = DKM
MHCAP-Test_partialImage/$(MODE_DIR)/% : DEFINES = 
MHCAP-Test_partialImage/$(MODE_DIR)/% : EXPAND_DBG = 0
MHCAP-Test_partialImage/$(MODE_DIR)/% : VX_CPU_FAMILY = ppc
MHCAP-Test_partialImage/$(MODE_DIR)/% : CPU = PPC32
MHCAP-Test_partialImage/$(MODE_DIR)/% : TOOL_FAMILY = diab
MHCAP-Test_partialImage/$(MODE_DIR)/% : TOOL = e500v2diab
MHCAP-Test_partialImage/$(MODE_DIR)/% : TOOL_PATH = 
MHCAP-Test_partialImage/$(MODE_DIR)/% : CC_ARCH_SPEC = -tPPCE500V2FH:vxworks69
MHCAP-Test_partialImage/$(MODE_DIR)/% : VSB_DIR = $(WIND_BASE)/target/lib
MHCAP-Test_partialImage/$(MODE_DIR)/% : VSB_CONFIG_FILE = $(VSB_DIR)/h/config/vsbConfig.h
MHCAP-Test_partialImage/$(MODE_DIR)/% : LIBPATH = 
MHCAP-Test_partialImage/$(MODE_DIR)/% : LIBS = 
MHCAP-Test_partialImage/$(MODE_DIR)/% : OBJ_DIR := MHCAP-Test_partialImage/$(MODE_DIR)

MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/cmd_proc.o : $(PRJ_ROOT_DIR)/cmd_proc.c $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)dcc $(DEBUGFLAGS_C-Compiler) $(CC_ARCH_SPEC) -W:c:,-Xclib-optim-off -Xansi -Xlocal-data-area-static-only  -W:c++:.CPP -Xc-new -Xdialect-c89  -Xstsw-slow -ei1518,4177,4223,4301,4550,5409,1606 -ei4193,4826,4381,4237,1573,4007,4082,4177,4223,4260,4550,5361,5828,2273,5387,5388 -ei1522,4092,4111,4144,4152,4167,4171,4174,4186,4188,4191,4192,4223,4231,4236,4284,4375,4494,4513,5152,5457 -Xforce-declarations   -Xmake-dependency=0xd $(IDE_DEFINES) $(DEFINES) $(ADDED_CFLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -o "$@" -c "$<"


MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/common.o : $(PRJ_ROOT_DIR)/common.c $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)dcc $(DEBUGFLAGS_C-Compiler) $(CC_ARCH_SPEC) -W:c:,-Xclib-optim-off -Xansi -Xlocal-data-area-static-only  -W:c++:.CPP -Xc-new -Xdialect-c89  -Xstsw-slow -ei1518,4177,4223,4301,4550,5409,1606 -ei4193,4826,4381,4237,1573,4007,4082,4177,4223,4260,4550,5361,5828,2273,5387,5388 -ei1522,4092,4111,4144,4152,4167,4171,4174,4186,4188,4191,4192,4223,4231,4236,4284,4375,4494,4513,5152,5457 -Xforce-declarations   -Xmake-dependency=0xd $(IDE_DEFINES) $(DEFINES) $(ADDED_CFLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -o "$@" -c "$<"


MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/link_lib.o : $(PRJ_ROOT_DIR)/link_lib.c $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)dcc $(DEBUGFLAGS_C-Compiler) $(CC_ARCH_SPEC) -W:c:,-Xclib-optim-off -Xansi -Xlocal-data-area-static-only  -W:c++:.CPP -Xc-new -Xdialect-c89  -Xstsw-slow -ei1518,4177,4223,4301,4550,5409,1606 -ei4193,4826,4381,4237,1573,4007,4082,4177,4223,4260,4550,5361,5828,2273,5387,5388 -ei1522,4092,4111,4144,4152,4167,4171,4174,4186,4188,4191,4192,4223,4231,4236,4284,4375,4494,4513,5152,5457 -Xforce-declarations   -Xmake-dependency=0xd $(IDE_DEFINES) $(DEFINES) $(ADDED_CFLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -o "$@" -c "$<"


MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/link_test.o : $(PRJ_ROOT_DIR)/link_test.c $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)dcc $(DEBUGFLAGS_C-Compiler) $(CC_ARCH_SPEC) -W:c:,-Xclib-optim-off -Xansi -Xlocal-data-area-static-only  -W:c++:.CPP -Xc-new -Xdialect-c89  -Xstsw-slow -ei1518,4177,4223,4301,4550,5409,1606 -ei4193,4826,4381,4237,1573,4007,4082,4177,4223,4260,4550,5361,5828,2273,5387,5388 -ei1522,4092,4111,4144,4152,4167,4171,4174,4186,4188,4191,4192,4223,4231,4236,4284,4375,4494,4513,5152,5457 -Xforce-declarations   -Xmake-dependency=0xd $(IDE_DEFINES) $(DEFINES) $(ADDED_CFLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -o "$@" -c "$<"


MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/lkp502.o : $(PRJ_ROOT_DIR)/lkp502.c $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)dcc $(DEBUGFLAGS_C-Compiler) $(CC_ARCH_SPEC) -W:c:,-Xclib-optim-off -Xansi -Xlocal-data-area-static-only  -W:c++:.CPP -Xc-new -Xdialect-c89  -Xstsw-slow -ei1518,4177,4223,4301,4550,5409,1606 -ei4193,4826,4381,4237,1573,4007,4082,4177,4223,4260,4550,5361,5828,2273,5387,5388 -ei1522,4092,4111,4144,4152,4167,4171,4174,4186,4188,4191,4192,4223,4231,4236,4284,4375,4494,4513,5152,5457 -Xforce-declarations   -Xmake-dependency=0xd $(IDE_DEFINES) $(DEFINES) $(ADDED_CFLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -o "$@" -c "$<"


MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/mhcap_main.o : $(PRJ_ROOT_DIR)/mhcap_main.c $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)dcc $(DEBUGFLAGS_C-Compiler) $(CC_ARCH_SPEC) -W:c:,-Xclib-optim-off -Xansi -Xlocal-data-area-static-only  -W:c++:.CPP -Xc-new -Xdialect-c89  -Xstsw-slow -ei1518,4177,4223,4301,4550,5409,1606 -ei4193,4826,4381,4237,1573,4007,4082,4177,4223,4260,4550,5361,5828,2273,5387,5388 -ei1522,4092,4111,4144,4152,4167,4171,4174,4186,4188,4191,4192,4223,4231,4236,4284,4375,4494,4513,5152,5457 -Xforce-declarations   -Xmake-dependency=0xd $(IDE_DEFINES) $(DEFINES) $(ADDED_CFLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -o "$@" -c "$<"


MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/mhcap_task.o : $(PRJ_ROOT_DIR)/mhcap_task.c $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)dcc $(DEBUGFLAGS_C-Compiler) $(CC_ARCH_SPEC) -W:c:,-Xclib-optim-off -Xansi -Xlocal-data-area-static-only  -W:c++:.CPP -Xc-new -Xdialect-c89  -Xstsw-slow -ei1518,4177,4223,4301,4550,5409,1606 -ei4193,4826,4381,4237,1573,4007,4082,4177,4223,4260,4550,5361,5828,2273,5387,5388 -ei1522,4092,4111,4144,4152,4167,4171,4174,4186,4188,4191,4192,4223,4231,4236,4284,4375,4494,4513,5152,5457 -Xforce-declarations   -Xmake-dependency=0xd $(IDE_DEFINES) $(DEFINES) $(ADDED_CFLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -o "$@" -c "$<"


MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/vme.o : $(PRJ_ROOT_DIR)/vme.c $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)dcc $(DEBUGFLAGS_C-Compiler) $(CC_ARCH_SPEC) -W:c:,-Xclib-optim-off -Xansi -Xlocal-data-area-static-only  -W:c++:.CPP -Xc-new -Xdialect-c89  -Xstsw-slow -ei1518,4177,4223,4301,4550,5409,1606 -ei4193,4826,4381,4237,1573,4007,4082,4177,4223,4260,4550,5361,5828,2273,5387,5388 -ei1522,4092,4111,4144,4152,4167,4171,4174,4186,4188,4191,4192,4223,4231,4236,4284,4375,4494,4513,5152,5457 -Xforce-declarations   -Xmake-dependency=0xd $(IDE_DEFINES) $(DEFINES) $(ADDED_CFLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -o "$@" -c "$<"


MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/vme_disp.o : $(PRJ_ROOT_DIR)/vme_disp.c $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)dcc $(DEBUGFLAGS_C-Compiler) $(CC_ARCH_SPEC) -W:c:,-Xclib-optim-off -Xansi -Xlocal-data-area-static-only  -W:c++:.CPP -Xc-new -Xdialect-c89  -Xstsw-slow -ei1518,4177,4223,4301,4550,5409,1606 -ei4193,4826,4381,4237,1573,4007,4082,4177,4223,4260,4550,5361,5828,2273,5387,5388 -ei1522,4092,4111,4144,4152,4167,4171,4174,4186,4188,4191,4192,4223,4231,4236,4284,4375,4494,4513,5152,5457 -Xforce-declarations   -Xmake-dependency=0xd $(IDE_DEFINES) $(DEFINES) $(ADDED_CFLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -o "$@" -c "$<"


MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/vme_test.o : $(PRJ_ROOT_DIR)/vme_test.c $(FORCE_FILE_BUILD)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)dcc $(DEBUGFLAGS_C-Compiler) $(CC_ARCH_SPEC) -W:c:,-Xclib-optim-off -Xansi -Xlocal-data-area-static-only  -W:c++:.CPP -Xc-new -Xdialect-c89  -Xstsw-slow -ei1518,4177,4223,4301,4550,5409,1606 -ei4193,4826,4381,4237,1573,4007,4082,4177,4223,4260,4550,5361,5828,2273,5387,5388 -ei1522,4092,4111,4144,4152,4167,4171,4174,4186,4188,4191,4192,4223,4231,4236,4284,4375,4494,4513,5152,5457 -Xforce-declarations   -Xmake-dependency=0xd $(IDE_DEFINES) $(DEFINES) $(ADDED_CFLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -o "$@" -c "$<"


OBJECTS_MHCAP-Test_partialImage = MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/cmd_proc.o \
	 MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/common.o \
	 MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/link_lib.o \
	 MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/link_test.o \
	 MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/lkp502.o \
	 MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/mhcap_main.o \
	 MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/mhcap_task.o \
	 MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/vme.o \
	 MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/vme_disp.o \
	 MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/vme_test.o

ifeq ($(TARGET_JOBS),1)
MHCAP-Test_partialImage/$(MODE_DIR)/MHCAP-Test_partialImage.o : $(OBJECTS_MHCAP-Test_partialImage)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)dld -tPPCE500V2FH:vxworks69 -X -r5  -o "$@" $(OBJECTS_MHCAP-Test_partialImage) $(ADDED_OBJECTS) $(IDE_LIBRARIES) $(LIBPATH) $(LIBS) $(ADDED_LIBPATH) $(ADDED_LIBS) && if [ "$(EXPAND_DBG)" = "1" ]; then plink "$@";fi

else
MHCAP-Test_partialImage/$(MODE_DIR)/MHCAP-Test_partialImage.o : MHCAP-Test_partialImage/$(MODE_DIR)/MHCAP-Test_partialImage.o_jobs

endif
MHCAP-Test_partialImage/$(MODE_DIR)/MHCAP-Test_partialImage_compile_file : $(FILE) ;

_clean :: MHCAP-Test_partialImage/$(MODE_DIR)/MHCAP-Test_partialImage_clean

MHCAP-Test_partialImage/$(MODE_DIR)/MHCAP-Test_partialImage_clean : 
	$(TRACE_FLAG)if [ -d "MHCAP-Test_partialImage" ]; then cd "MHCAP-Test_partialImage"; rm -rf $(MODE_DIR); fi

force : 

TARGET_JOBS_RULE?=echo "Update the makefile template via File > Import > Build Settings : Update makefile template";exit 1
%_jobs : 
	$(TRACE_FLAG)$(TARGET_JOBS_RULE)

DEP_FILES := MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/cmd_proc.d MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/common.d MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/link_lib.d \
	 MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/link_test.d MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/lkp502.d MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/mhcap_main.d \
	 MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/mhcap_task.d MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/vme.d MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/vme_disp.d \
	 MHCAP-Test_partialImage/$(MODE_DIR)/Objects/MHCAP-Test/vme_test.d
-include $(DEP_FILES)


WIND_SCOPETOOLS_BASE := $(subst \,/,$(WIND_SCOPETOOLS_BASE))

clean_scopetools :
	$(TRACE_FLAG)rm -rf $(PRJ_ROOT_DIR)/.coveragescope/db

CLEAN_STEP := clean_scopetools


-include $(PRJ_ROOT_DIR)/*.makefile

-include *.makefile

TARGET_JOBS_RULE=$(MAKE) -f $(MAKEFILE) --jobs $(TARGET_JOBS) $(MFLAGS) $* TARGET_JOBS=1
ifeq ($(JOBS),1)
main_all : external_build  $(PROJECT_TARGETS)
	@echo "make: built targets of `pwd`"
else
main_all : external_build 
	@$(MAKE) -f $(MAKEFILE) --jobs $(JOBS) $(MFLAGS) $(PROJECT_TARGETS) TARGET_JOBS=1 &&\
	echo "make: built targets of `pwd`"
endif 

# entry point for extending the build
external_build ::
	@echo ""

# main entry point for pre processing prior to the build
pre_build :: $(PRE_BUILD_STEP) generate_sources
	@echo ""

# entry point for generating sources prior to the build
generate_sources ::
	@echo ""

# main entry point for post processing after the build
post_build :: $(POST_BUILD_STEP) deploy_output
	@echo ""

# entry point for deploying output after the build
deploy_output ::
	@echo ""

clean :: external_clean $(CLEAN_STEP) _clean

# entry point for extending the build clean
external_clean ::
	@echo ""
