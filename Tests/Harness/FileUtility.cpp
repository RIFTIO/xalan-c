/*
*
* FileUtility.cpp
*
* Copyright 2000 Lotus Development Corporation. All rights reserved.
* This software is subject to the Lotus Software Agreement, Restricted
* Rights for U.S. government users and applicable export regulations.
*/

#include <stdlib.h>
#include <cstdio>
#include <ctime>
#include <vector>
#include <string.h>
#include <time.h>

// Added for directory creation 
#include <strstream>
#include <stdio.h>
#include <direct.h>


#if defined(XALAN_OLD_STREAM_HEADERS)
#include <iostream.h>
#else
#include <iostream>
#endif

#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif

// XERCES HEADERS...
//	Are included by HarnessInit.hpp
#include <sax/SAXException.hpp>

// XALAN HEADERS...
//	Are included by FileUtility.hpp

#include "FileUtility.hpp"
#include "XMLFileReporter.hpp"

char *xalanNodeTypes[]=
	{"UNKNOWN_NODE",
	"ELEMENT_NODE",
	"ATTRIBUTE_NODE",
	"TEXT_NODE",
	"CDATA_SECTION_NODE",
	"ENTITY_REFERENCE_NODE",
	"ENTITY_NODE",
	"PROCESSING_INSTRUCTION_NODE",
	"COMMENT_NODE",
	"DOCUMENT_NODE",
	"DOCUMENT_TYPE_NODE",
	"DOCUMENT_FRAGMENT_NODE",
	"NOTATION_NODE"};

/*	This routine retrieves test file names from specified directories.
//	Inputs: baseDir:	typically "conf" or "perf"
//			relDir:		sub-directory to search.
//
//	Notes:	It builds the searchSpecification by concatenating all the 
//			necessary components.
//																			*/	
FileNameVectorType FileUtility::getTestFileNames(XalanDOMString baseDir, XalanDOMString relDir, bool useDirPrefix)
{
	const XalanDOMString	searchSuffix(XALAN_STATIC_UCODE_STRING("*.xsl"));
	XalanDOMString	searchSpecification;

	// Allow directory search w/o mandating files start with directory name. Required for files
	// garnered from XSLTMARK performance directory exm.
	if (useDirPrefix)
	{
		assign(searchSpecification, baseDir + relDir + pathSep + relDir + searchSuffix);
	}
	else
	{
		assign(searchSpecification, baseDir + relDir + pathSep + searchSuffix); 
	}


	DirectoryEnumeratorFunctor<FileNameVectorType, XalanDOMString>	theEnumerator;
	FileNameVectorType	theFiles;
	theEnumerator(searchSpecification, theFiles);

	return theFiles;
}

/*	This routine retrieves all sub-directories from the specified directories.
//	Inputs: rootDirectory:	typically "conf" or "perf"
//
//	Notes:	The searchSpecification in this case is just "*". 
//																			*/	
FileNameVectorType FileUtility::getDirectoryNames(XalanDOMString rootDirectory)
{
	const XalanDOMString	dirSpec(XALAN_STATIC_UCODE_STRING("*"));

	DirectoryEnumeratorFunctor<FileNameVectorType, XalanDOMString, DirectoryFilterPredicate> theEnumerator;
	FileNameVectorType	theFiles;
	theEnumerator(XalanDOMString(rootDirectory), XalanDOMString(dirSpec), theFiles);

	return theFiles;
}

bool FileUtility::checkDir(XalanDOMString directory )
{
char buffer[_MAX_PATH];

	_getcwd( buffer, _MAX_PATH );

	if ( _chdir(c_str(TranscodeToLocalCodePage(directory))) )
	{
		_chdir(buffer);
		return false;
	}
	else
	{
		return true;
	}
}


