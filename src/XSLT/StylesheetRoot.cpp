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
 *
 * $ Id: $
 *
 */

// Class header file
#include "StylesheetRoot.hpp"



#include <algorithm>
#include <iostream>
#include <memory>



#include <sax/SAXException.hpp>



#include <util/XMLURL.hpp>



#include <XalanDOM/XalanDocumentFragment.hpp>



#include <PlatformSupport/AttributeListImpl.hpp>
#include <PlatformSupport/StringTokenizer.hpp>
#include <PlatformSupport/PrintWriter.hpp>



#include <XPath/XPathFactory.hpp>
#include <XPath/XPathProcessor.hpp>



#include <XMLSupport/Formatter.hpp>
#include <XMLSupport/FormatterToHTML.hpp>
#include <XMLSupport/FormatterToText.hpp>
#include <XMLSupport/FormatterToXML.hpp>
#include <XMLSupport/FormatterToDOM.hpp>



#include "Constants.hpp"
#include "ElemApplyTemplates.hpp" 
#include "ElemTemplate.hpp" 
#include "ElemValueOf.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"
#include "TraceListener.hpp"
#include "XSLTResultTarget.hpp"



/**
 * Constructor for a Stylesheet needs a Document.
 * @exception XSLProcessorException thrown if the active ProblemListener and
 *            XMLParserLiaison decide the error condition is severe enough to
 *            halt processing.
 */
StylesheetRoot::StylesheetRoot(
        const XalanDOMString&			baseIdentifier,
		StylesheetConstructionContext&	constructionContext) :
	Stylesheet(*this,
			   baseIdentifier,
			   constructionContext),
	m_importStack(),
	m_resultNameSpaceURL(),
	m_outputMethod(FormatterListener::OUTPUT_METHOD_NONE),
	m_version(),
	m_indentResult(false),
	m_encoding(),
	m_mediatype(),
	m_doctypeSystem(),
	m_doctypePublic(),
	m_omitxmlDecl(false),
	m_standalone(),
	m_cdataSectionElems(),
	m_defaultTextRule(0),
	m_defaultRule(0),
	m_defaultRootRule(0)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::auto_ptr;
#endif

	if (length(baseIdentifier) != 0)
	{
		typedef StylesheetConstructionContext::URLAutoPtrType	URLAutoPtrType;

		URLAutoPtrType	url(constructionContext.getURLFromString(m_baseIdent));

		if (url.get() != 0)
		{
			m_baseIdent = url->getURLText();

			URLAutoPtrType	url2(constructionContext.getURLFromString(m_baseIdent));

			if (url2.get() != 0)
			{
				m_importStack.push_back(url2.get());

				// Release the auto_ptr<>...
				url2.release();
			}
		}
	}
}				



StylesheetRoot::~StylesheetRoot()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	// Clean up all entries in the vector.
	for_each(m_importStack.begin(),
			 m_importStack.end(),
			 DeleteFunctor<XMLURL>());

	delete m_defaultRule;
	delete m_defaultTextRule;
	delete m_defaultRootRule;
}



