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
 *	  notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in
 *	  the documentation and/or other materials provided with the
 *	  distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *	  if any, must include the following acknowledgment:  
 *		 "This product includes software developed by the
 *		  Apache Software Foundation (http://www.apache.org/)."
 *	  Alternately, this acknowledgment may appear in the software itself,
 *	  if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *	  not be used to endorse or promote products derived from this
 *	  software without prior written permission. For written 
 *	  permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *	  nor may "Apache" appear in their name, without prior written
 *	  permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.	IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
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

#if !defined(XALAN_STYLESHEET_HEADER_GUARD)
#define XALAN_STYLESHEET_HEADER_GUARD



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



#include <list>
#include <map>
#include <vector>



#include <XalanDOM/XalanDocument.hpp>
#include <XalanDOM/XalanEmptyNamedNodeMap.hpp>
#include <XalanDOM/XalanNodeListSurrogate.hpp>


#include <XPath/PrefixResolver.hpp>
#include <XPath/NameSpace.hpp>
#include <XPath/QNameByValue.hpp>



#include "NamespacesHandler.hpp"
#include "KeyDeclaration.hpp"
#include "StylesheetExecutionContext.hpp"



class AttributeList;
class ExtensionNSHandler;
class XalanDecimalFormatSymbols;
class ElemAttributeSet;
class ElemDecimalFormat;
class ElemTemplate;
class ElemTemplateElement;
class ElemVariable;
class KeyTable;
class MatchPattern2;
class NodeRefListBase;
class PrefixResolver;
class StylesheetConstructionContext;
class StylesheetRoot;
class XMLURL;
class XObject;
class XPath;
class StylesheetExecutionContext;



/**
 * This class represents the base stylesheet or an "import" stylesheet.
 * "include" stylesheets are combined with the including stylesheet.
 */
class XALAN_XSLT_EXPORT Stylesheet : public XalanDocument, private PrefixResolver
{	

public:

	typedef StylesheetExecutionContext::ParamVectorType		ParamVectorType;
	typedef NamespacesHandler::NamespaceVectorType			NamespaceVectorType;
	typedef NamespacesHandler::NamespacesStackType			NamespacesStackType;

#if defined(XALAN_NO_NAMESPACES)
	typedef map<XalanDOMString,
				XalanDOMString,
				less<XalanDOMString> >				StringToStringMapType;
	typedef map<XalanDOMString,
				ExtensionNSHandler*,
				less<XalanDOMString> >				ExtensionNamespacesMapType;
	typedef map<QNameByValue,
				ElemTemplate*,
				less<QName> >						ElemTemplateMapType;
	typedef vector<ElemAttributeSet*> 				AttributeSetMapType;
	typedef vector<ElemVariable*> 					ElemVariableVectorType;
	typedef vector<KeyDeclaration>					KeyDeclarationVectorType;
	typedef map<const XalanNode*,
				KeyTable*,
				less<const XalanNode*> >			KeyTablesTableType;
	typedef vector<QNameByValue> 					QNameVectorType;
	typedef vector<Stylesheet*>						StylesheetVectorType;
	typedef vector<XalanDOMString>					URLStackType;
	typedef vector<const XPath*>					XPathVectorType;
	typedef vector<ElemDecimalFormat*>				ElemDecimalFormatVectorType;
#else
	typedef std::map<XalanDOMString, XalanDOMString>		StringToStringMapType;
	typedef std::map<XalanDOMString, ExtensionNSHandler*>	ExtensionNamespacesMapType;
	typedef std::map<QNameByValue, ElemTemplate*>			ElemTemplateMapType;
	typedef std::vector<ElemAttributeSet*> 					AttributeSetMapType;
	typedef std::vector<ElemVariable*> 						ElemVariableVectorType;
	typedef std::vector<KeyDeclaration>						KeyDeclarationVectorType;
	typedef std::map<const XalanNode*, KeyTable*>			KeyTablesTableType;
	typedef std::vector<QNameByValue> 						QNameVectorType;
	typedef std::vector<Stylesheet*>						StylesheetVectorType;
	typedef std::vector<XalanDOMString>						URLStackType;
	typedef std::vector<const XPath*>						XPathVectorType;
	typedef std::vector<ElemDecimalFormat*>					ElemDecimalFormatVectorType;
#endif

	/**
	 * Constructor for a Stylesheet needs a Document.
	 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
	 * the error condition is severe enough to halt processing.
	 */
	Stylesheet(
			StylesheetRoot& 				root,
			const XalanDOMString&			baseIdentifier,
			StylesheetConstructionContext&	constructionContext);

	virtual
	~Stylesheet();

	/**
	 * Retrieve XSLT version number
	 * 
	 * @return number representing XSLT version
	 */
	double
	getXSLTVerDeclared() const
	{
		return m_XSLTVerDeclared;
	}

	/**
	 * Set XSLT version number
	 * 
	 * @param ver number representing XSLT version
	 */
	void
	setXSLTVerDeclared(double ver)
	{
		m_XSLTVerDeclared = ver;
	}

	/**
	 * Retrieve the root stylesheet object
	 * 
	 * @return const reference to the stylesheet object
	 */
	const StylesheetRoot&
	getStylesheetRoot() const
	{
		return m_stylesheetRoot;
	}

	/**
	 * Retrieve the root stylesheet object
	 * 
	 * @return reference to the stylesheet object
	 */
	StylesheetRoot&
	getStylesheetRoot()
	{
		return m_stylesheetRoot;
	}

	/**
	 * Retrieve the stack of namespace lists
	 * 
	 * @return vector of namespace vectors
	 */
	const NamespacesStackType&
	getNamespaces() const
	{ 
		return m_namespaces;
	}

	/**
	 * Retrieve the stack of namespace lists
	 * 
	 * @return vector of namespace vectors
	 */
	NamespacesStackType&
	getNamespaces()
	{ 
		return m_namespaces;
	}

	const NamespacesHandler&
	getNamespacesHandler() const
	{
		return m_namespacesHandler;
	}

	NamespacesHandler&
	getNamespacesHandler()
	{
		return m_namespacesHandler;
	}

	/**
	 * Retrieve the list of namespace declarations currently in effect
	 * 
	 * @return vector of namespace vectors
	 */
	const NamespaceVectorType&
	getNamespaceDecls() const
	{
		return m_namespaceDecls;
	}

	/**
	 * Retrieve the list of namespace declarations currently in effect
	 * 
	 * @return vector of namespace vectors
	 */
	NamespaceVectorType&
	getNamespaceDecls()
	{
		return m_namespaceDecls;
	}

	/**
	 * Set the list of namespace declarations currently in effect
	 * 
	 * @param ns vector of namespace vectors
	 */
	void
	setNamespaceDecls(const NamespaceVectorType& ns)
	{
		m_namespaceDecls = ns;
	}

	/*
	 * Get the top entry on the namespace stack, or 0, if
	 * there is nothing on the stack.
	 */
	const NamespaceVectorType&
	getCurrentNamespace() const;

	/** 
	 * Push the namespace declarations from the current attribute 
	 * list onto the namespace stack.
	 * 
	 * @param atts attribute list constaining namespaces
	 */
	void
	pushNamespaces(const AttributeList& 	atts);

	/**
	 * Pop a namespace declaration from the namespace stack.
	 */
	void
	popNamespaces();

	/**
	 * Called after construction is completed.
	 */
	virtual void
	postConstruction();

	/** 
	 * See if this is a xmlns attribute, and, if so, process it.
	 * 
	 * @param attrName qualified name of attribute
	 * @param atts	   attribute list where the element comes from (not used at 
	 *				   this time)
	 * @param which    index into the attribute list (not used at this time)
	 * @return		   true if this is a namespace name
	 */
	bool
	isAttrOK(
			const XalanDOMChar*				attrName,
			const AttributeList&			atts,
			int 							which,
			StylesheetConstructionContext&	constructionContext) const;

	/**
	 * Get the namespace from a qualified name.
	 * 
	 * @param nodeName name of node
	 * @return namespace string for node
	 */
	const XalanDOMString&
	getNamespaceFromStack(const XalanDOMString& 	nodeName) const;

	/**
	 * Get the namespace from a qualified name.
	 * 
	 * @param nodeName name of node
	 * @return namespace string for node
	 */
	const XalanDOMString&
	getNamespaceFromStack(const XalanDOMChar* 	nodeName) const;

	/**
	 * Get the namespace from a prefix by searching the stack of namespace
	 * lists.
	 * 
	 * @param prefix prefix to search
	 * @return namespace corresponding to prefix
	 */
	const XalanDOMString&
	getNamespaceForPrefixFromStack(const XalanDOMString&	prefix) const;

	/**
	 * Get the namespace from a prefix by searching the stack of namespace
	 * lists.
	 * 
	 * @param prefix prefix to search
	 * @return namespace corresponding to prefix
	 */
	const XalanDOMString&
	getNamespaceForPrefixFromStack(const XalanDOMChar*	prefix) const;

	/**
	 * See if there is a namespace alias.
	 * 
	 * @param uri the URI of the namespace.
	 * @return the alias URI, if found.
	 */
	XalanDOMString
	getAliasNamespaceURI(const XalanDOMChar*	uri) const;

	/**
	 * See if there is a namespace alias.
	 * 
	 * @param uri the URI of the namespace.
	 * @return the alias URI, if found.
	 */
	XalanDOMString
	getAliasNamespaceURI(const XalanDOMString&	uri) const;

	/**
	 * See if a namespace should be excluded.
	 * 
	 * @param theValue the prefix of the namespace.
	 * @param theConstructionContext the current construction context.
	 * @return
	 */
	void
	processExcludeResultPrefixes(
		const XalanDOMChar*				theValue,
		StylesheetConstructionContext&	theConstructionContext);

	/**
	 * This recursive function is called starting from the
	 * stylesheet root, and tries to find a match for the
	 * passed stylesheet, and then will return the previous
	 * sibling, or 0 if there was no previous sibling.
	 *
	 * @param stylesheet the stylesheet to search
	 * @return the stylesheet's previous import if found, or 0 not found.
	 */
	const Stylesheet*
	getPreviousImport(const Stylesheet*		stylesheet) const;

	/**
	 * Add a template to the list of names templates
	 * 
	 * @param tmpl template to add
	 * @param constructionContext context for construction
	 */
	void
	addTemplate(
			ElemTemplate*					tmpl,
			StylesheetConstructionContext&	constructionContext);

	/**
	 * Process an attribute that has the value of 'yes' or 'no'.
	 * 
	 * @param aname name of attribute
	 * @param val value
	 * @param constructionContext context for construction
	 * @return true if value equals string constant for "yes," false otherwise
	 */
	virtual bool
	getYesOrNo(
			const XalanDOMChar*				aname,
			const XalanDOMChar*				val,
			StylesheetConstructionContext&	constructionContext) const;

	/**
	 * Tell if this is the root of the stylesheet tree.
	 * 
	 * @return true if it is the root
	 */
	bool
	isRoot() const
	{
		return m_isRoot;
	}

	/**
	 * Retrieve the base identifier with which this stylesheet is associated.
	 * 
	 * @return string for base identifier
	 */
	const XalanDOMString&
	getBaseIdentifier() const
	{
		return m_baseIdent;
	}

	/**
	 * Set the base identifier with which this stylesheet is associated.
	 * 
	 * @param str string for base identifier
	 */
	void
	setBaseIdentifier(const XalanDOMString&		str)
	{
		m_baseIdent = str;
	}

	/**
	 * Retrieve the base identifier for the most recently
	 * included stylesheet.  This will return the same value
	 * as getBaseIdentifier(), if no include is being
	 * processed.
	 * 
	 * @return string for base identifier
	 */
	const XalanDOMString&
	getCurrentIncludeBaseIdentifier() const;

	/**
	 * Process an xsl:namespace-alias element.
	 *
	 * @param name   the element name.
	 * @param attrs	 the current attribute list
	 * @param constructionContext  the active construction context
	 */
	void
	processNSAliasElement(
			const XalanDOMChar*				name,
			const AttributeList&			atts,
			StylesheetConstructionContext&	constructionContext);

	/**
	 * Process an xsl:decimal-format element.
	 *
	 * @param elemDecimalFormat   the element
	 * @param attrs	 the current attribute list
	 * @param constructionContext  the active construction context
	 */
	void
	processDecimalFormatElement(
			ElemDecimalFormat*				elemDecimalFormat,
			const AttributeList&			atts,
			StylesheetConstructionContext&	constructionContext);

	/**
	 * Retrieve the XalanDecimalFormatSymbols instance associated with
	 * the name.
	 *
	 * @param name the name for the lookup
	 * @return a pointer to the matching instance, or 0 if none was found
	 */
	const XalanDecimalFormatSymbols*
	getDecimalFormatSymbols(const XalanDOMString&	name) const;

	/**
	 * Add an attribute set to the list.
	 *
	 * @param qname   qualified name of attribute set
	 * @param attrSet pointer to attribute set to add
	 */
	void
	addAttributeSet(
		const QName&		qname, 
		ElemAttributeSet*	attrSet);

	/**
	 * Apply the set of named attributes to a node in a given context with a
	 * given mode
	 *
	 * @param attributeSetsNames list of attribute set names
	 * @param executionContext	 current execution context
	 * @param sourceTree		 node for source tree
	 * @param sourceNode		 source node
	 * @param mode				 execution mode
	 */
	void
	applyAttrSets(
			const QNameVectorType&			attributeSetsNames,
			StylesheetExecutionContext& 	executionContext, 
			XalanNode*						sourceTree,
			XalanNode*						sourceNode,
			const QName&					mode) const;
  
	/**
	 * Determine whether default whitespace processing is in effect
	 * 
	 * @return true if default whitespace processing is in effect
	 */
	bool
	isDefaultSpaceProcessing() const
	{
		return m_defaultSpace;
	}

	/**
	 * Set whether default whitespace processing is in effect
	 * 
	 * @param bEnabled true if default processing should be enabled
	 */
	void
	setDefaultSpaceProcessing(bool bEnabled)
	{
		m_defaultSpace = bEnabled;
	}

	/**
	 * Add an imported stylesheet.
	 *
	 * @param theStylesheet The stylesheet to add.
	 * @param fFront If true, the stylesheet is added to the front of the imports, instead of the end.
	 */
	void
	addImport(
			Stylesheet*		theStylesheet,
			bool			fFront)
	{
		m_imports.insert(fFront ? m_imports.begin() : m_imports.end(), theStylesheet);
	}

	/**
	 * Retrieve the manufactured template to use if there is no wrapper.
	 * 
	 * @return pointer to template
	 */
	const ElemTemplate*
	getWrapperlessTemplate()
	{
		return m_wrapperlessTemplate;
	}

	/**
	 * Set the manufactured template to use if there is no wrapper.
	 * 
	 * @param templ template to use
	 */
	void
	setWrapperlessTemplate(ElemTemplate*	templ)
	{
		m_wrapperlessTemplate = templ;
	}

	/**
	 * whether there is a wrapper template
	 * 
	 * @return true is there is a wrapper
	 */
	bool
	isWrapperless() const
	{
		return m_isWrapperless;
	}

	/**
	 * Set whether there is a wrapper template
	 * 
	 * @param b true is there is a wrapper
	 */
	void
	setWrapperless(bool b)
	{
		m_isWrapperless = b;
	}

	/**
	 * Retrieve the stack of who's including who
	 * 
	 * @return stack of includes
	 */
	URLStackType&
	getIncludeStack()
	{
		return m_includeStack;
	}

	/**
	 * Process the xsl:key element.
	 * 
	 * @param nsContext 		  element providing context for namespaces
	 * @param atts				  attribute list for element
	 * @param constructionContext context for evaluation
	 */
	/*
	 * (Notes to myself)
	 * What we need to do is:
	 * 1) As this function is called, build a table of KeyDeclarations.
	 * 2) During either XML processing, or upon request, walk the XML 
	 * document tree, and build a hash table:
	 * a) keyed by name,
	 * b) each with a value of a hashtable, keyed by the value returned by 
	 *	  the use attribute,
	 * c) each with a value that is a nodelist.
	 * Thus, for a given key or keyref, look up hashtable by name, 
	 * look up the nodelist by the given reference.
	 */
	void
	processKeyElement(
			ElemTemplateElement*			nsContext,
			const AttributeList&			atts,
			StylesheetConstructionContext&	constructionContext);
  
	/**
	 * Locate a template via the "name" attribute.
	 * 
	 * @param name				 name of template
	 * @param executionContext	 current execution context
	 * @return pointer to template found or 0 if none found
	 */
	const ElemTemplate*
	findNamedTemplate(
			const XalanDOMString&			name,
			StylesheetExecutionContext& 	executionContext) const;
	/**
	 * Locate a template via the "name" attribute.
	 * 
	 * @param name				 qualified name of template
	 * @param executionContext	 current execution context
	 * @return pointer to template found or 0 if none found
	 */
	const ElemTemplate*
	findNamedTemplate(
			const QName&					qname,
			StylesheetExecutionContext& 	executionContext) const;

	/**
	 * Given a target element, find the template that best matches in the given
	 * XSL document, according to the rules specified in the xsl draft. 
	 *
	 * @param executionContext current execution context
	 * @param sourceTree	   where the targetElem is to be found
	 * @param targetNode	   element that needs a rule
	 * @return				   pointer to rule that best matches targetNode
	 */
	const ElemTemplate*
	findTemplate(
			StylesheetExecutionContext& 	executionContext,
			XalanNode*						sourceTree,
			XalanNode*						targetNode) const;

	/**
	 * Given a target element, find the template that best matches in the given
	 * XSL document, according to the rules specified in the xsl draft. 
	 *
	 * @param executionContext current execution context
	 * @param sourceTree        where the targetElem is to be found
	 * @param targetElem        element that needs a rule
	 * @param mode              string indicating the display mode
	 * @param useImports        means that this is an xsl:apply-imports commend
	 * @return pointer to rule that best matches targetElem
	 */
	const ElemTemplate*
	findTemplate(
			StylesheetExecutionContext& 	executionContext,
			XalanNode*						sourceTree, 
			XalanNode*						targetNode, 
			const QName&					mode,
			bool							useImports,
			const Stylesheet*&				foundStylesheet) const;

	/**
	 * A class to contain a match pattern and it's corresponding template.
	 * This class also defines a node in a match pattern linked list.
	 */
	class MatchPattern2
	{
	public:

		/**
		 * Construct a match pattern from a pattern and template.
		 *
		 * @param pat string that contains element pattern
		 * @param exp XPath expression for pattern
		 * @param theTemplate node that contains the template for this pattern
		 * @param posInStylesheet position in stylesheet
		 * @param targetString target string
		 * @param stylesheet stylesheet for pattern
		 */
		MatchPattern2(
				const XalanDOMString&	pat,
				const XPath*			exp,
				const ElemTemplate*		theTemplate,
				int 					posInStylesheet, 
				const XalanDOMString&	targetString,
				const Stylesheet* 		stylesheet); 

		~MatchPattern2();

		/**
		 * Retrieve stylesheet associated with pattern.
		 * 
		 * @return stylesheet for pattern
		 */
		const Stylesheet*
		getStylesheet() const
		{
			return m_stylesheet;
		}
		
		/**
		 * Retrieve string for target.
		 * 
		 * @return target string
		 */
		const XalanDOMString&
		getTargetString() const
		{ 
			return m_targetString;
		}
		
		/**
		 * Retrieve expression associated with pattern.
		 * 
		 * @return XPath expression for pattern
		 */
		const XPath*
		getExpression() const
		{
			return m_expression;
		}
		
		/**
		 * Retrieve position of pattern in stylesheet.
		 * 
		 * @return position in stylesheet
		 */
		int
		getPositionInStylesheet() const
		{
			return m_posInStylesheet;
		}
		
		/**
		 * Retrieve pattern string.
		 * 
		 * @return string that contains element pattern
		 */
		const XalanDOMString&
		getPattern() const
		{
			return m_pattern;
		}
		
		/**
		 * Retrieve node that contains the template for this pattern.
		 * 
		 * @return template node
		 */
		const ElemTemplate*
		getTemplate() const
		{
			return m_template;
		}
		
	private:

		const Stylesheet* const		m_stylesheet;
		const XalanDOMString		m_targetString;
		const XPath* const			m_expression;
		const int					m_posInStylesheet;
		const XalanDOMString		m_pattern;
		const ElemTemplate*	const	m_template; // ref to the corresponding template

		// Not implemented...
		MatchPattern2();
		MatchPattern2& operator=(const MatchPattern2&);
	};

#if defined(XALAN_NO_NAMESPACES)
	typedef list<MatchPattern2*>				PatternTableListType;

	typedef vector<const MatchPattern2*>		PatternTableVectorType;

	typedef map<XalanDOMString,
			    PatternTableListType,
				less<XalanDOMString> >			PatternTableMapType;
#else
	typedef std::list<MatchPattern2*>			PatternTableListType;

	typedef std::vector<const MatchPattern2*>	PatternTableVectorType;

	typedef std::map<XalanDOMString,
					 PatternTableListType>		PatternTableMapType;
#endif

	/**
	 * Add object to list of match patterns if not already there.
	 *
	 * @param thePattern pattern to add
	 * @param theVector  vector of patterns to add to
	 */
	static void
	addObjectIfNotFound(
			const MatchPattern2*		thePattern,
			PatternTableVectorType& 	theVector);

	/**
	 * Given a source node, locate the start of a list of possible template
	 * matches, according to its type.
	 *
	 * @param sourceNode source node for search of match patterns
	 */
	const PatternTableListType*
	locateMatchPatternList2(XalanNode*	sourceNode) const;

	/**
	 * Given an element type, locate the start of a list of 
	 * possible template matches, possibly trying wild card matches.
	 *
	 * @param sourceElementType type of element to search
	 * @param tryWildCard		if true, use wild card matching
	 */
	const PatternTableListType*
	locateMatchPatternList2(
			const XalanDOMString&	sourceElementType,
			bool					tryWildCard = false) const;

	/**
	 * Add an extension namespace handler. This provides methods for calling
	 * an element extension as well as for function calls (which is passed
	 * on to XPath).
	 *
	 * @param uri the URI of the extension namespace
	 * @param nsh handler
	 */
	void
	addExtensionNamespace(
			const XalanDOMString&	uri,
			ExtensionNSHandler* 	nsh);

	/**
	 * Return the handler for a given extension namespace.
	 *
	 * @param uri the URI of the extension namespace.
	 * @return pointer to extension handler
	 */
	ExtensionNSHandler*
	lookupExtensionNSHandler(const XalanDOMString&	uri) const
	{
		const ExtensionNamespacesMapType::const_iterator	it = 
		  m_extensionNamespaces.find(uri);

		return it == m_extensionNamespaces.end() ? 0 : (*it).second;
	}

	/**
	 * Set a top level variable, to be serialized with the rest of the
	 * stylesheet.
	 *
	 * @param var top-level variable declared with "xsl:variable" or
	 *			  xsl:param-variable.
	 */
	void
	setTopLevelVariable(ElemVariable*	var)
	{
		m_topLevelVariables.push_back(var);
	}

	/**
	 * Set a list of top level variables in the specified execution context
	 * stylesheet.
	 *
	 * @param executionContext current execution context
	 * @param topLevelParams   list of top level parameters
	 */
	void
	pushTopLevelVariables(
			StylesheetExecutionContext& 	executionContext,
			const ParamVectorType&			topLevelParams) const;

	const XPathVectorType&
	getWhitespacePreservingElements() const
	{
		return m_whitespacePreservingElements;
	}

	void
	pushWhitespacePreservingElement(const XPath*	theXPath)
	{
		m_whitespacePreservingElements.push_back(theXPath);
	}

	const XPathVectorType&
	getWhitespaceStrippingElements() const
	{
		return m_whitespaceStrippingElements;
	}

	void
	pushWhitespaceStrippingElement(const XPath* theXPath)
	{
		m_whitespaceStrippingElements.push_back(theXPath);
	}

	// These interfaces are inherited from XalanDocument...

	virtual const XalanDOMString&
	getNodeName() const;

	virtual const XalanDOMString&
	getNodeValue() const;

	virtual NodeType
	getNodeType() const;

	virtual XalanNode*
	getParentNode() const;

	virtual const XalanNodeList*
	getChildNodes() const;

	virtual XalanNode*
	getFirstChild() const;

	virtual XalanNode*
	getLastChild() const;

	virtual XalanNode*
	getPreviousSibling() const;

	virtual XalanNode*
	getNextSibling() const;

	virtual const XalanNamedNodeMap*
	getAttributes() const;

	virtual XalanDocument*
	getOwnerDocument() const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual XalanNode*
#else
	virtual Stylehseet*
#endif
	cloneNode(bool deep) const;

	virtual XalanNode*
	insertBefore(
			XalanNode*	newChild,
			XalanNode*	refChild);

	virtual XalanNode*
	replaceChild(
			XalanNode*	newChild,
			XalanNode*	oldChild);

	virtual XalanNode*
	removeChild(XalanNode*	oldChild);

	virtual XalanNode*
	appendChild(XalanNode*	newChild);

	virtual bool
	hasChildNodes() const;

	virtual void
	setNodeValue(const XalanDOMString&		nodeValue);

	virtual void
	normalize();

	virtual bool
	supports(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const;

	virtual const XalanDOMString&
	getNamespaceURI() const;

	virtual const XalanDOMString&
	getPrefix() const;

	virtual const XalanDOMString&
	getLocalName() const;

	virtual void
	setPrefix(const XalanDOMString& prefix);

	virtual unsigned long
	getIndex() const;

	virtual XalanElement*
	createElement(const XalanDOMString& tagName);

	virtual XalanDocumentFragment*
	createDocumentFragment();

	virtual XalanText*
	createTextNode(const XalanDOMString&	data);

	virtual XalanComment*
	createComment(const XalanDOMString& data);

	virtual XalanCDATASection*
	createCDATASection(const XalanDOMString&	data);

	virtual XalanProcessingInstruction*
	createProcessingInstruction(
			const XalanDOMString&	target,
			const XalanDOMString&	data);

	virtual XalanAttr*
	createAttribute(const XalanDOMString&	name);

	virtual XalanEntityReference*
	createEntityReference(const XalanDOMString &name);

	virtual XalanDocumentType*
	getDoctype() const;

	virtual XalanDOMImplementation*
	getImplementation() const;

	virtual XalanElement*
	getDocumentElement() const;

	virtual XalanNodeList*
	getElementsByTagName(const XalanDOMString&		tagname) const;

	virtual XalanNode*
	importNode(
			XalanNode*	importedNode,
			bool		deep);

	virtual XalanElement*
	createElementNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	qualifiedName);

	virtual XalanAttr*
	createAttributeNS(
			const XalanDOMString& namespaceURI,
			const XalanDOMString& qualifiedName);

	virtual XalanNodeList*
	getElementsByTagNameNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const;

	virtual XalanElement*
	getElementById(const XalanDOMString&	elementId) const;

	virtual bool
	isIndexed() const;

	// These interfaces are inherited from PrefixResolver...

	virtual const XalanDOMString&
	getNamespaceForPrefix(const XalanDOMString& 	prefix) const;

	virtual const XalanDOMString&
	getURI() const;

	const XalanDOMString&
	getXSLTNamespaceURI() const
	{
		return m_XSLTNamespaceURI;
	}

	void
	setXSLTNamespaceURI(const XalanDOMString&	theURI)
	{
		m_XSLTNamespaceURI = theURI;
	}

protected:

	/**
	 * The root of the stylesheet tree.
	 */
	StylesheetRoot& 					m_stylesheetRoot;

	/**
	 * This is set to true if an xsl:key directive is found.
	 */
	bool								m_needToBuildKeysTable;

	/**
	 * The base URL of the XSL document.
	 */
	XalanDOMString						m_baseIdent;

	/**
	 * Table of KeyDeclaration objects, which are set by the 
	 * xsl:key element.
	 */
	KeyDeclarationVectorType			m_keyDeclarations;

private:	

	// Not defined...
	Stylesheet(const Stylesheet&);

	Stylesheet&
	operator=(const Stylesheet&);

	bool
	operator==(const Stylesheet&) const;

	/**
	 * The full XSLT Namespace URI.  To be replaced by the one actually
	 * found.
	 */
	XalanDOMString							m_XSLTNamespaceURI;

	/**
	 * A lookup table of all space preserving elements.
	 */
	XPathVectorType 						m_whitespacePreservingElements;
  
	/**
	 * A lookup table of all space stripping elements.
	 */
	XPathVectorType 						m_whitespaceStrippingElements;

	/**
	 * A vector of the -imported- XSL Stylesheets.
	 */
	StylesheetVectorType					m_imports;

	/**
	 * A stack to keep track of the result tree namespaces.
	 */
	NamespacesStackType 					m_namespaces;

	/** 
	 * A list of namespace declarations,
	 * for mapping from prefix to namespace URI.
	 */
	NamespaceVectorType 					m_namespaceDecls;

	/**
	 * This is pushed on the m_resultNameSpaces stack 'till a xmlns attribute is
	 * found.
	 */
	static const NamespaceVectorType		s_emptyNamespace;

	/**
	 * Tells if the stylesheet tables need to be rebuilt.
	 */
	bool									m_tablesAreInvalid;

	/**
	 * Tells if the stylesheet is without an xsl:stylesheet and xsl:template
	 * wrapper.
	 */
	bool									m_isWrapperless;

	/**
	 * The manufactured template if there is no wrapper.
	 */
	ElemTemplate*							m_wrapperlessTemplate;
  
	/**
	 * The table of extension namespaces.
	 */
	ExtensionNamespacesMapType				m_extensionNamespaces;

  
	/**
	 * The first template of the template children.
	 */
	ElemTemplateElement*					m_firstTemplate;
  
	/**
	 * A stack of who's including who is needed in order to support "It is an
	 * error if a stylesheet directly or indirectly includes itself."
	 */
	URLStackType							m_includeStack;

	/** 
	 * Tell if this stylesheet has the default space handling
	 * turned off or on according to the xml:space attribute.
	 * @serial
	 */
	bool									m_defaultSpace;
  
	/**
	 * Keyed on string macro names, and holding values that are macro elements
	 * in the XSL DOM tree. Initialized in initMacroLookupTable, and used in
	 * findNamedTemplate.
	 */
	ElemTemplateMapType						m_namedTemplates;
  
	/**
	 * Table for defined constants, keyed on the names.
	 */
	ElemVariableVectorType					m_topLevelVariables;


	/**
	 * The version of XSL that was declared.
	 */
	double									m_XSLTVerDeclared;

	const bool								m_isRoot;

	/**
	 * This table is keyed on the target elements of patterns, and contains linked
	 * lists of the actual patterns that match the target element to some degree
	 * of specifity.
	 */
	PatternTableMapType 					m_patternTable;

	/**
	 * Table of attribute sets, keyed by set name.
	 */
	AttributeSetMapType 					m_attributeSets;

	XalanNodeListSurrogate					m_surrogateChildren;

	XalanEmptyNamedNodeMap					m_fakeAttributes;

	ElemDecimalFormatVectorType				m_elemDecimalFormats;

	StringToStringMapType					m_prefixAliases;

	NamespacesHandler						m_namespacesHandler;

	static const XalanDOMString				s_emptyString;
};



#endif	// XALAN_STYLESHEET_HEADER_GUARD
