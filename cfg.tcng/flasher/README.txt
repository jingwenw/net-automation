/*
 *     Copyright Tantalus(c) 2017
 *     All rights reserved.
 *
 */

Description:
============

  This tool package is used to flash the TCNG dev_board with the 32mb image. And it contains the followings:
  - README.txt
  - running tool: myex.pl, and you can get helps by "./myex.pl --help"
  - tcng flashing configuration files
  - libraries: sam-ba tool, Phidget APIs, perl libs

Prerequisites:
==============

  - The PC running Ubuntu is connecting to the TCNG board with a USB cable
  - The following packages/libs required to run this tool:libxssl, build-essential.
  - The TCNG board must be in RomBOOT prompt before flashing, and this can
    be confirmed by the bash cmd "ls /dev/ttyACM0": if it there, the board
    is in RomBOOT prompt and ready for flashing.
  - This tool is developed and tested on Ubuntu 16.04 server
  - The user must have been a member of sudoers and has the permissions to
    access the USD device /dev/tty*, which means it must be member of
    dialout/plugdev etc.

Use Cases:
=========

  - To backup the original image from the board flash:

      ./myex.pl --config cfg.tcng/tcng_backup_devboard.cfg

    The backup file will be /tmp/tcng_flash_image_backup.bin with size of 32mb

  - To backup the original image from the board then flash it with a new image with NO manufacture data overwritten:

      export FLASH_IMAGE_PATH=<the_image_full_path>; ./myex.pl --config cfg.tcng/tcng_backup_and_flash_except_mfg.cfg

    The backup file will be /tmp/tcng_flash_image_backup.bin with size of 32mb, and the following 2 address sections of the image will be written into the flash:
      . 0x0 - 0x7fff ( 32kb )
      . 0xc000 - 0x1ffffff ( 32mb - 48kb )

  - To flash the TCNG board with a new image with NO manufacture overwritten:

      export FLASH_IMAGE_PATH=<the_image_full_path>; ./myex.pl --config cfg.tcng/tcng_flash_devboard_except_mfg.cfg

    The following 2 address sections of the image will be written into the flash:
      . 0x0 - 0x7fff ( 32kb )
      . 0xc000 - 0x1ffffff (32mb - 48kb)

  - To fully flash the TCNG dev_board with an image passed as $FLASH_IMAGE_PATH:
      export FLASH_IMAGE_PATH=<the_image_full_path>; ./myex.pl --config cfg.tcng/tcng_flash_devboard_full.cfg

    The who 32mb image will be written into the board.

Steps to use this tool with the tarball:
========================================

  1. Bring up the board in RomBOOT prompt
  2. export FLASH_IMAGE_PATH=<the full path of the image to be flashed>
  3. cd /tmp; tar -zxvf tcng_flasher.tar.gz
  4. cd tcng_flasher/
  5. ./myex.pl --config cfg.tcng/tcng_backup_and_flash_except_mfg.cfg


