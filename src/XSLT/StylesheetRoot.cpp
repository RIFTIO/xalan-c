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



#include <Include/DOMHelper.hpp>

#include <XPath/XPathFactory.hpp>
#include <XPath/XPathProcessor.hpp>

#include <XMLSupport/Formatter.hpp>
#include <XMLSupport/FormatterToHTML.hpp>
#include <XMLSupport/FormatterToText.hpp>
#include <XMLSupport/FormatterToXML.hpp>
#include <XMLSupport/FormatterToDOM.hpp>

#include <PlatformSupport/StringTokenizer.hpp>
#include <PlatformSupport/AttributeListImpl.hpp>

#include <XercesPlatformSupport/XercesDOMPrintWriter.hpp>
#include <XercesPlatformSupport/XercesStdTextOutputStream.hpp>


#include "ElemApplyTemplates.hpp" 
#include "ElemTemplate.hpp" 
#include "ElemValueOf.hpp"

#include "Constants.hpp"
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
        const DOMString&				baseIdentifier,
		StylesheetConstructionContext&	constructionContext) :
	Stylesheet(*this, baseIdentifier, constructionContext),
	m_importStack(),
	m_resultNameSpaceURL(),
	m_outputmethod(Formatter::OUTPUT_METH_XML),
	m_version(baseIdentifier),
	m_indentResult(false),
	m_encoding(),
	m_mediatype(),
	m_doctypeSystem(),
	m_doctypePublic(),
	m_xmlDecl(true),
	m_standalone(),
	m_cdataSectionElems(),
	m_defaultTextRule(0),
	m_defaultRule(0),
	m_defaultRootRule(0)
{
    // For some reason, the imports aren't working right if I 
    // don't set the baseIdent to full url.  I think this is OK, 
    // and probably safer and faster in general.
	std::auto_ptr<XMLURL>	url(constructionContext.getURLFromString(m_baseIdent));

	if (url.get() != 0)
	{
		m_baseIdent = url->getURLText();

		std::auto_ptr<XMLURL>	url2(constructionContext.getURLFromString(m_baseIdent));

		if (url2.get() != 0)
		{
			m_importStack.push_back(url2.get());

			// Release the auto_ptr<>...
			url2.release();
		}
	}
}				



StylesheetRoot::~StylesheetRoot()
{
	// Clean up all entries in the vector.
	std::for_each(m_importStack.begin(),
			 m_importStack.end(),
			 DeleteFunctor<XMLURL>());

	if (m_defaultRule != 0)
	{
		NodeImpl* const		child = m_defaultRule->getFirstChild();
		m_defaultRule->removeChild(child);
		delete child;
		delete m_defaultRule;
	}

	if (m_defaultTextRule != 0)
	{
		NodeImpl* const		child = m_defaultTextRule->getFirstChild();
		m_defaultTextRule->removeChild(child);
		delete child;
		delete m_defaultTextRule;
	}

	if (m_defaultRootRule != 0)
	{
		NodeImpl* const		child = m_defaultRootRule->getFirstChild();
		m_defaultRootRule->removeChild(child);
		delete child;
		delete m_defaultRootRule;
	}
}



