REM make CHIP=at91cap9 BOARD=at91cap9-tan MEMORY=sdram_samba DYN_TRACES=5 INST_PREFIX=..\lib clean all
REM copy /Y bin\isp-serialflash-at91cap9.bin ..\..\..\lib\AT91CAP9-DK\

REM if building for 16MB SDRAM use the first of these make lines, otherwise use the second one for 32MB
REM 16 MB SDRAM
make CHIP=at91cap9 BOARD=at91cap9-e1 MEMORY=sdram_samba DYN_TRACES=5 INST_PREFIX=..\lib clean all
REM 32 MB SDRAM
REM make CHIP=at91cap9 BOARD=at91cap9-e0 SDRAM_SIZE_32MB=1 MEMORY=sdram_samba DYN_TRACES=5 INST_PREFIX=..\lib clean all


rem make CHIP=at91cap9 BOARD=at91cap9-tan MEMORY=sdram_samba DYN_TRACES=5 INST_PREFIX=..\lib clean all