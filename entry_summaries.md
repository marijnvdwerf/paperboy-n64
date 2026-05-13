## 0x7660  (`asm/7660.s`)

Display/graphics subsystem with message queue-based thread coordination. Sets up multiple threads to manage framebuffer synchronization, VI (video interface) events, and maintains linked lists of message queue subscribers for inter-thread notifications.

## 0x7FE0  (`src/7FE0.cpp`)

Custom character encoding/decoding—`func_800073E0` decodes from a custom character set to ASCII, while `func_8000752C` encodes ASCII back to the custom set. Likely used for compressed or custom-rendered text storage in the game.

## 0x8260  (`src/8260.cpp`)

Game context and initialization: manages memory allocation, ROM loading via DMA, cache invalidation, and subsystem startup (constructors, schedulers, actor initialization). GameContext is the main game state container.

## 0x8DA0  (`asm/8DA0.s`)

Game data and object initialization code with various state-setup helper functions.

## 0x8EB0  (`asm/8EB0.s`)

Game state/context management—initializes and updates the main game context (`GameSubContext`), manages an array of ~28 game entities (each 0x144 bytes), handles game systems (graphics, audio, data), and coordinates state transitions.

## 0x9E10  (`src/9E10.cpp`)

Singleton system manager with initialization/cleanup lifecycle and virtual function dispatch for polymorphic subsystem behavior.

## 0xA0B0  (`asm/A0B0.s`)

Audio synthesis or audio playback initialization — uses `alHeapDBAlloc` (audio memory), `init_lpfilter` (low-pass filter), and sets up complex structures with frequency/timing parameters selected by type.

## 0xA670  (`asm/A670.s`)

Animation command processing and interpolation. The main function dequeues and dispatches animation events (cases 0-16), calculates interpolation curves, and generates RSP/RDP display list commands with magic numbers like `0x09000000`.

## 0xB010  (`asm/B010.s`)

RSP/graphics display list generation and buffer management. The functions manipulate RSP command structures (opcodes like `0x04000000`, `0x01000000`) and manage graphics command lists with alignment constraints.

## 0xB720  (`asm/B720.s`)

Hardware initialization, likely graphics-related RSP/RDP setup based on the magic values and pointer manipulation pattern. The empty second function suggests incomplete decompilation or a linker stub.

## 0xB7A0  (`asm/B7A0.s`)

Audio synthesis/processing command builders for the RSP, with parameter setting and display list construction for sound playback.

## 0xC380  (`src/C380.c`)

Init/cleanup routines for a subsystem — `func_8000B780` initializes two static state variables and calls an external function, while `func_8000B7BC` resets them and calls a cleanup handler.

## 0xC400  (`asm/C400.s`)

Audio synthesis player management — `alSynAddPlayer` registers an audio player with the synthesis system, linking it into a global queue while managing interrupt state.

## 0xC450  (`asm/C450.s`)

Data structure access utilities — minimal code involving offset calculations on a global pointer and calls to external functions, but too generic to determine a specific purpose.

## 0xC4B0  (`asm/C4B0.s`)

Audio synthesis voice allocation and management — allocating audio channels (`alSynAllocVoice`), finding available voices from linked lists, and initializing voice state.

## 0xC6B0  (`asm/C6B0.s`)

Various things—appears to include resource management utilities (allocation/cleanup), state queries, and memory address accessors for what looks like a subsystem managing global state.

## 0xC780  (`asm/C780.s`)

Interrupt handler list management — removes a node from a linked list (stored at `D_8006ADD4`) while disabling interrupts via `osSetIntMask`. The second function is incomplete/malformed.

## 0xC820  (`asm/C820.s`)

Audio or MIDI parameter/command handling—sets up a structured object with fields (velocity, offset, flags) and passes it to an external function.

## 0xC8C0  (`asm/C8C0.s`)

Audio playback or sample processing — uses fixed-point scaling on an offset into a structured buffer (the `(arg0 << 0x10) >> 0xE` pattern is typical of audio DSP math) and reads interpolation or filter parameters from it.

## 0xC950  (`asm/C950.s`)