void StylesheetRoot::process(
			const DOM_Node&					sourceTree, 
			XSLTResultTarget&				outputTarget,
			StylesheetExecutionContext&		executionContext)
{
		// Find the root pattern in the XSL.
		ElemTemplate* rootRule =
			dynamic_cast<ElemTemplate*>(findTemplate(executionContext, sourceTree, sourceTree));

		if(0 == rootRule)
		{
			rootRule = m_defaultRootRule;
			assert(rootRule);
		}
		executionContext.setStylesheetRoot(this);

		FormatterListener* flistener = 0;
		bool newListener = false;
		// $$$ ToDo: Why is this Xerces-specific?
		XercesDOMPrintWriter* pw = 0;
		bool newPW = false;

		/*
		 * Output target has a document handler
		 */
		if(0 != outputTarget.getDocumentHandler())
		{
			// Stuff a DocumentHandler into a FormatterListener
			flistener = 
				dynamic_cast<FormatterListener *>(outputTarget.getDocumentHandler());
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
				// @@ JMD: is this right ??
				// java: pw = new PrintWriter(outputTarget.getCharacterStream());
				pw = static_cast<XercesDOMPrintWriter*>
					(outputTarget.getCharacterStream());
			}
			else
			{
/*
				java:
				DOMString mimeEncoding;
				DOMString encoding;
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
					assert(0);	// @@ JMD: not supported yet ??
					// java: OutputStreamWriter osw = new OutputStreamWriter(outputTarget.getByteStream(), encoding);
					// java: pw = new PrintWriter( new BufferedWriter(osw) );
				}
				else if(! isEmpty(outputTarget.getFileName()))
				{
					assert(0);	// @@ JMD: not supported yet ??
				/*
					FileOutputStream fos = new FileOutputStream(outputTarget.getFileName());
					try
					{
						// pw = new PrintWriter( new BufferedWriter(new FileWriter(outputTarget.getFileName())) );
						OutputStreamWriter osw = new OutputStreamWriter(fos, encoding);
						pw = new PrintWriter( new BufferedWriter(osw) );
					}
					catch(java.io.UnsupportedEncodingException uee)
					{
						mimeEncoding = "UTF-8";
						encoding = FormatterToXML.convertMime2JavaEncoding(mimeEncoding);
						OutputStreamWriter osw = new OutputStreamWriter(fos, encoding);
						pw = new PrintWriter( new BufferedWriter(osw) );
					}
					m_processor->setOutputFileName(outputTarget.getFileName());
				*/
				}
				else
				{
					// $$$ ToDo:  THIS IS A MEMORY LEAK!!!
					pw = new XercesDOMPrintWriter(*new XercesStdTextOutputStream(std::cout));
					newPW = true;
				}
			}
			
			int indentAmount = executionContext.getIndent();
			bool doIndent = (indentAmount > -1) ? true : m_indentResult;
			// Make sure we don't have a negative indent amount if we're
			// indenting
			if (m_indentResult && (indentAmount < 0))
				indentAmount = 0;
			
			switch(m_outputmethod)
			{
			case Formatter::OUTPUT_METH_HTML:
				flistener = new FormatterToHTML(
					*pw, m_version, doIndent, indentAmount, m_encoding, m_mediatype,
					m_doctypeSystem, m_doctypePublic, m_xmlDecl, m_standalone,
					&m_cdataSectionElems);
				newListener = true;
				break;
			case Formatter::OUTPUT_METH_TEXT:
				flistener = new FormatterToText(*pw);
				newListener = true;
				break;
			case Formatter::OUTPUT_METH_XML:
			default:
				flistener = new FormatterToXML(
					*pw, m_version, doIndent, indentAmount, m_encoding, m_mediatype,
					m_doctypeSystem, m_doctypePublic, m_xmlDecl, m_standalone,
					&m_cdataSectionElems);
				newListener = true;
				break;
			}
		}
		/*
		 * Output target has a node
		 */
		else if(0 != outputTarget.getNode())
		{
			switch(outputTarget.getNode().getNodeType())
			{
			case DOM_Node::DOCUMENT_NODE:
				flistener = new 
					FormatterToDOM(static_cast<DOM_Document&>(outputTarget.getNode()));
				newListener = true;
				break;
			case DOM_Node::DOCUMENT_FRAGMENT_NODE:
				flistener = new 
					FormatterToDOM(executionContext.createDocument(),
						static_cast<DOM_DocumentFragment&>(outputTarget.getNode()));
				newListener = true;
				break;
			case DOM_Node::ELEMENT_NODE:
				flistener = new 
					FormatterToDOM(executionContext.createDocument(),
						static_cast<DOM_Element&>(outputTarget.getNode()));
				newListener = true;
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
			flistener = new 
				FormatterToDOM(static_cast<DOM_Document&>(outputTarget.getNode()));
			newListener = true;
		}
		
		executionContext.setFormatterListener(flistener);
		executionContext.resetCurrentState(sourceTree, sourceTree);
		// @@ JMD: Is this OK ??
		executionContext.setRootDocument(static_cast<const DOM_Document&>(sourceTree));
		
		if(executionContext.doDiagnosticsOutput())
		{
			executionContext.diag("=============================");
			executionContext.diag("Transforming...");
			executionContext.pushTime(&sourceTree);
		}
		
		executionContext.pushContextMarker(DOM_Node(), DOM_Node());

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
			executionContext.displayDuration("transform", &sourceTree);
		}
		if (newListener) delete flistener;		
		// Can't release this until flistener is gone, since it contains a
		// reference to it
		if (newPW != 0) delete pw;

}