void FileUtility::checkAndCreateDir(XalanDOMString directory )
{
char buffer[_MAX_PATH];

	_getcwd( buffer, _MAX_PATH );


	if ( (_chdir(c_str(TranscodeToLocalCodePage(directory)))) )
	{
		//cout << "Couldn't change to " << directory << ", will create it." << endl;
		if ( !(_mkdir(c_str(TranscodeToLocalCodePage(directory)))))
		{
			cout << directory << " created." << endl;
		}
	}

	_chdir(buffer);
}

/*	This routine generates file names based on the provide suffix
//	Inputs: theXMLFileName:	typically "conf" or "perf"
//			suffix:			typically "xsl" or "out".
//
//	Notes:	
*/	

XalanDOMString FileUtility::GenerateFileName(const XalanDOMString&  theXMLFileName, char* suffix)
{
	XalanDOMString	theResult;
	int				thePeriodIndex = -1;
	const int		theLength = length(theXMLFileName);

	for (int i = theLength - 1; i > 0; i--)
	{
		if (charAt(theXMLFileName, i) == XalanUnicode::charFullStop)
		{
			thePeriodIndex = i;        // charFullStop is the dot (x2E)
			break;
		}
	}

	if (thePeriodIndex != -1)
	{
		theResult = substring(theXMLFileName,
							  0,
							  thePeriodIndex + 1);


		theResult += XalanDOMString(suffix);
	}

	return theResult;
}


/*	This routine generates a Unique Runid. 
//	Inputs: None
//			
//	Notes: The format is mmddhhmm. For example
//		   03151046 is "Mar 15 10:46"	
*/

XalanDOMString FileUtility::GenerateUniqRunid()
{

		struct tm *newtime;
		time_t long_time;
		char tmpbuf[10];

		time( &long_time );                /* Get time as long integer. */
		newtime = localtime( &long_time ); /* Convert to local time. */

		strftime( tmpbuf, 10,"%m%d%H%M",newtime );

		return(XalanDOMString(tmpbuf));

}

//	This routine gets Xerces Version number. It's used to put the Xerces Version
//	into the output xml results file as an attribute of 'PerfData' element.
//	Inputs: None
//				

XalanDOMString FileUtility::getXercesVersion()
{

	return(XalanDOMString(gXercesFullVersionStr));

}

/*	This routine creates a FormatterToXML FormatterListener. This is used to format
//	the output DOM so a comparision can be done with the expected GOLD file. 
//	Inputs: None
//				
*/


FormatterListener* 
FileUtility::getXMLFormatter(bool		shouldWriteXMLHeader,
				bool					stripCData,
				bool					escapeCData,
				PrintWriter&			resultWriter,
				int						indentAmount,
				const XalanDOMString&	mimeEncoding,
				const StylesheetRoot*	stylesheet)
{
	FormatterListener*	formatter = 0;

		XalanDOMString	version;
		bool			outputIndent= 0;
		XalanDOMString	mediatype;
		XalanDOMString	doctypeSystem;
		XalanDOMString	doctypePublic;
		XalanDOMString	standalone;

		if (stylesheet != 0)
		{
			version = stylesheet->m_version;

			mediatype = stylesheet->m_mediatype;
			doctypeSystem = stylesheet->getOutputDoctypeSystem();
			doctypePublic = stylesheet->getOutputDoctypePublic();
			standalone = stylesheet->m_standalone;
			outputIndent = stylesheet->m_indentResult;
		}

		FormatterToXML* const	fToXML =
			new FormatterToXML(
					resultWriter,
					version,
					outputIndent,
					indentAmount,
					mimeEncoding,
					mediatype,
					doctypeSystem,
					doctypePublic,
					true,	// xmlDecl
					standalone);

		formatter = fToXML;
		return formatter;
}