void
StylesheetRoot::process(
			XalanNode*						sourceTree, 
			XSLTResultTarget&				outputTarget,
			StylesheetExecutionContext&		executionContext) const
{
		// Find the root pattern in the XSL.
		ElemTemplate* rootRule =
			findTemplate(executionContext, sourceTree, sourceTree);

		if(0 == rootRule)
		{
			rootRule = m_defaultRootRule;
			assert(rootRule);
		}

		executionContext.setStylesheetRoot(this);

		FormatterListener* flistener = 0;

#if !defined(XALAN_NO_NAMESPACES)
		using std::auto_ptr;
#endif

		Writer* pw = 0;

		flistener = outputTarget.getFormatterListener();

		if(flistener == 0)
		{
			// $$$ ToDo: For right now, XSLTResultTarget::getDocumentHandler()
			// and XSLTResultTarget::getFormatterListener() both return a
			// FormatterListener*.  When we have RTTI on all platforms, that
			// may change...
			flistener = outputTarget.getDocumentHandler();
		}

		if (flistener != 0)
		{
			// Do encoding stuff here...
		}
		/*
		 * Output target has a character or byte stream or file
		 */
		else if((0 != outputTarget.getCharacterStream()) ||
						(0 != outputTarget.getByteStream()) ||
						(0 != outputTarget.getFileName().length()))
		{
			if(0 != outputTarget.getCharacterStream())
			{
				pw = outputTarget.getCharacterStream();
			}
			else
			{
/*
				java:
				XalanDOMString mimeEncoding;
				XalanDOMString encoding;
				mimeEncoding = getOutputEncoding();
				encoding = getJavaOutputEncoding();
				if(0 == encoding)
				{
					m_processor->m_diagnosticsPrintWriter.println("Encoding not supported: "+mimeEncoding);
					mimeEncoding = "UTF-8";
					encoding = FormatterToXML.convertMime2JavaEncoding(mimeEncoding);
				}
*/

				if(0 != outputTarget.getByteStream())
				{
					pw = executionContext.createPrintWriter(*outputTarget.getByteStream());
				}
				else if(!isEmpty(outputTarget.getFileName()))
				{
					pw = executionContext.createPrintWriter(
								outputTarget.getFileName(),
								XalanDOMString());
				}
				else
				{
#if !defined(XALAN_NO_NAMESPACES)
					using std::cout;
#endif

					pw = executionContext.createPrintWriter(cout);
				}
			}

			int			indentAmount = executionContext.getIndent();
			const bool	doIndent = (indentAmount > -1) ? true : m_indentResult;
			
			switch(m_outputMethod)
			{
			case FormatterListener::OUTPUT_METHOD_HTML:
				{
					if (doIndent == true && indentAmount < 0)
					{
						indentAmount = FormatterToHTML::eDefaultIndentAmount;
					}

					flistener = executionContext.createFormatterToHTML(
						*pw, m_encoding, m_mediatype, m_doctypeSystem, m_doctypePublic,
						doIndent, indentAmount, m_version, m_standalone, !m_omitxmlDecl);
				}
				break;

			case FormatterListener::OUTPUT_METHOD_TEXT:
				flistener = executionContext.createFormatterToText(*pw);
				break;

			case FormatterListener::OUTPUT_METHOD_NONE:
			case FormatterListener::OUTPUT_METHOD_XML:
			default:
				{
					// Make sure we don't have a negative indent amount if we're
					// indenting
					if (doIndent == true && indentAmount < 0)
					{
						indentAmount = FormatterToXML::eDefaultIndentAmount;
					}

					flistener = executionContext.createFormatterToXML(
						*pw, m_version, doIndent, indentAmount, m_encoding, m_mediatype,
						m_doctypeSystem, m_doctypePublic, !m_omitxmlDecl, m_standalone);
				}
				break;
			}

			executionContext.setFormatterListener(flistener);
		}
		/*
		 * Output target has a node
		 */
		else if(0 != outputTarget.getNode())
		{
			switch(outputTarget.getNode()->getNodeType())
			{
			case XalanNode::DOCUMENT_NODE:
				flistener =
					executionContext.createFormatterToDOM(static_cast<XalanDocument*>(outputTarget.getNode()));
				break;

			case XalanNode::DOCUMENT_FRAGMENT_NODE:
				flistener =
					executionContext.createFormatterToDOM(executionContext.createDocument(),
						static_cast<XalanDocumentFragment*>(outputTarget.getNode()));
				break;

			case XalanNode::ELEMENT_NODE:
				flistener =
					executionContext.createFormatterToDOM(executionContext.createDocument(),
						static_cast<XalanElement*>(outputTarget.getNode()));
				break;

			default:
				executionContext.error("Can only output to an Element, DocumentFragment, Document, or PrintWriter.");
			}
		}
		/*
		 * Create an empty document and set the output target node to this
		 */
		else
		{
			outputTarget.setNode(executionContext.createDocument());
			flistener =
				executionContext.createFormatterToDOM(static_cast<XalanDocument*>(outputTarget.getNode()));
		}
		
		executionContext.setFormatterListener(flistener);
		executionContext.resetCurrentState(sourceTree, sourceTree);

		executionContext.setRootDocument(static_cast<XalanDocument*>(sourceTree));
		
		if(executionContext.doDiagnosticsOutput())
		{
			executionContext.diag(XALAN_STATIC_UCODE_STRING("============================="));
			executionContext.diag(XALAN_STATIC_UCODE_STRING("Transforming..."));
			executionContext.pushTime(&sourceTree);
		}
		
		executionContext.pushContextMarker(0, 0);

		try
		{
			executionContext.resolveTopLevelParams();
		}
		// java: catch(Exception e)
		catch(...)
		{
			throw SAXException("StylesheetRoot.process error");
		}

		executionContext.startDocument();

		// Output the action of the found root rule.	All processing
		// occurs from here.	buildResultFromTemplate is highly recursive.
		// java: rootRule->execute(*m_processor, sourceTree, sourceTree, 0);
		rootRule->execute(executionContext, sourceTree, sourceTree, QName());

		executionContext.endDocument();
		
		// Reset the top-level params for the next round.
		executionContext.clearTopLevelParams();

		if(executionContext.doDiagnosticsOutput())
		{
			executionContext.diag(XALAN_STATIC_UCODE_STRING(""));
			executionContext.displayDuration(XALAN_STATIC_UCODE_STRING("transform"), &sourceTree);
			executionContext.diag(XALAN_STATIC_UCODE_STRING(""));
		}
}



