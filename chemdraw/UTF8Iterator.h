// Added by Glydade: Reimplementation of CDMap

// BSD 3-Clause License
//
// Copyright (c) 2025, Glysade Inc
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
#ifndef UTF8_ITERATOR_H
#define UTF8_ITERATOR_H

#include <string>
#include <stdexcept>
#include <cstdint>
#include <iterator>
#include <iostream>
#include <locale>
#include <codecvt>


// A forward cursor over the UTF-8 code points of a std::string.
//
// The cursor identifies the "current" character by the byte offset `pos` at
// which that character begins. Nothing is decoded or advanced in the
// constructor, so the accessors describe the character that begins at `pos`:
//   - GetByteIndex(): byte offset where the current character begins
//   - GetUTF8Length(): number of bytes the current character occupies (1-4)
//   - GetCharacter(): decoded Unicode code point of the current character
//   - AtEnd(): true once the cursor is at or past the end of the string
// operator++ advances `pos` by the byte length of the current character.
//
// Malformed lead/continuation bytes are treated defensively as a single-byte
// character so iteration always makes forward progress and never reads out of
// bounds.
class UTF8Iterator {
public:
    UTF8Iterator(const std::string& str)
      : data(str), pos(0) {}

    // Number of bytes occupied by the character that begins at the current
    // position, clamped to the bytes actually remaining in the string.
    size_t GetUTF8Length() const {
        if (AtEnd())
            return 0;

        const unsigned char byte1 = static_cast<unsigned char>(data[pos]);
        size_t length = 1;
        if (byte1 < 0x80)
            length = 1;
        else if ((byte1 & 0xE0) == 0xC0)
            length = 2;
        else if ((byte1 & 0xF0) == 0xE0)
            length = 3;
        else if ((byte1 & 0xF8) == 0xF0)
            length = 4;
        else
            length = 1;  // invalid lead byte: treat as a single byte

        // Never run past the end of the buffer for a truncated sequence.
        if (pos + length > data.size())
            length = 1;
        return length;
    }

    size_t GetByteIndex() const { return pos; }

    // Returns the current Unicode code point without advancing the iterator.
    uint32_t GetCharacter() const {
        if (AtEnd())
            throw std::out_of_range("Iterator has reached the end of the string.");
        return Decode(GetUTF8Length());
    }

    // Checks if the iterator has reached the end of the string.
    bool AtEnd() const {
        return pos >= data.size();
    }

    // Post-increment operator: advances to the next character.
    UTF8Iterator operator++(int) {
        UTF8Iterator temp = *this;  // Copy current state
        Advance();
        return temp;  // Return old state
    }

    UTF8Iterator& operator++() {
        Advance();
        return *this;
    }

private:
    const std::string& data;
    size_t pos;

    void Advance() {
        if (!AtEnd())
            pos += GetUTF8Length();
    }

    // Decodes the character of `length` bytes that begins at the current
    // position. Does not modify the iterator.
    uint32_t Decode(size_t length) const {
        const unsigned char byte1 = static_cast<unsigned char>(data[pos]);
        if (length == 1)
            return byte1;

        uint32_t codePoint = 0;
        if (length == 2) {
            codePoint = (byte1 & 0x1F) << 6;
            codePoint |= (static_cast<unsigned char>(data[pos + 1]) & 0x3F);
        } else if (length == 3) {
            codePoint = (byte1 & 0x0F) << 12;
            codePoint |= (static_cast<unsigned char>(data[pos + 1]) & 0x3F) << 6;
            codePoint |= (static_cast<unsigned char>(data[pos + 2]) & 0x3F);
        } else {  // length == 4
            codePoint = (byte1 & 0x07) << 18;
            codePoint |= (static_cast<unsigned char>(data[pos + 1]) & 0x3F) << 12;
            codePoint |= (static_cast<unsigned char>(data[pos + 2]) & 0x3F) << 6;
            codePoint |= (static_cast<unsigned char>(data[pos + 3]) & 0x3F);
        }
        return codePoint;
    }
};
#endif
