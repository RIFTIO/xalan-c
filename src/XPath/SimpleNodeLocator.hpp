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
#if !defined(SIMPLENODELOCATOR_HEADER_GUARD_1357924680)
#define SIMPLENODELOCATOR_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <XalanDOM/XalanDOMString.hpp>
#include <XalanDOM/XalanNode.hpp>



#include <XPath/MutableNodeRefList.hpp>
#include <XPath/XObject.hpp>
#include <XPath/XPath.hpp>



class XPathExpression;



/**
 * SimpleNodeLocator implements a search of one or more DOM trees.  It
 * has no constructors, since it is never instantiated.
 */
class XALAN_XPATH_EXPORT SimpleNodeLocator
{
public:

	static const XObjectPtr
	locationPath(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode& 				context, 
			int 					opPos);

	static XPath::eMatchScore
	locationPathPattern(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode& 				context, 
			int 					opPos);

	class NodeTester
	{
	public:

		NodeTester(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			int 					opPos,
			int 					argLen,
			int 					stepType);

		XPath::eMatchScore
		operator()(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const
		{
			assert(context.getNodeType() == nodeType);

			return (this->*m_testFunction)(context, nodeType);
		}

	private:

		typedef XPath::eMatchScore (NodeTester::*MemberFunctionPtr)(const XalanNode&, XalanNode::NodeType) const;


		XPath::eMatchScore
		testComment(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		XPath::eMatchScore
		testText(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		XPath::eMatchScore
		testPI(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		XPath::eMatchScore
		testPIName(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		XPath::eMatchScore
		testNode(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		XPath::eMatchScore
		testRoot(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		XPath::eMatchScore
		testAttributeNCName(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		XPath::eMatchScore
		testAttributeQName(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		XPath::eMatchScore
		testAttributeNamespaceOnly(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		XPath::eMatchScore
		testAttributeTotallyWild(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		XPath::eMatchScore
		testElementNCName(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		XPath::eMatchScore
		testElementQName(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		XPath::eMatchScore
		testElementNamespaceOnly(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		XPath::eMatchScore
		testElementTotallyWild(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		XPath::eMatchScore
		testNamespaceNCName(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		XPath::eMatchScore
		testNamespaceTotallyWild(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		XPath::eMatchScore
		testDefault(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		bool
		matchLocalName(const XalanNode&		context) const;

		bool
		matchNamespaceURI(const XalanNode&	context) const;

		bool
		matchLocalNameAndNamespaceURI(const XalanNode&	context) const;

		bool
		matchNamespace(const XalanNode&		context) const;

		bool
		shouldStripSourceNode(const XalanNode&	context) const;

		// Data members...
		XPathExecutionContext&	m_executionContext;

		const XalanDOMString*	m_targetNamespace;

		const XalanDOMString*	m_targetLocalName;

		MemberFunctionPtr		m_testFunction;
	};

protected:

	static void
	step(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			MutableNodeRefList&		queryResults);

	/**
	 * Execute a step in a location path.
	 *
	 * @param xpath The xpath that is executing
	 * @param context The current source tree context node
	 * @param opPos The current position in the xpath operation map array
	 * @param scoreHolder a reference to an XPath::eMatchScore to receive
	 * the result.
	 * @return the last matched context node
	 */
	static XalanNode*
	stepPattern(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			XPath::eMatchScore& 	scoreHolder);

	static int
	findNodeSet(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	static int
	findRoot(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	static int
	findParent(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	static int
	findSelf(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	static int
	findAncestors(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	static int
	findAncestorsOrSelf(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	static int
	findAttributes(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	static int
	findChildren(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	static int
	findDescendants(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	static int
	findFollowing(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	static int
	findFollowingSiblings(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	static int
	findPreceeding(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	static int
	findPreceedingSiblings(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	static int
	findNamespace(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	static int
	findNodesOnUnknownAxis(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	static XPath::eMatchScore
	nodeTest(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context,
			XalanNode::NodeType		nodeType,
			int 					opPos,
			int 					argLen,
			int 					stepType);

	static void
	predicates(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			MutableNodeRefList& 	subQueryResults,
			int&					endPredicatesPos);

	static XPath::eMatchScore
	handleFoundIndex(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				localContext,
			int 					startOpPos);

private:

	// Not defined...
	SimpleNodeLocator();

	~SimpleNodeLocator();


	static const XalanDOMString		s_emptyString;
};



#endif	// SIMPLENODELOCATOR_HEADER_GUARD_1357924680