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
#include "URISupport.hpp"



#include <algorithm>
#include <vector>



#include <xercesc/util/PlatformUtils.hpp>



#include <Include/STLHelper.hpp>



#include "XalanUnicode.hpp"



const XalanDOMChar	URISupport::s_fileProtocolString1[] =
{
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	XalanUnicode::charColon,
	XalanUnicode::charSolidus,
	XalanUnicode::charSolidus,
	0
};



const XalanDOMChar	URISupport::s_fileProtocolString2[] =
{
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	XalanUnicode::charColon,
	XalanUnicode::charSolidus,
	XalanUnicode::charSolidus,
	XalanUnicode::charSolidus,
	0
};



URISupport::URLAutoPtrType
URISupport::getURLFromString(const XalanDOMString&	urlString)
{
	return getURLFromString(c_wstr(urlString));
}



URISupport::URLAutoPtrType
URISupport::getURLFromString(const XalanDOMChar*	urlString)
{
	URLAutoPtrType	url(new XMLURL);

	url->setURL(c_wstr(getURLStringFromString(urlString)));

	return url;
}



XalanDOMString
URISupport::getURLStringFromString(const XalanDOMString&	urlString)
{
	return getURLStringFromString(c_wstr(urlString));
}



XalanDOMString
URISupport::getURLStringFromString(const XalanDOMChar*	urlString)
{
	assert(urlString != 0);

	XalanDOMString	theNormalizedURI(urlString);

	// Let's see what sort of URI we have...
	const XalanDOMString::size_type		len = length(theNormalizedURI);

	if (len != 0)
	{
		const XalanDOMString::size_type		index = indexOf(theNormalizedURI, XalanUnicode::charColon);

		bool	protocolPresent = false;

		if (index != len)
		{
			// $$$ ToDo: XMLURL::lookupByName() is supposed to be static, but is not.
			const XMLURL::Protocols		theProtocol =
				XMLURL().lookupByName(c_wstr(substring(theNormalizedURI, 0 , index)));

			if (theProtocol != XMLURL::Unknown)
			{
				protocolPresent = true;
			}
		}

		if (protocolPresent == true)
		{
			NormalizeURIText(theNormalizedURI);
		}
		else
		{
			// Assume it's a file specification...
			const XalanArrayAutoPtr<XalanDOMChar>	theFullPath(XMLPlatformUtils::getFullPath(c_wstr(urlString)));
			assert(theFullPath.get() != 0);

			theNormalizedURI = theFullPath.get();
			assert(length(theNormalizedURI) > 0);

			NormalizeURIText(theNormalizedURI);

			if (indexOf(theNormalizedURI, XalanUnicode::charSolidus) == 0)
			{
				insert(theNormalizedURI, 0, &s_fileProtocolString1[0]);
			}
			else
			{
				insert(theNormalizedURI, 0, &s_fileProtocolString2[0]);
			}
		}
	}

	return theNormalizedURI;
}



URISupport::URLAutoPtrType
URISupport::getURLFromString(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base)
{	
	return getURLFromString(getURLStringFromString(urlString, base));
}



XalanDOMString
URISupport::getURLStringFromString(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base)
{
	return getURLStringFromString(c_wstr(urlString), c_wstr(base));
}



XalanDOMString
URISupport::getURLStringFromString(
			const XalanDOMChar*		urlString,
			const XalanDOMChar*		base)
{
	XalanDOMString	context(base);

	NormalizeURIText(context);

	const XalanDOMString::size_type		theContextLength = length(context);

	const XalanDOMString::size_type		indexOfSlash = theContextLength == 0 ?
							0 :
							lastIndexOf(context, XalanUnicode::charSolidus);

	bool				hasPath = true;

	if (indexOfSlash < theContextLength)
	{
		context = substring(context, 0, indexOfSlash + 1);
	}
	else
	{
		hasPath = false;
	}

	// OK, now let's look at the urlString...

	// Is there a colon, indicating some sort of drive spec, or protocol?
	const XalanDOMString::size_type		theURLStringLength = length(urlString);
	const XalanDOMString::size_type		theColonIndex = indexOf(urlString, XalanUnicode::charColon);

	if (theColonIndex == theURLStringLength)
	{
		// No colon, so just use the urlString as is...

		// Strip off file name from context...
		if (indexOfSlash < theContextLength)
		{
			context = substring(context, 0, indexOfSlash + 1);
		}

		if (hasPath == true)
		{
			context += urlString;
		}
		else
		{
			context = urlString;
		}
	}
	else
	{
		const XalanDOMString	theProtocolString(substring(urlString, 0, theColonIndex));

		// $$$ ToDo: XMLURL::lookupByName() is supposed to be static, but is not.
		const XMLURL::Protocols		theProtocol =
			XMLURL().lookupByName(c_wstr(theProtocolString));

		if (theColonIndex == 1 &&
			theProtocol == XMLURL::Unknown)
		{
			// Ahh, it's a drive letter, so ignore the context...
			context = urlString;
		}
		else
		{
			// It's a protocol...
			if (startsWith(context, theProtocolString) == false)
			{
				// OK, not the same protocol, so what can we do???
				context = urlString;
			}
			else
			{
				// They share the same protocol...

				// Check if this is an absolute URI (starts with a leading '//')
				const XalanDOMString::size_type		protoLength = length(theProtocolString);

				if (protoLength + 3 <= theURLStringLength &&
					urlString[protoLength + 1] == XalanUnicode::charSolidus &&
					urlString[protoLength + 2] == XalanUnicode::charSolidus)
				{
					// It's an absolute URI -- use it in full...
					context = urlString;
				}
				else
				{
					// Strip off file name from context...
					if (indexOfSlash < theContextLength)
					{
						context = substring(context, 0, indexOfSlash + 1);
					}

					// OK, everything looks good, so strip off the protocol 
					// and colon...
					context += substring(urlString, theColonIndex + 1, theURLStringLength);
				}
			}
		}
	}

	return getURLStringFromString(context);
}



XalanDOMString&
URISupport::NormalizeURIText(XalanDOMString&	uriString)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::replace;
#endif

	// OK, look for a quick, cheap exit...
	const XalanDOMString::size_type		len = length(uriString);
	const XalanDOMString::size_type		index = indexOf(uriString, XalanUnicode::charReverseSolidus);

	if (index != len)
	{
		XalanDOMCharVectorType	theVector =
			MakeXalanDOMCharVector(uriString);

		// Start replacing at the index point, since that's the
		// first one...
		replace(
				theVector.begin(),
				theVector.end(),
				XalanDOMCharVectorType::value_type(XalanUnicode::charReverseSolidus),
				XalanDOMCharVectorType::value_type(XalanUnicode::charSolidus));

		uriString = XalanDOMString(&theVector[0]);
	}

	return uriString;
}



URISupport::InvalidURIException::InvalidURIException(const XalanDOMString&	theMessage) :
	XSLException(theMessage,
				 TranscodeFromLocalCodePage("InvalidURIException"))
{
}



URISupport::InvalidURIException::~InvalidURIException()
{
}