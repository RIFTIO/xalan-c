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
#if !defined(XALAN_AVT_HEADER_GUARD)
#define XALAN_AVT_HEADER_GUARD 

/**
 * $Id$
 * 
 * $State$
 * 
 */

// Base include file.  Must be first.
#include <XSLT/XSLTDefinitions.hpp>



#include <vector>



#include <XalanDOM/XalanDOMString.hpp>



class AVTPart;
class Locator;
class PrefixResolver;
class XPathExecutionContext;
class XalanNode;
class StringTokenizer;
class StylesheetConstructionContext;



/**
 * Class to hold an Attribute Value Template.
 */
class AVT //: public AVTPart
{
public:

	/**
	 * Construct an Attribute Value Template(AVT) by parsing the string, and
	 * either constructing a vector of AVTParts, or simply hold on to the
	 * string if the AVT is simple.
	 *
	 * @param ownerElement		  the Locator for the AVT.  May be null.
	 * @param name                name of AVT
	 * @param type                type of AVT
	 * @param stringedValue       string value to parse
	 * @param resolver            resolver for namespace resolution
	 * @param constructionContext context for construction of AVT
	 */
	AVT(
			const Locator*					locator,
			const XalanDOMChar*				name,
			const XalanDOMChar*				type,
			const XalanDOMChar*				stringedValue,
			const PrefixResolver&			resolver,
			StylesheetConstructionContext&	constructionContext);

	virtual
	~AVT();

	/**
	 * Retrieve the name of the Attribute Value Template
	 * 
	 * @return name of AVT
	 */
    const XalanDOMString&
	getName() const
	{
		return m_name;
	}

	/**
	 * Retrieve the prefix of the name of the Attribute Value Template,
	 * if any.
	 * 
	 * @return The prefix part of the AVT's name
	 */
    const XalanDOMString&
	getPrefix() const
	{
		return m_prefix;
	}

	/**
	 * Retrieve the type of the Attribute Value Template
	 * 
	 * @return type of AVT
	 */
	const XalanDOMString&
	getType() const
	{
		return m_pcType;
	}

	/**
	 * Retrieve the "simple" value
	 * 
	 * @return The "simple" value of the AVT.
	 */
	const XalanDOMString&
	getSimpleValue() const
	{
		return m_simpleString;
	}

	void
	evaluate(
			XalanDOMString&			buf,
			XalanNode*				contextNode,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext) const;

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<const AVTPart*>		AVTPartPtrVectorType;
#else
	typedef std::vector<const AVTPart*>	AVTPartPtrVectorType;
#endif

private:

	void
	nextToken(
			StylesheetConstructionContext&	constructionContext,
			const Locator*					locator,
			StringTokenizer&				tokenizer,
			XalanDOMString&					token);

	/**
	 * Get the prefix from theName, if any.
	 * 
	 * @param theName name of AVT
	 *
	 * @return A string containing the prefix.
	 */
	XalanDOMString
	getPrefix(const XalanDOMChar*	theName);

	// not implemented
	AVT(const AVT &);
	AVT& operator=(const AVT &);	

	AVTPartPtrVectorType	m_parts;

	XalanDOMString			m_simpleString;

	const XalanDOMString	m_name;	

	const XalanDOMString	m_prefix;

	const XalanDOMString	m_pcType;
};



#endif	// XALAN_AVT_HEADER_GUARD