Event or command queueing system — functions create different types of events (distinguished by `unk8` values like 0xC and 7) with different parameter types, initialize them, and queue them via `func_8000A1FC`.

## 0xCA60  (`asm/CA60.s`)

Object/message creation and attachment, possibly for an event or command queueing system (though the second function is incomplete/broken).

## 0xCB10  (`asm/CB10.s`)

Event or work-queue processing — allocates objects via `func_8000C4DC()`, initializes fields, and passes them to a handler function. The second function appears incomplete/decompilation-broken.

## 0xCBA0  (`asm/CBA0.s`)

Audio command or event initialization and queueing. The function allocates a command structure, populates fields with parameters (including what looks like volume/amplitude clamping at 0x7F), and dispatches it via a handler function.

## 0xCCA0  (`asm/CCA0.s`)

Audio decompression and playback initialization, likely managing an ADPCM or similar codec context with heap-allocated frame buffers, linked lists, and microsecond-based timing calculations.

## 0xD260  (`asm/D260.s`)

Empty stub functions (possibly padding or unimplemented stubs).

## 0xD2C0  (`src/D2C0.cpp`)

Base class for game entities or objects, providing initialization, getters for various state fields and bit flags, and delegation to child handlers and structures.

## 0xD730  (`src/D730.cpp`)

Main game state manager handling initialization, timing, frame deltas, and display configuration.

## 0xDD50  (`asm/DD50.s`)

Fixed-size string and buffer utilities (8-byte and 12-byte variants) with parsing, copying, comparison, and manipulation functions; likely for structured game data with field names or identifiers of fixed widths.

## 0xE2C0  (`asm/E2C0.s`)

3D math library: matrix operations, quaternion transformations (slerp, conversions), vector rotations, geometric collision tests (point-in-triangle, sphere collision), and reflection operations.

## 0x109C0  (`src/109C0.c`)

OS boot sequence with threading, video initialization, and C++ constructor/destructor management. Sets up the main thread, parser for command-line arguments, and runs the program entry point.

## 0x10CC0  (`asm/10CC0.s`)

Text/string buffer management library. Functions handle copying, comparing, and appending multi-byte character arrays (u16), with operations for number-to-string conversion, newline detection, and format conversion between character types.

## 0x11570  (`src/11570.cpp`)

A linked-list container base class (`StructYYSubA8Base`) that manages a collection of `StructYYSubA8Node` items and delegates virtual method calls (init, update, cleanup) across the list. Most of the many virtual function stubs return constant values (0 or 1), suggesting it implements a large interface with default no-op behavior.

## 0x11890  (`asm/11890.s`)

Parameter parsing/state management — the main function (`func_80010C90`) parses character-based commands and reads float/integer values from a stream into a structure, with supporting functions for data transfer and modulo arithmetic.

## 0x11B40  (`asm/11B40.s`)

Data structure initialization, cleanup, and accessor functions for an indexed collection with coordinate/geometry data; likely animation frames, mesh vertices, or similar.

## 0x12480  (`asm/12480.s`)

Image/texture loading and decompression. Handles header parsing, color palettes, different bit depths (4/8/16/24/32-bit), and scaled/tiled image processing.

## 0x146D0  (`asm/146D0.s`)

Camera system with matrix transforms and view projection calculations. Large functions perform trigonometric operations and cross products for building transformation matrices; numerous getters/setters manage camera parameters like FOV, aspect ratio, and focal distance.

## 0x16110  (`asm/16110.s`)

Bit field utilities: leading-zero counting, LSB position finding, and bit extraction/shifting operations on structures with 4 packed 32-bit mask fields (likely color channel or texture format handling).

## 0x168C0  (`asm/168C0.s`)

Linked list management (insertion, removal, initialization). The functions manipulate nodes with forward (`unk4`) and back (`unk0`) pointers, managing a list headed at `arg0->unk8`.

## 0x16970  (`src/16970.cpp`)

Surface blitting and fill operations for different pixel formats (4/8/16/24/32-bit). The main functions handle clearing surfaces with a color value and copying pixel data with clipping support.

## 0x17440  (`src/17440.cpp`)

