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
#if !defined(XALAN_TRANSFORMER_HEADER_GUARD)
#define XALAN_TRANSFORMER_HEADER_GUARD



// Base include file.  Must be first.
#include <XalanTransformer/XalanTransformerDefinitions.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPathFactoryDefault.hpp>



#include <XSLT/StylesheetConstructionContextDefault.hpp>
#include <XSLT/StylesheetExecutionContextDefault.hpp>
#include <XSLT/StylesheetRoot.hpp>
#include <XSLT/XSLTEngineImpl.hpp>
#include <XSLT/XSLTInit.hpp>
#include <XSLT/XSLTInputSource.hpp>
#include <XSLT/XSLTProcessorEnvSupportDefault.hpp>
#include <XSLT/XSLTResultTarget.hpp>



#include <XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <XalanSourceTree/XalanSourceTreeParserLiaison.hpp>



#include <XalanTransformer/XalanTransformerOutputStream.hpp>



#include <iostream>



#if !defined(XALAN_NO_NAMESPACES)
	using std::ostream;
	using std::istream;
#endif



/**
 * This is a simple C++ interface for some common usage patterns. It's 
 * the responsibility of the caller to call initialize and terminate 
 * before creating and after deleting any XalanTransformer instances 
 * respectively. As well, releasing any memory allocated from calling   
 * XalanTransformToData by calling XalanFreeData.
 */
class XALAN_TRANSFORMER_EXPORT XalanTransformer
{
public:
	
	XalanTransformer();

	virtual ~XalanTransformer();

	/**
	 * Initialize Xerces and Xalan.
	 * Should be called only once before creating any
	 * instances of XalanTransformer. See class XSLTInit.
	 */
	static void
	initialize();

	/**
	 * Terminate Xalan and Xerces.
	 * Should be called only once after deleting all
	 * instances of XalanTransformer. See class XSLTInit.
	 */
	static void
	terminate();

	/**
	 * Transform the source tree to the output in the given result tree target.
	 * The processor will process the input source, the stylesheet source,
	 * and transform to the output target. Call internally by all transform
	 * overloads.
	 *
	 * @param inputSource		input source
	 * @param stylesheetSource	stylesheet source
	 * @param outputTarget		output source tree
	 * @return	0 for success and 1 for failure 
	 */
	int
	transform(
			const XSLTInputSource&		inputSource, 
			const XSLTInputSource&		stylesheetSource,
			XSLTResultTarget&			outputTarget);

	/**
	 * Transform the XML source tree to the given result file.
	 * The processor will process the input file, the stylesheet file,
	 * and transform to new output file.
	 *
	 * @param theXMLFileName	filename of XML input source
	 * @param theXSLFileName	filename of stylesheet source
	 * @param theOutFileName	filename of output source
	 * @return	0 for success and 1 for failure 
	 */
	int
	transform(
			const char*					theXMLFileName, 
			const char*					theXSLFileName,
			const char*					theOutFileName);

	/**
	 * Transform the XML source tree to an output stream.
	 * The processor will process the input file, the stylesheet file,
	 * and transform to an output stream.
	 *
	 * @param theXMLFileName	filename of XML input source
	 * @param theXSLFileName	filename of stylesheet source
	 * @param theOutStream		a pointer to a std ostream for the output
	 * @return	0 for success and 1 for failure 
	 */
	int
	transform(
			const char*					theXMLFileName, 
			const char*					theXSLFileName,
			ostream*					theOutStream);

	/**
	 * Transform the XML source tree to an output stream.
	 * The processor will process the input stream, the stylesheet input 
	 * stream, and transform to an output stream.
	 *
	 * @param theXMLInStream	a pointer to a std istream for the input
	 * @param theXSLInStream	a pointer to a std istream for the input
	 * @param theOutStream		a pointer to a std ostream for the output
	 * @return	0 for success and 1 for failure 
	 */
	int
	XalanTransformer::transform(
			istream*					theXMLInStream, 
			istream*					theXSLInStream,
			ostream*					theOutStream);

	/**
	 * Transform the XML source tree to the address of a callback.
	 * The processor will process the input file, the stylesheet file,
	 * and transform to output to a callback function in allocated 
	 * blocks. Xalan will release any memory allocated upon 
	 * termination. Data passed to the callback is not guaranteed to 
	 * be null terminated. - See XalanTransformerOutputStream and 
	 * XalanOutputHandlerType for more details.
	 * 
	 * @param theXMLFileName	filename of XML input source
	 * @param theXSLFileName	filename of stylesheet source	 
	 * @param theOutputHandle	void pointer passed through to callback.
	 * @param theOutputHandler	address of a user defined (callback) function.
	 * @return	0 for success and 1 for failure 
	 */
	int
	XalanTransformer::transform(
			const char*					theXMLFileName, 
			const char*					theXSLFileName,
			const void*					theOutputHandle, 
			XalanOutputHandlerType		theOutputHandler);

protected:

private:

	XalanSourceTreeDOMSupport				m_domSupport;
	XalanSourceTreeParserLiaison			m_parserLiaison;
	XSLTProcessorEnvSupportDefault			m_xsltprocessorEnvSupport;
	XObjectFactoryDefault					m_xobjectFactory;
	XPathFactoryDefault						m_xpathFactory;
	XSLTEngineImpl							m_processor;
	StylesheetConstructionContextDefault	m_stylesheetConstructionContext;
	StylesheetExecutionContextDefault		m_stylesheetExecutionContext;

	static XSLTInit*	m_xsltInit;
};



#endif	// XALAN_TRANSFORMER_HEADER_GUARD