/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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
 *
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */
#if !defined(XPATHCONSTRUCTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680)
#define XPATHCONSTRUCTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <PlatformSupport/XalanDOMStringCache.hpp>
#include <PlatformSupport/XalanDOMStringPool.hpp>



// Base class header file...
#include <XPath/XPathConstructionContext.hpp>



XALAN_CPP_NAMESPACE_BEGIN



/**
 *
 * An default implementation of an abtract class which provides support for
 * constructing the internal representation  of an XPath.
 *
 */
class XALAN_XPATH_EXPORT XPathConstructionContextDefault : public XPathConstructionContext
{
public:

	/*
	 * Construct an instance.
	 */
	XPathConstructionContextDefault();

	virtual
	~XPathConstructionContextDefault();

	virtual void
	reset();

	virtual const XalanDOMString&
	getPooledString(const XalanDOMString&	theString);

	virtual const XalanDOMString&
	getPooledString(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength = XalanDOMString::npos);

	virtual XalanDOMString&
	getCachedString();

	virtual bool
	releaseCachedString(XalanDOMString&		theString);

private:

	XalanDOMStringPool		m_stringPool;

	XalanDOMStringCache		m_stringCache;
};



XALAN_CPP_NAMESPACE_END



#endif	// XPATHCONSTRUCTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680
