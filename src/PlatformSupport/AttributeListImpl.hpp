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
#if !defined(ATTRIBUTELISTIMPL_HEADER_GUARD_1357924680)
#define ATTRIBUTELISTIMPL_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



#include <map>
#include <vector>



#include <sax/AttributeList.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/STLHelper.hpp>



class XALAN_PLATFORMSUPPORT_EXPORT AttributeListImpl : public AttributeList
{
public:

	explicit
	AttributeListImpl();

	virtual
	~AttributeListImpl();

    AttributeListImpl(const AttributeListImpl&	theSource);

    AttributeListImpl(const AttributeList&	theSource);

    AttributeListImpl&
	operator=(const AttributeListImpl&	theRHS);

	AttributeListImpl&
	operator=(const AttributeList&	theRHS);

	// These are inherited from AttributeList
    virtual unsigned int
	getLength() const;

    virtual const XMLCh*
	getName(const unsigned int index) const;

    virtual const XMLCh*
	getType(const unsigned int index) const;

    virtual const XMLCh*
	getValue(const unsigned int index) const;

    virtual const XMLCh*
	getType(const XMLCh* const name) const;

    virtual const XMLCh*
	getValue(const XMLCh* const name) const;

    virtual const XMLCh*
	getValue(const char* const name) const;

	// The mutators are new to this class.
	virtual void
	clear();

	virtual bool
	addAttribute(const XMLCh* const name,
				 const XMLCh* const type,
				 const XMLCh* const value);

	virtual bool
	removeAttribute(const XMLCh* const name);

protected:

	// This is not implemented.
    bool
	operator==(const AttributeListImpl&) const;


	// A struct to hold information about each attribute.
	struct AttributeVectorEntry
	{
		AttributeVectorEntry(const XMLCharVectorType&	theName = XMLCharVectorType(),
							 const XMLCharVectorType&	theValue = XMLCharVectorType(),
							 const XMLCharVectorType&	theType = XMLCharVectorType()) :
			m_Name(theName),
			m_Value(theValue),
			m_Type(theType)
		{
		}

		const XMLCharVectorType		m_Name;
		const XMLCharVectorType		m_Value;
		const XMLCharVectorType		m_Type;
	};

#if defined(XALAN_NO_NAMESPACES)
	// This vector will hold the entries.
	typedef vector<const AttributeVectorEntry*>				AttributeVectorType;

	// This map will associate a name with a pointer to the entry that corresponds
	// to that name.
	typedef map<const XMLCh*,
					 const AttributeVectorEntry*,
					 less_null_terminated_arrays<XMLCh> >	AttributeKeyMapType;
#else
	// This vector will hold the entries.
	typedef std::vector<const AttributeVectorEntry*>		AttributeVectorType;

	// This map will associate a name with a pointer to the entry that corresponds
	// to that name.
	typedef std::map<const XMLCh*,
					 const AttributeVectorEntry*,
					 less_null_terminated_arrays<XMLCh> >	AttributeKeyMapType;
#endif

	AttributeKeyMapType		m_AttributeKeyMap;
	AttributeVectorType		m_AttributeVector;
};



#endif	// ATTRIBUTELISTIMPL_HEADER_GUARD_1357924680
