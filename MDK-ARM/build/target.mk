###########################################################################
#  !!! This file is Auto-Generated By Embedded IDE, Don't modify it !!!
###########################################################################

# eide version
EIDE_VER = 2

# current target
CUR_TARGET := simple_7inches

# current compiler
COMPILER_TYPE := AC5

# include folders
INCLUDE_FOLDERS += ../Drivers/STM32F4xx_HAL_Driver/Inc
INCLUDE_FOLDERS += ../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
INCLUDE_FOLDERS += ../Drivers/CMSIS/Device/ST/STM32F4xx/Include
INCLUDE_FOLDERS += ../Drivers/CMSIS/Include
INCLUDE_FOLDERS += ../Core/User/app/common/inc
INCLUDE_FOLDERS += ../Core/User/app/system/inc
INCLUDE_FOLDERS += ../Core/User/boot/Inc
INCLUDE_FOLDERS += ../Core/User/doc
INCLUDE_FOLDERS += ../Core/User/lib
INCLUDE_FOLDERS += ../Core/User/lib/bspLib/inc
INCLUDE_FOLDERS += ../Core/User/lib/bspLib/usb/FATFS/inc
INCLUDE_FOLDERS += ../Core/User/lib/bspLib/usb/STM32_USB_OTG_Driver/inc
INCLUDE_FOLDERS += ../Core/User/lib/bspLib/usb/STM32_USB_HOST_Library/Class/MSC/inc
INCLUDE_FOLDERS += ../Core/User/lib/bspLib/usb/STM32_USB_HOST_Library/Core/inc
INCLUDE_FOLDERS += ../Core/User/lib/bspLib/usb/USB_APP
INCLUDE_FOLDERS += ../Core/User/lib/doPTLib/inc
INCLUDE_FOLDERS += .eide/deps

# library search folders

# c source files
C_SOURCES += ../Core/User/app/common/src/Mit_funtion.c
C_SOURCES += ../Core/User/app/common/src/Mit_menu_files.c
C_SOURCES += ../Core/User/app/common/src/Patfun.c
C_SOURCES += ../Core/User/app/common/src/ScrollBar.c
C_SOURCES += ../Core/User/app/common/src/Toast.c
C_SOURCES += ../Core/User/app/common/src/communication.c
C_SOURCES += ../Core/User/app/common/src/exfunction.c
C_SOURCES += ../Core/User/app/common/src/flashsave.c
C_SOURCES += ../Core/User/app/common/src/function.c
C_SOURCES += ../Core/User/app/common/src/lcddisp.c
C_SOURCES += ../Core/User/app/common/src/msgpic.c
C_SOURCES += ../Core/User/app/common/src/register.c
C_SOURCES += ../Core/User/app/common/src/upflash.c
C_SOURCES += ../Core/User/app/system/src/menu_convert.c
C_SOURCES += ../Core/User/app/system/src/menu_create.c
C_SOURCES += ../Core/User/app/system/src/menu_documents.c
C_SOURCES += ../Core/User/app/system/src/menu_function.c
C_SOURCES += ../Core/User/app/system/src/menu_help.c
C_SOURCES += ../Core/User/app/system/src/menu_iotest.c
C_SOURCES += ../Core/User/app/system/src/menu_modify.c
C_SOURCES += ../Core/User/app/system/src/menu_p.c
C_SOURCES += ../Core/User/app/system/src/menu_program.c
C_SOURCES += ../Core/User/app/system/src/viewPattern.c
C_SOURCES += ../Core/User/boot/Src/main.c
C_SOURCES += ../Core/User/boot/Src/systemMain.c
C_SOURCES += ../Core/User/boot/Src/system_stm32f4xx.c
C_SOURCES += ../Core/User/boot/Src/utility.c

# cpp source files

# asm source files
ASM_SOURCES += startup_stm32f429xx.s

# object files
OBJ_SOURCES += ../Core/User/lib/cpulib.lib
OBJ_SOURCES += ../Core/User/lib/doptlib.lib

# macro defines
DEFINES += STM32F429_439xx
DEFINES += USE_USB_OTG_FS
DEFINES += APPLICACION
DEFINES += USE_STDPERIPH_DRIVER
DEFINES += SYSTEM1
DEFINES += STM32F429xx