Display or viewport state controller. Manages configuration (dimensions in `unk0`/`unk4`/`unk8`), status flags in `unkC` via numerous bit checkers, and delegates operations to a child object (`unk10`). Mostly getters, setters, and initialization logic.

## 0x177B0  (`src/177B0.cpp`)

Graphics surface buffer management—initializes and configures a drawable surface with width, height, and color format, plus flag checking methods.

## 0x17880  (`asm/17880.s`)

Object initialization and accessor helpers — simple utility functions for managing pointers and values, possibly part of a class/object system.

## 0x17910  (`asm/17910.s`)

Sprite/field data parser — loads and parses entries from a file format (likely `.fdb`), handling per-entry positions, colors, and flags via a character-based state machine.

## 0x18120  (`asm/18120.s`)

Font rendering and text layout system. Functions handle font data loading, character lookup via binary search, text measurement, word wrapping, glyph rendering with positioning and scaling, and support for different pixel formats and character sets.

## 0x1AA60  (`asm/1AA60.s`)

Grid layout and tiling system, handling dimension calculations, cell generation, and constraint-based sizing for spatial partitioning.

## 0x1BE30  (`asm/1BE30.s`)

Image format conversion and color palette management — decompresses and converts between different color formats (4/8/16/24/32-bit) and palette-indexed images.

## 0x1F7F0  (`asm/1F7F0.s`)

3D character/object animation and rendering system — handles keyframe interpolation, matrix transforms, geometry loading, materials, bounding volumes, and render state management.

## 0x25690  (`asm/25690.s`)

Hash table implementation for 8-byte keys mapping to s32 values, with insert, lookup, initialization, and cleanup operations.

## 0x25B30  (`asm/25B30.s`)

Spatial partitioning and collision detection system—manages hierarchical linked lists of objects with array-based caching, performs spatial queries comparing values across multiple axes, and provides virtual method dispatch for updating/querying the structure.

## 0x27750  (`asm/27750.s`)

Color palette or RGBA color buffer management — allocates/deallocates arrays of 4-byte color entries, searches for matching colors, and provides read/write accessors.

## 0x27B10  (`asm/27B10.s`)

Surface/texture resource management — allocates and initializes memory for rendering surfaces, with cleanup and state management.

## 0x27D10  (`src/27D10.c`)

Graphics and sprite data loading infrastructure—handles palette/color format conversions (RGB555/565), RLE decompression, tile graphics processing, ROM I/O, and geometric collision detection for graphics objects.

## 0x2A4A0  (`asm/2A4A0.s`)

Surface object initialization routines — likely constructors or setup code for a graphics surface type (Surface16970), with a thin wrapper stub and initialization of object fields.

## 0x2A500  (`src/2A500.cpp`)

Video display initialization and VI (Video Interface) mode management—handles NTSC/PAL mode selection, resolution scaling, gamma/dither settings, and scheduler setup for the N64 output.

## 0x2A930  (`asm/2A930.s`)

3D camera system: projection/view matrix setup and transformations. Functions handle viewport configuration, FOV calculations, matrix operations for 3D-to-screen transformations, and camera initialization.

## 0x2B9C0  (`asm/2B9C0.s`)

3D transformation and scene graph system — handles matrix operations, quaternion conversions, hierarchical transforms (recursive tree traversal), and RDP display list command buffering for rendering.

## 0x2C9A0  (`src/2C9A0.cpp`)

Double-buffered surface management for N64 VI (video interface) framebuffers—allocates two buffers that can be cleared, swapped, and output via `osViSwapBuffer()`. The class handles buffer allocation, color fills, copying between buffers, and cleanup.

## 0x2CD50  (`asm/2CD50.s`)

Container/manager class initialization, cleanup, and dynamic memory allocation for multiple array types.

## 0x2D1C0  (`asm/2D1C0.s`)

Graphics rendering and RSP/RDP display list generation. The functions build N64 graphics commands (0xE7000000, 0xFA000000, etc.), manage geometry/texture state, and construct command buffers for the Reality engine.

## 0x2DC10  (`asm/2DC10.s`)

Textured surface rendering system with display list generation and bounds-based clipping.

## 0x2EBF0  (`asm/2EBF0.s`)

