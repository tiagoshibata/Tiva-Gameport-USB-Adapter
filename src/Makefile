#******************************************************************************
#
# Makefile - Rules for building the project example.
#
# Copyright (c) 2013-2017 Texas Instruments Incorporated.  All rights reserved.
# Software License Agreement
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions
#   are met:
#
#   Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
#
#   Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the
#   distribution.
#
#   Neither the name of Texas Instruments Incorporated nor the names of
#   its contributors may be used to endorse or promote products derived
#   from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# This is part of revision 2.1.4.178 of the Tiva Firmware Development Package.
#
#******************************************************************************
# Changed by Tiago Shibata for Gameport to USB adapter.

#
# Defines the part type that this project uses.
#
PART=TM4C123GH6PM

#
# The base directory for TivaWare.
#
ROOT?=../TivaWare

#
# Include the common make definitions.
#
include ${ROOT}/makedefs

#
# Where to find header files that do not live in the source directory.
#
IPATH?=$(ROOT)

#
# The default rule, which causes the project example to be built.
#
all: ${COMPILER}
all: ${COMPILER}/gameport.axf

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${COMPILER} ${wildcard *~}

#
# The rule to create the target directory.
#
${COMPILER}:
	@mkdir -p ${COMPILER}

#
# Rules for building the project example.
#
${COMPILER}/gameport.axf: ${COMPILER}/input.o
${COMPILER}/gameport.axf: ${COMPILER}/leds.o
${COMPILER}/gameport.axf: ${COMPILER}/main.o
${COMPILER}/gameport.axf: ${COMPILER}/usb.o
${COMPILER}/gameport.axf: ${COMPILER}/startup_${COMPILER}.o
${COMPILER}/gameport.axf: ${ROOT}/usblib/${COMPILER}/libusb.a
${COMPILER}/gameport.axf: ${ROOT}/driverlib/${COMPILER}/libdriver.a
${COMPILER}/gameport.axf: project.ld
SCATTERgcc_gameport=project.ld
ENTRY_gameport=ResetISR
CFLAGSgcc=-DTARGET_IS_TM4C123_RB1 -include stdint.h -include stdbool.h -include stdlib.h

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
