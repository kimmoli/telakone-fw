# Dummy QtC project file for telakone

TEMPLATE=aux
SOURCES += \
    main.c \
    adc.c \
    blinker.c \
    cmd_reboot.c \
    cmd_status.c \
    shellcommands.c \
    board/board.c

HEADERS += \
    adc.h \
    blinker.h \
    chconf.h \
    halconf.h \
    mcuconf.h \
    shellcommands.h \
    shellconf.h \
    board/board.h

OTHER_FILES += \
    Makefile \
    docs/joystick.png \
    docs/README.md

INCLUDEPATH += \
 ../ChibiOS/os/license \
 ../ChibiOS/os/common/startup/ARMCMx/compilers/GCC \
 ../ChibiOS/os/common/startup/ARMCMx/devices/STM32F2xx \
 ../ChibiOS/os/common/ext/CMSIS/include \
 ../ChibiOS/os/common/ext/CMSIS/ST/STM32F2xx \
 ../ChibiOS/os/rt/include \
 ../ChibiOS/os/common/oslib/include \
 ../ChibiOS/os/common/ports/ARMCMx \
 ../ChibiOS/os/common/ports/ARMCMx/compilers/GCC \
 ../ChibiOS/os/hal/osal/rt \
 ../ChibiOS/os/hal/include \
 ../ChibiOS/os/hal/ports/common/ARMCMx \
 ../ChibiOS/os/hal/ports/STM32/STM32F4xx \
 ../ChibiOS/os/hal/ports/STM32/LLD/ADCv2 \
 ../ChibiOS/os/hal/ports/STM32/LLD/CANv1 \
 ../ChibiOS/os/hal/ports/STM32/LLD/DACv1 \
 ../ChibiOS/os/hal/ports/STM32/LLD/DMAv2 \
 ../ChibiOS/os/hal/ports/STM32/LLD/EXTIv1 \
 ../ChibiOS/os/hal/ports/STM32/LLD/GPIOv2 \
 ../ChibiOS/os/hal/ports/STM32/LLD/I2Cv1 \
 ../ChibiOS/os/hal/ports/STM32/LLD/MACv1 \
 ../ChibiOS/os/hal/ports/STM32/LLD/OTGv1 \
 ../ChibiOS/os/hal/ports/STM32/LLD/QUADSPIv1 \
 ../ChibiOS/os/hal/ports/STM32/LLD/RTCv2 \
 ../ChibiOS/os/hal/ports/STM32/LLD/SDIOv1 \
 ../ChibiOS/os/hal/ports/STM32/LLD/SPIv1 \
 ../ChibiOS/os/hal/ports/STM32/LLD/TIMv1 \
 ../ChibiOS/os/hal/ports/STM32/LLD/USARTv1 \
 ../ChibiOS/os/hal/ports/STM32/LLD/xWDGv1 \
 ../ChibiOS/os/various/shell \
 ../ChibiOS/os/hal/lib/streams

