/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
#if !defined(DOMSTRINGHELPER_HEADER_GUARD_1357924680)
#define DOMSTRINGHELPER_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



#include <cassert>
#include <functional>
#if defined(XALAN_NO_IOSFWD)
#include <ostream>
#else
#include <iosfwd>
#endif
#include <vector>

#if defined(XALAN_LSTRSUPPORT)
#include <cwchar>
#endif

#if defined(AIX)
#include <wchar.h>
#elif defined(__GNUC__)
#include <wctype.h>
#endif


#include <XalanDOM/XalanDOMString.hpp>



#include <PlatformSupport/XalanUnicode.hpp>
#include <PlatformSupport/XalanXMLChar.hpp>



class XalanOutputStream;



// This macro has been defined to deal with certain C++ compilers which
// do not create Unicode strings when the "L" string constant prefix is
// used.  It is meant _only_ for use with static strings.
// It is _not_ designed to be thread-safe, because there will always be
// at least one global static transcoded string that will trigger the
// code at startup.
#if defined(XALAN_LSTRSUPPORT)

#define XALAN_STATIC_UCODE_STRING(str) L##str

#else

// Makes sure the Xerces platform is initialized, then
// transcodes the string.
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(const XalanDOMString)
initializeAndTranscode(const char*	theString);

#define XALAN_STATIC_UCODE_STRING(str) initializeAndTranscode(str)

#endif



/**
 * Initialize static data.  Must be called before any
 * other functions are called.  See PlatformSupportInit.
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
DOMStringHelperInitialize();



/**
 * Destroy static data.  After thus function is called,
 * no other functions can be called.  See PlatformSupportInit.
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
DOMStringHelperTerminate();



/**
 * Reserve some space in the string for more efficient
 * concatenation...
 * 
 * @param theString target string
 * @param theCount The amount of space to reserve
 */
inline void
reserve(
			XalanDOMString&		theString,
			unsigned int		theCount)
{
	theString.reserve(theCount);
}



/**
 * Get the underlying representation of the target XalanDOMString as a
 * null-terminated string
 * 
 * @param theString target string
 * @return null-terminated string of XalanDOMChar
 */
inline const XalanDOMChar*
c_wstr(const XalanDOMString&	theString)
{
	const XalanDOMChar* const	ptr = theString.rawBuffer();

	assert(!ptr || ptr[theString.length()] == '\0');

	return ptr;
}



/**
 * Get the underlying representation of the wide string as a
 * UNICODE null-terminated string.  This is here simply for
 * consistency in the code.  On certain platforms, compiler-
 * generated wide strings will not contain Unicode code
 * points.  Another macro converts those into XalanDOMStrings,
 * which are then transcoded.  In these cases, the previous
 * defined c_sstr() function gets called.
 *
 * On platforms where the compiler does generate Unicode wide
 * strings, this function will be called instead.
 * 
 * @param theString target string
 * @return null-terminated string of XalanDOMChar
 */
inline const XalanDOMChar*
c_wstr(const XalanDOMChar*	theString)
{
	return theString;
}



/**
 * Get the underlying representation of the target XalanDOMString as an array of
 * XalanDOMChar, not guaranteed to be null-terminated.
 * 
 * @param theString target string
 * @return array of XalanDOMChar
 */
inline const XalanDOMChar*
toCharArray(const XalanDOMString&	theString)
{
	return theString.rawBuffer();
}



/**
 * Simulates the java String method length() for a XalanDOMString
 * 
 * @param theDOMString target string
 * @return the length of the target string
 */
inline unsigned int
length(const XalanDOMString&	theString)
{
	return theString.length();
}



/**
 * Simulates the java String method length() for a null-terminated buffer of
 * XalanDOMChar characters
 * 
 * @param theBuffer target string
 * @return the length of the target string
 */
inline unsigned int
length(const XalanDOMChar*	theBuffer)
{
	assert(theBuffer != 0);

	// For the time being, we're using our own custom routine,
	// since performance is better.
#if defined(XALAN_USE_WCHAR_SUPPORT)
	return wcslen(theBuffer);
#else
	const XalanDOMChar*		theBufferPointer = theBuffer;

	while(*theBufferPointer != 0)
	{
		theBufferPointer++;
	}

	return theBufferPointer - theBuffer;
#endif
}



/**
 * Determines if the target string contains any elements
 * 
 * @param str target string
 * @return true if the target string has a non-zero length
 */
inline bool 
isEmpty(const XalanDOMString&	str)
{
	return length(str) == 0 ? true : false; 
}



/**
 * Simulates the java String method indexOf().
 * 
 * @param theString string to search
 * @param theChar   character searched for
 * @return the index of theChar in theString,
 * or length(theString) if the character is not
 * found.    
 */
