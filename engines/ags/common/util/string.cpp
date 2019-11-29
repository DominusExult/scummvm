/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "ags/common/util/math.h"
//include "util/string_compat.h"
#include "ags/common/util/string.h"
#include "ags/common/util/stream.h"
#include "ags/std/vector.h"
#include "common/algorithm.h"

namespace AGS {
namespace Shared {

String::String(char c, size_t count) : ::Common::String() {
	FillString(c, count);
}

void String::Read(Stream *in, size_t max_chars, bool stop_at_limit) {
    Empty();
    if (!in) {
        return;
    }
    if (max_chars == 0 && stop_at_limit) {
        return;
    }

    char buffer[1024];
    char *read_ptr = buffer;
    size_t read_size = 0;
    int ichar;
    do {
        ichar = in->ReadByte();
        read_size++;
        if (read_size > max_chars)
        {
            continue;
        }
        *read_ptr = (char)(ichar >= 0 ? ichar : 0);
        if (!*read_ptr || ((read_ptr - buffer) == (sizeof(buffer) - 1 - 1)))
        {
            buffer[sizeof(buffer) - 1] = 0;
            Append(buffer);
            read_ptr = buffer;
        }
        else
        {
            read_ptr++;
        }
    }
    while(ichar > 0 && !(stop_at_limit && read_size == max_chars));
}

void String::ReadCount(Stream *in, size_t count) {
    clear();
	
	if (in && count > 0) {
		// Read the characters
		char *buf = new char[count + 1];
		in->Read(buf, count);
		buf[count] = '\0';

		// Set the string
		*this = buf;
    }
}

void String::Write(Stream *out) const {
    if (out) {
        out->Write(GetCStr(), GetLength() + 1);
    }
}

void String::WriteCount(Stream *out, size_t count) const
{
    if (out)
    {
        size_t str_out_len = Math::Min(count - 1, GetLength());
        if (str_out_len > 0)
            out->Write(GetCStr(), str_out_len);
        size_t null_out_len = count - str_out_len;
        if (null_out_len > 0)
            out->WriteByteCount(0, null_out_len);
    }
}

/* static */ void String::WriteString(const char *cstr, Stream *out)
{
    if (out)
    {
        cstr = cstr ? cstr : "";
        out->Write(cstr, strlen(cstr) + 1);
    }
}

int String::CompareLeft(const char *cstr, size_t count) const
{
    cstr = cstr ? cstr : "";
    return strncmp(GetCStr(), cstr, count != -1 ? count : strlen(cstr));
}

int String::CompareLeftNoCase(const char *cstr, size_t count) const
{
    cstr = cstr ? cstr : "";
    return scumm_strnicmp(GetCStr(), cstr, count != -1 ? count : strlen(cstr));
}

int String::CompareMid(const char *cstr, size_t from, size_t count) const
{
    cstr = cstr ? cstr : "";
    from = Math::Min(from, GetLength());
    return strncmp(GetCStr() + from, cstr, count != -1 ? count : strlen(cstr));
}

int String::CompareMidNoCase(const char *cstr, size_t from, size_t count) const
{
    cstr = cstr ? cstr : "";
    from = Math::Min(from, GetLength());
    return scumm_strnicmp(GetCStr() + from, cstr, count != -1 ? count : strlen(cstr));
}

int String::CompareRight(const char *cstr, size_t count) const
{
    cstr = cstr ? cstr : "";
    count = count != -1 ? count : strlen(cstr);
    size_t off = Math::Min(GetLength(), count);
    return strncmp(GetCStr() + GetLength() - off, cstr, count);
}

int String::CompareRightNoCase(const char *cstr, size_t count) const
{
    cstr = cstr ? cstr : "";
    count = count != -1 ? count : strlen(cstr);
    size_t off = Math::Min(GetLength(), count);
    return scumm_strnicmp(GetCStr() + GetLength() - off, cstr, count);
}

int String::FindChar(char c, size_t from) const
{
	if (!empty() && c && from < size()) {
		const char *found_cstr = strchr(c_str() + from, c);
        return found_cstr ? found_cstr - c_str() : -1;
    }

	return -1;
}

int String::FindCharReverse(char c, size_t from) const
{
    if (empty() || !c) {
        return -1;
    }

    from = Math::Min(from, size() - 1);
    const char *seek_ptr = c_str() + from;
    while (seek_ptr >= c_str()) {
        if (*seek_ptr == c)
        {
            return seek_ptr - c_str();
        }
        seek_ptr--;
    }
    return -1;
}

int String::FindString(const char *cstr, size_t from) const {
    if (!empty() && cstr && from < size()) {
        const char * found_cstr = strstr(c_str() + from, cstr);
        return found_cstr ? found_cstr - c_str() : -1;
    }

    return -1;
}

bool String::FindSection(char separator, size_t first, size_t last, bool exclude_first_sep, bool exclude_last_sep,
                        size_t &from, size_t &to) const
{
    if (empty() || !separator) {
        return false;
    }
    if (first > last)
    {
        return false;
    }

    int this_field = 0;
    int slice_from = 0;
    int slice_to = size();
    int slice_at = -1;
    do
    {
        slice_at = FindChar(separator, slice_at + 1);
        if (slice_at == -1)
            slice_at = size();
        // found where previous field ends
        if (this_field == (int)last)
        {
            // if previous field is the last one to be included,
            // then set the section tail
            slice_to = exclude_last_sep ? slice_at : slice_at + 1;
        }
        if (slice_at != (int)size())
        {
            this_field++;
            if (this_field == (int)first)
            {
                // if the new field is the first one to be included,
                // then set the section head
                slice_from = exclude_first_sep ? slice_at + 1 : slice_at;
            }
        }
    }
    while (slice_at < (int)size() && this_field <= (int)last);

    // the search is a success if at least the first field was found
    if (this_field >= (int)first)
    {
        // correct the indices to stay in the [0; length] range
        assert(slice_from <= slice_to);
        from = Math::Clamp(slice_from, 0, (int)size());
        to   = Math::Clamp(slice_to, 0, (int)size());
        return true;
    }
    return false;
}

int String::ToInt() const
{
    return atoi(GetCStr());
}

/* static */ String String::FromFormat(const char *fcstr, ...)
{
    fcstr = fcstr ? fcstr : "";
    String str;
    va_list argptr;
    va_start(argptr, fcstr);
    str.FormatV(fcstr, argptr);
    va_end(argptr);
    return str;
}

/* static */ String String::FromFormatV(const char *fcstr, va_list argptr)
{
    String str;
    str.FormatV(fcstr, argptr);
    return str;
}

/* static */ String String::FromStream(Stream *in, size_t max_chars, bool stop_at_limit)
{
    String str;
    str.Read(in, max_chars, stop_at_limit);
    return str;
}

/* static */ String String::FromStreamCount(Stream *in, size_t count)
{
    String str;
    str.ReadCount(in, count);
    return str;
}

String String::Lower() const
{
    String str = *this;
    str.MakeLower();
    return str;
}

String String::Upper() const
{
    String str = *this;
    str.MakeUpper();
    return str;
}

String String::Left(size_t count) const
{
    count = Math::Min(count, GetLength());
    return count == GetLength() ? *this : String(GetCStr(), count);
}

String String::Mid(size_t from, size_t count) const
{
    Math::ClampLength(from, count, (size_t)0, GetLength());
    return count == GetLength() ? *this : String(GetCStr() + from, count);
}

String String::Right(size_t count) const
{
    count = Math::Min(count, GetLength());
    return count == GetLength() ? *this : String(GetCStr() + GetLength() - count, count);
}

String String::LeftSection(char separator, bool exclude_separator) const
{
    if (!empty() && separator)
    {
        size_t slice_at = FindChar(separator);
        if (slice_at != -1)
        {
            slice_at = exclude_separator ? slice_at : slice_at + 1;
            return Left(slice_at);
        }
    }
    return *this;
}

String String::RightSection(char separator, bool exclude_separator) const
{
    if (!empty() && separator)
    {
        size_t slice_at = FindCharReverse(separator);
        if (slice_at != -1)
        {
            size_t count = exclude_separator ? size() - slice_at - 1 : size() - slice_at;
            return Right(count);
        }
    }
    return *this;
}

String String::Section(char separator, size_t first, size_t last,
                          bool exclude_first_sep, bool exclude_last_sep) const
{
    if (empty() || !separator)
    {
        return String();
    }

    size_t slice_from;
    size_t slice_to;
    if (FindSection(separator, first, last, exclude_first_sep, exclude_last_sep,
        slice_from, slice_to))
    {
        return Mid(slice_from, slice_to - slice_from);
    }
    return String();
}

std::vector<String> String::Split(char separator) const {
    std::vector<String> result;
    if (empty() || !separator)
        return result;
    const char *ptr = c_str();
    while (*ptr)
    {
        const char *found_cstr = strchr(ptr, separator);
        if (!found_cstr) break;
        result.push_back(String(ptr, found_cstr - ptr));
        ptr = found_cstr + 1;
    }
    result.push_back(String(ptr));
    return result;
}

void String::Reserve(size_t max_length)
{
	ensureCapacity(max_length, true);
}

void String::ReserveMore(size_t more_length)
{
    Reserve(GetLength() + more_length);
}

void String::Compact() {
	String tempString(_str);
	*this = tempString;
}

void String::Append(const char *cstr) {
	operator+=(cstr);
}

void String::AppendChar(char c)
{
	operator+=(c);
}

void String::ClipLeft(size_t count)
{
    if (!empty() && count > 0) {
		String tempString(c_str() + count);
		*this = tempString;
    }
}

void String::ClipMid(size_t from, size_t count)
{
	if (!empty() && from < size()) {
		String tempString(c_str() + from, c_str() + Math::Min(size(), from + count));
		*this = tempString;
	}
}

void String::ClipRight(size_t count)
{
    if (!empty() && count > 0) {
        count = Math::Min(count, size());

		String tempString(c_str() + size() - count, c_str() + size());
		*this = tempString;
    }
}

void String::ClipLeftSection(char separator, bool include_separator)
{
    if (!empty() && separator)
    {
        size_t slice_at = FindChar(separator);
        if (slice_at != -1)
        {
            ClipLeft(include_separator ? slice_at + 1 : slice_at);
        }
        else
            Empty();
    }
}

void String::ClipRightSection(char separator, bool include_separator)
{
    if (!empty() && separator)
    {
        size_t slice_at = FindCharReverse(separator);
        if (slice_at != -1)
        {
            ClipRight(include_separator ? size() - slice_at : size() - slice_at - 1);
        }
        else
            Empty();
    }
}

void String::ClipSection(char separator, size_t first, size_t last,
		bool include_first_sep, bool include_last_sep)
{
    if (empty() || !separator) {
        return;
    }

    size_t slice_from;
    size_t slice_to;
    if (FindSection(separator, first, last, !include_first_sep, !include_last_sep,
        slice_from, slice_to))
    {
        ClipMid(slice_from, slice_to - slice_from);
    }
}

void String::FillString(char c, size_t count) {
	Empty();
	Reserve(count);

	for (size_t idx = 0; idx < count; ++idx)
		operator+=(c);
}

void String::Format(const char *fcstr, ...) {
    va_list argptr;
    va_start(argptr, fcstr);
	FormatV(fcstr, argptr);
    va_end(argptr);
}

void String::FormatV(const char *fcstr, va_list argptr) {
	::Common::String newString = ::Common::String::vformat(fcstr, argptr);
	*this = newString.c_str();
}

void String::Free() {
	clear();
}

void String::Prepend(const char *cstr) {
    if (cstr) {
		String tempString(cstr);
		tempString += *this;

		*this = tempString.c_str();
    }
}

void String::PrependChar(char c) {
    if (c) {
		String tempString(c, 1);
		tempString += *this;

		*this = tempString.c_str();
    }
}

void String::Replace(char what, char with) {
	for (uint idx = 0; idx < size(); ++idx) {
		if ((*this)[idx] == what)
			setChar(with, idx);
	}
}

void String::ReplaceMid(size_t from, size_t count, const char *cstr) {
    if (!cstr)
        cstr = "";

	String tempString;
	if (from < size())
		tempString = String(c_str(), from);

	tempString += cstr;

	from += count;
	if (from < size())
		tempString += c_str() + from;

	*this = tempString;
}

void String::Reverse() {
    if (size() <= 1)
        return;

	for (char *fw = _str, *bw = _str + size() - 1; *fw; ++fw, --bw)
		SWAP(*fw, *bw);
}

void String::SetString(const char *cstr, size_t length) {
	*this = String(cstr, cstr + length);
}

void String::Trim(char c)
{
    TrimLeft(c);
    TrimRight(c);
}

void String::TrimLeft(char c) {
	while (!empty() && Common::isSpace(firstChar()))
		deleteChar(0);
}

void String::TrimRight(char c) {
	while (!empty() && Common::isSpace(lastChar()))
		deleteLastChar();
}

void String::TruncateToLeft(size_t count) {
	String tempString(c_str(), c_str() + Math::Min(count, size()));
	*this = tempString;
}

void String::TruncateToMid(size_t from, size_t count)
{
    if (!empty()) {
        Math::ClampLength(from, count, (size_t)0, size());

		if (count != 0) {
			String tempString(c_str() + from, c_str() + from + count);
			*this = tempString;
		}
    }
}

void String::TruncateToRight(size_t count)
{
    if (!empty()) {
        count = Math::Min(count, GetLength());
        if (count < size()) {
			String tempString(c_str() + size() - count, c_str() + size());
			*this = tempString;
        }
    }
}

void String::TruncateToLeftSection(char separator, bool exclude_separator)
{
    if (!empty() && separator)
    {
        size_t slice_at = FindChar(separator);
        if (slice_at != -1)
        {
            TruncateToLeft(exclude_separator ? slice_at : slice_at + 1);
        }
    }
}

void String::TruncateToRightSection(char separator, bool exclude_separator)
{
    if (!empty() && separator)
    {
        size_t slice_at = FindCharReverse(separator);
        if (slice_at != -1)
        {
            TruncateToRight(exclude_separator ? size() - slice_at - 1 : size() - slice_at);
        }
    }
}

void String::TruncateToSection(char separator, size_t first, size_t last,
                          bool exclude_first_sep, bool exclude_last_sep)
{
    if (empty() || !separator)
    {
        return;
    }

    size_t slice_from;
    size_t slice_to;
    if (FindSection(separator, first, last, exclude_first_sep, exclude_last_sep,
        slice_from, slice_to))
    {
        TruncateToMid(slice_from, slice_to - slice_from);
    }
    else
    {
        Empty();
    }
}

} // End of namespace Shared
} // End of namespace AGS
