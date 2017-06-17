file names for binaries in Tantalus-tcng-E1Dev16.tcl have to match names of the actual binaries.

rename isp-extram-at91cap9.bin        isp-extram-tcnge116mb.bin"
rename isp-serialflash-tcnge1.bin.cs0 isp-serialflash-tcnge116mb.bin.cs0
rename isp-serialflash-tcnge1.bin.cs1 isp-serialflash-tcnge116mb.bin.cs1

################################################################################
## SDRAM
################################################################################
...
set RAM::appletFileName   "$libPath(extLib)/Tantalus-tcng-E1Dev16/isp-extram-tcnge116mb.bin"

################################################################################
## SERIALFLASH - ChipSelect 0
################################################################################
...
set SERIALFLASH::appletFileName   "$libPath(extLib)/Tantalus-tcng-E1Dev16/isp-serialflash-tcnge116mb.bin.cs0

################################################################################
## SERIALFLASH - ChipSelect 1
################################################################################
...
set SERIALFLASH1::appletFileName   "$libPath(extLib)/Tantalus-tcng-E1Dev16/isp-serialflash-tcnge116mb.bin.cs1"
