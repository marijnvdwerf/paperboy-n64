#include "common.h"

// Controller pak: check PBOY.1 file, start save/load
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E4060);

#ifndef PAL
// Controller pak event dispatch (big switch)
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E4184); // mismatch
#else
__asm__(".globl func_800E4184\nfunc_800E4184:\n");
TEXT_PAD(0x450);
#endif

// Check if pak slot is valid and accessible
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E45B0); // -> ACB94

// Check pak file integrity (checksum over 16 blocks)
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E46D0);

// Detect pak insertion/removal state changes
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E47AC);

// Handle pak state per mode (1-5): start appropriate flow
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E4898);

// Save game to pak: write PBOY.1 then load scene
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E4A34);

// Load game from pak (mode 5)
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E4BD4);

// Load game from pak (mode 4)
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E4DF8);

// Serialize game state to save buffer
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E5044);

// Deserialize save buffer to game state
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E5684);

// Wrapper: get pak controller, return 1
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E5B98);

// Start new game: check pak, begin game flow
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E5BB8);

// Continue game: check pak for modes 2/3
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E5CE8);

// Load saved game (mode 5 flow)
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E5F1C);

// Pak poll: periodic check for pak changes during gameplay
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E605C);

// Select active controller slot
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E623C);

// Update pak state and controller assignment
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E6330);

// Get active controller pak handle
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E641C);

// Get active controller slot index
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E64A8);

// Check if address 0x300 is zero
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E6510);

// Validate pak file (check 16 blocks, compare threshold)
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E6520);

// Read PBOY.1 from pak into buffer, verify magic
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E65C4);

// Check if pak is inserted and has valid file
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E6750);

// Set unkC1C based on rumble pak presence
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E67E4);

// Clear unkC1C and reset pak file pointer
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E6830);

// Detach and cleanup pak controller
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E686C);

// Init pak controller: allocate, configure "EYPB" magic
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E68C4);

// Init pak name field
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E6964);

// Init D_80127954 pak state
INCLUDE_ASM("asm/nonmatchings/AC440", _._4ObjB);

// Set D_80127954
INCLUDE_ASM("asm/nonmatchings/AC440", __4ObjB);

// Cleanup wrapper
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E69BC);

// Cleanup wrapper
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E69D8);

#ifndef PAL
// Return negative float (stub)
INCLUDE_ASM("asm/nonmatchings/AC440", func_800E69F4);
#endif