Vector/array container initialization and deallocation with constructor/destructor callbacks for fixed-size elements (0x54 bytes).

## 0x2EEA0  (`asm/2EEA0.s`)

Dynamic array manager for fixed-size (0x10-byte) game objects or particles, with getters/setters for position (3 s16 components), color/attributes (4 u8 values), and intensity/velocity (scaled floats).

## 0x2F430  (`asm/2F430.s`)

Vector/container allocation and lifecycle management — allocates, initializes, iterates, and destroys arrays of 0x24-byte elements via virtual methods.

## 0x2F700  (`asm/2F700.s`)

Initialization or setup code—checks a flag and conditionally calls an init function, though most of the functions are empty stubs.

## 0x2F750  (`asm/2F750.s`)

N64 graphics display list generation and transformation — building RCP rendering commands with various effects like color manipulation, coordinate scaling, and alpha blending based on different input parameters and magic command opcodes.

## 0x318E0  (`src/318E0.c`)

Graphics rendering and display list generation for surfaces; matrix transformations, RSP command building, and texture/sprite blitting.

## 0x38440  (`asm/38440.s`)

Texture handling and display list generation for N64 graphics—managing different texture formats and building RDP commands for rendering.

## 0x39890  (`asm/39890.s`)

Object instantiation and cleanup factories — numerous wrappers that allocate objects, call type-specific init functions, and provide virtual destructor dispatch for what appear to be game object types or subsystems.

## 0x3A0B0  (`asm/3A0B0.s`)

Vertex/mesh data management—loading and storing 3D vertex attributes (positions, colors, and normalized parameters) from a binary stream into dynamically allocated arrays.

## 0x3A950  (`asm/3A950.s`)

Graphics/material property management — likely a Light or Material class that stores and manages color, position, and scale attributes in indexed 16-byte structures.

## 0x3B720  (`asm/3B720.s`)

Vertex color and geometry data loading — reads textured vertex attributes from a stream, stores position and color arrays, and performs color channel shifting/clamping operations.

## 0x3BEB0  (`asm/3BEB0.s`)

Surface object lifecycle management — allocation, initialization with dimensions, and cleanup.

## 0x3C030  (`asm/3C030.s`)

World/scene resource initialization and management—allocates multiple arrays of different data types (entities, materials, etc.), loads associated data files (.tdf, .mdf, .adf, .gdf, .sdf, .bdf, .maf), handles cleanup, and provides accessor functions.

## 0x3D5E0  (`src/3D5E0.cpp`)

A simple parent class that wraps a single u32 field (`unk0`) with basic initialization and accessor functionality. Likely a base class for related structures.

## 0x3D640  (`asm/3D640.s`)

Spatial audio physics with Doppler effect calculations—computes attenuation and pitch shift based on listener/source distance and relative velocity (uses 343.0f speed of sound constant).

## 0x3DE00  (`asm/3DE00.s`)

Stub functions or dead code — eight empty implementations with no logic.

## 0x3DE40  (`src/3DE40.cpp`)

Audio/music system manager for N64 — initializes General MIDI synthesis (genmidi.ptr/wbk), manages memory heaps, and maintains scheduler integration for synchronized playback via linked-list node management.

## 0x3E8E0  (`src/3E8E0.cpp`)

StructVV data manager: loads fixed-size items from a file into a buffer, and manages a linked list of associated StructUU nodes.

## 0x3EE50  (`asm/3EE50.s`)

Audio sample/stream player: loads .bin files, manages playback state with flags (unk18), controls volume/pitch by converting floats to integer values (0–256 range), and handles playback lifecycle.

## 0x3F580  (`src/3F580.c`)

3D positional audio source / emitter — a thin wrapper class around an embedded audio playback object at `+0x40`. Nearly every method delegates to the audio resource manager in the next TU (`0x3F820`, range `0x3FB9C–0x3FFC8`); the constructor also calls into the music system manager (`0x3DE40`). Defaults `unk0=unk4=unk8=unk10=1.0f` look like volume/pitch/pan; `unk2C=8.0f` and `unk30=1024.0f` look like attenuation min/max distance, not a graphics far plane.

## 0x3F820  (`src/3F820.c`)

