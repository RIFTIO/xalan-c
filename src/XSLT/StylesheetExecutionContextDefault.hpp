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
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */
#if !defined(STYLESHEETEXECUTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680)
#define STYLESHEETEXECUTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680



// Base class include file.
#include "StylesheetExecutionContext.hpp"



#include <ctime>
#include <deque>
#include <memory>
#include <set>
#include <vector>



#include <XPath/XPathExecutionContextDefault.hpp>



#if defined(XALAN_AUTO_PTR_REQUIRES_DEFINITION)
#include <XalanSourceTree/XalanSourceTreeDocument.hpp>
#endif



#include <XSLT/CountersTable.hpp>
#include <XSLT/Stylesheet.hpp>
#include <XSLT/VariablesStack.hpp>



class XalanSourceTreeDocument;
class XPathProcessor;
class XObjectFactory;
class XSLTEngineImpl;



//
// An class which provides support for executing stylesheets.
//
class XALAN_XSLT_EXPORT StylesheetExecutionContextDefault : public StylesheetExecutionContext
{
public:

#if defined(XALAN_NO_NAMESPACES)
	typedef deque<const ElemTemplateElement*>			ElementRecursionStackType;
	typedef set<FormatterListener*,
				less<FormatterListener*> >				FormatterListenerSetType;
	typedef set<PrintWriter*,
				less<PrintWriter*> >					PrintWriterSetType;
	typedef set<XalanOutputStream*,
				less<XalanOutputStream*> >				OutputStreamSetType;
	typedef set<const KeyDeclaration*,
				less<const KeyDeclaration*> >			KeyDeclarationSetType;
	typedef pair<const XPath*, clock_t>					XPathCacheEntry;
	typedef map<XalanDOMString,
				XPathCacheEntry,
				less<XalanDOMString> >					XPathCacheMapType;
#else
	typedef std::deque<const ElemTemplateElement*>		ElementRecursionStackType;
	typedef std::set<FormatterListener*>				FormatterListenerSetType;
	typedef std::set<PrintWriter*>						PrintWriterSetType;
	typedef std::set<XalanOutputStream*>				OutputStreamSetType;
	typedef std::set<const KeyDeclaration*>				KeyDeclarationSetType;
	typedef std::pair<const XPath*, clock_t>			XPathCacheEntry;
	typedef std::map<XalanDOMString, XPathCacheEntry>	XPathCacheMapType;
#endif

	typedef Stylesheet::KeyTablesTableType				KeyTablesTableType;
	typedef VariablesStack::ParamsVectorType			ParamsVectorType;


	/**
	 * Construct a StylesheetExecutionContextDefault object
	 *
	 * @param theXPathEnvSupport XPath environment support class instance
	 * @param theDOMSupport		 DOMSupport class instance
	 * @param theXobjectFactory  factory class instance for XObjects
	 * @param theCurrentNode     current node in the source tree
	 * @param theContextNodeList node list for current context
	 * @param thePrefixResolver  pointer to prefix resolver to use
	 */	
	StylesheetExecutionContextDefault(
			XSLTEngineImpl&			xsltProcessor,
			XPathEnvSupport&		theXPathEnvSupport,
			DOMSupport&				theDOMSupport,
			XObjectFactory&			theXObjectFactory,
			XalanNode*				theCurrentNode = 0,
			const NodeRefListBase*	theContextNodeList = 0,
			const PrefixResolver*	thePrefixResolver = 0);

	virtual
	~StylesheetExecutionContextDefault();

	/**
	 * Get the value of the flag that controls whether result tree
	 * fragments are created using a DOM factory, or a XalanSourceTreeDocument.
	 *
	 * @return The value
	 */
	bool
	getUseDOMResultTreeFactory() const
	{
		return m_useDOMResultTreeFactory;
	}

	/**
	 * Set the value of the flag that controls whether result tree
	 * fragments are created using a DOM factory, or a XalanSourceTreeDocument.
	 *
	 * @param theValue The boolean value
	 */
	void
	setUseDOMResultTreeFactory(bool		theValue)
	{
		m_useDOMResultTreeFactory = theValue;
	}

	/**
	 * Set the value of the flag that controls whether HTML output will
	 * check for namespace declarations on HTML elements.
	 *
	 * Set this to true if you want the HTML output formatter to skip
	 * checking for namespaces on elements.  This makes HTML output more
	 * efficient, but can result in non-conforming behavior, since the XSLT
	 * recommendation requires XML output for elements with namespaces.
	 *
	 * @param theValue The boolean value
	 */
	bool
	setIgnoreHTMLElementNamespaces() const
	{
		return m_ignoreHTMLElementNamespaces;
	}

