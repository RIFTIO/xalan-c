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
#if !defined(ICUXALANNUMBERFORMATPROXY_HEADER_GUARD_1357924680)
#define ICUXALANNUMBERFORMATPROXY_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <ICUBridge/ICUBridgeDefinitions.hpp>



#include <PlatformSupport/XalanNumberFormat.hpp>



// We're stuck here.  We don't want to include the ICU header files, since we're trying
// to keep them hidden, but we need their namespace.  So we're just duplicating here.  When
// it changes, we'll have to track that change.  This is more desirable than forcing users
// to have the ICU sources.
//
// We could fix this by using factories, rather than allowing user access to these
// implementation classes.  It's certainly worth pursuing...
#if defined(XALAN_NO_NAMESPACES)
class DecimalFormat;
#else
namespace icu_2_0
{
	class DecimalFormat;
};
#endif



// Class that implements the XSLT function format-number using the ICU.
//
class XALAN_ICUBRIDGE_EXPORT ICUXalanNumberFormatProxy : public XalanNumberFormat
{
public:

	explicit
	ICUXalanNumberFormatProxy();

	virtual
	~ICUXalanNumberFormatProxy();

	virtual XalanDOMString
	format(double	theValue);

	virtual void
	format(
			double				theValue,
			XalanDOMString&		theResult);

	virtual XalanDOMString
	format(int	theValue);

	virtual void
	format(
			int					theValue,
			XalanDOMString&		theResult);

	virtual XalanDOMString
	format(unsigned int		theValue);

	virtual void
	format(
			unsigned int		theValue,
			XalanDOMString&		theResult);

	virtual XalanDOMString
	format(long		theValue);

	virtual void
	format(
			long				theValue,
			XalanDOMString&		theResult);

	virtual XalanDOMString
	format(unsigned long	theValue);

	virtual void
	format(
			unsigned long		theValue,
			XalanDOMString&		theResult);

	virtual bool
	isGroupingUsed() const;

	virtual void
	setGroupingUsed(bool bUsed);

	virtual void
	setGroupingSize(unsigned long	size);

	virtual void
	setGroupingSeparator(const XalanDOMString&	s);

private:

#if defined(XALAN_NO_NAMESPACES)
	DecimalFormat*				m_decimalFormat;
#else
	icu_2_0::DecimalFormat*		m_decimalFormat;
#endif
};



#endif	// ICUXALANNUMBERFORMATPROXY_HEADER_GUARD_1357924680
