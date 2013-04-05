/*This file is prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief FLASHCDW driver for AVR32 UC3.
 *
 * AVR32 Flash Controller driver module.
 *
 * - Compiler:           IAR EWAVR32 and GNU GCC for AVR32
 * - Supported devices:  All AVR32 devices with a FLASHCDW module can be used.
 * - AppNote:
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 ******************************************************************************/

/* Copyright (c) 2009 Atmel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an Atmel
 * AVR product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 *
 */

#include <avr32/io.h>
#include <stddef.h>
#include "utils/compiler.h"
#include "flashcdw.h"


/*! \name FLASHCDW Writable Bit-Field Registers
 */
//! @{

typedef union
{
  unsigned long                   fcr;
  avr32_flashcdw_fcr_t            FCR;
} u_avr32_flashcdw_fcr_t;

typedef union
{
  unsigned long                   fcmd;
  avr32_flashcdw_fcmd_t           FCMD;
} u_avr32_flashcdw_fcmd_t;

//! @}


/*! \name Flash Properties
 */
//! @{


unsigned int flashcdw_get_flash_size(void)
{
  static const unsigned int FLASH_SIZE[1 << AVR32_FLASHCDW_FPR_FSZ_SIZE] =
  {
      4 << 10,
      8 << 10,
      16 << 10,
      32 << 10,
      48 << 10,
      64 << 10,
      96 << 10,
     128 << 10,
     192 << 10,
     256 << 10,
     384 << 10,
     512 << 10,
     768 << 10,
    1024 << 10,
    2048 << 10
  };
  return FLASH_SIZE[(AVR32_FLASHCDW.fpr & AVR32_FLASHCDW_FPR_FSZ_MASK) >> AVR32_FLASHCDW_FPR_FSZ_OFFSET];
}


unsigned int flashcdw_get_page_count(void)
{
  return flashcdw_get_flash_size() / AVR32_FLASHCDW_PAGE_SIZE;
}


unsigned int flashcdw_get_page_count_per_region(void)
{
  return flashcdw_get_page_count() / AVR32_FLASHCDW_REGIONS;
}


unsigned int flashcdw_get_page_region(int page_number)
{
  return ((page_number >= 0) ? page_number : flashcdw_get_page_number()) / flashcdw_get_page_count_per_region();
}


unsigned int flashcdw_get_region_first_page_number(unsigned int region)
{
  return region * flashcdw_get_page_count_per_region();
}


//! @}


/*! \name FLASHC Control
 */
//! @{


unsigned int flashcdw_get_wait_state(void)
{
  return (AVR32_FLASHCDW.fcr & AVR32_FLASHCDW_FCR_FWS_MASK) >> AVR32_FLASHCDW_FCR_FWS_OFFSET;
}


void flashcdw_set_wait_state(unsigned int wait_state)
{
  u_avr32_flashcdw_fcr_t u_avr32_flashcdw_fcr = {AVR32_FLASHCDW.fcr};
  u_avr32_flashcdw_fcr.FCR.fws = wait_state;
  AVR32_FLASHCDW.fcr = u_avr32_flashcdw_fcr.fcr;
}


void flashcdw_set_flash_waitstate_and_readmode(unsigned long cpu_f_hz)
{
  if(cpu_f_hz > AVR32_FLASHCDW_FWS_0_MAX_FREQ)    // > 15MHz
  {
    if(cpu_f_hz <= AVR32_FLASHCDW_FWS_1_MAX_FREQ) // <= 30MHz
    {
      // Set a wait-state, disable the high-speed read mode.
      flashcdw_set_wait_state(1);
      flashcdw_issue_command(AVR32_FLASHCDW_FCMD_CMD_HSDIS, -1);
    }
    else
    {
      // Set a wait-state, enable the high-speed read mode.
      flashcdw_set_wait_state(1);
      flashcdw_issue_command(AVR32_FLASHCDW_FCMD_CMD_HSEN, -1);
    }
  }
  else  // <= 15MHz
  {
    // No wait-state, disable the high-speed read mode
    flashcdw_set_wait_state(0);
    flashcdw_issue_command(AVR32_FLASHCDW_FCMD_CMD_HSDIS, -1);
  }
}


Bool flashcdw_is_ready_int_enabled(void)
{
  return ((AVR32_FLASHCDW.fcr & AVR32_FLASHCDW_FCR_FRDY_MASK) != 0);
}


void flashcdw_enable_ready_int(Bool enable)
{
  u_avr32_flashcdw_fcr_t u_avr32_flashcdw_fcr = {AVR32_FLASHCDW.fcr};
  u_avr32_flashcdw_fcr.FCR.frdy = (enable != false);
  AVR32_FLASHCDW.fcr = u_avr32_flashcdw_fcr.fcr;
}


Bool flashcdw_is_lock_error_int_enabled(void)
{
  return ((AVR32_FLASHCDW.fcr & AVR32_FLASHCDW_FCR_LOCKE_MASK) != 0);
}


void flashcdw_enable_lock_error_int(Bool enable)
{
  u_avr32_flashcdw_fcr_t u_avr32_flashcdw_fcr = {AVR32_FLASHCDW.fcr};
  u_avr32_flashcdw_fcr.FCR.locke = (enable != false);
  AVR32_FLASHCDW.fcr = u_avr32_flashcdw_fcr.fcr;
}


