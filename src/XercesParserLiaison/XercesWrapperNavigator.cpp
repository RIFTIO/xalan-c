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
 */
#include "XercesWrapperNavigator.hpp"



#include <XalanDOM/XalanNode.hpp>
#include <XalanDOM/XalanElement.hpp>



#include "XercesAttrWrapper.hpp"
#include "XercesDocumentWrapper.hpp"
#include "XercesTextWrapper.hpp"
#include "XercesDOMWrapperException.hpp"



const XalanDOMString	XercesWrapperNavigator::s_emptyString;


// I'm using this to distinguish between null nodes, which are valid, and
// an uninitialized cached node address.  This is pretty bogus, and I'll
// probably just change this to 0, but this is experimental anyway...
#if defined(XALAN_OLD_STYLE_CASTS)
static XalanNode* const		invalidNodeAddress = (XalanNode*)1;
#else
static XalanNode* const		invalidNodeAddress = reinterpret_cast<XalanNode*>(1);
#endif



XercesWrapperNavigator::XercesWrapperNavigator(
			XercesDocumentWrapper*	theOwnerDocument,
			bool					mappingMode) :
	m_ownerDocument(theOwnerDocument),
	m_parentNode(mappingMode == true ? invalidNodeAddress : 0),
	m_previousSibling(mappingMode == true ? invalidNodeAddress : 0),
	m_nextSibling(mappingMode == true ? invalidNodeAddress : 0),
	m_firstChild(mappingMode == true ? invalidNodeAddress : 0),
	m_lastChild(mappingMode == true ? invalidNodeAddress : 0),
	m_index(UINT_MAX)
{
	assert(theOwnerDocument != 0);
}



XercesWrapperNavigator::XercesWrapperNavigator(const XercesWrapperNavigator&	theSource) :
	m_ownerDocument(theSource.m_ownerDocument),
	m_parentNode(theSource.m_parentNode),
	m_previousSibling(theSource.m_previousSibling),
	m_nextSibling(theSource.m_nextSibling),
	m_firstChild(theSource.m_firstChild),
	m_lastChild(theSource.m_lastChild),
	m_index(theSource.m_index)
{
}



XercesWrapperNavigator::~XercesWrapperNavigator()
{
}



XalanNode*
XercesWrapperNavigator::mapNode(const DOMNode*	theXercesNode) const
{
	return m_ownerDocument->mapNode(theXercesNode);
}



XalanAttr*
XercesWrapperNavigator::mapNode(const DOMAttr*	theXercesNode) const
{
	return m_ownerDocument->mapNode(theXercesNode);
}



const DOMNode*
XercesWrapperNavigator::mapNode(XalanNode*	theXalanNode) const
{
	return m_ownerDocument->mapNode(theXalanNode);
}



const DOMAttr*
XercesWrapperNavigator::mapNode(XalanAttr*	theXalanNode) const
{
	return m_ownerDocument->mapNode(theXalanNode);
}



XalanNode*
XercesWrapperNavigator::getParentNode(const DOMNode*	theXercesNode) const
{
	if (m_parentNode == invalidNodeAddress)
	{
#if defined(XALAN_NO_MUTABLE)
		((XercesWrapperNavigator*)this)->m_parentNode = m_ownerDocument->mapNode(theXercesNode->getParentNode());
#else
		m_parentNode = m_ownerDocument->mapNode(theXercesNode->getParentNode());
#endif
	}

	return m_parentNode;
}



XalanNode*
XercesWrapperNavigator::getPreviousSibling(const DOMNode*	theXercesNode) const
{
	if (m_previousSibling == invalidNodeAddress)
	{
#if defined(XALAN_NO_MUTABLE)
		((XercesWrapperNavigator*)this)->m_previousSibling = m_ownerDocument->mapNode(theXercesNode->getPreviousSibling());
#else
		m_previousSibling = m_ownerDocument->mapNode(theXercesNode->getPreviousSibling());
#endif
	}

	return m_previousSibling;
}



XalanNode*
XercesWrapperNavigator::getNextSibling(const DOMNode*	theXercesNode) const
{
	if (m_nextSibling == invalidNodeAddress)
	{
#if defined(XALAN_NO_MUTABLE)
		((XercesWrapperNavigator*)this)->m_nextSibling = m_ownerDocument->mapNode(theXercesNode->getNextSibling());
#else
		m_nextSibling = m_ownerDocument->mapNode(theXercesNode->getNextSibling());
#endif
	}

	return m_nextSibling;
}



XalanNode*
XercesWrapperNavigator::getFirstChild(const DOMNode*	theXercesNode) const
{
	if (m_firstChild == invalidNodeAddress)
	{
#if defined(XALAN_NO_MUTABLE)
		((XercesWrapperNavigator*)this)->m_firstChild = m_ownerDocument->mapNode(theXercesNode->getFirstChild());
#else
		m_firstChild = m_ownerDocument->mapNode(theXercesNode->getFirstChild());
#endif
	}

	return m_firstChild;
}



XalanNode*
XercesWrapperNavigator::getLastChild(const DOMNode*	theXercesNode) const
{
	if (m_lastChild == invalidNodeAddress)
	{
#if defined(XALAN_NO_MUTABLE)
		((XercesWrapperNavigator*)this)->m_lastChild = m_ownerDocument->mapNode(theXercesNode->getLastChild());
#else
		m_lastChild = m_ownerDocument->mapNode(theXercesNode->getLastChild());
#endif
	}

	return m_lastChild;
}



XalanElement*
XercesWrapperNavigator::getOwnerElement(const DOMAttr*	theXercesAttr) const
{
	assert(theXercesAttr != 0);

	if (m_parentNode != invalidNodeAddress)
	{
		assert(m_parentNode->getNodeType() == XalanNode::ELEMENT_NODE);

#if defined(XALAN_OLD_STYLE_CASTS)
		return (XalanElement*)m_parentNode;
#else
		return static_cast<XalanElement*>(m_parentNode);
#endif
	}
	else
	{
		return m_ownerDocument->mapNode(theXercesAttr->getOwnerElement());
	}
}



const XalanDOMString&
XercesWrapperNavigator::getPooledString(const XMLCh*	theString) const
{
	if (theString == 0)
	{
		return s_emptyString;
	}
	else
	{
		return m_ownerDocument->getPooledString(theString, XalanDOMString::length(theString));
	}
}