Audio/music resource manager that loads sound effect and instrument banks (`.ptr`, `.bfx`, `.wbk` files) and manages audio object instances.

## 0x40790  (`asm/40790.s`)

Sound effect/audio playback controller with pitch, volume, and pan control. Functions manage state flags, parameter setting (sample ID, pitch via table lookup, volume/pan in 0-256 range), and lifecycle management for active sound instances.

## 0x40CC0  (`asm/40CC0.s`)

Various things; mostly empty stub functions and trivial value returns.

## 0x40D10  (`asm/40D10.s`)

Binary file format parser and I/O system—opens files, reads typed records (cases for different data types), and processes hierarchical binary data structures.

## 0x41770  (`asm/41770.s`)

Circular buffer queue implementation. Multiple queue enqueue/dequeue variants manage a fixed-capacity ring buffer with read/write pointers and size tracking.

## 0x41CE0  (`src/41CE0.c`)

InputDeviceBase class implementation—manages input device state with constructors, virtual function pointers, getters/setters for various input flags and button states, and logic for tracking input changes across multiple input items.

## 0x42D90  (`asm/42D90.s`)

Hash table implementation with linked-list chaining for collision resolution. Includes initialization, string lookup, insertion, and statistics gathering functions.

## 0x43DD0  (`src/43DD0.cpp`)

Local file I/O interface (LocalIOParent class) with path handling, file operations, and buffering. Functions manage file reads/writes, path resolution, and internal buffers for a file I/O abstraction layer.

## 0x455F0  (`asm/455F0.s`)

Resource/archive manager loading and accessing data from a "LJAM" format container with hierarchical name lookups and path resolution (directories separated by backslashes).

## 0x462D0  (`asm/462D0.s`)

C++ class initialization and state management with virtual method dispatch and dynamic memory allocation — likely for a controller or input device handler.

## 0x465A0  (`asm/465A0.s`)

Binary resource loader for paired data structures — loads indices and data from a file stream, provides lookup and search functionality.

## 0x46B40  (`asm/46B40.s`)

Stream/file I/O parser with support for multiple data formats and compression schemes. The class reads encoded values (floats, ints, bytes) from a buffer with state-based decoding, manages filenames/extensions, and includes constructor/destructor logic for memory management.

## 0x476E0  (`asm/476E0.s`)

LZ77-style data decompression (reads compressed stream with bit flags, copies from previous output buffer). Second function is malformed/incomplete.

## 0x477C0  (`asm/477C0.s`)

N64 controller input polling, rumble pack control, and persistent pak file system management via a worker thread.

## 0x48AD0  (`src/48AD0.cpp`)

N64 controller input device abstraction with button and analog stick handling, deadzone normalization, event dispatch to listeners, and controller Pak (rumble/memory) support.

## 0x49630  (`src/49630.cpp`)

ROM/cartridge I/O handling with DMA and message queue-based synchronization. The `LocalIOBase` class manages buffered reads and seeking on the N64 cartridge, with both blocking and non-blocking variants.

## 0x49A80  (`asm/49A80.s`)

I/O device or stream initialization and control—functions for setting up, configuring, and processing I/O operations with status code returns.

## 0x49D50  (`asm/49D50.s`)

Input device or controller manager — manages 4 devices with stride 0x7C each, with initialization, state tracking (via bit flags), and accessor functions.

## 0x4A000  (`src/4A000.c`)

File lookup/search system with alphabetic indexing. Functions parse filenames with extensions, search through indexed records, build A-Z lookup tables, and return standardized error codes.

## 0x4ACE0  (`asm/4ACE0.s`)

Configuration or data structure parser with custom token handling (switches on ASCII characters like `'` and `(` alongside numeric tokens).

## 0x4B220  (`src/4B220.c`)

Various things: bit-counting utilities that iterate through 32-element arrays checking for specific flag conditions (bit 0x40000000 or sign bit), plus a memset wrapper for zeroing structures.

## 0x4B2F0  (`src/4B2F0.c`)

Memory allocator/heap manager. Functions handle allocation (`func_8004A6F0`), deallocation with coalescing (`func_8004A8C4`), initialization, and free-list traversal using a linked-list structure with bit-packed metadata.