Bool flashcdw_is_prog_error_int_enabled(void)
{
  return ((AVR32_FLASHCDW.fcr & AVR32_FLASHCDW_FCR_PROGE_MASK) != 0);
}


void flashcdw_enable_prog_error_int(Bool enable)
{
  u_avr32_flashcdw_fcr_t u_avr32_flashcdw_fcr = {AVR32_FLASHCDW.fcr};
  u_avr32_flashcdw_fcr.FCR.proge = (enable != false);
  AVR32_FLASHCDW.fcr = u_avr32_flashcdw_fcr.fcr;
}


//! @}


/*! \name FLASHC Status
 */
//! @{


Bool flashcdw_is_ready(void)
{
  return ((AVR32_FLASHCDW.fsr & AVR32_FLASHCDW_FSR_FRDY_MASK) != 0);
}


void flashcdw_default_wait_until_ready(void)
{
  while (!flashcdw_is_ready());
}


void (*volatile flashcdw_wait_until_ready)(void) = flashcdw_default_wait_until_ready;


/*! \brief Gets the error status of the FLASHCDW.
 *
 * \return The error status of the FLASHCDW built up from
 *         \c AVR32_FLASHCDW_FSR_LOCKE_MASK and \c AVR32_FLASHCDW_FSR_PROGE_MASK.
 *
 * \warning This hardware error status is cleared by all functions reading the
 *          Flash Status Register (FSR). This function is therefore not part of
 *          the driver's API which instead presents \ref flashcdw_is_lock_error
 *          and \ref flashcdw_is_programming_error.
 */
static unsigned int flashcdw_get_error_status(void)
{
  return AVR32_FLASHCDW.fsr & (AVR32_FLASHCDW_FSR_LOCKE_MASK |
                             AVR32_FLASHCDW_FSR_PROGE_MASK);
}


//! Sticky error status of the FLASHCDW.
//! This variable is updated by functions that issue FLASHCDW commands. It
//! contains the cumulated FLASHCDW error status of all the FLASHCDW commands issued
//! by a function.
static unsigned int flashcdw_error_status = 0;


Bool flashcdw_is_lock_error(void)
{
  return ((flashcdw_error_status & AVR32_FLASHCDW_FSR_LOCKE_MASK) != 0);
}


Bool flashcdw_is_programming_error(void)
{
  return ((flashcdw_error_status & AVR32_FLASHCDW_FSR_PROGE_MASK) != 0);
}


Bool flashcdw_is_high_speed_enabled(void)
{
  return ((AVR32_FLASHCDW.fsr & AVR32_FLASHCDW_FSR_HSMODE_MASK) != 0);
}


//! @}


/*! \name FLASHCDW Command Control
 */
//! @{


unsigned int flashcdw_get_command(void)
{
  return (AVR32_FLASHCDW.fcmd & AVR32_FLASHCDW_FCMD_CMD_MASK) >> AVR32_FLASHCDW_FCMD_CMD_OFFSET;
}


unsigned int flashcdw_get_page_number(void)
{
  return (AVR32_FLASHCDW.fcmd & AVR32_FLASHCDW_FCMD_PAGEN_MASK) >> AVR32_FLASHCDW_FCMD_PAGEN_OFFSET;
}


void flashcdw_issue_command(unsigned int command, int page_number)
{
  u_avr32_flashcdw_fcmd_t u_avr32_flashcdw_fcmd;
  flashcdw_wait_until_ready();
  u_avr32_flashcdw_fcmd.fcmd = AVR32_FLASHCDW.fcmd;
  u_avr32_flashcdw_fcmd.FCMD.cmd = command;
  if (page_number >= 0) u_avr32_flashcdw_fcmd.FCMD.pagen = page_number;
  u_avr32_flashcdw_fcmd.FCMD.key = AVR32_FLASHCDW_FCMD_KEY_KEY;
  AVR32_FLASHCDW.fcmd = u_avr32_flashcdw_fcmd.fcmd;
  flashcdw_error_status = flashcdw_get_error_status();
  flashcdw_wait_until_ready();
}


//! @}


/*! \name FLASHCDW Global Commands
 */
//! @{


void flashcdw_no_operation(void)
{
  flashcdw_issue_command(AVR32_FLASHCDW_FCMD_CMD_NOP, -1);
}


void flashcdw_erase_all(void)
{
  flashcdw_issue_command(AVR32_FLASHCDW_FCMD_CMD_EA, -1);
}


//! @}


/*! \name FLASHCDW Protection Mechanisms
 */
//! @{


Bool flashcdw_is_security_bit_active(void)
{
  return ((AVR32_FLASHCDW.fsr & AVR32_FLASHCDW_FSR_SECURITY_MASK) != 0);
}


void flashcdw_activate_security_bit(void)
{
  flashcdw_issue_command(AVR32_FLASHCDW_FCMD_CMD_SSB, -1);
}


unsigned int flashcdw_get_bootloader_protected_size(void)
{
  unsigned int bootprot = (1 << AVR32_FLASHCDW_FGPFRLO_BOOTPROT_SIZE) - 1 -
                          flashcdw_read_gp_fuse_bitfield(AVR32_FLASHCDW_FGPFRLO_BOOTPROT_OFFSET,
                                                       AVR32_FLASHCDW_FGPFRLO_BOOTPROT_SIZE);
  return (bootprot) ? AVR32_FLASHCDW_PAGE_SIZE << bootprot : 0;
}


