/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights 
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
#if !defined(XALANXMLCHAR_HEADER_GUARD_1357924680)
#define XALANXMLCHAR_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



#include <XalanDOM/XalanDOMString.hpp>



struct XALAN_PLATFORMSUPPORT_EXPORT XalanXMLChar
{

public:

	enum eType
	{
		XML_XX = 0,
		XML_BC = 1,
		XML_ID = 2,
		XML_EX = 3,
		XML_DI = 4,
		XML_CC = 5,
		XML_WS = 6
	};

    static const char theUnicodeTable[];

	static bool 
	isBaseChar(XalanDOMChar c)
	{
		return theUnicodeTable[c] == XML_BC;
	}

	static bool 
	isIdeographic(XalanDOMChar c)
	{
		return theUnicodeTable[c] == XML_ID;
	}

	static bool 
	isExtender(XalanDOMChar c) 
	{
		return theUnicodeTable[c] == XML_EX;
	}

	static bool 
	isDigit(XalanDOMChar c) 
	{
		return theUnicodeTable[c] == XML_DI;
	}

	static bool 
	isCombiningChar(XalanDOMChar c) 
	{
		return theUnicodeTable[c] == XML_CC;
	}

	static bool 
	isWhitespace(XalanDOMChar c) 
	{
		return theUnicodeTable[c] == XML_WS;
	}

	static bool 
	isLetter(XalanDOMChar c) 
	{
		return theUnicodeTable[c] == XML_BC || theUnicodeTable[c] == XML_ID;
	}


};

#endif	// XALANXMLCHAR_HEADER_GUARD_1357924680