## 0x4B7D0  (`asm/4B7D0.s`)

Memory allocator and linked-list pool management — handles allocation/deallocation, block splitting, and defragmentation of 64-byte-aligned memory chunks from a fixed pool of 16 slots.

## 0x4BF70  (`asm/4BF70.s`)

Simple queue or stack data structure—push/pop operations on a bounded array with counter, wrapped in C++ memory management operators.

## 0x4C060  (`asm/4C060.s`)

UI or scene manager initialization and state management — initialization/cleanup functions with bitfield manipulations, virtual method calls, and string resources ("General", "ui", "GAMEDATA").

## 0x4C630  (`src/4C630.c`)

Audio/sound effect manager handling sound lifecycle, state transitions, and volume fading.

## 0x4CD90  (`asm/4CD90.s`)

Audio resource or object management — likely for audio initialization, with functions for managing collections of audio data (allocation, deletion, searching) and setting up audio paths and objects.

## 0x4D0D0  (`src/4D0D0.c`)

Collection/container manager for dynamic arrays. Functions add items (allocate and store pointers), iterate through them, search for matches, and clean up. Likely a C++ object wrapping a simple growable array with callback support.

## 0x4D370  (`asm/4D370.s`)

Manager for a pool of 8 game objects or particles—add/remove/update operations with 3D transformations and search by criteria.

## 0x4DF80  (`asm/4DF80.s`)

Animation or effect system with ID-based lookup, linked list traversal, and random variant selection. The core `func_8004D614` performs the lookup and selection logic, while peripheral functions initialize, allocate, and delegate operations to a global manager (D_80076D50).

## 0x4E3C0  (`asm/4E3C0.s`)

Dynamic container and linked list implementations (vector-like and linked-list management with allocation, insertion, searching, and cleanup operations).

## 0x4E9C0  (`asm/4E9C0.s`)

Audio data loading and initialization—parses an SPB file from `GAMEDATA\AUDIO`, processes audio entries with volume percentages and flags, and stores them in memory structures.

## 0x4ECE0  (`asm/4ECE0.s`)

Audio resource loading/initialization. Functions parse and iterate through audio data structures, handle paths like "GAMEDATA\\AUDIO", and process audio items with configuration flags.

## 0x4F0A0  (`asm/4F0A0.s`)

Audio/music system driver and playback control — initializes channels, manages volume/pitch/effects, processes audio commands, and coordinates with the synthesis/DSP backend.

## 0x545A0  (`asm/545A0.s`)

Audio initialization and ROM DMA management—sets up audio heaps, effects (FX) parameters with low-pass filters, and handles virtual-to-physical ROM address mapping for DMA transfers.

## 0x55220  (`src/55220.c`)

Music interrupt scheduler initialization and message queue setup for audio processing on the OS scheduler.

## 0x553E0  (`asm/553E0.s`)

Audio/music manager initialization and playback thread management for libultra-based audio system. Sets up audio frequency, allocates buffers, creates an audio processing thread, and manages a 3-buffer round-robin audio output loop.

## 0x55680  (`asm/55680.s`)

Buffer allocation and bounds checking — calculates memory regions in fixed 0xB8-byte chunks, tracks buffer offsets, and validates access within bounds.

## 0x557A0  (`asm/557A0.s`)

N64 system library implementation (kernel threading, audio synthesis, graphics task scheduling, hardware I/O).

## 0x60270  (`asm/60270.s`)

N64 Pak File System (PFS) implementation, covering file allocation/deletion/reading/writing, corruption detection and repair, pak initialization, and low-level controller/SI hardware operations.

## 0x63A60  (`asm/63A60.s`)

N64 libultra OS initialization and low-level runtime support—sets up exception handlers, CPU registers, serial device I/O, TLB mappings, and provides 64-bit division/modulo builtins.

## 0x64F20  (`asm/64F20.s`)

C++ memory allocation operators (`operator new` and `operator new[]`) and related runtime support.

## 0x64F40  (`asm/64F40.s`)

C++ operator delete functions (`__builtin_delete` and `__builtin_vec_delete` for array deletion).
