/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights 
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
#if !defined(XERCESBRIDGEHELPER_HEADER_GUARD_1357924680)
#define XERCESBRIDGEHELPER_HEADER_GUARD_1357924680



#include <XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <cassert>



#include <xercesc/dom/DOM_Node.hpp>



#include <XalanDOM/XalanDOMString.hpp>



#include <XercesParserLiaison/XercesBridgeNavigator.hpp>



class DOM_CharacterData;



class XALAN_XERCESPARSERLIAISON_EXPORT XercesBridgeHelper
{
public:

	typedef unsigned int	XercesStringLengthType;

	static const DOMString
	XalanDOMStringToXercesDOMString(const XalanDOMString&	theString)
	{
		assert(XercesStringLengthType(theString.length()) == theString.length());

		return DOMString(&theString[0], XercesStringLengthType(theString.length()));
	}

	static void
	setNodeValue(
			DOM_Node&				theXercesNode,
			const XalanDOMString&	nodeValue);

	static void
	normalize(DOM_Node&		theXercesNode);

	static bool
	isSupported(
			const DOM_Node&			theXercesNode,
			const XalanDOMString&	feature,
			const XalanDOMString&	version);

	static void
	setPrefix(
			DOM_Node&				theXercesNode,
			const XalanDOMString&	prefix);

	static const XalanDOMString
	substringData(
			const DOM_CharacterData&	theXercesNode,
			unsigned int				offset, 
			unsigned int				count);

	static void
	appendData(
			DOM_CharacterData&		theXercesNode,
			const XalanDOMString&	arg);

	static void
	insertData(
			DOM_CharacterData&		theXercesNode,
			unsigned int			offset,
			const  XalanDOMString& 	arg);

	static void
	deleteData(
			DOM_CharacterData&	theXercesNode,
			unsigned int		offset, 
			unsigned int		count);

	static void
	replaceData(
			DOM_CharacterData&		theXercesNode,
			unsigned int			offset, 
			unsigned int			count, 
			const XalanDOMString&	arg);
private:

	// Not implemented...
	XercesBridgeHelper();

	XercesBridgeHelper(const XercesBridgeHelper&);
};



#endif	// !defined(XERCESBRIDGEHELPER_HEADER_GUARD_1357924680)
