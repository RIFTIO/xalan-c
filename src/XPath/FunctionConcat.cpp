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
#include "FunctionConcat.hpp"



#include "XObjectFactory.hpp"



FunctionConcat::FunctionConcat()
{
}



FunctionConcat::~FunctionConcat()
{
}



XObjectPtr
FunctionConcat::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				/* context */,			
			const XObjectPtr		arg1,
			const XObjectPtr		arg2,
			const Locator*			/* locator */) const
{
	assert(arg1.null() == false && arg2.null() == false);	

	XPathExecutionContext::GetAndReleaseCachedString	theResult(executionContext);

	XalanDOMString&			theString = theResult.get();

	const XalanDOMString&	theArg1 = arg1->str();
	const XalanDOMString&	theArg2 = arg2->str();

	reserve(theString, length(theArg1) + length(theArg2) + 1);

	append(theString, theArg1);
	append(theString, theArg2);

	return executionContext.getXObjectFactory().createString(theResult);
}



XObjectPtr
FunctionConcat::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				/* context */,			
			const XObjectPtr		arg1,
			const XObjectPtr		arg2,
			const XObjectPtr		arg3,
			const Locator*			/* locator */) const
{
	assert(arg1.null() == false && arg2.null() == false && arg3.null() == false);	

	XPathExecutionContext::GetAndReleaseCachedString	theResult(executionContext);

	XalanDOMString&			theString = theResult.get();

	const XalanDOMString&	theArg1 = arg1->str();
	const XalanDOMString&	theArg2 = arg2->str();
	const XalanDOMString&	theArg3 = arg3->str();

	reserve(theString, length(theArg1) + length(theArg2) + length(theArg3) + 1);

	append(theString, theArg1);
	append(theString, theArg2);
	append(theString, theArg3);

	return executionContext.getXObjectFactory().createString(theResult);
}



XObjectPtr
FunctionConcat::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						/* context */,
			const XObjectArgVectorType&		args,
			const Locator*					/* locator */) const
{
	unsigned int	theCombinedLength = 0;

	const XObjectArgVectorType::const_iterator	theEnd = args.end();

	{
		XObjectArgVectorType::const_iterator	i = args.begin();

		for(; i != theEnd; ++i)
		{
			assert((*i).null() == false);

			theCombinedLength += length((*i)->str());
		}
	}

	XPathExecutionContext::GetAndReleaseCachedString	theResult(executionContext);

	XalanDOMString&		theString = theResult.get();

	reserve(theString, theCombinedLength + 1);

	{
		XObjectArgVectorType::const_iterator	i = args.begin();

		for(; i != theEnd; ++i)
		{
			append(theString, (*i)->str());
		}
	}

	return executionContext.getXObjectFactory().createString(theResult);
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionConcat*
#endif
FunctionConcat::clone() const
{
	return new FunctionConcat(*this);
}



const XalanDOMString
FunctionConcat::getError() const
{
	return XALAN_STATIC_UCODE_STRING("The concat() function takes at least two arguments!");
}