inline unsigned int
indexOf(
			const XalanDOMChar*		theString,
			XalanDOMChar			theChar)
{
	// For the time being, we're using our own custom routine,
	// since performance is better.
#if defined(XALAN_USE_WCHAR_SUPPORT)

	const XalanDOMChar* const	thePointer =
			wcschr(theString, theChar);

	if (thePointer == 0)
	{
		return length(theString);
	}
	else
	{
		return thePointer - theString;
	}

#else

	const XalanDOMChar*		thePointer = theString;

	while(*thePointer != theChar && *thePointer != 0)
	{
		++thePointer;
	}

	return thePointer - theString;

#endif
}



/**
 * Simulates the java String method indexOf().
 * 
 * @param theString string to search
 * @param theChar   character searched for
 * @return the index of theChar in theString,
 * or length(theString) if the character is not
 * found.    
 */
inline unsigned int
indexOf(
			const XalanDOMString&	theString,
			XalanDOMChar			theChar)
{
	if (theString.length() == 0)
	{
		return 0;
	}
	else
	{
		return indexOf(c_wstr(theString), theChar);
	}
}



/**
 * Simulates the java String method indexOf().
 * 
 * @param theString string to search
 * @param theSubstring substring searched for
 * @return the index of theSubstring in theString, 
 * or length(theString) if the string is not
 * found.
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(unsigned int)
indexOf(
			const XalanDOMChar*		theString,
			const XalanDOMChar*		theSubstring);


/**
 * Simulates the java String method indexOf().
 * 
 * @param theString string to search
 * @param theSubstring substring searched for
 * @return the index of theSubstring in theString, 
 * or length(theString) if the string is not
 * found.
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(unsigned int)
indexOf(
			const XalanDOMString&	theString,
			const XalanDOMString&	theSubstring);



/**
 * Simulates the java String method lastIndexOf().
 * 
 * @param theString string to search
 * @param theChar   character searched for
 * @return the index of theChar in theString,
 * or length(theString) if the character is not
 * found.
 */

// For the time being, we're using our own custom routine,
// since performance is better.
#if defined(XALAN_USE_WCHAR_SUPPORT)

inline unsigned int
lastIndexOf(
			const XalanDOMChar*		theString,
			XalanDOMChar			theChar)
{
	const XalanDOMChar* const	thePointer =
			wcsrchr(theString, theChar);

	if (thePointer == 0)
	{
		return length(theString);
	}
	else
	{
		return thePointer - theString;
	}
}

#else

XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(unsigned int)
lastIndexOf(
			const XalanDOMChar*		theString,
			XalanDOMChar			theChar);

#endif

/**
 * Simulates the java String method lastIndexOf().
 * 
 * @param theString string to search
 * @param theChar   character searched for
 * @return the index of theChar in theString,
 * or length(theString) if the character is not
 * found.
 */
inline unsigned int
lastIndexOf(
			const XalanDOMString&	theString,
			XalanDOMChar			theChar)
{
	return lastIndexOf(c_wstr(theString), theChar);
}



/**
 * Simulates the java String method startsWith().
 * 
 * @param theDOMString target string to search
 * @param theSubstring substring searched for
 * @return true if the target string begins with the substring
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
startsWith(
			const XalanDOMChar*		theString,
			const XalanDOMChar*		theSubstring);



/**
 * Simulates the java String method startsWith().
 * 
 * @param theDOMString target string to search
 * @param theSubstring substring searched for
 * @return true if the target string begins with the substring
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
startsWith(
			const XalanDOMString&	theDOMString,
			const XalanDOMString&	theSubstring);



/**
 * Simulates the java String method startsWith().
 * 
 * @param theDOMString target string to search
 * @param theSubstring substring searched for
 * @return true if the target string begins with the substring
 */
inline bool
startsWith(
			const XalanDOMString&	theDOMString,
			const char*				theSubstring)
{
	return startsWith(theDOMString,
					  XalanDOMString(theSubstring));
}


/**
 * Simulates the java String method endsWith().
 * 
 * @param theDOMString target string to search
 * @param theSubstring substring searched for
 * @return true if the target string ends with the substring
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
endsWith(
			const XalanDOMChar*		theString,
			const XalanDOMChar*		theSubstring);



/**
 * Simulates the java String method endsWith().
 * 
 * @param theDOMString target string to search
 * @param theSubstring substring searched for
 * @return true if the target string ends with the substring
 */
inline bool
endsWith(
			const XalanDOMString&	theDOMString,
			const XalanDOMString&	theSubstring)
{
	return endsWith(c_wstr(theDOMString), c_wstr(theSubstring));
}