/*	This routine is used to compares the results of a transform and report the results.
//  When a failure is detected the 'data' structure used to report detailed info about 
//  a failure is filled in.
//	Inputs: 
//		goldFile    - Name of gold file
//		outputFile  - Name of result file.
//		logfile	    - Name of log file reporter.
//		
//	Returns: 
//		Void
*/
void
FileUtility::checkResults(const XalanDOMString& outputFile, 
						  const XalanDOMString& goldFile, 
						  XMLFileReporter& logfile)
{
	int ambgFlag = data.nogold;	// get the current number of tests w/o gold files.

	// Compare the results, report success if compareSerializedResults returns true.
	if(compareSerializedResults(outputFile, goldFile))
	{
		cout << "Passed: " << data.testOrFile << endl;
		logfile.logCheckPass(data.testOrFile);
		data.pass += 1;
	}
	else
	{	// if the compairson fails gather up the failure data and determine if it failed 
		// due to bad output or missing Gold file. Lastly, log the failure.
		Hashtable attrs;
		Hashtable actexp;
		reportError();

		attrs.insert(Hashtable::value_type(XalanDOMString("reason"), XalanDOMString(data.msg)));
		attrs.insert(Hashtable::value_type(XalanDOMString("atNode"), data.currentNode));
		actexp.insert(Hashtable::value_type(XalanDOMString("exp"), data.expected));
		actexp.insert(Hashtable::value_type(XalanDOMString("act"), data.actual));

		if (ambgFlag < data.nogold)
		{
			logfile.logCheckAmbiguous(data.testOrFile);
		}
		else
		{
			logfile.logCheckFail(data.testOrFile, attrs, actexp);
		}
	}

}

void
FileUtility::checkAPIResults(const XalanDOMString& actual, 
						  const XalanDOMString& expected, 
						  char* msg,
						  XMLFileReporter& logfile)
{
	if(actual == expected)
	{
		data.pass += 1;
		cout << "Passed: " << data.testOrFile << endl;
		logfile.logCheckPass(data.testOrFile);
	}
	else
	{	data.actual = actual;
		data.expected = expected;
		data.currentNode = "API Test";
		data.msg = msg;
		data.fail += 1;

		reportError();
		Hashtable actexp;

		actexp.insert(Hashtable::value_type(XalanDOMString("exp"), expected));
		actexp.insert(Hashtable::value_type(XalanDOMString("act"), actual));

		logfile.logCheckFail(data.testOrFile, actexp);

	}

}



/*	This routine compares the results of a transform with the gold file.
//	It in turn call the domCompare routine to do the actual comparision. 
//	Inputs: 
//		gold - Dom tree for the expected results
//		doc  - Dom tree created during transformation
//		filename - Current filename
//		
//	Returns: 
//		Void
//		
*/
void
FileUtility::checkDOMResults(const XalanDOMString& theOutputFile, 
			   const XalanCompiledStylesheet* compiledSS, 
			   XalanSourceTreeDocument* dom,
			   const XSLTInputSource& goldInputSource,
			   XMLFileReporter& logfile)
{
	int ambgFlag = data.nogold;
	const XalanDOMString mimeEncoding("");
	XalanFileOutputStream myOutput(theOutputFile);
	XalanOutputStreamPrintWriter myResultWriter(myOutput);
	FormatterListener* theFormatter = getXMLFormatter(true,true,true,
															myResultWriter,0,
															mimeEncoding,
															compiledSS->getStylesheetRoot());

	FormatterTreeWalker theTreeWalker(*theFormatter);
	theTreeWalker.traverse(dom);
	delete theFormatter;


	XalanSourceTreeDOMSupport domSupport;
	XalanSourceTreeParserLiaison parserLiaison(domSupport);
	domSupport.setParserLiaison(&parserLiaison);
	
	XalanDocument* goldDom = parserLiaison.parseXMLStream(goldInputSource);

	
	if(domCompare(*goldDom, *dom))
	{
		cout << "Passed: " << data.testOrFile << endl;
		logfile.logCheckPass(data.testOrFile);
		data.pass += 1;
	}
	else
	{	// if the compairson fails gather up the failure data and determine if it failed 
		// due to bad output or missing Gold file. Lastly, log the failure.
		Hashtable attrs;
		Hashtable actexp;
		reportError();

		attrs.insert(Hashtable::value_type(XalanDOMString("reason"), XalanDOMString(data.msg)));
		attrs.insert(Hashtable::value_type(XalanDOMString("atNode"), data.currentNode));
		actexp.insert(Hashtable::value_type(XalanDOMString("exp"), data.expected));
		actexp.insert(Hashtable::value_type(XalanDOMString("act"), data.actual));

		if (ambgFlag < data.nogold)
		{
			logfile.logCheckAmbiguous(data.testOrFile);
		}
		else
		{
			logfile.logCheckFail(data.testOrFile, attrs, actexp);
		}
	}
}

