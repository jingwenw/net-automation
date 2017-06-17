# ----------------------------------------------------------------------------
#         ATMEL Microcontroller Software Support 
# ----------------------------------------------------------------------------
# Copyright (c) 2008, Atmel Corporation
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# - Redistributions of source code must retain the above copyright notice,
# this list of conditions and the disclaimer below.
#
# Atmel's name may not be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
# DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
# OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# ----------------------------------------------------------------------------

 
 
chdir("extram");
system("make CHIP=at91cap9 BOARD=at91cap9-dk MEMORY=sram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91cap9 BOARD=at91cap9-stk MEMORY=sram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9260 BOARD=at91sam9260-ek MEMORY=sram_samba DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9261 BOARD=at91sam9261-ek MEMORY=sram_samba DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9263 BOARD=at91sam9263-ek MEMORY=sram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9g20 BOARD=at91sam9g20-ek MEMORY=sram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9rl64 BOARD=at91sam9rl-ek MEMORY=sram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9xe128 BOARD=at91sam9xe-ek MEMORY=sram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9xe256 BOARD=at91sam9xe-ek MEMORY=sram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9xe512 BOARD=at91sam9xe-ek MEMORY=sram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7se32 BOARD=at91sam7se-ek MEMORY=sram DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7se256 BOARD=at91sam7se-ek MEMORY=sram DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7se512 BOARD=at91sam7se-ek MEMORY=sram DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make clean");


chdir("..\\nandflash");
system("make CHIP=at91cap9 BOARD=at91cap9-dk MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91cap9 BOARD=at91cap9-stk MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9260 BOARD=at91sam9260-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9261 BOARD=at91sam9261-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9263 BOARD=at91sam9263-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9g20 BOARD=at91sam9g20-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9rl64 BOARD=at91sam9rl-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9xe128 BOARD=at91sam9xe-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9xe256 BOARD=at91sam9xe-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9xe512 BOARD=at91sam9xe-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7se32 BOARD=at91sam7se-ek MEMORY=sdram DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7se256 BOARD=at91sam7se-ek MEMORY=sdram DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7se512 BOARD=at91sam7se-ek MEMORY=sdram DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make clean");


chdir("..\\norflash");
system("make CHIP=at91cap9 BOARD=at91cap9-dk MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9260 BOARD=at91sam9260-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9263 BOARD=at91sam9263-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9g20 BOARD=at91sam9g20-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9rl64 BOARD=at91sam9rl-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9xe128 BOARD=at91sam9xe-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9xe256 BOARD=at91sam9xe-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9xe512 BOARD=at91sam9xe-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7se32 BOARD=at91sam7se-ek MEMORY=sdram DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7se256 BOARD=at91sam7se-ek MEMORY=sdram DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7se512 BOARD=at91sam7se-ek MEMORY=sdram DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make clean");


chdir("..\\dataflash");
system("make CHIP=at91sam7a3 BOARD=at91sam7a3-ek MEMORY=sram DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91cap9 BOARD=at91cap9-dk MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91cap9 BOARD=at91cap9-stk MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9260 BOARD=at91sam9260-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9261 BOARD=at91sam9261-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9263 BOARD=at91sam9263-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9g20 BOARD=at91sam9g20-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9rl64 BOARD=at91sam9rl-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9xe128 BOARD=at91sam9xe-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9xe256 BOARD=at91sam9xe-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9xe512 BOARD=at91sam9xe-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7x128 BOARD=at91sam7x-ek MEMORY=sram DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7x256 BOARD=at91sam7x-ek MEMORY=sram DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7x512 BOARD=at91sam7x-ek MEMORY=sram DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7se32 BOARD=at91sam7se-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7se256 BOARD=at91sam7se-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7se512 BOARD=at91sam7se-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make clean");


chdir("..\\serialflash");
system("make CHIP=at91sam7a3 BOARD=at91sam7a3-ek MEMORY=sram DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9260 BOARD=at91sam9260-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9261 BOARD=at91sam9261-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9263 BOARD=at91sam9263-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9g20 BOARD=at91sam9g20-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9rl64 BOARD=at91sam9rl-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9xe128 BOARD=at91sam9xe-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9xe256 BOARD=at91sam9xe-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9xe512 BOARD=at91sam9xe-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7x128 BOARD=at91sam7x-ek MEMORY=sram DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7x256 BOARD=at91sam7x-ek MEMORY=sram DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7x512 BOARD=at91sam7x-ek MEMORY=sram DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7se32 BOARD=at91sam7se-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7se256 BOARD=at91sam7se-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7se512 BOARD=at91sam7se-ek MEMORY=sdram_samba DYN_TRACES=1 INST_PREFIX=$ARGV[0] clean all");
system("make clean");


chdir("..\\flash");
system("make CHIP=at91sam7a3 BOARD=at91sam7a3-ek MEMORY=sram DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7l64 BOARD=at91sam7l-ek MEMORY=sram_samba DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7l128 BOARD=at91sam7l-ek MEMORY=sram_samba DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7s32 BOARD=at91sam7s-ek MEMORY=sram DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7s321 BOARD=at91sam7s-ek MEMORY=sram DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7s64 BOARD=at91sam7s-ek MEMORY=sram DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7s128 BOARD=at91sam7s-ek MEMORY=sram DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7s256 BOARD=at91sam7s-ek MEMORY=sram DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7s512 BOARD=at91sam7s-ek MEMORY=sram DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7x128 BOARD=at91sam7x-ek MEMORY=sram DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7x256 BOARD=at91sam7x-ek MEMORY=sram DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7x512 BOARD=at91sam7x-ek MEMORY=sram DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7se32 BOARD=at91sam7se-ek MEMORY=sdram DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7se256 BOARD=at91sam7se-ek MEMORY=sram DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam7se512 BOARD=at91sam7se-ek MEMORY=sram DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9xe128 BOARD=at91sam9xe-ek MEMORY=sram_samba DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9xe256 BOARD=at91sam9xe-ek MEMORY=sram_samba DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make CHIP=at91sam9xe512 BOARD=at91sam9xe-ek MEMORY=sram_samba DYN_TRACES=0 INST_PREFIX=$ARGV[0] clean all");
system("make clean");