/**
 * Converts a double value into a XalanDOMString
 * 
 * @param theDouble number to be converted
 * @return decimal string representation of the number
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
DoubleToDOMString(double	theDouble);



/**
 * Converts a long value into a XalanDOMString
 * 
 * @param theInt number to be converted
 * @return hexadecimal string representation of the number
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
LongToHexDOMString(long		theLong);



/**
 * Converts a long value into a XalanDOMString
 * 
 * @param theInt number to be converted
 * @return hexadecimal string representation of the number
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
UnsignedLongToHexDOMString(unsigned long	theUnsignedLong);



/**
 * Converts a long value into a XalanDOMString
 * 
 * @param theInt number to be converted
 * @return decimal string representation of the number
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
LongToDOMString(long	theLong);



/**
 * Converts an unsigned long value into a XalanDOMString
 * 
 * @param theInt number to be converted
 * @return decimal string representation of the number
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
UnsignedLongToDOMString(unsigned long	theInt);



/**
 * Converts a wide string into an integer value
 * 
 * @param theString target string
 * @return integer value of target string
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
WideStringToInt(const XalanDOMChar*		theString);



/**
 * Converts a wide string into a long value
 * 
 * @param theString target string
 * @return long value of target string
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(long)
WideStringToLong(const XalanDOMChar*	theString);



/**
 * Converts a wide string into an unsigned long value
 * 
 * @param theString target string
 * @return unsigned long value of target string
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(unsigned long)
WideStringToUnsignedLong(const XalanDOMChar*	theString);



/**
 * Converts a wide string into a double value
 * 
 * @param theString target string
 * @return double value of target string
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(double)
WideStringToDouble(const XalanDOMChar*	theString);



/**
 * Converts a XalanDOMString into an integer value
 * 
 * @param theString target string
 * @return integer value of target string
 */
inline int
DOMStringToInt(const XalanDOMString&	theString)
{
	return WideStringToInt(c_wstr(theString));
}



/**
 * Converts a XalanDOMString into a long value
 * 
 * @param theString target string
 * @return long value of target string
 */
inline long
DOMStringToLong(const XalanDOMString&	theString)
{
	return WideStringToLong(c_wstr(theString));
}



/**
 * Converts a XalanDOMString into a long value
 * 
 * @param theString target string
 * @return unsigned long value of target string
 */
inline unsigned long
DOMStringToUnsignedLong(const XalanDOMString&	theString)
{
	return WideStringToUnsignedLong(c_wstr(theString));
}



/**
 * Converts a XalanDOMString into a double value
 * 
 * @param theString target string
 * @return double value of target string
 */
inline double
DOMStringToDouble(const XalanDOMString&		theString)
{
	return WideStringToDouble(c_wstr(theString));
}



// Standard vector of XalanDOMChars and chars
#if defined(XALAN_NO_NAMESPACES)
typedef vector<XalanDOMChar>		XalanDOMCharVectorType;

typedef vector<char>				CharVectorType;
#else
typedef std::vector<XalanDOMChar>	XalanDOMCharVectorType;

typedef std::vector<char>			CharVectorType;
#endif



/**
 * Get the underlying representation of the target CharVectorType as a
 * null-terminated string
 * 
 * @param theString target string
 * @return null-terminated string of chars
 */
inline const char*
c_str(const CharVectorType&		theString)
{
	const char* const	ptr = &theString[0];

	assert(!ptr || ptr[theString.size() - 1] == '\0');

	return ptr;
}



/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @see operator<<
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
OutputString(
			XalanOutputStream&		theStream,
			const CharVectorType&	theString);



/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @see operator<<
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
OutputString(
#if defined(XALAN_NO_NAMESPACES)
			ostream&				theStream,
#else
			std::ostream&			theStream,
#endif
			const CharVectorType&	theString);



/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @see operator<<
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
OutputString(
			XalanOutputStream&		theStream,
			const XalanDOMChar*		theString);



/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @see operator<<
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
OutputString(
#if defined(XALAN_NO_NAMESPACES)
			ostream&				theStream,
#else
			std::ostream&			theStream,
#endif
			const XalanDOMChar*		theString);



/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @see operator<<
 */
inline void
OutputString(
			XalanOutputStream&		theStream,
			const XalanDOMString&	theString)
{
	if (isEmpty(theString) == false)
	{
		OutputString(theStream, c_wstr(theString));
	}
}



/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @see operator<<
 */
inline void
OutputString(
#if defined(XALAN_NO_NAMESPACES)
			ostream&				theStream,
#else
			std::ostream&			theStream,
#endif
			const XalanDOMString&	theString)
{
	OutputString(theStream, c_wstr(theString));
}



/**
 * Outputs the string to the specified stream
 * 
 * @param theStream output stream
 * @param theString the string to output
 * @see OutputString
 */
inline XalanOutputStream&
operator<<(
			XalanOutputStream&		theStream,
			const CharVectorType&	theString)
{
	OutputString(theStream, theString);

	return theStream;
}