	/**
	 * Set the value of the flag that controls whether HTML output will
	 * check for namespace declarations on HTML elements.
	 *
	 * @return The value
	 */
	void
	setIgnoreHTMLElementNamespaces(bool		theValue)
	{
		m_ignoreHTMLElementNamespaces = theValue;
	}

	// These interfaces are inherited from StylesheetExecutionContext...

	virtual bool
	getQuietConflictWarnings() const;

	virtual XalanNode*
	getRootDocument() const;

	virtual void
	setRootDocument(XalanNode*	theDocument);

	virtual XalanDocument*
	createDocument() const;

	virtual void
	setStylesheetRoot(const StylesheetRoot*		theStylesheet);

	virtual const QName*
	getCurrentMode() const;

	virtual	void
	setCurrentMode(const QName* theMode); 

	virtual void
	resetCurrentState(
			XalanNode*	xmlNode);

	virtual bool
	doDiagnosticsOutput() const;

	virtual void
	diag(const XalanDOMString&	theString);

	virtual void
	pushTime(const void*	theKey);

	virtual void
	displayDuration(
			const XalanDOMString&	theMessage,
			const void*				theKey);

	virtual bool
	isElementPending() const;

	virtual void
	replacePendingAttribute(
			const XalanDOMChar*		theName,
			const XalanDOMChar*		theNewType,
			const XalanDOMChar*		theNewValue);

	virtual void
	pushOutputContext(FormatterListener*	flistener = 0);

	virtual void
	popOutputContext();

	virtual void
	addResultAttribute(
			const XalanDOMString&	aname,
			const XalanDOMString&	value);

	virtual void
	copyNamespaceAttributes(const XalanNode&	src);

	virtual const XalanDOMString&
	getResultPrefixForNamespace(const XalanDOMString&	theNamespace) const;

	virtual const XalanDOMString&
	getResultNamespaceForPrefix(const XalanDOMString&	thePrefix) const;

	virtual XalanDOMString
	getUniqueNamespaceValue() const;

	virtual void
	getUniqueNamespaceValue(XalanDOMString&		theValue) const;

	virtual FormatterListener*
	getFormatterListener() const;

	virtual void
	setFormatterListener(FormatterListener*		flistener);

	virtual int
	getIndent() const;

	// $$$ ToDo: Get rid of this!!!!
	virtual const XObjectPtr
	executeXPath(
			const XalanDOMString&	str,
			XalanNode*				contextNode,
			const XalanElement&		resolver);

	virtual const XPath*
	createMatchPattern(
			const XalanDOMString&	str,
			const PrefixResolver&	resolver);

	virtual void
	returnXPath(const XPath*	xpath);

	virtual void
	pushTopLevelVariables(const ParamVectorType&	topLevelParams);

	virtual const XObjectPtr
	createVariable(
			const ElemTemplateElement*	element,
			const XPath&				xpath,
			XalanNode*					contextNode,
			const PrefixResolver&		resolver);

	virtual const XObjectPtr
	createVariable(
			const ElemTemplateElement*	element,
			const ElemTemplateElement&	templateChild,
			XalanNode*					sourceNode);

	virtual void
	pushVariable(
			const QName&				name,
			const ElemTemplateElement*	element,
			const XalanDOMString&		str,
			XalanNode*					contextNode,
			const PrefixResolver&		resolver);

	virtual void
	pushVariable(
			const QName&				name,
			const XObjectPtr			val,
			const ElemTemplateElement*	element);

	virtual void
	pushVariable(
			const QName&				name,
			const ElemVariable*			var,
			const ElemTemplateElement*	element);

	virtual void
	pushVariable(
			const QName&				name,
			const ElemTemplateElement*	element,
			const XPath&				xpath,
			XalanNode*					contextNode,
			const PrefixResolver&		resolver);

	virtual void
	pushVariable(
			const QName&				name,
			const ElemTemplateElement*	element,
			const ElemTemplateElement&	templateChild,
			XalanNode*					sourceNode);

	virtual void
	pushContextMarker();

	virtual void
	popContextMarker();

	virtual void
	resolveTopLevelParams();

	virtual void
	clearTopLevelParams();

	virtual	void
	pushParams(
			const ElemTemplateElement&	xslCallTemplateElement,
			XalanNode*					sourceNode,
			const ElemTemplateElement*	targetTemplate);

	virtual const XObjectPtr
	getParamVariable(const QName&	theName);

	virtual void
	pushElementFrame(const ElemTemplateElement*		elem);

	virtual void
	popElementFrame(const ElemTemplateElement*	elem);