unsigned int flashcdw_set_bootloader_protected_size(unsigned int bootprot_size)
{
  flashcdw_set_gp_fuse_bitfield(AVR32_FLASHCDW_FGPFRLO_BOOTPROT_OFFSET,
                              AVR32_FLASHCDW_FGPFRLO_BOOTPROT_SIZE,
                              (1 << AVR32_FLASHCDW_FGPFRLO_BOOTPROT_SIZE) - 1 -
                              ((bootprot_size) ?
                               32 - clz((((min(max(bootprot_size, AVR32_FLASHCDW_PAGE_SIZE << 1),
                                               AVR32_FLASHCDW_PAGE_SIZE <<
                                               ((1 << AVR32_FLASHCDW_FGPFRLO_BOOTPROT_SIZE) - 1)) +
                                           AVR32_FLASHCDW_PAGE_SIZE - 1) /
                                          AVR32_FLASHCDW_PAGE_SIZE) << 1) - 1) - 1 :
                               0));
  return flashcdw_get_bootloader_protected_size();
}


Bool flashcdw_is_external_privileged_fetch_locked(void)
{
  return (!flashcdw_read_gp_fuse_bit(AVR32_FLASHCDW_FGPFRLO_EPFL_OFFSET));
}


void flashcdw_lock_external_privileged_fetch(Bool lock)
{
  flashcdw_set_gp_fuse_bit(AVR32_FLASHCDW_FGPFRLO_EPFL_OFFSET, !lock);
}

Bool flashcdw_is_jtag_user_protection_enabled(void)
{
  return (!flashcdw_read_gp_fuse_bit(AVR32_FLASHCDW_FGPFRLO_UPROT_OFFSET));
}

void flashcdw_enable_jtag_user_protection(void)
{
  flashcdw_set_gp_fuse_bit(AVR32_FLASHCDW_FGPFRLO_UPROT_OFFSET, false);
}

void flashcdw_disable_jtag_user_protection(void)
{
  flashcdw_set_gp_fuse_bit(AVR32_FLASHCDW_FGPFRLO_UPROT_OFFSET, true);
}

Bool flashcdw_is_secure_state_enabled(void)
{
  U64 temp = flashcdw_read_gp_fuse_bitfield(AVR32_FLASHCDW_FGPFRLO_SECURE_OFFSET,
                                            AVR32_FLASHCDW_FGPFRLO_SECURE_SIZE);
  if((AVR32_FLASHCDW_FGPFRLO_SECURE_SSEN_SSDDIS == temp) || (AVR32_FLASHCDW_FGPFRLO_SECURE_SSEN_SSDEN == temp))
    return true;
  else
    return false;
}

Bool flashcdw_is_secure_state_debug_enabled(void)
{
  return( AVR32_FLASHCDW_FGPFRLO_SECURE_SSEN_SSDEN
          == flashcdw_read_gp_fuse_bitfield(AVR32_FLASHCDW_FGPFRLO_SECURE_OFFSET,
                                            AVR32_FLASHCDW_FGPFRLO_SECURE_SIZE) );
}

void flashcdw_enable_secure_state_no_debug(void)
{
  flashcdw_write_gp_fuse_bitfield(AVR32_FLASHCDW_FGPFRLO_SECURE_OFFSET,
                                  AVR32_FLASHCDW_FGPFRLO_SECURE_SIZE,
                                  AVR32_FLASHCDW_FGPFRLO_SECURE_SSEN_SSDDIS);
}

void flashcdw_enable_secure_state_with_debug(void)
{
  flashcdw_write_gp_fuse_bitfield(AVR32_FLASHCDW_FGPFRLO_SECURE_OFFSET,
                                  AVR32_FLASHCDW_FGPFRLO_SECURE_SIZE,
                                  AVR32_FLASHCDW_FGPFRLO_SECURE_SSEN_SSDEN);
}

Bool flashcdw_is_page_region_locked(int page_number)
{
  return flashcdw_is_region_locked(flashcdw_get_page_region(page_number));
}


Bool flashcdw_is_region_locked(unsigned int region)
{
  return ((AVR32_FLASHCDW.fsr & AVR32_FLASHCDW_FSR_LOCK0_MASK << (region & (AVR32_FLASHCDW_REGIONS - 1))) != 0);
}


void flashcdw_lock_page_region(int page_number, Bool lock)
{
  flashcdw_issue_command((lock) ? AVR32_FLASHCDW_FCMD_CMD_LP : AVR32_FLASHCDW_FCMD_CMD_UP, page_number);
}


void flashcdw_lock_region(unsigned int region, Bool lock)
{
  flashcdw_lock_page_region(flashcdw_get_region_first_page_number(region), lock);
}


void flashcdw_lock_all_regions(Bool lock)
{
  unsigned int error_status = 0;
  unsigned int region = AVR32_FLASHCDW_REGIONS;
  while (region)
  {
    flashcdw_lock_region(--region, lock);
    error_status |= flashcdw_error_status;
  }
  flashcdw_error_status = error_status;
}


//! @}


/*! \name Access to General-Purpose Fuses
 */
//! @{


Bool flashcdw_read_gp_fuse_bit(unsigned int gp_fuse_bit)
{
  return ((flashcdw_read_all_gp_fuses() & 1ULL << (gp_fuse_bit & 0x3F)) != 0);
}