/** 
 * Return the output method that was specified in the stylesheet. 
 * The returned value is one of Formatter.OUTPUT_METH_XML,
 * Formatter.OUTPUT_METH_HTML, or Formatter.OUTPUT_METH_TEXT.
 */
int 
StylesheetRoot::getOutputMethod() const
{ 
	return m_outputmethod; 
}



/** Get the version string that was specified in the stylesheet. */
DOMString 
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
DOMString 
StylesheetRoot::getOutputEncoding() const
{ 
    return m_encoding; 
}


/** Get the encoding string that was specified in the stylesheet. */
DOMString 
StylesheetRoot::getJavaOutputEncoding() const 
{ 
    DOMString encoding;
    if(isEmpty(m_encoding))
      encoding  = DOMString("UTF8");
    else if( equalsIgnoreCase(m_encoding, DOMString("UTF-16") ) )
      encoding  = DOMString("Unicode");
// @@ JMD: do we need this ??
//    else
// @@ to do       encoding = FormatterToXML.convertMime2JavaEncoding( m_encoding ); 
    return encoding;
}


/** Get the media-type string that was specified in the stylesheet. */
DOMString 
StylesheetRoot::getOutputMediaType() const 
{ 
	return m_mediatype; 
}


/** Get the doctype-system-id string that was specified in the stylesheet. */
DOMString 
StylesheetRoot::getOutputDoctypeSystem() const 
{ 
	return m_doctypeSystem; 
}


/** Get the doctype-public-id string that was specified in the stylesheet. */
DOMString 
StylesheetRoot::getOutputDoctypePublic() const
{ 
	return m_doctypePublic; 
}

/**
 * Process the xsl:output element.
 */
void 
StylesheetRoot::processOutputSpec(
			const DOMString&				name, 
			const AttributeList&			atts,
			StylesheetConstructionContext&	constructionContext)
{
	int nAttrs = atts.getLength();
	bool didSpecifyIndent = false;
	for(int i = 0; i < nAttrs; i++)
	{
		DOMString aname = atts.getName(i);
		if(equals(aname, Constants::ATTRNAME_OUTPUT_METHOD))
		{
			DOMString method = atts.getValue(i);
			if(equals(method, Constants::ATTRVAL_OUTPUT_METHOD_HTML))
				m_outputmethod = Formatter::OUTPUT_METH_HTML;
			else if(equals(method, Constants::ATTRVAL_OUTPUT_METHOD_XML))
				m_outputmethod = Formatter::OUTPUT_METH_XML;
			else if(equals(method, Constants::ATTRVAL_OUTPUT_METHOD_TEXT))
				m_outputmethod = Formatter::OUTPUT_METH_TEXT;
		}
		else if(equals(aname,Constants::ATTRNAME_OUTPUT_VERSION))
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
		else if(equals(aname,Constants::ATTRNAME_OUTPUT_XMLDECL))
		{
			m_xmlDecl = getYesOrNo(aname, atts.getValue(i), constructionContext);
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
				DOMString token = tokenizer.nextToken();
				QName qname(token, getNamespaces());
				m_cdataSectionElems.push_back(qname);
			}
		}
		else
		{
			constructionContext.error(name+DOMString(" has an illegal attribute: ")+aname);
		}
	}
	if((Formatter::OUTPUT_METH_HTML == m_outputmethod) &&
		 (false == didSpecifyIndent))
	{
		m_indentResult = true;
	}
}


/**
 * Create the default rule if needed.
 */