/*	This routine takes the result file and gold file and parses them.
//  If either of the files fails to parse and a SAXException is throw,
//  then the files are compared using a char by char file compare,
//  otherwise the domCompare routine is used.
//	Inputs: 
//		outputFile:  Name of result file
//		goldFile:	 Name of gold file
//		
//	Returns: 
//		True or False
//		
*/
bool
FileUtility::compareSerializedResults(const XalanDOMString& outputFile,
									const XalanDOMString& goldFile)
{

	const XSLTInputSource resultInputSource(c_wstr(outputFile));
	const XSLTInputSource goldInputSource(c_wstr(goldFile));

	XalanSourceTreeDOMSupport domSupport;
	XalanSourceTreeParserLiaison parserLiaison(domSupport);
	domSupport.setParserLiaison(&parserLiaison);

	XalanDocument* goldDom = 0;
	XalanDocument* transformDom = 0;

	try
	{
		transformDom = parserLiaison.parseXMLStream(resultInputSource);
		goldDom = parserLiaison.parseXMLStream(goldInputSource);
		return domCompare(*goldDom, *transformDom);
	}

	// This exception is being reported prior to this Catch, however, however, I clarify that it's a SAX exception.
	// It's a good indication that the Gold file is not a valid XML.  When this happens the transform result needs
	// to be compared with the Gold,  with a character by character basis,  not via the DOM compair. 
	catch (SAXException)
	{
		cout << "SAXException: Using fileCompare to check output.\n";
		return fileCompare(c_str(TranscodeToLocalCodePage(goldFile)), c_str(TranscodeToLocalCodePage(outputFile)));
	}
 
}

/*	This routine is used to compare the results against the gold when one or both of 
//  fails to parse without throwing a SAXException. When a failure is detected the 'data' 
//  structure used to report detailed info about a failure is filled in.
//	Inputs: 
//		outputFile:  Name of result file
//		goldFile:	 Name of gold file
//		
//	Returns: 
//		True or False
//		
*/
bool
FileUtility::fileCompare(const char* goldFile,
						const char* outputFile)
{
	FILE *result, *gold;		// declare files
	char rline[132] = {'0'};	// declare buffers to hold single line from file
	char gline[132] = {'0'};	
	char temp[10];				// buffer to hold line number
	char lineNum = 1;

	// Set fail data incase there are i/o problems with the files to compare.
	data.expected = XalanDOMString(" ");
	data.actual = XalanDOMString(" ");
	data.currentNode = XalanDOMString("Line: 0");

	// Attempt to open the files. 
	result = fopen(outputFile, "r");
	gold   = fopen(goldFile, "r");

	// If the result file fails to open report this as a failure.
	if (!result)
	{
		data.msg = "No Result (Transform failed)";
		data.fail += 1;
		return false;
	}
	
	// If the gold file fails to open report this as ambiguous.
	if (!gold )
	{
		data.msg = "No Gold file";
		data.nogold += 1;
		return false;
	}

	// Start file comparison,  line by line..
	while( !feof(result) && !feof(gold))
	{
		fgets(gline, sizeof(gline), gold );
		fgets(rline, sizeof(rline), result );
		sprintf(temp,"%d",lineNum);

		if (ferror(gold) || ferror(result))
		{
			data.msg = "Read Error - Gold/Result file";
			data.currentNode = XalanDOMString("Line: ") + XalanDOMString(temp);
			return false;
		}

		// Compare the lines character by charcter ....
		unsigned int i = 0;
		while(i < strlen(gline)) 
		{
			if (gline[i] == rline[i]) 
			{
				i++;
				continue;
			}
			else 
			{	// If there is a mismatch collect up the fail data and return false.  To ensure that 
				// the results can be seen in the browser enclose the actual/expected in CDATA Sections.
				data.msg = "Text based comparison failure";
				data.expected = XalanDOMString("<![CDATA[") + XalanDOMString(gline) + XalanDOMString("]]>");
				data.actual = XalanDOMString("<![CDATA[") + XalanDOMString(rline) + XalanDOMString("]]>");
				data.currentNode = XalanDOMString("Line: ") + XalanDOMString(temp);
				data.fail += 1;
				return false;
			}
		}

		lineNum += 1;
	}

	return true;
}