U64 flashcdw_read_gp_fuse_bitfield(unsigned int pos, unsigned int width)
{
  return flashcdw_read_all_gp_fuses() >> (pos & 0x3F) & ((1ULL << min(width, 64)) - 1);
}


U8 flashcdw_read_gp_fuse_byte(unsigned int gp_fuse_byte)
{
  return flashcdw_read_all_gp_fuses() >> ((gp_fuse_byte & 0x07) << 3);
}


U64 flashcdw_read_all_gp_fuses(void)
{
  return AVR32_FLASHCDW.fgpfrlo | (U64)AVR32_FLASHCDW.fgpfrhi << 32;
}


Bool flashcdw_erase_gp_fuse_bit(unsigned int gp_fuse_bit, Bool check)
{
  flashcdw_issue_command(AVR32_FLASHCDW_FCMD_CMD_EGPB, gp_fuse_bit & 0x3F);
  return (check) ? flashcdw_read_gp_fuse_bit(gp_fuse_bit) : true;
}


Bool flashcdw_erase_gp_fuse_bitfield(unsigned int pos, unsigned int width, Bool check)
{
  unsigned int error_status = 0;
  unsigned int gp_fuse_bit;
  pos &= 0x3F;
  width = min(width, 64);
  for (gp_fuse_bit = pos; gp_fuse_bit < pos + width; gp_fuse_bit++)
  {
    flashcdw_erase_gp_fuse_bit(gp_fuse_bit, false);
    error_status |= flashcdw_error_status;
  }
  flashcdw_error_status = error_status;
  return (check) ? (flashcdw_read_gp_fuse_bitfield(pos, width) == (1ULL << width) - 1) : true;
}


Bool flashcdw_erase_gp_fuse_byte(unsigned int gp_fuse_byte, Bool check)
{
  unsigned int error_status;
  unsigned int current_gp_fuse_byte;
  U64 value = flashcdw_read_all_gp_fuses();
  flashcdw_erase_all_gp_fuses(false);
  error_status = flashcdw_error_status;
  for (current_gp_fuse_byte = 0; current_gp_fuse_byte < 8; current_gp_fuse_byte++, value >>= 8)
  {
    if (current_gp_fuse_byte != gp_fuse_byte)
    {
      flashcdw_write_gp_fuse_byte(current_gp_fuse_byte, value);
      error_status |= flashcdw_error_status;
    }
  }
  flashcdw_error_status = error_status;
  return (check) ? (flashcdw_read_gp_fuse_byte(gp_fuse_byte) == 0xFF) : true;
}


Bool flashcdw_erase_all_gp_fuses(Bool check)
{
  flashcdw_issue_command(AVR32_FLASHCDW_FCMD_CMD_EAGPF, -1);
  return (check) ? (flashcdw_read_all_gp_fuses() == 0xFFFFFFFFFFFFFFFFULL) : true;
}


void flashcdw_write_gp_fuse_bit(unsigned int gp_fuse_bit, Bool value)
{
  if (!value)
    flashcdw_issue_command(AVR32_FLASHCDW_FCMD_CMD_WGPB, gp_fuse_bit & 0x3F);
}


void flashcdw_write_gp_fuse_bitfield(unsigned int pos, unsigned int width, U64 value)
{
  unsigned int error_status = 0;
  unsigned int gp_fuse_bit;
  pos &= 0x3F;
  width = min(width, 64);
  for (gp_fuse_bit = pos; gp_fuse_bit < pos + width; gp_fuse_bit++, value >>= 1)
  {
    flashcdw_write_gp_fuse_bit(gp_fuse_bit, value & 0x01);
    error_status |= flashcdw_error_status;
  }
  flashcdw_error_status = error_status;
}


void flashcdw_write_gp_fuse_byte(unsigned int gp_fuse_byte, U8 value)
{
  flashcdw_issue_command(AVR32_FLASHCDW_FCMD_CMD_PGPFB, (gp_fuse_byte & 0x07) | value << 3);
}


void flashcdw_write_all_gp_fuses(U64 value)
{
  unsigned int error_status = 0;
  unsigned int gp_fuse_byte;
  for (gp_fuse_byte = 0; gp_fuse_byte < 8; gp_fuse_byte++, value >>= 8)
  {
    flashcdw_write_gp_fuse_byte(gp_fuse_byte, value);
    error_status |= flashcdw_error_status;
  }
  flashcdw_error_status = error_status;
}


void flashcdw_set_gp_fuse_bit(unsigned int gp_fuse_bit, Bool value)
{
  if (value)
    flashcdw_erase_gp_fuse_bit(gp_fuse_bit, false);
  else
    flashcdw_write_gp_fuse_bit(gp_fuse_bit, false);
}


void flashcdw_set_gp_fuse_bitfield(unsigned int pos, unsigned int width, U64 value)
{
  unsigned int error_status = 0;
  unsigned int gp_fuse_bit;
  pos &= 0x3F;
  width = min(width, 64);
  for (gp_fuse_bit = pos; gp_fuse_bit < pos + width; gp_fuse_bit++, value >>= 1)
  {
    flashcdw_set_gp_fuse_bit(gp_fuse_bit, value & 0x01);
    error_status |= flashcdw_error_status;
  }
  flashcdw_error_status = error_status;
}


