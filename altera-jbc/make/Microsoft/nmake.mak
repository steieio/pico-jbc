#
# Module:           makefile
#
#                                                                           
#                   Copyright 2025 Altera Corporation                       
#                                                                           
# SPDX-License-Identifier: MIT-0                                            
#                                                                           
# Permission is hereby granted, free of charge, to any person obtaining a   
# copy of this software and associated documentation files (the             
# "Software"),to deal in the Software without restriction, including        
# without limitation the rights to use, copy, modify, merge, publish,       
# distribute, sublicense, and/or sell copies of the Software, and to permit 
# persons to whom the Software is furnished to do so.                       
#                                                                           
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS   
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF                
# MERCHANTABILITY,  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY      
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT 
# OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR  
# THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                
#                                                                           
# Description:      Makefile for JAM Bytecode Player
#
#

OBJS = \
	jbistub.obj \
	jbimain.obj \
	jbicomp.obj \
	jbijtag.obj


!IF "$(MEM_TRACKER)" != ""
!IF "$(STATIC_MEMORY_SIZE)" != ""
# MEMORY TRACKER ON, USE 'STATIC_MEMORY_SIZE' KB OF STATIC MEMORY
.c.obj::
	cl /W4 /c /O2 /ML /DWINNT /DMEM_TRACKER /DUSE_STATIC_MEMORY=$(STATIC_MEMORY_SIZE) $< 
!ELSE
# MEMORY TRACKER ON, USE DYNAMIC MEMORY
.c.obj::
	cl /W4 /c /O2 /ML /DWINNT /DMEM_TRACKER $< 
!ENDIF
!ELSE
!IF "$(STATIC_MEMORY_SIZE)" != ""
# MEMORY TRACKER OFF, USE 'STATIC_MEMORY_SIZE' KB OF STATIC MEMORY
.c.obj::
	cl /W4 /c /O2 /ML /DWINNT /DUSE_STATIC_MEMORY=$(STATIC_MEMORY_SIZE) $< 
!ELSE
# MEMORY TRACKER OFF, USE DYNAMIC MEMORY
.c.obj::
	cl /W4 /c /O2 /ML /DWINNT $< 
!ENDIF
!ENDIF

jbi.exe : $(OBJS)
	link $(OBJS) advapi32.lib /out:jbi.exe

# Dependencies:

jbistub.obj : \
	jbistub.c \
	jbiport.h \
	jbiexprt.h

jbimain.obj : \
	jbimain.c \
	jbiport.h \
	jbiexprt.h \
	jbijtag.h \
	jbicomp.h

jamcomp.obj : \
	jamcomp.c \
	jbiport.h \
	jbiexprt.h \
	jbicomp.h

jbijtag.obj : \
	jbijtag.c \
	jbiport.h \
	jbiexprt.h \
	jbijtag.h