/** 
 * Return the output method that was specified in the stylesheet. 
 * The returned value is one of Formatter.OUTPUT_METH_XML,
 * Formatter.OUTPUT_METH_HTML, or Formatter.OUTPUT_METH_TEXT.
 */
FormatterListener::eFormat
StylesheetRoot::getOutputMethod() const
{ 
	return m_outputMethod; 
}



/** Get the version string that was specified in the stylesheet. */
XalanDOMString 
StylesheetRoot::getOutputVersion() const
{ 
	return m_version; 
}


/** Get the media-type string that was specified in the stylesheet. */
bool 
StylesheetRoot::getOutputIndent() const
{ 
	return m_indentResult;
}


/** Get the encoding string that was specified in the stylesheet. */
XalanDOMString 
StylesheetRoot::getOutputEncoding() const
{ 
    return m_encoding; 
}


/** Get the encoding string that was specified in the stylesheet. */
XalanDOMString 
StylesheetRoot::getJavaOutputEncoding() const 
{ 
    XalanDOMString	encoding;

    if(isEmpty(m_encoding))
	{
		encoding  = XALAN_STATIC_UCODE_STRING("UTF8");
	}
    else if(equalsIgnoreCase(m_encoding, XALAN_STATIC_UCODE_STRING("UTF-16")))
	{
		encoding  = XALAN_STATIC_UCODE_STRING("Unicode");
	}

// @@ JMD: do we need this ??
//    else
// @@ to do       encoding = FormatterToXML.convertMime2JavaEncoding( m_encoding ); 
    return encoding;
}


/** Get the media-type string that was specified in the stylesheet. */
XalanDOMString 
StylesheetRoot::getOutputMediaType() const 
{ 
	return m_mediatype; 
}


/** Get the doctype-system-id string that was specified in the stylesheet. */
XalanDOMString 
StylesheetRoot::getOutputDoctypeSystem() const 
{ 
	return m_doctypeSystem; 
}


/** Get the doctype-public-id string that was specified in the stylesheet. */
XalanDOMString 
StylesheetRoot::getOutputDoctypePublic() const
{ 
	return m_doctypePublic; 
}

/**
 * Process the xsl:output element.
 */
void 
StylesheetRoot::processOutputSpec(
			const XalanDOMChar*				name, 
			const AttributeList&			atts,
			StylesheetConstructionContext&	constructionContext)
{
	const unsigned int	nAttrs = atts.getLength();

	bool				didSpecifyIndent = false;

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar*	const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_OUTPUT_METHOD))
		{
			const XalanDOMChar*	const	method = atts.getValue(i);

			if(equals(method, Constants::ATTRVAL_OUTPUT_METHOD_HTML))
				m_outputMethod = FormatterListener::OUTPUT_METHOD_HTML;
			else if(equals(method, Constants::ATTRVAL_OUTPUT_METHOD_XML))
				m_outputMethod = FormatterListener::OUTPUT_METHOD_XML;
			else if(equals(method, Constants::ATTRVAL_OUTPUT_METHOD_TEXT))
				m_outputMethod = FormatterListener::OUTPUT_METHOD_TEXT;
		}
		else if(equals(aname, Constants::ATTRNAME_OUTPUT_VERSION))
		{
			m_version = atts.getValue(i);
		}
		else if(equals(aname,Constants::ATTRNAME_OUTPUT_INDENT))
		{
			m_indentResult = getYesOrNo(aname, atts.getValue(i), constructionContext);
			didSpecifyIndent = true;
		}
		else if(equals(aname,Constants::ATTRNAME_OUTPUT_ENCODING))
		{
			m_encoding = atts.getValue(i);
		}
		else if(equals(aname,Constants::ATTRNAME_OUTPUT_MEDIATYPE))
		{
			m_mediatype = atts.getValue(i);
		}
		else if(equals(aname,Constants::ATTRNAME_OUTPUT_DOCTYPE_SYSTEM))
		{
			m_doctypeSystem = atts.getValue(i);
		}
		else if(equals(aname,Constants::ATTRNAME_OUTPUT_DOCTYPE_PUBLIC))
		{
			m_doctypePublic = atts.getValue(i);
		}
		else if(equals(aname,Constants::ATTRNAME_OUTPUT_OMITXMLDECL))
		{
			m_omitxmlDecl = getYesOrNo(aname, atts.getValue(i), constructionContext);
		}
		else if(equals(aname,Constants::ATTRNAME_OUTPUT_STANDALONE))
		{
			m_standalone = atts.getValue(i);
		}
		else if(equals(aname,Constants::ATTRNAME_OUTPUT_CDATA_SECTION_ELEMENTS))
		{
			StringTokenizer tokenizer(atts.getValue(i));

			while(tokenizer.hasMoreTokens())
			{
				const XalanDOMString	token = tokenizer.nextToken();

				const QName				qname(token, getNamespaces());

				m_cdataSectionElems.push_back(qname);
			}
		}
		else
		{
			constructionContext.error(name + XalanDOMString(" has an illegal attribute: ")+aname);
		}
	}

	if((FormatterListener::OUTPUT_METHOD_HTML == m_outputMethod) &&
		 (false == didSpecifyIndent))
	{
		m_indentResult = true;
	}
}