void flashcdw_set_gp_fuse_byte(unsigned int gp_fuse_byte, U8 value)
{
  unsigned int error_status;
  switch (value)
  {
  case 0xFF:
    flashcdw_erase_gp_fuse_byte(gp_fuse_byte, false);
    break;
  case 0x00:
    flashcdw_write_gp_fuse_byte(gp_fuse_byte, 0x00);
    break;
  default:
    flashcdw_erase_gp_fuse_byte(gp_fuse_byte, false);
    error_status = flashcdw_error_status;
    flashcdw_write_gp_fuse_byte(gp_fuse_byte, value);
    flashcdw_error_status |= error_status;
  }
}


void flashcdw_set_all_gp_fuses(U64 value)
{
  unsigned int error_status;
  switch (value)
  {
  case 0xFFFFFFFFFFFFFFFFULL:
    flashcdw_erase_all_gp_fuses(false);
    break;
  case 0x0000000000000000ULL:
    flashcdw_write_all_gp_fuses(0x0000000000000000ULL);
    break;
  default:
    flashcdw_erase_all_gp_fuses(false);
    error_status = flashcdw_error_status;
    flashcdw_write_all_gp_fuses(value);
    flashcdw_error_status |= error_status;
  }
}


//! @}


/*! \name Access to Flash Pages
 */
//! @{


void flashcdw_clear_page_buffer(void)
{
  flashcdw_issue_command(AVR32_FLASHCDW_FCMD_CMD_CPB, -1);
}


Bool flashcdw_is_page_erased(void)
{
  return ((AVR32_FLASHCDW.fsr & AVR32_FLASHCDW_FSR_QPRR_MASK) != 0);
}


Bool flashcdw_quick_page_read(int page_number)
{
  flashcdw_issue_command(AVR32_FLASHCDW_FCMD_CMD_QPR, page_number);
  return flashcdw_is_page_erased();
}


Bool flashcdw_erase_page(int page_number, Bool check)
{
  Bool page_erased = true;
  flashcdw_issue_command(AVR32_FLASHCDW_FCMD_CMD_EP, page_number);
  if (check)
  {
    unsigned int error_status = flashcdw_error_status;
    page_erased = flashcdw_quick_page_read(-1);
    flashcdw_error_status |= error_status;
  }
  return page_erased;
}


Bool flashcdw_erase_all_pages(Bool check)
{
  Bool all_pages_erased = true;
  unsigned int error_status = 0;
  unsigned int page_number = flashcdw_get_page_count();
  while (page_number)
  {
    all_pages_erased &= flashcdw_erase_page(--page_number, check);
    error_status |= flashcdw_error_status;
  }
  flashcdw_error_status = error_status;
  return all_pages_erased;
}


void flashcdw_write_page(int page_number)
{
  flashcdw_issue_command(AVR32_FLASHCDW_FCMD_CMD_WP, page_number);
}


Bool flashcdw_quick_user_page_read(void)
{
  flashcdw_issue_command(AVR32_FLASHCDW_FCMD_CMD_QPRUP, -1);
  return flashcdw_is_page_erased();
}


Bool flashcdw_erase_user_page(Bool check)
{
  flashcdw_issue_command(AVR32_FLASHCDW_FCMD_CMD_EUP, -1);
  return (check) ? flashcdw_quick_user_page_read() : true;
}


void flashcdw_write_user_page(void)
{
  flashcdw_issue_command(AVR32_FLASHCDW_FCMD_CMD_WUP, -1);
}


volatile void *flashcdw_memset8(volatile void *dst, U8 src, size_t nbytes, Bool erase)
{
  return flashcdw_memset16(dst, src | (U16)src << 8, nbytes, erase);
}


volatile void *flashcdw_memset16(volatile void *dst, U16 src, size_t nbytes, Bool erase)
{
  return flashcdw_memset32(dst, src | (U32)src << 16, nbytes, erase);
}


volatile void *flashcdw_memset32(volatile void *dst, U32 src, size_t nbytes, Bool erase)
{
  return flashcdw_memset64(dst, src | (U64)src << 32, nbytes, erase);
}


