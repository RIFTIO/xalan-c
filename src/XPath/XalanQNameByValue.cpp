/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights 
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
// Class header file...
#include "XalanQNameByValue.hpp"



#include <PlatformSupport/PrefixResolver.hpp>
#include <PlatformSupport/XSLException.hpp>



#include <DOMSupport/DOMServices.hpp>
#include <DOMSupport/DOMSupport.hpp>



#include "ElementPrefixResolverProxy.hpp"



XalanQNameByValue::XalanQNameByValue() :
	XalanQName(),
	m_namespace(),
	m_localpart()
{
}



XalanQNameByValue::XalanQNameByValue(const XalanQNameByValue&	theSource) :
	XalanQName(),
	m_namespace(theSource.m_namespace),
	m_localpart(theSource.m_localpart)
{
}



XalanQNameByValue::XalanQNameByValue(const XalanQName&	theSource) :
	XalanQName(),
	m_namespace(theSource.getNamespace()),
	m_localpart(theSource.getLocalPart())
{
}



XalanQNameByValue::XalanQNameByValue(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	theLocalPart) :
	XalanQName(),
	m_namespace(theNamespace),
	m_localpart(theLocalPart)
{
}



XalanQNameByValue::XalanQNameByValue(
			const XalanDOMString&		qname,
			const NamespacesStackType&	namespaces,
			const Locator*				locator,
			bool						fUseDefault) :
	m_namespace(),
	m_localpart()
{
	initialize(c_wstr(qname), namespaces, locator, fUseDefault);
}



XalanQNameByValue::XalanQNameByValue(
			const XalanDOMChar*			qname,
			const NamespacesStackType&	namespaces,
			const Locator*				locator,
			bool						fUseDefault) :
	m_namespace(),
	m_localpart()
{
	assert(qname != 0);

	initialize(qname, namespaces, locator, fUseDefault);
}



XalanQNameByValue::XalanQNameByValue(
			const XalanDOMString&	qname,
			const XalanElement*		namespaceContext,
			const XPathEnvSupport&	envSupport,
			const DOMSupport& 		domSupport,
			const Locator*			locator) :
	m_namespace(),
	m_localpart()
{
	ElementPrefixResolverProxy	theProxy(namespaceContext, envSupport, domSupport);

	resolvePrefix(qname, &theProxy, locator);
}



XalanQNameByValue::XalanQNameByValue(
			const XalanDOMString&	qname,
			const PrefixResolver*	theResolver,
			const Locator*			locator) :
	m_namespace(),
	m_localpart()
{
	resolvePrefix(qname, theResolver, locator);
}



XalanQNameByValue::~XalanQNameByValue()
{
}



const XalanDOMString&
XalanQNameByValue::getLocalPart() const
{
	return m_localpart;
}



const XalanDOMString&
XalanQNameByValue::getNamespace() const
{
	return m_namespace;
}



void
throwException(
			const XalanDOMString&	theMessage,
			const Locator*			theLocator)
{
	if (theLocator == 0)
	{
		throw XSLException(theMessage);
	}
	else
	{
		throw XSLException(*theLocator, theMessage);
	}
}




void
XalanQNameByValue::initialize(
			const XalanDOMChar*			qname,
			const NamespacesStackType&	namespaces,
			const Locator*				locator,
			bool						fUseDefault)
{
	const XalanDOMString::size_type		indexOfNSSep = indexOf(qname, XalanUnicode::charColon);

	if (indexOfNSSep == 0)
	{
		throwException(TranscodeFromLocalCodePage("A prefix of length 0 was detected"), locator);
	}
	else if(indexOfNSSep < length(qname))
	{
		const XalanDOMString		prefix = substring(qname, 0, indexOfNSSep);

		if(::equals(prefix, DOMServices::s_XMLNamespace))
			return;

		const XalanDOMString* const		theNamespace = 
				getNamespaceForPrefix(namespaces, prefix);

		if(theNamespace == 0 || 0 == length(*theNamespace))
		{
			throwException(TranscodeFromLocalCodePage("Prefix must resolve to a namespace: ") + prefix, locator);
		}
		else
		{
			m_namespace = *theNamespace;
		}

		m_localpart =  substring(qname, indexOfNSSep + 1);
	}
	else
	{
		if (fUseDefault == true)
		{
			const XalanDOMString* const		theNamespace = 
					getNamespaceForPrefix(namespaces, s_emptyString);

			if(theNamespace != 0 && 0 != length(*theNamespace))
			{
				m_namespace = *theNamespace;
			}
		}

		m_localpart = qname;
	}
}



void
XalanQNameByValue::resolvePrefix(
			const XalanDOMString&	qname,
			const PrefixResolver*	theResolver,
			const Locator*			locator)
{
	const XalanDOMString::size_type		indexOfNSSep = indexOf(qname, XalanUnicode::charColon);
	const XalanDOMString::size_type		theLength = length(qname);

	if(indexOfNSSep >= theLength)
	{
		m_localpart = qname;
	}
	else
	{
		const XalanDOMString	prefix = substring(qname, 0, indexOfNSSep);

		if(::equals(prefix, DOMServices::s_XMLString))
		{
			m_namespace = DOMServices::s_XMLNamespaceURI;
		}
		// The default namespace is not resolved.
		else if(::equals(prefix, DOMServices::s_XMLNamespace))
		{
			return;
		}
		else if (theResolver == 0)
		{
			throwException(
				TranscodeFromLocalCodePage("Unable to resolve prefix '") +
					prefix +
					TranscodeFromLocalCodePage("'"),
				locator);
		}
		else
		{
			const XalanDOMString* const		theNamespace =
				theResolver->getNamespaceForPrefix(prefix);

			if (theNamespace != 0)
			{
				m_namespace = *theNamespace;
			}
		}  

		if(0 == length(m_namespace))
		{
			throwException(
				TranscodeFromLocalCodePage("The prefix '") +
					prefix +
					TranscodeFromLocalCodePage("' must resolve to a namespace."),
				locator);
		}

		m_localpart = substring(qname, indexOfNSSep + 1);
	}
}