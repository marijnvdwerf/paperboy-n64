# Binary File Documentation

> **Author:** Will Kirkby (2013)
> **Source:** <https://web.archive.org/web/20190119160555/http://will.kirk.by/labs/lr1/binfiles.htm>

The binary files in Lego Racers (GDB, SDB, MIB, etc.) all follow a common data structure, which I will attempt to document here.

> **Warning!** This documentation is incomplete. I'll improve it over time.

## A guide to tokens

The file consists of a series of tokens, each of which may have a few bytes of payload data attached to it.

Note: all multi-byte datatypes (int16, float, etc) are stored in little-endian byte order.

| Token  | Name      | Payload |
| ------ | --------- | ------- |
| `0x02` | `string`  | Null-terminated ASCII string |
| `0x03` | `float`   | 32-bit floating point (IEEE 754-1985) |
| `0x04` | `int`     | 32-bit signed integer |
| `0x05` | `{`       | None |
| `0x06` | `}`       | None |
| `0x07` | `[`       | None |
| `0x08` | `]`       | None |
| `0x0B` | `sbyte`   | 8-bit signed integer. Sometimes used as a 4.4 fixed-point. |
| `0x0C` | `byte`    | 8-bit unsigned integer |
| `0x0D` | `short`   | 16-bit signed integer. Sometimes used as an 8.8 fixed-point. |
| `0x0E` | `ushort`  | 16-bit unsigned integer |
| `0x0F` | `fixed4096` | s16 fixed-point payload, decoded as float by multiplying by `1/4096` (i.e. Q3.12). |
| `0x10` | `fixed32` | s16 fixed-point payload, decoded as float by multiplying by `1/32` (i.e. Q10.5). |
| `0x11` | `shortf`  | s16 payload, decoded as float (no scaling). |
| `0x12` | `normbyte` | u8 payload, decoded as float by multiplying by `1/127` (approx `0.00787402`). |
| `0x14` | `array`   | `{ ushort length; byte type; }` — followed by `length` payloads of type `type`, *without* their token headers. |
| `0x16` | `struct`  | `{ byte id; byte length; byte tokens[length]; }` — any time throughout the file that `id` is found as a token, it will be followed by a series of payloads of the types specified in `tokens`. For example, a struct `{ 0x17; 0x03; [0x02, 0x04, 0x04]; }` would mean that any use of the token `0x17` would be followed by a string and then two int32s, *without* their token headers. |
| `0xXX` | `keyword` | If the token in question has been defined as a struct ID, read data according to that struct definition. Otherwise, there is no payload, and this is a file-specific block/property ID. |

## Decompressing a file

Ideally when opening a file, you'll want to decompress it to a buffer before reading the data; this makes life a lot easier and is how I've implemented it in my library (source coming eventually).

Because the compression is simply an extension of the format, the game will accept any decompressed file that you give it; this saves us a *lot* of effort later on.

Here, I will detail the recursive function I use for decompression. It's implemented in C#, but should be easy to convert to other languages.

You'll notice I'm not bothering to parse fixed-size payloads (I'm just copying the raw bytes), since it's not necessary for the decompression phase.

```csharp
ushort ReadUShort(Stream src) {
    byte[] buffer = new byte[2];
    src.Read(buffer, 0, buffer.Length);
    return BitConverter.ToUInt16(buffer, 0);
}

Stream Decompress(Stream src) {
    // create an empty key/value map to store structs.
    Dictionary<byte, byte[]> structs = new Dictionary<byte, byte[]>();

    // create an empty stream to write the decompressed data to.
    Stream dst = new MemoryStream();

    // loop until we've run out of data
    while (src.Position < src.Length) {
        // read a token from the stream.
        byte token = (byte)src.ReadByte();

        RecursiveDecompress(token, src, dst, structs);
    }
    dst.Position = 0;   // set the read cursor back to the beginning
    return dst;         // return the buffer of decompressed data
}

// since src/dest/structs are all reference types, I don't need to explicty pass them by reference.
void RecursiveDecompress(byte token, Stream src, Stream dst, Dictionary<byte, byte[]> structs) {
    switch (token) {
        case TYPE_STRING: {     // 0x02
            dst.WriteByte(token);   // copy the token
            while (true) {
                byte buffer = (byte)src.ReadByte();
                dst.WriteByte(buffer);
                if (buffer == 0)
                    break;
            }
        } break;
        case TYPE_FLOAT:    // 0x03
        case TYPE_INT32: {  // 0x04
            dst.WriteByte(token);
            byte[] buffer = new byte[4];
            src.Read(buffer, 0, buffer.Length);
            dst.Write(buffer, 0, buffer.Length);
        } break;
        case TYPE_LEFT_CURLY:       // 0x05
        case TYPE_RIGHT_CURLY:      // 0x06
        case TYPE_LEFT_BRACKET:     // 0x07
        case TYPE_RIGHT_BRACKET: {  // 0x08
            dst.WriteByte(token);   // no payload, just clone the token
        } break;
        case TYPE_SBYTE:    // 0x0B
        case TYPE_BYTE: {   // 0x0C
            dst.WriteByte(token);
            dst.WriteByte((byte)src.ReadByte());
        } break;
        case TYPE_SHORT:    // 0x0D
        case TYPE_USHORT: { // 0x0E
            dst.WriteByte(token);
            byte[] buffer = new byte[2];
            src.Read(buffer, 0, buffer.Length);
            dst.Write(buffer, 0, buffer.Length);
        } break;
        case TYPE_ARRAY: {  // 0x14
            ushort length = ReadUShort(src);
            byte type = (byte)src.ReadByte();
            for (int i = 0; i < length; i++)
                RecursiveDecompress(type, src, dst, structs);
        } break;
        case TYPE_STRUCT: { // 0x16
            byte id = (byte)src.ReadByte();
            byte length = (byte)src.ReadByte();
            byte[] tokens = new byte[length];
            src.Read(tokens, 0, length);
            structs.Add(id, tokens);
        } break;
        default: {
            if (structs.ContainsKey(token)) // if it's a struct ID
                for (int i = 0; i < structs[token].Length; i++)
                    RecursiveDecompress(structs[token][i], src, dst, structs);
            else                            // if it's a file-specific keyword
                dst.WriteByte(token);
        } break;
    }
}
```

## Reading a file

Now we reach the fun part. I'll write this in later.