/*	This routine performs a DOM Comparision. 
//	Inputs: 
//		gold - Dom tree for the expected results
//		doc  - Dom tree created during transformation
//		filename - Current filename
//		
//	Returns: 
//		True or False
//		
*/
bool 
FileUtility::domCompare(const XalanNode& gold ,const XalanNode& doc)
{
	const XalanNode::NodeType	docNodeType  = doc.getNodeType();
	const XalanNode::NodeType	goldNodeType = gold.getNodeType();

	const XalanDOMString&  docNodeName  = doc.getNodeName();	
	const XalanDOMString&  goldNodeName = gold.getNodeName();

	if (goldNodeType != docNodeType)
	{
		collectData("NodeType mismatch.",
					docNodeName,
					XalanDOMString(xalanNodeTypes[docNodeType]),
					XalanDOMString(xalanNodeTypes[goldNodeType]));
		return false;
	}

	switch (goldNodeType)
	{
	case XalanNode::ELEMENT_NODE:	// ATTRIBUTE_NODE's are processed with diffElement().
	{ 
		if ( ! diffElement(gold, doc) ) 
		{
			return false;
		}

		break;
	}
	case XalanNode::TEXT_NODE:	
	{
		const XalanDOMString&	docNodeValue  = doc.getNodeValue();
		const XalanDOMString&	goldNodeValue = gold.getNodeValue();
		
		//debugNodeData(docNodeName, docNodeValue);
		
		if(goldNodeValue != docNodeValue)
		{
			collectData("Text node mismatch. ", 
						 docNodeName,
						 goldNodeValue,
						 docNodeValue);
			return false;
		}

		// Need to process textnode siblings.  Note that text nodes do not have child nodes.
		const XalanNode	*goldNextNode;
		const XalanNode	*domNextNode;
		goldNextNode = gold.getNextSibling();
		domNextNode = doc.getNextSibling();

		if (0 != goldNextNode)
		{
			if (0 != domNextNode)
			{
				if ( ! domCompare(*goldNextNode, *domNextNode) )
					return false;
			}
			else
			{
				collectData("Element missing SiblingNode. ", 
						 docNodeName,
						 goldNextNode->getNodeName(),
						 goldNextNode->getNodeName());
				return false;
			}
		}

		break;
	}
	case XalanNode::CDATA_SECTION_NODE:
	case XalanNode::ENTITY_REFERENCE_NODE:
	case XalanNode::ENTITY_NODE:
	case XalanNode::PROCESSING_INSTRUCTION_NODE:
	case XalanNode::COMMENT_NODE:
	{
		break;
	}
	case XalanNode::DOCUMENT_NODE:
	{
		//debugNodeData(docNodeName);

		const XalanNode	*goldNextNode;
		const XalanNode	*domNextNode;

		goldNextNode = gold.getFirstChild();
		domNextNode = doc.getFirstChild();

		if (0 != goldNextNode)
		{
			if( ! domCompare(*goldNextNode,*domNextNode) )
				return false;
		}

		break;
	}

	case XalanNode::DOCUMENT_TYPE_NODE:
	case XalanNode::DOCUMENT_FRAGMENT_NODE:
	case XalanNode::NOTATION_NODE:
	{
		break;
	}
	default:
		cout << "What are you doing? " << endl;
	}

	return true;
}

