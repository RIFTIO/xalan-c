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
#if !defined(XALAN_GenerateEvent_HEADER_GUARD)
#define XALAN_GenerateEvent_HEADER_GUARD 



// Base include file.  Must be first.
#include <XSLT/XSLTDefinitions.hpp>



#include <XalanDOM/XalanDOMString.hpp>



class XSLTEngineImpl;
class AttributeList;



/** 
 * This is the class for events generated by the XSL processor
 * after it generates a new node in the result tree.  It responds
 * to, and so is modeled from, the SAX events that are sent to the
 * FormatterListener classes.
 * 
 * @see org.apache.xml.xpath4j.xml.FormatterListener
 */
class XALAN_XSLT_EXPORT GenerateEvent
{
public:

	enum EventType
	{
		/** 
		 * Event type generated when a document begins.
		 * 
		 */
		EVENTTYPE_STARTDOCUMENT = 1,
			
		/** 
		 * Event type generated when a document ends.
		 */
		EVENTTYPE_ENDDOCUMENT = 2,
		
		/** 
		 * Event type generated when an element begins (after the attributes have been processed but before the children have been added).
		 */
		EVENTTYPE_STARTELEMENT = 3,
		
		/** 
		 * Event type generated when an element ends, after it's children have been added.
		 */
		EVENTTYPE_ENDELEMENT = 4,
		
		/** 
		 * Event type generated for character data (CDATA and Ignorable Whitespace have their own events).
		 */
		EVENTTYPE_CHARACTERS = 5,
		
		/** 
		 * Event type generated for ignorable whitespace (I'm not sure how much this is actually called.
		 */
		EVENTTYPE_IGNORABLEWHITESPACE = 6,

		/** 
		 * Event type generated for processing instructions.
		 */
		EVENTTYPE_PI = 7,

		/** 
		 * Event type generated after a comment has been added.
		 */
		EVENTTYPE_COMMENT = 8,

		/** 
		 * Event type generate after an entity ref is created.
		 */
		EVENTTYPE_ENTITYREF = 9,

		/** 
		 * Event type generated after CDATA is generated.
		 */
		EVENTTYPE_CDATA = 10
	};


	/** 
	 * Constructor for startDocument, endDocument events.
	 * 
	 * @param processor XSLT Processor instance
	 * @param eventType one of the EVENTTYPE_XXX constants
	 */
	GenerateEvent(
			XSLTEngineImpl*		processor,
			EventType			eventType);
	
	/** 
	* Constructor for startElement, endElement events.
	* 
	* @param processor XSLT Processor Instance
	* @param eventType one of the EVENTTYPE_XXX constants
	* @param name      name of the element
	* @param atts      SAX attribute list
	*/
	GenerateEvent(
			XSLTEngineImpl*			processor,
			EventType				eventType,
			const XalanDOMString&	name,
			AttributeList*			atts);
	
	/** 
	* Constructor for characters, cdate events.
	* 
	* @param processor XSLT Processor instance
	* @param eventType one of the EVENTTYPE_XXX constants
	* @param ch        char array from the SAX event
	* @param start     start offset to be used in the char array
	* @param length    end offset to be used in the chara array
	*/
	GenerateEvent(
			XSLTEngineImpl*			processor,
			EventType				eventType,
			const XalanDOMString&	ch,
			unsigned int			start,
			unsigned int			length);
	
	/** 
	* Constructor for processingInstruction events.
	* 
	* @param processor instance of the XSLT processor
	* @param eventType one of the EVENTTYPE_XXX constants
	* @param name      name of the processing instruction
	* @param data      processing instruction data
	*/
	GenerateEvent(
			XSLTEngineImpl*			processor,
			EventType				eventType,
			const XalanDOMString&	name,
			const XalanDOMString&	data);
	
	/** 
	* Constructor for comment and entity ref events.
	* 
	* @param processor XSLT processor instance
	* @param eventType one of the EVENTTYPE_XXX constants
	* @param data      comment or entity ref data
	*/
	GenerateEvent(
			XSLTEngineImpl*			processor,
			EventType				eventType,
			const XalanDOMString&	data);

	/** 
	 * The type of SAX event that was generated, as enumerated in the
	 * EVENTTYPE_XXX constants above.
	 */
	EventType			m_eventType;


	/** 
	 * Character data from a character or cdata event.
	 */
	XalanDOMString		m_characters;

	/** 
	 * The start position of the current data in m_characters.
	 */
	unsigned int		m_start;

	/** 
	 * The length of the current data in m_characters.
	 */
	unsigned int		m_length;

	/** 
	 * The name of the element or PI.
	 */
	XalanDOMString		m_name;

	/** 
	 * The string data in the element (comments and PIs).
	 */
	XalanDOMString		m_data;

	/** 
	 * The current attribute list.
	 */
	AttributeList*		m_pAtts;
	
private:

	XSLTEngineImpl*		m_pProcessor;
};



#endif	//XALAN_GenerateEvent_HEADER_GUARD