void 
StylesheetRoot::initDefaultRule(StylesheetConstructionContext&	constructionContext)
{
	if (m_defaultRule == 0)
	{
		assert(m_defaultTextRule == 0);
		assert(m_defaultRootRule == 0);

		const int				lineNumber = 0;
		const int				columnNumber = 0;

		AttributeListImpl		attrs;

		const XalanDOMString	xslPrefix(XALAN_STATIC_UCODE_STRING("xsl:"));

		attrs.addAttribute(c_wstr(Constants::ATTRNAME_MATCH),
	 					   c_wstr(XALAN_STATIC_UCODE_STRING("CDATA")),
						   c_wstr(XALAN_STATIC_UCODE_STRING("*")));

		m_defaultRule = new ElemTemplate(constructionContext,	// @@ JMD: should be null 
										 *this,
										 xslPrefix + Constants::ELEMNAME_TEMPLATE_STRING, 
										 attrs,
										 lineNumber,
										 columnNumber);

		attrs.clear();

		ElemApplyTemplates* childrenElement 
		  = new ElemApplyTemplates(constructionContext,
								   *this,
								   xslPrefix + Constants::ELEMNAME_APPLY_TEMPLATES_STRING,
								   attrs,
								   lineNumber,
								   columnNumber);

		childrenElement->setDefaultTemplate(true);
		m_defaultRule->appendChildElem(childrenElement);
    
		// -----------------------------
    
		attrs.clear();
		attrs.addAttribute(c_wstr(Constants::ATTRNAME_MATCH),
	 					   c_wstr(XALAN_STATIC_UCODE_STRING("CDATA")),
						   c_wstr(XALAN_STATIC_UCODE_STRING("text() | @*")));

		m_defaultTextRule = new ElemTemplate(constructionContext,
											 *this,
											 xslPrefix + Constants::ELEMNAME_TEMPLATE_STRING,
											 attrs,
											 lineNumber,
											 columnNumber);

		attrs.clear();
		attrs.addAttribute(c_wstr(Constants::ATTRNAME_SELECT),
	 					   c_wstr(XALAN_STATIC_UCODE_STRING("CDATA")),
						   c_wstr(XALAN_STATIC_UCODE_STRING(".")));

		ElemValueOf* elemValueOf =
			new ElemValueOf(constructionContext,
							*this,
							xslPrefix + Constants::ELEMNAME_VALUEOF_STRING,
							attrs,
							lineNumber,
							columnNumber);

		m_defaultTextRule->appendChildElem(elemValueOf);
    
		//--------------------------------
    
		attrs.clear();
		attrs.addAttribute(c_wstr(Constants::ATTRNAME_MATCH),
	 					   c_wstr(XALAN_STATIC_UCODE_STRING("CDATA")),
						   c_wstr(XALAN_STATIC_UCODE_STRING("/")));

		m_defaultRootRule =
			new ElemTemplate(constructionContext,
							 *this,
							 xslPrefix + Constants::ELEMNAME_TEMPLATE_STRING,
							 attrs,
							 lineNumber,
							 columnNumber);

		attrs.clear();

		childrenElement =
			new ElemApplyTemplates(constructionContext,
								   *this,
								   xslPrefix + Constants::ELEMNAME_APPLY_TEMPLATES_STRING,
								   attrs,
								   lineNumber,
								   columnNumber);

		childrenElement->setDefaultTemplate(true);

		m_defaultRootRule->appendChildElem(childrenElement);
	}

	assert(m_defaultRule != 0);
	assert(m_defaultTextRule != 0);
	assert(m_defaultRootRule != 0);
}