volatile void *flashcdw_memset64(volatile void *dst, U64 src, size_t nbytes, Bool erase)
{
  // Use aggregated pointers to have several alignments available for a same address.
  UnionCVPtr flash_array_end;
  UnionVPtr dest;
  Union64 source = {0};
  StructCVPtr dest_end;
  UnionCVPtr flash_page_source_end;
  Bool incomplete_flash_page_end;
  Union64 flash_dword;
  UnionVPtr tmp;
  unsigned int error_status = 0;
  unsigned int i;

  // Reformat arguments.
  flash_array_end.u8ptr = AVR32_FLASH + flashcdw_get_flash_size();
  dest.u8ptr = dst;
  for (i = (Get_align((U32)dest.u8ptr, sizeof(U64)) - 1) & (sizeof(U64) - 1);
       src; i = (i - 1) & (sizeof(U64) - 1))
  {
    source.u8[i] = src;
    src >>= 8;
  }
  dest_end.u8ptr = dest.u8ptr + nbytes;

  // If destination is outside flash, go to next flash page if any.
  if (dest.u8ptr < AVR32_FLASH)
  {
    dest.u8ptr = AVR32_FLASH;
  }
  else if (flash_array_end.u8ptr <= dest.u8ptr && dest.u8ptr < AVR32_FLASHCDW_USER_PAGE)
  {
    dest.u8ptr = AVR32_FLASHCDW_USER_PAGE;
  }

  // If end of destination is outside flash, move it to the end of the previous flash page if any.
  if (dest_end.u8ptr > AVR32_FLASHCDW_USER_PAGE + AVR32_FLASHCDW_USER_PAGE_SIZE)
  {
    dest_end.u8ptr = AVR32_FLASHCDW_USER_PAGE + AVR32_FLASHCDW_USER_PAGE_SIZE;
  }
  else if (AVR32_FLASHCDW_USER_PAGE >= dest_end.u8ptr && dest_end.u8ptr > flash_array_end.u8ptr)
  {
    dest_end.u8ptr = flash_array_end.u8ptr;
  }

  // Align each end of destination pointer with its natural boundary.
  dest_end.u16ptr = (U16 *)Align_down((U32)dest_end.u8ptr, sizeof(U16));
  dest_end.u32ptr = (U32 *)Align_down((U32)dest_end.u16ptr, sizeof(U32));
  dest_end.u64ptr = (U64 *)Align_down((U32)dest_end.u32ptr, sizeof(U64));

  // While end of destination is not reached...
  while (dest.u8ptr < dest_end.u8ptr)
  {
    // Clear the page buffer in order to prepare data for a flash page write.
    flashcdw_clear_page_buffer();
    error_status |= flashcdw_error_status;

    // Determine where the source data will end in the current flash page.
    flash_page_source_end.u64ptr =
      (U64 *)min((U32)dest_end.u64ptr,
                 Align_down((U32)dest.u8ptr, AVR32_FLASHCDW_PAGE_SIZE) + AVR32_FLASHCDW_PAGE_SIZE);

    // Determine if the current destination page has an incomplete end.
    incomplete_flash_page_end = (Align_down((U32)dest.u8ptr, AVR32_FLASHCDW_PAGE_SIZE) >=
                                 Align_down((U32)dest_end.u8ptr, AVR32_FLASHCDW_PAGE_SIZE));

    // Use a flash double-word buffer to manage unaligned accesses.
    flash_dword.u64 = source.u64;

    // If destination does not point to the beginning of the current flash page...
    if (!Test_align((U32)dest.u8ptr, AVR32_FLASHCDW_PAGE_SIZE))
    {
      // Fill the beginning of the page buffer with the current flash page data.
      // This is required by the hardware, even if page erase is not requested,
      // in order to be able to write successfully to erased parts of flash
      // pages that have already been written to.
      for (tmp.u8ptr = (U8 *)Align_down((U32)dest.u8ptr, AVR32_FLASHCDW_PAGE_SIZE);
           tmp.u64ptr < (U64 *)Align_down((U32)dest.u8ptr, sizeof(U64));
           tmp.u64ptr++)
      {
        *tmp.u32ptr = *tmp.u32ptr;
        *(tmp.u32ptr+1) = *(tmp.u32ptr+1);
      }

      // If destination is not 64-bit aligned...
      if (!Test_align((U32)dest.u8ptr, sizeof(U64)))
      {
        // Fill the beginning of the flash double-word buffer with the current
        // flash page data.
        // This is required by the hardware, even if page erase is not
        // requested, in order to be able to write successfully to erased parts
        // of flash pages that have already been written to.
        for (i = 0; i < Get_align((U32)dest.u8ptr, sizeof(U64)); i++)
          flash_dword.u8[i] = *tmp.u8ptr++;

        // Align the destination pointer with its 64-bit boundary.
        dest.u64ptr = (U64 *)Align_down((U32)dest.u8ptr, sizeof(U64));

        // If the current destination double-word is not the last one...
        if (dest.u64ptr < dest_end.u64ptr)
        {
          // Write the flash double-word buffer to the page buffer and reinitialize it.
          *dest.u32ptr++ = flash_dword.u32[0];
          *dest.u32ptr++ = flash_dword.u32[1];
          flash_dword.u64 = source.u64;
        }
      }
    }

    // Write the source data to the page buffer with 64-bit alignment.
    for (i = flash_page_source_end.u64ptr - dest.u64ptr; i; i--)
    {
      *dest.u32ptr++ = source.u32[0];
      *dest.u32ptr++ = source.u32[1];
    }

    // If the current destination page has an incomplete end...
    if (incomplete_flash_page_end)
    {
      // This is required by the hardware, even if page erase is not requested,
      // in order to be able to write successfully to erased parts of flash
      // pages that have already been written to.
      {
        tmp.u8ptr = (volatile U8 *)dest_end.u8ptr;

        // If end of destination is not 64-bit aligned...
        if (!Test_align((U32)dest_end.u8ptr, sizeof(U64)))
        {
          // Fill the end of the flash double-word buffer with the current flash page data.
          for (i = Get_align((U32)dest_end.u8ptr, sizeof(U64)); i < sizeof(U64); i++)
            flash_dword.u8[i] = *tmp.u8ptr++;

          // Write the flash double-word buffer to the page buffer.
          *dest.u32ptr++ = flash_dword.u32[0];
          *dest.u32ptr++ = flash_dword.u32[1];
        }

        // Fill the end of the page buffer with the current flash page data.
        for (; !Test_align((U32)tmp.u64ptr, AVR32_FLASHCDW_PAGE_SIZE); tmp.u64ptr++)
        {
          *tmp.u32ptr = *tmp.u32ptr;
          *(tmp.u32ptr+1) = *(tmp.u32ptr+1);
        }
      }
    }

    // If the current flash page is in the flash array...
    if (dest.u8ptr <= AVR32_FLASHCDW_USER_PAGE)
    {
      // Erase the current page if requested and write it from the page buffer.
      if (erase)
      {
        flashcdw_erase_page(-1, false);
        error_status |= flashcdw_error_status;
      }
      flashcdw_write_page(-1);
      error_status |= flashcdw_error_status;

      // If the end of the flash array is reached, go to the User page.
      if (dest.u8ptr >= flash_array_end.u8ptr)
        dest.u8ptr = AVR32_FLASHCDW_USER_PAGE;
    }
    // If the current flash page is the User page...
    else
    {
      // Erase the User page if requested and write it from the page buffer.
      if (erase)
      {
        flashcdw_erase_user_page(false);
        error_status |= flashcdw_error_status;
      }
      flashcdw_write_user_page();
      error_status |= flashcdw_error_status;
    }
  }

  // Update the FLASHC error status.
  flashcdw_error_status = error_status;

  // Return the initial destination pointer as the standard memset function does.
  return dst;
}


