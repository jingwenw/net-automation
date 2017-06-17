echo Making external ram for at91cap9-e1 board 
rem make CHIP=at91cap9 BOARD=at91cap9-e1 MEMORY=sram_samba SDRAM_SIZE_32MB=1 DYN_TRACES=1 INST_PREFIX=..\lib clean all
make CHIP=at91cap9 BOARD=at91cap9-e1 MEMORY=sram_samba DYN_TRACES=1 INST_PREFIX=..\lib clean all