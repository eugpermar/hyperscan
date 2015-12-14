/*
 * Copyright (c) 2015, Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of Intel Corporation nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** \file
 * \brief Rose data structures to do with role programs.
 */

#ifndef ROSE_ROSE_PROGRAM_H
#define ROSE_ROSE_PROGRAM_H

#include "rose_internal.h"
#include "ue2common.h"

/** \brief Minimum alignment for each instruction in memory. */
#define ROSE_INSTR_MIN_ALIGN 8U

/** \brief Role program instruction opcodes. */
enum RoseInstructionCode {
    ROSE_INSTR_ANCHORED_DELAY,    //!< Delay until after anchored matcher.
    ROSE_INSTR_CHECK_ONLY_EOD,    //!< Role matches only at EOD.
    ROSE_INSTR_CHECK_BOUNDS,      //!< Bounds on distance from offset 0.
    ROSE_INSTR_CHECK_NOT_HANDLED, //!< Test & set role in "handled".
    ROSE_INSTR_CHECK_LOOKAROUND,  //!< Lookaround check.
    ROSE_INSTR_CHECK_LEFTFIX,     //!< Leftfix must be in accept state.
    ROSE_INSTR_SOM_ADJUST,        //!< Set SOM from a distance to EOM.
    ROSE_INSTR_SOM_LEFTFIX,       //!< Acquire SOM from a leftfix engine.
    ROSE_INSTR_TRIGGER_INFIX,     //!< Trigger an infix engine.
    ROSE_INSTR_TRIGGER_SUFFIX,    //!< Trigger a suffix engine.
    ROSE_INSTR_REPORT,            //!< Fire an ordinary report.
    ROSE_INSTR_REPORT_CHAIN,      //!< Fire a chained report (MPV).
    ROSE_INSTR_REPORT_EOD,        //!< Fire a callback at EOD time.
    ROSE_INSTR_REPORT_SOM_INT,    //!< Manipulate SOM only.
    ROSE_INSTR_REPORT_SOM,        //!< Manipulate SOM and report.
    ROSE_INSTR_REPORT_SOM_KNOWN,  //!< Rose role knows its SOM offset.
    ROSE_INSTR_SET_STATE,         //!< Switch a state index on.
    ROSE_INSTR_SET_GROUPS,        //!< Set some literal group bits.
    ROSE_INSTR_END                //!< End of program.
};

struct ROSE_STRUCT_ANCHORED_DELAY {
    u8 code; //!< From enum RoseRoleInstructionCode.
    u8 depth; //!< Depth for this state.
    rose_group groups; //!< Bitmask.
    u32 done_jump; //!< Jump forward this many bytes if successful.
};

struct ROSE_STRUCT_CHECK_ONLY_EOD {
    u8 code; //!< From enum RoseRoleInstructionCode.
    u32 fail_jump; //!< Jump forward this many bytes on failure.
};

struct ROSE_STRUCT_CHECK_BOUNDS {
    u8 code; //!< From enum RoseRoleInstructionCode.
    u32 min_bound; //!< Min distance from zero.
    u32 max_bound; //!< Max distance from zero (or ROSE_BOUND_INF).
    u32 fail_jump; //!< Jump forward this many bytes on failure.
};

struct ROSE_STRUCT_CHECK_NOT_HANDLED {
    u8 code; //!< From enum RoseRoleInstructionCode.
    u32 key; //!< Key in the "handled_roles" fatbit in scratch.
    u32 fail_jump; //!< Jump forward this many bytes if we have seen key before.
};

struct ROSE_STRUCT_CHECK_LOOKAROUND {
    u8 code; //!< From enum RoseRoleInstructionCode.
    u32 index;
    u32 count;
    u32 fail_jump; //!< Jump forward this many bytes on failure.
};

struct ROSE_STRUCT_CHECK_LEFTFIX {
    u8 code; //!< From enum RoseRoleInstructionCode.
    u32 queue; //!< Queue of leftfix to check.
    u32 lag; //!< Lag of leftfix for this case.
    ReportID report; //!< ReportID of leftfix to check.
    u32 fail_jump; //!< Jump forward this many bytes on failure.
};

struct ROSE_STRUCT_SOM_ADJUST {
    u8 code; //!< From enum RoseRoleInstructionCode.
    u32 distance; //!< Distance to EOM.
};

struct ROSE_STRUCT_SOM_LEFTFIX {
    u8 code; //!< From enum RoseRoleInstructionCode.
    u32 queue; //!< Queue index of leftfix providing SOM.
    u32 lag; //!< Lag of leftfix for this case.
};

struct ROSE_STRUCT_TRIGGER_INFIX {
    u8 code; //!< From enum RoseRoleInstructionCode.
    u8 cancel; //!< Cancels previous top event.
    u32 queue; //!< Queue index of infix.
    u32 event; //!< Queue event, from MQE_*.
};

struct ROSE_STRUCT_TRIGGER_SUFFIX {
    u8 code; //!< From enum RoseRoleInstructionCode.
    u32 queue; //!< Queue index of suffix.
    u32 event; //!< Queue event, from MQE_*.
};

struct ROSE_STRUCT_REPORT {
    u8 code; //!< From enum RoseRoleInstructionCode.
    ReportID report;
};

struct ROSE_STRUCT_REPORT_CHAIN {
    u8 code; //!< From enum RoseRoleInstructionCode.
    ReportID report;
};

struct ROSE_STRUCT_REPORT_EOD {
    u8 code; //!< From enum RoseRoleInstructionCode.
    ReportID report;
};

struct ROSE_STRUCT_REPORT_SOM_INT {
    u8 code; //!< From enum RoseRoleInstructionCode.
    ReportID report;
};

struct ROSE_STRUCT_REPORT_SOM {
    u8 code; //!< From enum RoseRoleInstructionCode.
    ReportID report;
};

struct ROSE_STRUCT_REPORT_SOM_KNOWN {
    u8 code; //!< From enum RoseRoleInstructionCode.
    ReportID report;
};

struct ROSE_STRUCT_SET_STATE {
    u8 code; //!< From enum RoseRoleInstructionCode.
    u8 depth; //!< Depth for this state.
    u32 index; //!< State index in multibit.
};

struct ROSE_STRUCT_SET_GROUPS {
    u8 code; //!< From enum RoseRoleInstructionCode.
    rose_group groups; //!< Bitmask.
};

struct ROSE_STRUCT_END {
    u8 code; //!< From enum RoseRoleInstructionCode.
};

#endif // ROSE_ROSE_PROGRAM_H