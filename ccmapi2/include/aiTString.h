/*
 * (c) Copyright 2006-2008 ActivIdentity
 * All Rights Reserved.
 *
 * This program is an unpublished copyrighted work which is proprietary
 * to ActivIdentity. This computer program includes Confidential,
 * Proprietary Information and is a Trade Secret of ActivIdentity.
 * Any use, disclosure, modification and/or reproduction is prohibited
 * unless authorized in writing by ActivIdentity.
 *
 * WARNING:  Unauthorized reproduction of this program as well as
 * unauthorized preparation of derivative works based upon the
 * program or distribution of copies by sale, rental, lease or
 * lending are violations of federal copyright laws and state trade
 * secret laws, punishable by civil and criminal penalties.
 * 
 * Created on May 5, 2006
 */
#pragma once
#pragma warning(disable:4231)	// disable nonstandard extern warning
#pragma warning(disable:4275)	//
#include "aiDllUtil.h"
#include "aiVector.h"
#include <tchar.h>

#ifdef AIINTERFACES_EXPORTS
#define AITSTRING_API __declspec(dllexport)
#define EXPTSTRING_TEMPLATE
#else
#define AITSTRING_API __declspec(dllimport)
#define EXPTSTRING_TEMPLATE extern
#endif

namespace AI
{
	class TString;
	AITSTRING_API const TString operator+(const TString& lhs, const TString& rhs);
	AITSTRING_API bool operator==(const TString& lhs, const TString& rhs);
	AITSTRING_API bool operator!=(const TString& lhs, const TString& rhs);
	AITSTRING_API bool operator>(const TString& lhs, const TString& rhs);
	AITSTRING_API bool operator<(const TString& lhs, const TString& rhs);
	AITSTRING_API bool operator>=(const TString& lhs, const TString& rhs);
	AITSTRING_API bool operator<=(const TString& lhs, const TString& rhs);

	/**
	 * The string class represents a sequence of character strings.  It also permits
	 * a number of utility functions to facilitate manipulation of its contents.
	 */
	class AITSTRING_API TString : public DLL_OBJECT_BASE
	{
	public:
		// constructors
		/**
		 * Default Constructor.
		 */
		TString();

		/**
		 * Constructor.
		 * @param data source character.
		 */
		TString(const TCHAR data);

		/**
		 * Constructor.
		 * @param data Null-terminated source string.
		 * @param nLength Indicates substring of source string to be used (-1 indicates entire string)
		 */
		TString(const TCHAR* data, int nLength=-1);

		/**
		 * Copy Constructor.
		 * @param src source string.
		 */
		TString(const TString &src);

		/**
		 * Destructor.
		 */
		~TString();

		// operators
		/**
		 * Addition Arithmetic Operator. (concatination)
		 * @param lhs String object to form beginning of resulting string.
		 * @param rhs String object to form end of resulting string.
		 * @return Resulting concatination of two strings.
		 */
		friend AITSTRING_API const TString operator+(const TString& lhs, const TString& rhs);

		/**
		 * Equality Comparison Operator.
		 * @param lhs String object for comparison.
		 * @param rhs String object for comparison.
		 * @return Result of comparison operation.
		 */
		friend AITSTRING_API bool operator==(const TString& lhs, const TString& rhs);

		/**
		 * Inequality Comparison Operator.
		 * @param lhs String object for comparison.
		 * @param rhs String object for comparison.
		 * @return Result of comparison operation.
		 */
		friend AITSTRING_API bool operator!=(const TString& lhs, const TString& rhs);

		/**
		 * Greater-than Comparison Operator.
		 * @param lhs String object for comparison.
		 * @param rhs String object for comparison.
		 * @return Result of comparison operation.
		 */
		friend AITSTRING_API bool operator>(const TString& lhs, const TString& rhs);

		/**
		 * Less-than Comparison Operator.
		 * @param lhs String object for comparison.
		 * @param rhs String object for comparison.
		 * @return Result of comparison operation.
		 */
		friend AITSTRING_API bool operator<(const TString& lhs, const TString& rhs);

		/**
		 * Greater-than or Equal Comparison Operator.
		 * @param lhs String object for comparison.
		 * @param rhs String object for comparison.
		 * @return Result of comparison operation.
		 */
		friend AITSTRING_API bool operator>=(const TString& lhs, const TString& rhs);

		/**
		 * Less-than or Equal Comparison Operator.
		 * @param lhs String object for comparison.
		 * @param rhs String object for comparison.
		 * @return Result of comparison operation.
		 */
		friend AITSTRING_API bool operator<=(const TString& lhs, const TString& rhs);

		/**
		 * Provides a reference to internal string-data with a specified index in a string.
		 * @param index Position within the string of character to be retrieved.
		 * @return Reference to requested location in string buffer.
		 * <p><b>Note: </b><i>Returned buffer points to internal location and must not be released</i></p>
		 */
		TCHAR& operator[](unsigned int index);

		/**
		 * Assignment Operator.
		 * @param rhs Replace contents of string with copy.
		 * @return Resulting string.
		 */
		TString& operator=(const TString& rhs);

		/**
		 * Append Operator.
		 * @param rhs TString contents to be appended to the string
		 * @return Resulting concatination.
		 */
		TString& operator+=(const TString& rhs);

		/**
		 * TCHAR* Conversion Operator.
		 * @return Null-terminated string with same content.
		 * <p><b>Note: </b><i>Returned buffer points to internal location and must not be released</i></p>
		 */
		operator const TCHAR*() const	{ return m_data;		}

		// mutators
		/**
		 * The string content is erased, leaving its size at 0 characters.
		 */
		void clear();