/*	This routine compares two element nodes. 
//	Inputs: 
//		gold - Dom tree for the expected results
//		doc  - Dom tree created during transformation
//		filename - Current filenam
//		
//	Returns: 
//		True or False
//						
*/

bool
FileUtility::diffElement(const XalanNode& gold, const XalanNode& doc)
{
	const XalanDOMString&  docNodeName  = doc.getNodeName();	
	const XalanDOMString&  goldNodeName = gold.getNodeName();

	const XalanDOMString&  docNsUri  = doc.getNamespaceURI();
	const XalanDOMString&  goldNsUri = gold.getNamespaceURI();

	//debugNodeData(docNodeName);

	// This essentially checks 2 things, that the prefix and localname are the
	// same.  So specific checks of these items are not necessary.
	if (goldNodeName != docNodeName)
	{
		collectData("Element mismatch. ", 
						 docNodeName,
						 goldNodeName,
						 docNodeName);
		return false;
	}

	if ( goldNsUri != docNsUri)
	{
		collectData("Element NamespaceURI mismatch. ",
						 docNodeName,
						 goldNsUri,
						 docNsUri);
		return false;
	}

	// Get Attributes for each Element Node. 
	const XalanNamedNodeMap	*goldAttrs = gold.getAttributes();
	const XalanNamedNodeMap *docAttrs  = doc.getAttributes();

	// Get number of Attributes
	int numGoldAttr = goldAttrs->getLength();
	int numDomAttr  = docAttrs ->getLength();

	/*
	// This needs to be uncommented if 'compare.exe' is to work. 
	// If this is the 'root' element strip off the xmlns:xml namespace attribute,
	// that is lurking around on the gold file, but not the dom.  This is necessary
	// only for the 'compare' test, that uses a pure DOM, that has not been serialized.
	//if (goldNodeName == XalanDOMString("root"))
	{
		numGoldAttr -= 1;
		XalanNode *gXMLAttr = goldAttrs->item(1);
	}
	*/
	// Check that each Element has same number of Attributes. If they don't report error  
	if ( numGoldAttr == numDomAttr )
	{
		// Compare Attributes one at a time.
		//for (int i=1; i < numGoldAttr; i++)  // To be used with 'compare'
		for (int i=0; i < numGoldAttr; i++)
		{
			// Attribute order is irrelvant, so comparision is base on Attribute name.
			XalanNode *gAttr = goldAttrs->item(i);
			XalanDOMString goldAttrName = gAttr->getNodeName();

			XalanNode *dAttr = docAttrs->getNamedItem(goldAttrName);
			if (dAttr != 0)
			{
				if( ! (diffAttr(gAttr, dAttr)) )
					return false;
			}
			else
			{
				collectData("Element missing named Attribute. ",
						 docNodeName,
						 goldAttrName,
						 XalanDOMString("NOTHING"));
				return false;
			}
		}
	}
	else
	{
		char  buf1[2], buf2[2];
		sprintf(buf1, "%d", numGoldAttr);
		sprintf(buf2, "%d", numDomAttr);
		collectData("Wrong number of attributes. ",
						 docNodeName,
						 XalanDOMString(buf1),
						 XalanDOMString(buf2));
		return false;
	}

	const XalanNode	*goldNextNode;
	const XalanNode	*domNextNode;

	goldNextNode = gold.getFirstChild();
	domNextNode = doc.getFirstChild();

	if (0 != goldNextNode )
	{
		if (0 != domNextNode)
		{
			if ( ! domCompare(*goldNextNode, *domNextNode) )
				return false;
		}
		else
		{
			collectData("Element missing ChildNode. ", 
						 docNodeName,
						 XalanDOMString(goldNextNode->getNodeName()),
						 XalanDOMString("NOTHING"));
			return false;
		}
	}
	else if (domNextNode)
	{
		// The result doc has additional Children. If the additional node is a text node
		// then gather up the text and print it out.
		if ( domNextNode->getNodeType() == XalanNode::TEXT_NODE)
		{
			collectData("Result has additional Child node: ", 
					docNodeName,
					XalanDOMString("NOTHING"),		 
					XalanDOMString(domNextNode->getNodeName()) + XalanDOMString("  \"") +
					XalanDOMString(domNextNode->getNodeValue()) + XalanDOMString("\""));
		}
		// Additional node is NOT text, so just print it's Name.
		else
		{
			collectData("Result has additional Child node: ", 
						docNodeName,
						XalanDOMString("NOTHING"),		 
						XalanDOMString(domNextNode->getNodeName()));

		}
		return false;

	}

	goldNextNode = gold.getNextSibling();
	domNextNode = doc.getNextSibling();

	if (0 != goldNextNode)
	{
		if (0 != domNextNode)
		{
			if ( ! domCompare(*goldNextNode, *domNextNode) )
				return false;
		}
		else
		{	// domcomtest10 used to fail here,  now it is caught above, with the error
			// "Transformed Doc has additional Child nodes:"
			collectData("Element missing SiblingNode. ", 
						 docNodeName,
						 XalanDOMString(goldNextNode->getNodeName()),
						 XalanDOMString("NOTHING"));
			return false;
		}
	}
	else if ( domNextNode)
	{	
			// The result doc has additional siblings. If the additional node is a text node
			// then gather up the text and print it out.
			if ( domNextNode->getNodeType() == XalanNode::TEXT_NODE)
			{
				collectData("Result has additional sibling node: ", 
						docNodeName,
						XalanDOMString("NOTHING"),		 
						XalanDOMString(domNextNode->getNodeName()) + XalanDOMString("  \"") +
						XalanDOMString(domNextNode->getNodeValue()) + XalanDOMString("\""));
			}
			// Additional node is NOT text, so just print it's Name.
			else
			{
				collectData("Result has additional sibling node: ", 
						docNodeName,
						XalanDOMString("NOTHING"),		 
						XalanDOMString(domNextNode->getNodeName()));

			}
			return false;

	}

	return true;
}