	virtual int
	getCurrentStackFrameIndex() const;

	virtual void
	setCurrentStackFrameIndex(int currentStackFrameIndex = -1);

	virtual void
	startDocument();

	virtual void
	endDocument();

	virtual void
	startElement(const XalanDOMChar*	name);

	virtual void
	endElement(const XalanDOMChar*	name);

	virtual void
	characters(
			const XalanDOMChar*		ch,
			unsigned int			start,
			unsigned int			length);

	virtual void
	charactersRaw(
			const XalanDOMChar*		ch,
			unsigned int			start,
			unsigned int			length);

	virtual void
	comment(const XalanDOMChar*		data);

	virtual void
	processingInstruction(
			const XalanDOMChar*		target,
			const XalanDOMChar*		data);

	virtual void
	flushPending();

	virtual void
	cloneToResultTree(
			XalanNode&	node, 
			bool		isLiteral,
			bool		overrideStrip,
			bool		shouldCloneAttributes);

	virtual const XObjectPtr
	createXResultTreeFrag(
			const ElemTemplateElement&	templateChild,
			XalanNode*					sourceNode);

	virtual void
	outputToResultTree(const XObject&	xobj);

	virtual void
	outputResultTreeFragment(const XObject&		theTree);

	virtual const XalanDOMString&
	getXSLNameSpaceURL() const;

	virtual const XalanDOMString&
	getXalanXSLNameSpaceURL() const;

	virtual bool
	findOnElementRecursionStack(const ElemTemplateElement*	theElement) const;

	virtual void
	pushOnElementRecursionStack(const ElemTemplateElement*	theElement);

	virtual const ElemTemplateElement*
	popElementRecursionStack();

	virtual FormatterToXML*
	createFormatterToXML(
			Writer&					writer,
			const XalanDOMString&	version = XalanDOMString(),
			bool					doIndent = false,
			int						indent = 0,
			const XalanDOMString&	encoding = XalanDOMString(),
			const XalanDOMString&	mediaType = XalanDOMString(),
			const XalanDOMString&	doctypeSystem = XalanDOMString(),
			const XalanDOMString&	doctypePublic = XalanDOMString(),
			bool					xmlDecl = true,
			const XalanDOMString&	standalone = XalanDOMString());

	virtual FormatterToHTML*
	createFormatterToHTML(
			Writer&					writer,
			const XalanDOMString&	encoding = XalanDOMString(),
			const XalanDOMString&	mediaType = XalanDOMString(),
			const XalanDOMString&	doctypeSystem = XalanDOMString(),
			const XalanDOMString&	doctypePublic = XalanDOMString(),
			bool					doIndent = true,
			int						indent = 4,
			const XalanDOMString&	version = XalanDOMString(),
			const XalanDOMString&	standalone = XalanDOMString(),
			bool					xmlDecl = false);

	virtual FormatterToDOM*
	createFormatterToDOM(
			XalanDocument*			doc,
			XalanDocumentFragment*	docFrag,
			XalanElement*			currentElement);

	virtual FormatterToDOM*
	createFormatterToDOM(
			XalanDocument*	doc,
			XalanElement*	elem);

	virtual FormatterToText*
	createFormatterToText(
			Writer&					writer,
			const XalanDOMString&	encoding);

	virtual XalanNumberFormatAutoPtr
	createXalanNumberFormat();

	// A basic class to create XalanNumberFormat instances...
	class XALAN_XSLT_EXPORT XalanNumberFormatFactory
	{
	public:

		explicit
		XalanNumberFormatFactory();

		virtual
		~XalanNumberFormatFactory();

		virtual XalanNumberFormat*
		create();
	};

	static XalanNumberFormatFactory&
	getDefaultXalanNumberFormatFactory()
	{
		return s_defaultXalanNumberFormatFactory;
	}

	/**
	 * Static function to install a new XalanNumberFormatFactory.
	 * The caller owns the XalanNumberFormatFactory instance.
	 *
	 * @param a pointer to the new factory instance to use.
	 * @return a pointer to the old factory instance.
	 */
	static XalanNumberFormatFactory*
	installXalanNumberFormatFactory(XalanNumberFormatFactory*	theFactory);


	virtual unsigned long
	getTraceListeners() const;

	virtual void
	fireGenerateEvent(const GenerateEvent&	ge);

	virtual void
	fireTraceEvent(const TracerEvent&	te);

	virtual void
	fireSelectEvent(const SelectionEvent&	se);

	virtual bool
	getTraceSelects() const;

	virtual void
	traceSelect(
			const XalanElement&		theTemplate,
			const NodeRefListBase&	nl) const;

	virtual int
	collationCompare(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS);