		/**
		 * Replace string content using C-style <code>printf</code>/<code>sprintf</code> specification fields.
		 * @param dataFormat A format control string.
		 * @param ... Optional additional field arguments.
		 * @return Resulting string.
		 */
		TString& format(const TCHAR* dataFormat, ...);

		/**
		 * Replace string content with provided source string.
		 * @param data Null-terminated source string.
		 * @param nCount Indicates substring of source string to be used (-1 indicates entire string)
		 * @return Resulting string.
		 */
		TString& assign(const TCHAR* data, int nCount=-1);

		/**
		 * Append string content with provided source string.
		 * @param data Null-terminated source string.
		 * @param nCount Indicates substring of source string to be used (-1 indicates entire string)
		 * @return Resulting string.
		 */
		TString& append(const TCHAR* data, int nCount=-1);

		/**
		 * Append to string content using C-style <code>printf</code>/<code>sprintf</code> specification fields
		 * @param dataFormat A format control string.
		 * @param ... Optional additional field arguments.
		 * @return Resulting string.
		 */
		TString& appendFormat(const TCHAR* dataFormat, ...);

		/**
		 * Replaces a substring pattern with another.
		 * @param pattern A pointer to a Null-terminated string containing pattern to locate.
		 * @param replace A pointer to a Null-terminated string containing replacement for pattern.
		 * @return Resulting string.
		 */
		TString& replace(const TCHAR* pattern, const TCHAR* replace);

		// accessors
		/**
		 * Searches for position of substring pattern.
		 * @param pattern A pointer to a Null-terminated string containing pattern to locate.
		 * @param pos The index position to begin searching from.
		 * @return Position index of first character in string matching pattern; -1 if not found.
		 */
		int find(const TCHAR* pattern, unsigned int pos=0);

		/**
		 * Performs a case sensitive comparison with a specified string to determine if the 
		 * two strings are equal or if one is lexicographically less than the other. 
		 * @param data The source string that is to be compared to the operand string
		 * @return Zero if string are identical, Negative if data string is less than, 
		 * or Positive if data string is greater than content string.
		 */
		int compare(const TString& data) const;

		/**
		 * Performs a case sensitive comparison with a specified string to determine if the 
		 * two strings are equal or if one is lexicographically less than the other. 
		 * @param offset The index of the operand string at which the comparison begins.
		 * @param count The maximum number of characters from the operand string to be compared; -1 indicates entire string.
		 * @param data The source string that is to be compared to the operand string
		 * @return Zero if string are identical, Negative if data string is less than, 
		 * or Positive if data string is greater than content string.
		 */
		int compare(unsigned int offset, unsigned int count, const TString& data) const;

		/**
		 * Performs a case insensitive comparison with a specified string to determine if the 
		 * two strings are equal or if one is lexicographically less than the other. 
		 * @param data The source string that is to be compared to the operand string
		 * @return Zero if string are identical, Negative if data string is less than, 
		 * or Positive if data string is greater than content string.
		 */
		int compareIgnoreCase(const TString& data) const;

		/**
		 * Performs a case insensitive comparison with a specified string to determine if the 
		 * two strings are equal or if one is lexicographically less than the other. 
		 * @param offset The index of the operand string at which the comparison begins.
		 * @param count The maximum number of characters from the operand string to be compared; -1 indicates entire string.
		 * @param data The source string that is to be compared to the operand string
		 * @return Zero if string are identical, Negative if data string is less than, 
		 * or Positive if data string is greater than content string.
		 */
		int compareIgnoreCase(unsigned int offset, unsigned int count, const TString& data) const;

		/**
		 * Return length of string.
		 * @return Count of the number of characters in the string.
		 */
		unsigned int length() const		{ return m_length;		}

		/**
		 * Return length of string.
		 * @return Count of the number of characters in the string.
		 */
		unsigned int size() const		{ return m_length;		}

		/**
		 * Generates a C-Style Null-terminated string. 
		 * @return Null-terminated string with same content.
		 * <p><b>Note: </b><i>Returned buffer points to internal location and must not be released</i></p>
		 */
		const TCHAR* c_str() const		{ return m_data;		}

		/**
		 * Generates a C-Style Null-terminated string. 
		 * @return Null-terminated string with same content.
		 * <p><b>Note: </b><i>Returned buffer points to internal location and must not be released</i></p>
		 */
		const TCHAR* data()	const		{ return m_data;		}

		/**
		 * Tests contents of string whether if empty (size 0).
		 * @return true if the string is empty, otherwise false.
		 */
		bool empty() const				{ return m_length==0;	}

		/**
		 * Generates a substring coresponding to portion of string.
		 * @param offset Position index to be used as starting character for the substring.
		 * @param count Length of substring to extract; -1 indicates remainder of string.
		 * @return substring
		 */
		TString substr(unsigned int offset=0, int count=-1) const;

		/**
		 * Returns character specified by position.
		 * @param pos Position index of desired character.
		 * @return character
		 */
		TCHAR at(unsigned int pos) const;

		/**
		 * Trim any leading whitespace from string
		 * @return string with spaces removed
		 */
		TString& ltrim();

		/**
		 * Trim any trailing whitespace from string
		 * @return string with spaces removed
		 */
		TString& rtrim();

		/**
		 * Trim any leading or trailing whitespace from string
		 * @return string with spaces removed
		 */
		TString& trim()		{ return ltrim().rtrim(); }

	private:
		TString& setData(TCHAR* data, int nCount=-1);
		TString& setDataCopy(const TCHAR* data, int nCount=-1);

		TCHAR*	m_data;
		unsigned int m_length;
	};

	EXPTSTRING_TEMPLATE template class AITSTRING_API Vector<TString>;
	typedef Vector<TString>				TStringVector;

} // namespace AI
namespace ActivIdentity = AI;