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
#include <XPath/FunctionSubstringBefore.hpp>



FunctionSubstringBefore::FunctionSubstringBefore()
{
}



FunctionSubstringBefore::~FunctionSubstringBefore()
{
}



XObject*
FunctionSubstringBefore::execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						/* context */,			
		const XObject*					arg1,
		const XObject*					arg2)
{
	assert(arg1 != 0 || arg2 != 0);	

	const XalanDOMString&	theFirstString = arg1->str();
	const XalanDOMString&	theSecondString = arg2->str();

	const unsigned int		theIndex = indexOf(theFirstString,
											   theSecondString);

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<XalanDOMChar>		VectorType;
#else
	typedef std::vector<XalanDOMChar>	VectorType;
#endif

	// This buffer will hold the output characters.
	VectorType	theBuffer;

	if (theIndex < length(theFirstString))
	{
		// The result string can only be as large as the source string, so
		// just reserve the space now.  Also reserve a space for the
		// terminating 0.
		theBuffer.reserve(theIndex + 2);

		// Stop with the last character before the index.
		for (unsigned int i = 0; i < theIndex; i++)
		{
			theBuffer.push_back(charAt(theFirstString, i));
		}
	}

	const VectorType::size_type		theSize = theBuffer.size();

	if (theSize == 0)
	{
		return executionContext.getXObjectFactory().createString(XalanDOMString());
	}
	else
	{
		return executionContext.getXObjectFactory().createString(XalanDOMString(
			theBuffer.begin(), theSize));
	}
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionSubstringBefore*
#endif
FunctionSubstringBefore::clone() const
{
	return new FunctionSubstringBefore(*this);
}



const XalanDOMString
FunctionSubstringBefore::getError() const
{
	return XALAN_STATIC_UCODE_STRING(
		"The substring-before() function takes two arguments!");
}