volatile void *flashcdw_memcpy(volatile void *dst, const void *src, size_t nbytes, Bool erase)
{
  // Use aggregated pointers to have several alignments available for a same address.
  UnionCVPtr flash_array_end;
  UnionVPtr dest;
  UnionCPtr source;
  StructCVPtr dest_end;
  UnionCVPtr flash_page_source_end;
  Bool incomplete_flash_page_end;
  Union64 flash_dword;
  Bool flash_dword_pending = false;
  UnionVPtr tmp;
  unsigned int error_status = 0;
  unsigned int i, j;

  // Reformat arguments.
  flash_array_end.u8ptr = AVR32_FLASH + flashcdw_get_flash_size();
  dest.u8ptr = dst;
  source.u8ptr = src;
  dest_end.u8ptr = dest.u8ptr + nbytes;

  // If destination is outside flash, go to next flash page if any.
  if (dest.u8ptr < AVR32_FLASH)
  {
    source.u8ptr += AVR32_FLASH - dest.u8ptr;
    dest.u8ptr = AVR32_FLASH;
  }
  else if (flash_array_end.u8ptr <= dest.u8ptr && dest.u8ptr < AVR32_FLASHCDW_USER_PAGE)
  {
    source.u8ptr += (volatile uint8_t *)AVR32_FLASHCDW_USER_PAGE_ADDRESS - dest.u8ptr;
    dest.u8ptr = AVR32_FLASHCDW_USER_PAGE;
  }

  // If end of destination is outside flash, move it to the end of the previous flash page if any.
  if (dest_end.u8ptr > AVR32_FLASHCDW_USER_PAGE + AVR32_FLASHCDW_USER_PAGE_SIZE)
  {
    dest_end.u8ptr = AVR32_FLASHCDW_USER_PAGE + AVR32_FLASHCDW_USER_PAGE_SIZE;
  }
  else if (AVR32_FLASHCDW_USER_PAGE >= dest_end.u8ptr && dest_end.u8ptr > flash_array_end.u8ptr)
  {
    dest_end.u8ptr = flash_array_end.u8ptr;
  }

  // Align each end of destination pointer with its natural boundary.
  dest_end.u16ptr = (U16 *)Align_down((U32)dest_end.u8ptr, sizeof(U16));
  dest_end.u32ptr = (U32 *)Align_down((U32)dest_end.u16ptr, sizeof(U32));
  dest_end.u64ptr = (U64 *)Align_down((U32)dest_end.u32ptr, sizeof(U64));

  // While end of destination is not reached...
  while (dest.u8ptr < dest_end.u8ptr)
  {
    // Clear the page buffer in order to prepare data for a flash page write.
    flashcdw_clear_page_buffer();
    error_status |= flashcdw_error_status;

    // Determine where the source data will end in the current flash page.
    flash_page_source_end.u64ptr =
      (U64 *)min((U32)dest_end.u64ptr,
                 Align_down((U32)dest.u8ptr, AVR32_FLASHCDW_PAGE_SIZE) + AVR32_FLASHCDW_PAGE_SIZE);

    // Determine if the current destination page has an incomplete end.
    incomplete_flash_page_end = (Align_down((U32)dest.u8ptr, AVR32_FLASHCDW_PAGE_SIZE) >=
                                 Align_down((U32)dest_end.u8ptr, AVR32_FLASHCDW_PAGE_SIZE));

    // If destination does not point to the beginning of the current flash page...
    if (!Test_align((U32)dest.u8ptr, AVR32_FLASHCDW_PAGE_SIZE))
    {
      // Fill the beginning of the page buffer with the current flash page data.
      // This is required by the hardware, even if page erase is not requested,
      // in order to be able to write successfully to erased parts of flash
      // pages that have already been written to.
      for (tmp.u8ptr = (U8 *)Align_down((U32)dest.u8ptr, AVR32_FLASHCDW_PAGE_SIZE);
           tmp.u64ptr < (U64 *)Align_down((U32)dest.u8ptr, sizeof(U64));
           tmp.u64ptr++)
      {
        *tmp.u32ptr = *tmp.u32ptr;
        *(tmp.u32ptr+1) = *(tmp.u32ptr+1);
      }

      // If destination is not 64-bit aligned...
      if (!Test_align((U32)dest.u8ptr, sizeof(U64)))
      {
        // Fill the beginning of the flash double-word buffer with the current
        // flash page data.
        // This is required by the hardware, even if page erase is not
        // requested, in order to be able to write successfully to erased parts
        // of flash pages that have already been written to.
        for (i = 0; i < Get_align((U32)dest.u8ptr, sizeof(U64)); i++)
          flash_dword.u8[i] = *tmp.u8ptr++;

        // Fill the end of the flash double-word buffer with the source data.
        for (; i < sizeof(U64); i++)
          flash_dword.u8[i] = *source.u8ptr++;

        // Align the destination pointer with its 64-bit boundary.
        dest.u64ptr = (U64 *)Align_down((U32)dest.u8ptr, sizeof(U64));

        // If the current destination double-word is not the last one...
        if (dest.u64ptr < dest_end.u64ptr)
        {
          // Write the flash double-word buffer to the page buffer.
          *dest.u32ptr++ = flash_dword.u32[0];
          *dest.u32ptr++ = flash_dword.u32[1];
        }
        // If the current destination double-word is the last one, the flash
        // double-word buffer must be kept for later.
        else flash_dword_pending = true;
      }
    }

    // Read the source data with the maximal possible alignment and write it to
    // the page buffer with 64-bit alignment.
    switch (Get_align((U32)source.u8ptr, sizeof(U32)))
    {
    case 0:
      for (i = flash_page_source_end.u64ptr - dest.u64ptr; i; i--)
      {
        *dest.u32ptr++ = *source.u32ptr++;
        *dest.u32ptr++ = *source.u32ptr++;
      }
      break;

    case sizeof(U16):
      for (i = flash_page_source_end.u64ptr - dest.u64ptr; i; i--)
      {
        for (j = 0; j < sizeof(U64) / sizeof(U16); j++) flash_dword.u16[j] = *source.u16ptr++;
        *dest.u32ptr++ = flash_dword.u32[0];
        *dest.u32ptr++ = flash_dword.u32[1];
      }
      break;

    default:
      for (i = flash_page_source_end.u64ptr - dest.u64ptr; i; i--)
      {
        for (j = 0; j < sizeof(U64); j++) flash_dword.u8[j] = *source.u8ptr++;
        *dest.u32ptr++ = flash_dword.u32[0];
        *dest.u32ptr++ = flash_dword.u32[1];
      }
    }

    // If the current destination page has an incomplete end...
    if (incomplete_flash_page_end)
    {
      // If the flash double-word buffer is in use, do not initialize it.
      if (flash_dword_pending) i = Get_align((U32)dest_end.u8ptr, sizeof(U64));
      // If the flash double-word buffer is free...
      else
      {
        // Fill the beginning of the flash double-word buffer with the source data.
        for (i = 0; i < Get_align((U32)dest_end.u8ptr, sizeof(U64)); i++)
          flash_dword.u8[i] = *source.u8ptr++;
      }

      // This is required by the hardware, even if page erase is not requested,
      // in order to be able to write successfully to erased parts of flash
      // pages that have already been written to.
      {
        tmp.u8ptr = (volatile U8 *)dest_end.u8ptr;

        // If end of destination is not 64-bit aligned...
        if (!Test_align((U32)dest_end.u8ptr, sizeof(U64)))
        {
          // Fill the end of the flash double-word buffer with the current flash page data.
          for (; i < sizeof(U64); i++)
            flash_dword.u8[i] = *tmp.u8ptr++;

          // Write the flash double-word buffer to the page buffer.
          *dest.u32ptr++ = flash_dword.u32[0];
          *dest.u32ptr++ = flash_dword.u32[1];
        }

        // Fill the end of the page buffer with the current flash page data.
        for (; !Test_align((U32)tmp.u64ptr, AVR32_FLASHCDW_PAGE_SIZE); tmp.u64ptr++)
        {
          *tmp.u32ptr = *tmp.u32ptr;
          *(tmp.u32ptr+1) = *(tmp.u32ptr+1);
        }
      }
    }

    // If the current flash page is in the flash array...
    if (dest.u8ptr <= AVR32_FLASHCDW_USER_PAGE)
    {
      // Erase the current page if requested and write it from the page buffer.
      if (erase)
      {
        flashcdw_erase_page(-1, false);
        error_status |= flashcdw_error_status;
      }
      flashcdw_write_page(-1);
      error_status |= flashcdw_error_status;

      // If the end of the flash array is reached, go to the User page.
      if (dest.u8ptr >= flash_array_end.u8ptr)
      {
        source.u8ptr += (volatile uint8_t *)AVR32_FLASHCDW_USER_PAGE_ADDRESS - dest.u8ptr;
        dest.u8ptr = AVR32_FLASHCDW_USER_PAGE;
      }
    }
    // If the current flash page is the User page...
    else
    {
      // Erase the User page if requested and write it from the page buffer.
      if (erase)
      {
        flashcdw_erase_user_page(false);
        error_status |= flashcdw_error_status;
      }
      flashcdw_write_user_page();
      error_status |= flashcdw_error_status;
    }
  }

  // Update the FLASHC error status.
  flashcdw_error_status = error_status;

  // Return the initial destination pointer as the standard memcpy function does.
  return dst;
}

//! @}