/**
 * Outputs the string to the specified stream
 * 
 * @param theStream output stream
 * @param theString the string to output
 * @see OutputString
 */
#if defined(XALAN_NO_NAMESPACES)
inline ostream&
operator<<(
			ostream&				theStream,
#else
inline std::ostream&
operator<<(
			std::ostream&			theStream,
#endif
			const CharVectorType&	theString)
{
	OutputString(theStream, theString);

	return theStream;
}



/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @see OutputString
 */
inline XalanOutputStream&
operator<<(
			XalanOutputStream&		theStream,
			const XalanDOMChar*		theString)
{
	OutputString(theStream,
				 theString);

	return theStream;
}



/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @see OutputString
 */
#if defined(XALAN_NO_NAMESPACES)
inline ostream&
operator<<(
			ostream&				theStream,
#else
inline std::ostream&
operator<<(
			std::ostream&			theStream,
#endif
			const XalanDOMChar*		theString)
{
	OutputString(theStream,
				 theString);

	return theStream;
}



/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @see OutputString
 */
inline XalanOutputStream&
operator<<(
			XalanOutputStream&		theStream,
			const XalanDOMString&	theString)
{
	OutputString(theStream,
				 theString);

	return theStream;
}



/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @see OutputString
 */
#if defined(XALAN_NO_NAMESPACES)
inline ostream&
operator<<(
			ostream&				theStream,
#else
inline std::ostream&
operator<<(
			std::ostream&			theStream,
#endif
			const XalanDOMString&	theString)
{
	OutputString(theStream,
				 theString);

	return theStream;
}



/**
 * Creates a copy of the target string
 * 
 * @param theString target string
 * @return copy of string
 */
inline XalanDOMString
clone(const XalanDOMString&	theString)
{
	return theString.clone();
}



/**
 * Determines whether character represents white space
 * 
 * @param theChar target character
 * @return true if character represents white space
 */
inline bool
isXMLWhitespace(XalanDOMChar	theChar)
{
	return XalanXMLChar::isWhitespace(theChar) ? true : false;
}



/**
 * Retrieves a character at a specified index in the target string
 * 
 * @param theString target string
 * @param theIndex  index of character
 * @return character at specified index
 */
inline XalanDOMChar
charAt(
			const XalanDOMString&	theString,
			unsigned int			theIndex)
{
	return theString.charAt(theIndex);
}



/**
 * Determines whether character represents a digit
 * 
 * @param theChar target character
 * @return true if character represents a digit
 */
inline bool
isXMLDigit(XalanDOMChar	theChar)
{	
	return XalanXMLChar::isDigit(theChar) ? true : false;
}



/**
 * Determines whether character represents a letter or digit
 * 
 * @param theChar target character
 * @return true if character represents a letter or digit
 */
inline bool
isXMLLetterOrDigit(XalanDOMChar	theChar)
{
	return	XalanXMLChar::isDigit(theChar) || 
			XalanXMLChar::isLetter(theChar) ? true : false;
}



/**
 * Simulates the java String method substring(). Returns a new string that is
 * a substring of this string. The substring begins at the specified
 * theStartIndex and extends to the character at index theEndIndex - 1. Thus
 * the length of the substring is theEndIndex-theStartIndex.
 * 
 * @param theString     target string
 * @param theStartIndex starting index, inclusive
 * @param theEndIndex   ending index, exclusive
 * @return string containing the specified range of characters from target
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
substring(
			const XalanDOMChar*		theString,
			unsigned int			theStartIndex,
			unsigned int			theEndIndex = UINT_MAX);



/**
 * Simulates the java String method substring(). Returns a new string that is
 * a substring of this string. The substring begins at the specified
 * theStartIndex and extends to the character at index theEndIndex - 1. Thus
 * the length of the substring is theEndIndex-theStartIndex.
 * 
 * @param theString     target string
 * @param theStartIndex starting index, inclusive
 * @param theEndIndex   ending index, exclusive
 * @return string containing the specified range of characters from target
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
substring(
			const XalanDOMString&	theString,
			unsigned int			theStartIndex,
			unsigned int			theEndIndex = UINT_MAX);



/**
 * Simulates the java String method toLowerCase(). Returns a new string
 * containing only lower case characters of target string.
 * 
 * @param theString target string
 * @return string containing lower case characters
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
toLowerCase(const XalanDOMString&	theString);



/**
 * Simulates the java String method toUpperCase(). Returns a new string
 * containing only upper case characters of target string.
 * 
 * @param theString target string
 * @return string containing upper case characters
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
toUpperCase(const XalanDOMString&	theString);



#if !defined(XALAN_AMBIGUOUS_EVEN_IF_NOT_CALLED)
// These two function are specifically not defined, and
// should produce ambiguity during compilation.  This
// is necessary because the Xerces XalanDOMString class
// defines == as referring to the same underlying
// handle, not identical strings, as C++ programmers
// would expect.
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
operator==(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS);



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
operator!=(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS);
#endif



/**
 * Compare the contents of two strings.
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns 0 for equal strings, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 * @see operator<()
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
compare(
			const CharVectorType&	theLHS,
			const CharVectorType&	theRHS);



// For the time being, we're using our own custom routine,
// since performance is better.

#if defined(XALAN_USE_WCHAR_SUPPORT)

/**
 * Compare the contents of two strings.
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns 0 for equal strings, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 * @see operator<
 * @see collationCompare
 */
inline int
compare(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS)
{
	return wcscmp(theLHS, theRHS);
}

#else

XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
compare(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS);

#endif



/**
 * Compare the contents of two strings, in a case insensitive
 * manner
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns 0 for equal strings, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 * @see operator<
 * @see collationCompare
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
compareIgnoreCase(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS);



#if defined(XALAN_USE_WCHAR_SUPPORT)

/**
 * Compare the contents of two strings using the
 * the collation settings of the current code page.
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns 0 for equal strings, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 * @see operator<()
 * @see compare()
 */
inline int
collationCompare(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS)
{
	return wcscoll(theLHS, theRHS);
}

#else

// Can't really do it, so just call compare...
inline int
collationCompare(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS)
{
	return compare(theLHS, theRHS);
}

#endif



/**
 * Compare the contents of two strings.
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns 0 for equal strings, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 * @see operator<()
 * @see collationCompare()
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
compare(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS);


/**
 * Compare the contents of two strings, in a case insensitive
 * manner
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns 0 for equal strings, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 * @see operator<
 * @see collationCompare
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
compareIgnoreCase(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS);



/**
 * Compare the contents of two strings using the
 * the collation settings of the current code page.
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns 0 for equal strings, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 * @see operator<()
 * @see compare()
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
collationCompare(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS);



/**
 * Compare the contents of two strings for equality
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the contents of both strings are identical
 */
inline bool
equals(const XalanDOMChar*	theLHS,
	   const XalanDOMChar*	theRHS)
{
	return compare(theLHS, theRHS) == 0 ? true : false;
}



/**
 * Compare the contents of two strings for equality
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the contents of both strings are identical
 */
inline bool
equals(const XalanDOMChar*		theLHS,
	   const XalanDOMString&	theRHS)
{
	assert(theLHS != 0);

	return theRHS.equals(theLHS);
}



/**
 * Compare the contents of two strings for equality
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the contents of both strings are identical
 */
inline bool
equals(const XalanDOMString&	theLHS,
	   const XalanDOMChar*		theRHS)
{
	assert(theRHS != 0);

	return theLHS.equals(theRHS);
}



/**
 * Compare the contents of two strings for equality
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the contents of both strings are identical
 */
inline bool
equals(const XalanDOMString&	theLHS,
	   const char*				theRHS)
{
	assert(theRHS != 0);

	return theLHS.equals(theRHS) ? true : false;
}



/**
 * Compare the contents of two strings for equality
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the contents of both strings are identical
 */
inline bool
equals(const XalanDOMChar*	theLHS,
	   const char*			theRHS)
{
	return equals(theLHS, XalanDOMString(theRHS));
}



/**
 * Compare the contents of two strings for equality
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the contents of both strings are identical
 */
inline bool
equals(const char*			theLHS,
	   const XalanDOMChar*	theRHS)
{
	assert(theLHS != 0);
	assert(theRHS != 0);

	return equals(XalanDOMString(theLHS), theRHS);
}



/**
 * Compare the contents of two strings for equality
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the contents of both strings are identical
 */
inline bool
equals(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS)
{
	return theLHS.equals(theRHS) ? true : false;
}



/**
 * Compare the contents of two strings for equality, without regard for case
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the case-insensitive contents of both strings are identical
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
equalsIgnoreCase(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS);



/**
 * Compare the contents of two strings for equality, without regard for case
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the case-insensitive contents of both strings are identical
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
equalsIgnoreCase(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS);



/**
 * Implements operator< for CharVectorType.
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns true if theLHS is lexically
 * less than theRHS
 * @see compare
 */
inline bool
operator<(
			const CharVectorType&	theLHS,
			const CharVectorType&	theRHS)
{
	return compare(theLHS, theRHS) < 0 ? true : false;
}



/**
 * Implements operator< for DOMStrings.
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns true if theLHS is lexically
 * less than theRHS
 * @see compare
 */
inline bool
operator<(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS)
{
	return compare(theLHS, theRHS) < 0 ? true : false;
}



/**
 * Concatenate two strings
 * 
 * @param theString         target string
 * @param theStringToAppend string to add to target
 * @return string with contents of 'theStringToAppend' added to target string
 */
inline XalanDOMString&
append(
			XalanDOMString&			theString,
			const XalanDOMString&	theStringToAppend)
{
	theString.appendData(theStringToAppend);

	return theString;
}



/**
 * Remove trailing whitespace
 * 
 * @param theString         target string
 * @return string with contents of target string less trailing whitespace
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
trim(const XalanDOMString&	theString);



/**
 * Remove all elements from target string
 * 
 * @param theString target string
 */
inline void
clear(XalanDOMString&	theString)
{
#if defined(XALAN_OLD_STYLE_CASTS)
	theString = (DOM_NullPtr*)0;
#else
	theString = static_cast<DOM_NullPtr*>(0);
#endif
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
CopyWideStringToVector(
			const XalanDOMChar*		theString,
			CharVectorType&			theVector);



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
CopyStringToVector(
			const char*			theString,
			CharVectorType&		theVector);



/**
 * Get a pointer to the first element of the vector as
 * a null-terminated string
 *
 * @param theSVector target vector
 * @return null-terminated string of XalanDOMChar
 */
inline const XalanDOMChar*
c_wstr(const XalanDOMCharVectorType&	theVector)
{
	return &theVector[0];
}



/**
 * Compare the contents of two vectors for equality
 * 
 * @param theLHS first vector to compare
 * @param theRHS second vector to compare
 * @return true if the contents of both vectors are identical
 */
inline bool
equals(
			const XalanDOMCharVectorType&	theLHS,
			const XalanDOMCharVectorType&	theRHS)
{
	return theLHS == theRHS;
}



/**
 * Compare the contents of two strings for equality
 * 
 * @param theLHS XalanDOMCharVectorType to compare
 * @param theRHS string to compare
 * @return true if the contents of are identical
 */
inline bool
equals(
			const XalanDOMCharVectorType&	theLHS,
			const XalanDOMChar*				theRHS)
{
	return equals(c_wstr(theLHS), theRHS);
}



/**
 * Compare the contents of two strings for equality
 * 
 * @param theLHS string to compare
 * @param theRHS XalanDOMCharVectorType to compare
 * @return true if the contents are identical
 */
inline bool
equals(
			const XalanDOMChar*				theLHS,
			const XalanDOMCharVectorType&	theRHS)
{
	return equals(theLHS, c_wstr(theRHS));
}



/**
 * Compare the contents of a XalanDOMCharVectorType
 * and a XalanDOMString for equality
 * 
 * @param theLHS XalanDOMCharVectorType to compare
 * @param theRHS XalanDOMString to compare
 * @return true if the contents of both are identical
 */
inline bool
equals(
			const XalanDOMCharVectorType&	theLHS,
			const XalanDOMString&			theRHS)
{
	return equals(c_wstr(theLHS), c_wstr(theRHS));
}



/**
 * Compare the contents of a XalanDOMString and a
 * XalanDOMCharVectorType for equality
 * 
 * @param theLHS XalanDOMString to compare
 * @param theRHS XalanDOMCharVectorType to compare
 * @return true if the contents of both are identical
 */
inline bool
equals(
			const XalanDOMString&			theLHS,
			const XalanDOMCharVectorType&	theRHS)
{
	return equals(c_wstr(theLHS), c_wstr(theRHS));
}



/**
 * Compare the contents of two XalanDOMCharVectorTypes.
 * 
 * @param theLHS first vector to compare
 * @param theRHS second vector to compare
 * @return Returns 0 for equal vectors, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 * @see operator<
 * @see collationCompare
 */
inline int
compare(
			const XalanDOMCharVectorType&	theLHS,
			const XalanDOMCharVectorType&	theRHS)
{
	return compare(&theLHS[0], &theRHS[0]);
}



/**
 * Compare the contents of two XalanDOMCharVectorTypes, in a case insensitive
 * manner
 * 
 * @param theLHS first vector to compare
 * @param theRHS second vector to compare
 * @return Returns 0 for equal vectors, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 * @see operator<
 * @see collationCompare
 */
inline int
compareIgnoreCase(

			const XalanDOMCharVectorType&	theLHS,
			const XalanDOMCharVectorType&	theRHS)
{
	return compareIgnoreCase(&theLHS[0], &theRHS[0]);
}



/**
 * Compare the contents of two vectors using the
 * the collation settings of the current code page.
 * 
 * @param theLHS first vector to compare
 * @param theRHS second vector to compare
 * @return Returns 0 for equal vectors, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 * @see operator<()
 * @see compare()
 */
inline int
collationCompare(
			const XalanDOMCharVectorType&	theLHS,
			const XalanDOMCharVectorType&	theRHS)
{
	return collationCompare(&theLHS[0], &theRHS[0]);
}



/**
 * Implements operator< for XalanDOMCharVectorType.
 * 
 * @param theLHS first vector to compare
 * @param theRHS second vector to compare
 * @return Returns true if theLHS is lexically
 * less than theRHS
 * @see compare
 */
inline bool
operator<(
			const XalanDOMCharVectorType&	theLHS,
			const XalanDOMCharVectorType&	theRHS)
{
	return compare(theLHS, theRHS) < 0 ? true : false;
}



/**
 * Utility function to make a null-terminated vector of XMLChs, from a
 * null-terminated array of chars, via transcoding, if requested.
 * 
 * @param data array to be converted
 * @param whether or not to transcode
 * @return null-terminated vector of XalanDOMChar
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMCharVectorType)
MakeXalanDOMCharVector(
			const char*		data,
			bool			fTranscode = true);



/**
 * Utility function to make a null-terminated vector of XMLChs, from a
 * null-terminated array of XalanDOMChar.
 * 
 * @param data array to be converted
 * @return null-terminated vector of XalanDOMChar
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMCharVectorType)
MakeXalanDOMCharVector(const XalanDOMChar*	data);



/**
 * Utility function to make a null-terminated vector of XMLChs, from a
 * XalanDOMString
 * 
 * @param data XalanDOMString to be converted
 * @return null-terminated vector of XalanDOMChar
 */
inline XalanDOMCharVectorType
MakeXalanDOMCharVector(const XalanDOMString&	data)
{
	return MakeXalanDOMCharVector(c_wstr(data));
}



#if defined(XALAN_NO_NAMESPACES)
struct c_wstr_functor : public unary_function<XalanDOMString, const XalanDOMChar*>
#else
struct c_wstr_functor : public std::unary_function<XalanDOMString, const XalanDOMChar*>
#endif
{
	result_type
	operator() (const argument_type&	theString) const
	{
		return c_wstr(theString);
	}
};



/**
 * Hash functor for DOMStrings
 * 
 * @param theKey XalanDOMString to be hashed
 * @return hash value for XalanDOMString
 */
#if defined(XALAN_NO_NAMESPACES)
struct DOMStringHashFunction : public unary_function<const XalanDOMString&, size_t>
#else
struct DOMStringHashFunction : public std::unary_function<const XalanDOMString&, size_t>
#endif
{
	result_type
	operator() (argument_type	theKey) const
	{
		const XalanDOMChar*		theRawBuffer = c_wstr(theKey);

		unsigned long		theHashValue = 0L; 

		if (theRawBuffer != 0)
		{
			while (*theRawBuffer)
			{
				theHashValue = 5 * theHashValue + *theRawBuffer;

				theRawBuffer++;
			}
		}

		return result_type(theHashValue++);
	}
};



/**
 * Equals functor for DOMStrings
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the contents of both strings are identical
 */
#if defined(XALAN_NO_NAMESPACES)
struct DOMStringEqualsFunction : public binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#else
struct DOMStringEqualsFunction : public std::binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#endif
{
	result_type
	operator() (first_argument_type		theLHS,
				second_argument_type	theRHS) const
	{
		return equals(theLHS, theRHS);
	}
};



/**
 * Case-insensitive equals functor for DOMStrings
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the contents of both strings are equal, without respect to case
 */
#if defined(XALAN_NO_NAMESPACES)
struct DOMStringEqualsIgnoreCaseFunction : public binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#else
struct DOMStringEqualsIgnoreCaseFunction : public std::binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#endif
{
	result_type
	operator() (first_argument_type		theLHS,
				second_argument_type	theRHS) const
	{
		return equalsIgnoreCase(theLHS, theRHS);
	}
};



/**
 * Not equals functor for DOMStrings
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the contents of both strings are identical
 */
#if defined(XALAN_NO_NAMESPACES)
struct DOMStringNotEqualsFunction : public binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#else
struct DOMStringNotEqualsFunction : public std::binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#endif
{
	result_type
	operator() (first_argument_type		theLHS,
				second_argument_type	theRHS) const
	{
		return !equals(theLHS, theRHS);
	}
};



/**
 * Less than functor for DOMStrings
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the theLHS is less than theRHSl
 */
#if defined(XALAN_NO_NAMESPACES)
struct DOMStringLessThanFunction : public binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#else
struct DOMStringLessThanFunction : public std::binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#endif
{
	result_type
	operator() (first_argument_type		theLHS,
				second_argument_type	theRHS) const
	{
		return compare(theLHS, theRHS) < 0 ? true : false;
	}
};



/**
 * Less than functor for DOMStrings which ignores case
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the theLHS is less than theRHS, without respect to case.
 */
#if defined(XALAN_NO_NAMESPACES)
struct DOMStringLessThanIgnoreCaseFunction : public binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#else
struct DOMStringLessThanIgnoreCaseFunction : public std::binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#endif
{
	result_type
	operator() (first_argument_type		theLHS,
				second_argument_type	theRHS) const
	{
		return compareIgnoreCase(theLHS, theRHS) < 0 ? true : false;
	}
};



/**
 * Less than or equal functor for DOMStrings
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the theLHS is less than or equal to theRHS
 */
#if defined(XALAN_NO_NAMESPACES)
struct DOMStringLessThanOrEqualFunction : public binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#else
struct DOMStringLessThanOrEqualFunction : public std::binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#endif
{
	result_type
	operator() (first_argument_type		theLHS,
				second_argument_type	theRHS) const
	{
		return compare(theLHS, theRHS) <= 0 ? true : false;
	}
};



/**
 * Greater than functor for DOMStrings
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the theLHS is greater than theRHS
 */
#if defined(XALAN_NO_NAMESPACES)
struct DOMStringGreaterThanFunction : public binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#else
struct DOMStringGreaterThanFunction : public std::binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#endif
{
	result_type
	operator() (first_argument_type		theLHS,
				second_argument_type	theRHS) const
	{
		return compare(theLHS, theRHS) > 0 ? true : false;
	}
};



/**
 * Greater than or equal functor for DOMStrings
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the theLHS is greater than or equal to theRHS
 */
#if defined(XALAN_NO_NAMESPACES)
struct DOMStringGreaterThanOrEqualFunction : public binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#else
struct DOMStringGreaterThanOrEqualFunction : public std::binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#endif
{
	result_type
	operator() (first_argument_type		theLHS,
				second_argument_type	theRHS) const
	{
		return compare(theLHS, theRHS) >= 0 ? true : false;
	}
};



/**
 * Convert a XalanDOMChar string to C++ standard library
 * vector, transcoding to the default local code
 * page.
 * 
 * @param sourceString The source string
 * @param sourceStringLength The source string length.
 * @param targetVector The target string
 * @param terminate If true, the transcoded string will be null-terminated
 * @return true if successful, false if not.
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
TranscodeToLocalCodePage(
			const XalanDOMChar*		sourceString,
			unsigned int			sourceStringLength,
			CharVectorType&			targetVector,
			bool					terminate = false);



/**
 * Convert a XalanDOMChar string to C++ standard library
 * vector, transcoding to the default local code
 * page.  The string _must_ be null-terminated.
 * 
 * @param sourceString The source string
 * @param targetVector The target string
 * @param terminate If true, the transcoded string will be null-terminated
 * @return true if successful, false if not.
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
TranscodeToLocalCodePage(
			const XalanDOMChar*		sourceString,
			CharVectorType&			targetVector,
			bool					terminate = false);



/**
 * Convert XalanDOMString to C++ standard library
 * vector, transcoding to the default local code
 * page.  Null-terminate the sttring...
 *
 * @param theSourceString source string
 * @return The transcoded string.
 */
inline const CharVectorType
TranscodeToLocalCodePage(const XalanDOMChar*	sourceString)
{
	CharVectorType	theResult;

	TranscodeToLocalCodePage(sourceString, theResult, true);

	return theResult;
}



/**
 * Convert XalanDOMString to C++ standard library
 * vector, transcoding to the default local code
 * page.
 * 
 * @param sourceString The source string
 * @param targetVector The target string
 * @return true if successful, false if not.
 */
inline bool
TranscodeToLocalCodePage(
			const XalanDOMString&	sourceString,
			CharVectorType&			targetVector,
			bool					terminate = false)
{
	return TranscodeToLocalCodePage(c_wstr(sourceString), targetVector, terminate);
}



/**
 * Convert XalanDOMString to C++ standard library
 * vector, transcoding to the default local code
 * page.
 *
 * @param theSourceString source string
 * @return The transcoded string.
 */
inline const CharVectorType
TranscodeToLocalCodePage(const XalanDOMString&	sourceString)
{
	CharVectorType	theResult;

	TranscodeToLocalCodePage(sourceString, theResult, true);

	return theResult;
}



/**
 * Determines if the string contains only whitespace
 * 
 * @param theString target string
 * @return true if string contains only whitespace
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
isWhitespace(const XalanDOMString&	string);



/**
 * Determines if a range in an array contains only whitespace
 * 
 * @param ch target array
 * @param start starting index to examine
 * @param length number of characters to examine
 * @return true if specified range contains only whitespace
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
isWhitespace(
			const XalanDOMChar*		ch,
			unsigned int			start,
			unsigned int			length);



#endif	// DOMSTRINGHELPER_HEADER_GUARD_1357924680