	virtual int
	collationCompare(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS);

	class XALAN_XSLT_EXPORT CollationCompareFunctor
	{
	public:

		CollationCompareFunctor();

		virtual
		~CollationCompareFunctor();

		// Non-const version is suitable for use by
		// a singe thread.
		virtual int
		operator()(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS) = 0;

		// Const version is suitable for use by
		// multiple threads.
		virtual int
		operator()(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS) const = 0;
	};

	class XALAN_XSLT_EXPORT DefaultCollationCompareFunctor : public CollationCompareFunctor
	{
	public:

		DefaultCollationCompareFunctor();

		virtual
		~DefaultCollationCompareFunctor();

		virtual int
		operator()(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS);

		virtual int
		operator()(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS) const;
	};

	CollationCompareFunctor*
	installCollationCompareFunctor(CollationCompareFunctor*		theFunctor);

	virtual	bool
	getInConstruction(const KeyDeclaration&		keyDeclaration) const;

	virtual	void
	beginConstruction(const KeyDeclaration&		keyDeclaration);

	virtual	void
	endConstruction(const KeyDeclaration&	keyDeclaration);

	virtual const XalanDecimalFormatSymbols*
	getDecimalFormatSymbols(const XalanDOMString&	name);

	virtual PrintWriter*
	createPrintWriter(XalanOutputStream*		theTextOutputStream);

	virtual PrintWriter*
	createPrintWriter(
			const XalanDOMString&		theFileName,
			const XalanDOMString&		theEncoding);

	virtual PrintWriter*
#if defined(XALAN_NO_NAMESPACES)
	createPrintWriter(ostream&			theStream);
#else
	createPrintWriter(std::ostream&		theStream);
#endif

	virtual CountersTable&
	getCountersTable();

	// These interfaces are inherited from XPathExecutionContext...

	virtual void
	reset();

	virtual XalanNode*
	getCurrentNode() const;

	virtual void
	setCurrentNode(XalanNode*	theCurrentNode);

	virtual XObjectFactory&
	getXObjectFactory() const;

	virtual XObjectPtr
	createNodeSet(XalanNode&	theNode);

	virtual bool
	isNodeAfter(
			const XalanNode&	node1,
			const XalanNode&	node2) const;

	virtual const NodeRefListBase&
	getContextNodeList() const;

	virtual void	
	setContextNodeList(const NodeRefListBase&	theList);

	virtual unsigned int
	getContextNodeListLength() const;

	virtual unsigned int
	getContextNodeListPosition(const XalanNode&		contextNode) const;

	virtual bool
	elementAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	elementName) const;

	virtual bool
	functionAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	functionName) const;

	virtual const XObjectPtr
	extFunction(
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			functionName,
			XalanNode*						context,
			const XObjectArgVectorType&		argVec);

	virtual XLocator*
	getXLocatorFromNode(const XalanNode*	node) const;

	virtual void
	associateXLocatorToNode(
			const XalanNode*	node,
			XLocator*			xlocator);

	virtual XalanDocument*
	parseXML(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base) const;

	virtual MutableNodeRefList*
	borrowMutableNodeRefList();

	virtual bool
	returnMutableNodeRefList(MutableNodeRefList*	theList);

	virtual ResultTreeFragBase*
	borrowResultTreeFrag();

	virtual bool
	returnResultTreeFrag(ResultTreeFragBase*	theResultTreeFragBase);

	virtual MutableNodeRefList*
	createMutableNodeRefList() const;

	virtual XalanDOMString&
	getCachedString();

	virtual bool
	releaseCachedString(XalanDOMString&		theString);

	virtual void
	getNodeSetByKey(			
			XalanNode*				doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref,
			const PrefixResolver&	resolver,
			MutableNodeRefList&		nodelist);

	virtual const XObjectPtr
	getVariable(const QName&	name);

	virtual const PrefixResolver*
	getPrefixResolver() const;

	virtual void
	setPrefixResolver(const PrefixResolver*		thePrefixResolver);

	virtual const XalanDOMString&
	getNamespaceForPrefix(const XalanDOMString&		prefix) const;

	virtual XalanDOMString
	findURIFromDoc(const XalanDocument*		owner) const;

	virtual XalanDocument*
	getDOMFactory() const;

	virtual const XalanDOMString&
	getUnparsedEntityURI(
			const XalanDOMString&	theName,
			const XalanDocument&	theDocument) const;

	virtual bool
	shouldStripSourceNode(const XalanNode&	node);

	virtual bool
	getThrowFoundIndex() const;

	virtual void
	setThrowFoundIndex(bool 	fThrow);

	virtual XalanDocument*
	getSourceDocument(const XalanDOMString&		theURI) const;

	virtual void
	setSourceDocument(
			const XalanDOMString&	theURI,
			XalanDocument*			theDocument);


	// These interfaces are inherited from ExecutionContext...

	virtual void
	error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode*		styleNode = 0) const;

	virtual void
	error(
			const char*			msg,
			const XalanNode* 	sourceNode = 0,
			const XalanNode* 	styleNode = 0) const;

	virtual void
	warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode* 		styleNode = 0) const;

	virtual void
	warn(
			const char*			msg,
			const XalanNode* 	sourceNode = 0,
			const XalanNode* 	styleNode = 0) const;

	virtual void
	message(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode* 		styleNode = 0) const;

	virtual void
	message(
			const char*			msg,
			const XalanNode* 	sourceNode = 0,
			const XalanNode* 	styleNode = 0) const;


	class XPathCacheReturnFunctor
	{
	public:

		XPathCacheReturnFunctor(XSLTEngineImpl&		xsltProcessor) :
			m_xsltProcessor(xsltProcessor)
		{
		}

		void
		operator()(const XPathCacheMapType::value_type&		theCacheEntry);

	private:

		XSLTEngineImpl&		m_xsltProcessor;
	};