void 
StylesheetRoot::initDefaultRule(StylesheetConstructionContext&	constructionContext)
{
	int lineNumber = 0;
	int columnNumber = 0;
	// Then manufacture a default
     
	AttributeListImpl attrs;
	//const AttributeListImpl attrs() ;
    attrs.addAttribute(c_wstr(Constants::ATTRNAME_MATCH),
	 							c_wstr(DOMString("CDATA")),
								c_wstr(DOMString("*")));
    m_defaultRule = new ElemTemplate(constructionContext,	// @@ JMD: should be null 
									*this,
									DOMString("xsl:")+Constants::ELEMNAME_TEMPLATE_STRING, 
                                    attrs, lineNumber, columnNumber);
    attrs.clear();
    ElemApplyTemplates* childrenElement 
      = new ElemApplyTemplates(constructionContext, *this,
                                DOMString("xsl:")+Constants::ELEMNAME_APPLY_TEMPLATES_STRING,
                                attrs, lineNumber, columnNumber);
	childrenElement->setDefaultTemplate(true);
    m_defaultRule->appendChild(childrenElement);
    
    // -----------------------------
    
    attrs.clear();
    attrs.addAttribute(c_wstr(Constants::ATTRNAME_MATCH),
	 							c_wstr(DOMString("CDATA")),
								c_wstr(DOMString("text() | @*")));
    m_defaultTextRule = new ElemTemplate(constructionContext, 
											*this,
											DOMString("xsl:")+Constants::ELEMNAME_TEMPLATE_STRING,
											attrs, lineNumber, columnNumber);
    attrs.clear();
    attrs.addAttribute(c_wstr(Constants::ATTRNAME_SELECT),
	 							c_wstr(DOMString("CDATA")),
								c_wstr(DOMString(".")));
    ElemValueOf* elemValueOf 
      = (new ElemValueOf(constructionContext, *this,
                        DOMString("xsl:")+Constants::ELEMNAME_VALUEOF_STRING,
                        attrs, lineNumber, columnNumber));
    m_defaultTextRule->appendChild(elemValueOf);
    
    //--------------------------------
    
    attrs.clear();
    attrs.addAttribute(c_wstr(Constants::ATTRNAME_MATCH),
	 							c_wstr(DOMString("CDATA")),
								c_wstr(DOMString("/")));
    m_defaultRootRule = new ElemTemplate(constructionContext, *this,
                                        DOMString("xsl:")+Constants::ELEMNAME_TEMPLATE_STRING,
                                        attrs, lineNumber, columnNumber);
    attrs.clear();
    childrenElement 
      = (new ElemApplyTemplates(constructionContext, *this,
								DOMString("xsl:")+Constants::ELEMNAME_APPLY_TEMPLATES_STRING,
								attrs,	lineNumber, columnNumber));
	childrenElement->setDefaultTemplate(true);
    m_defaultRootRule->appendChild(childrenElement);
}



int
StylesheetRoot::getTraceListeners() const
{
	return m_traceListeners.size();
}



void 
StylesheetRoot::addTraceListener(TraceListener* tl)
{
    m_traceListeners.push_back(tl);
}



void 
StylesheetRoot::removeTraceListener(TraceListener* theListener)
{
	const ListenersVectorType::iterator		it =
		std::find(m_traceListeners.begin(),
				  m_traceListeners.end(),
				  theListener);

	if (it != m_traceListeners.end())
	{
		m_traceListeners.erase(it);
	}
}
 
/**
 * Fire a trace event.
 */
void StylesheetRoot::fireTraceEvent(const TracerEvent& te) const
{
	int nListeners = m_traceListeners.size();
	for(int i = 0; i < nListeners; i++)
	{
		TraceListener* tl = m_traceListeners[i];
		tl->trace(te);
	}
}
  
/**
 * Fire a selection event.
 */
void StylesheetRoot::fireSelectedEvent(const SelectionEvent& se) const
{
	int nListeners = m_traceListeners.size();
	for(int i = 0; i < nListeners; i++)
	{
		TraceListener* tl = m_traceListeners[i];
		tl->selected(se);
	}
}



#if 0

// Transfer these to StylesheetExecutionContext() and StylesheetConstructionContext(), etc...
XPath*
StylesheetRoot::createMatchPattern(
			const DOMString&		str,
			const PrefixResolver&	resolver)
{
	assert(m_processor != 0);
	assert(m_processor->getXPathProcessor() != 0);

	XPath* const	xpath = m_xpathFactory.create();

	m_processor->getXPathProcessor()->initMatchPattern(*xpath,
													   str,
													   resolver,
													   m_xobjectFactory,
													   m_processor->getXPathEnvSupport());

	return xpath;
}



XPath*
StylesheetRoot::createXPath(
			const DOMString&		str,
			const PrefixResolver&	resolver)
{
	assert(m_processor != 0);
	assert(m_processor->getXPathProcessor() != 0);

	XPath* const	xpath = m_xpathFactory.create();

	// It's OK to use the XPathEnvSupport of the processor,
	// since we're in construction phase.
	m_processor->getXPathProcessor()->initXPath(*xpath,
												str,
												resolver,
												m_xobjectFactory,
												m_processor->getXPathEnvSupport());

	return xpath;
}
#endif



/*
 *	$ Log: $
 */