/*	This routine compares two attribute nodes. 
//	Inputs: 
//		gAttr - attribute from Gold dom tree 
//		dAttr - attribute from Dom tree created during transformation
//		fileName - Current filenam
//		
//	Returns: 
//		True or False
//				
*/

bool FileUtility::diffAttr(const XalanNode* gAttr, const XalanNode* dAttr)
{

	const XalanDOMString& docAttrName  = dAttr->getNodeName();
	const XalanDOMString& goldAttrName = gAttr->getNodeName();

	//debugAttributeData(goldAttrName);

	const XalanDOMString& goldAttrValue = gAttr->getNodeValue();
	const XalanDOMString& docAttrValue	= dAttr->getNodeValue();

	if (goldAttrValue != docAttrValue)
	{
		collectData("Attribute Value mismatch. ",
						 docAttrName,
						 goldAttrValue,
						 docAttrValue);
		return false;
	}

	const XalanDOMString& goldAttrNsUri = gAttr->getNamespaceURI();
	const XalanDOMString& docAttrNsUri	= dAttr->getNamespaceURI();

	if (goldAttrNsUri != docAttrNsUri)
	{
		collectData("Attribute NamespaceURI mismatch. ", 
						 docAttrName,
						 goldAttrNsUri,
						 docAttrNsUri);
		return false;
	}

	return true;
}