private:

	/**
	 * Given a context, create the params for a template
	 * call.
	 *
	 * @param xslCallTemplateElement "call-template" element
	 * @param sourceNode             source node
	 * @param params The params
	 */
	void
	getParams(
			const ElemTemplateElement&	xslCallTemplateElement,
			XalanNode*					sourceNode,
			ParamsVectorType&			params);

	/**
	 * Get a XalanSourceTreeDocument, primarily for creating result 
	 * tree fragments.
	 */
	XalanSourceTreeDocument*
	getSourceTreeFactory() const;

	/**
	 * Determine if the XPath is one that we have cached.
	 *
	 * @param theXPath the XPath instance to check
	 * @return true if the instance has been cached, false if not.
	 */
	bool
	isCached(const XPath*	theXPath);

	/**
	 * Clear out the cache of XPath instances.
	 */
	void
	clearXPathCache();

	/**
	 * Add an XPath instance to the cache, clearing out an old entry
	 * if the cache is full.
	 *
	 * @param pattern the key for looking up the XPath instance in the cache.
	 * @param theXPath the XPath instance to cache
	 */
	void
	addToXPathCache(
			const XalanDOMString&	pattern,
			const XPath*			theXPath);


	/**
	 * Clean up anything that was created for use only during the transformation.
	 */
	void
	cleanUpTransients();

	XPathExecutionContextDefault	m_xpathExecutionContextDefault;

	// $$ ToDo: Try to remove this dependency, and rely only on XSLTProcessor...
	XSLTEngineImpl&					m_xsltProcessor;

	XalanNode*						m_rootDocument;

	enum { eDefaultVariablesCollectionSize = 10,
		   eXPathCacheMax = 50,
		   eDefaultVariablesStackSize = 200,
		   eDefaultParamsVectorSize = 10 };

	ElementRecursionStackType			m_elementRecursionStack;

	const PrefixResolver*				m_prefixResolver;

	const StylesheetRoot*				m_stylesheetRoot;

	FormatterListenerSetType			m_formatterListeners;

	PrintWriterSetType					m_printWriters;

	OutputStreamSetType					m_outputStreams;

	CollationCompareFunctor*			m_collationCompareFunctor;

	/**
	 * Holds all information about variables during execution.
	 */
	VariablesStack						m_variablesStack;

	ParamsVectorType					m_paramsVector;

	XPathCacheMapType					m_matchPatternCache;

	KeyTablesTableType					m_keyTables;

	KeyDeclarationSetType				m_keyDeclarationSet;

	CountersTable						m_countersTable;

	bool								m_useDOMResultTreeFactory;

	// If true, we will not check HTML output for elements with
	// namespaces.  This is an optimization which can lead to
	// non-conforming behavior.
	bool								m_ignoreHTMLElementNamespaces;

	// Holds the current mode.
	const QName*	                    m_mode;

	/**
	 * The factory that will be used to create result tree fragments based on our
	 * proprietary source tree.
	 */
	mutable XalanAutoPtr<XalanSourceTreeDocument>	m_sourceTreeResultTreeFactory;

	static XalanNumberFormatFactory		s_defaultXalanNumberFormatFactory;

	static XalanNumberFormatFactory*	s_xalanNumberFormatFactory;

	const static DefaultCollationCompareFunctor		s_defaultCollationFunctor;
};



#endif	// STYLESHEETEXECUTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680