/*	This routine reports DOM comparison errors. 
//	Inputs: 
//		file	-	Name of current file
//		node	-	Current node that fails
//		msg		-	Failure message
//				
*/
void
FileUtility::reportError()
{

	cout << endl << "* Failed "<< data.testOrFile 
		 << "  Error: " << data.msg << endl
		 << "	" << "Processing Node: " << data.currentNode << endl
		 << "	Expected:	" << data.expected << endl
		 << "	Actual:		" << data.actual << "\n\n";

}

/*	This routine collects up data pertinent to a dom comparison failure. 
//	Inputs: 
//		errmsg:			Reason for the failure.
//		currentnode:	Node in the dom tree where the mismatch occured
//		expdata:		Expected data based on the Gold file.
//		actdata:		Actual data returned in the result file.
//	Returns: 
//		Void
//						
*/
void 
FileUtility::collectData(char* errmsg, XalanDOMString currentnode, XalanDOMString expdata, XalanDOMString actdata)
{

	data.msg = errmsg;
	data.currentNode = currentnode;
	data.expected = expdata;
	data.actual = actdata;
	data.fail += 1;
}

void
FileUtility::reportPassFail(XMLFileReporter& logfile)
{
	Hashtable runResults;
	char temp[5];

	// Create entrys that contain numbers for Pass and Fail.
	sprintf(temp, "%d", data.pass);
	runResults.insert(Hashtable::value_type(XalanDOMString("Passed"), XalanDOMString(temp)));
	
	sprintf(temp, "%d", data.fail);
	runResults.insert(Hashtable::value_type(XalanDOMString("Failed"), XalanDOMString(temp)));

	sprintf(temp, "%d", data.nogold);
	runResults.insert(Hashtable::value_type(XalanDOMString("No_Gold_Files"), XalanDOMString(temp)));

	logfile.logElementWAttrs(10, "RunResults", runResults, "xxx");	

	cout << "\nPassed " << data.pass;
	cout << "\nFailed " << data.fail;
	cout << "\nMissing Gold " << data.nogold << endl;

}

void
FileUtility::reportPassFail(XMLFileReporter& logfile, const XalanDOMString& runid)
{
	Hashtable runResults;
	char temp[5];

	// Create entrys that contain runid, xerces version, and numbers for Pass, Fail and No Gold.

	runResults.insert(Hashtable::value_type(XalanDOMString("UniqRunid"), runid));
	runResults.insert(Hashtable::value_type(XalanDOMString("Xerces-Version "), getXercesVersion()));
	runResults.insert(Hashtable::value_type(XalanDOMString("ICU-Enabled "), XalanDOMString("No")));

#if defined(XALAN_USE_ICU)
	// At some point in time I want to be able to programatically check it the ICU is enabled.
	// Dave needs to add some code before I can do this. It will not be done via ifdef's. 
	runResults.insert(Hashtable::value_type(XalanDOMString("ICU-Enabled "), XalanDOMString("Yes")));
#endif

	sprintf(temp, "%d", data.pass);
	runResults.insert(Hashtable::value_type(XalanDOMString("Passed"), XalanDOMString(temp)));
	
	sprintf(temp, "%d", data.fail);
	runResults.insert(Hashtable::value_type(XalanDOMString("Failed"), XalanDOMString(temp)));

	sprintf(temp, "%d", data.nogold);
	runResults.insert(Hashtable::value_type(XalanDOMString("No_Gold_Files"), XalanDOMString(temp)));

	logfile.logElementWAttrs(10, "RunResults", runResults, "xxx");	

	cout << "\nPassed " << data.pass;
	cout << "\nFailed " << data.fail;
	cout << "\nMissing Gold " << data.nogold << endl;

}
