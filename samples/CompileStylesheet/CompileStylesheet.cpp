// Base header file.  Must be first.
#include <Include/PlatformDefinitions.hpp>



#include <cassert>



#if defined(XALAN_OLD_STREAM_HEADERS)
#include <fstream.h>
#include <iostream.h>
#include <strstream.h>
#else
#include <fstream>
#include <iostream>
#include <strstream>
#endif



#include <util/PlatformUtils.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPathFactoryDefault.hpp>



#include <XSLT/StylesheetConstructionContextDefault.hpp>
#include <XSLT/StylesheetExecutionContextDefault.hpp>
#include <XSLT/StylesheetRoot.hpp>
#include <XSLT/XSLTEngineImpl.hpp>
#include <XSLT/XSLTInit.hpp>
#include <XSLT/XSLTInputSource.hpp>
#include <XSLT/XSLTProcessorEnvSupportDefault.hpp>
#include <XSLT/XSLTResultTarget.hpp>



#include <XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <XalanSourceTree/XalanSourceTreeParserLiaison.hpp>



int
main(
			int				argc,
			const char*		/* argv */[])
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::endl;
	using std::ifstream;
	using std::ostrstream;
#endif

	if (argc != 1)
	{
		cerr << "Usage: CompileStylesheet"
			 << endl
			 << endl;
	}
	else
	{
		try
		{
			// Call the static initializer for Xerces...
			XMLPlatformUtils::Initialize();

			{
				// Initialize the Xalan XSLT subsystem...
				XSLTInit						theInit;

				// Create some support objects that are necessary for running the processor...
				XalanSourceTreeDOMSupport		theDOMSupport;
				XalanSourceTreeParserLiaison	theParserLiaison(theDOMSupport);

				// Hook the two together...
				theDOMSupport.setParserLiaison(&theParserLiaison);

				// Create some more support objects.
				XSLTProcessorEnvSupportDefault	theXSLTProcessorEnvSupport;
				XObjectFactoryDefault			theXObjectFactory;
				XPathFactoryDefault				theXPathFactory;

				// Create a processor...
				XSLTEngineImpl	theProcessor(
						theParserLiaison,
						theXSLTProcessorEnvSupport,
						theDOMSupport,
						theXObjectFactory,
						theXPathFactory);

				// Connect the processor to the support object...
				theXSLTProcessorEnvSupport.setProcessor(&theProcessor);

				// Create separate factory support objects so the stylesheet's
				// factory-created XPath instances are independent from the
				// processor's.
				XPathFactoryDefault				theStylesheetXPathFactory;

				// Create a stylesheet construction context, using the
				// stylesheet's factory support objects.
				StylesheetConstructionContextDefault	theConstructionContext(
							theProcessor,
							theXSLTProcessorEnvSupport,
							theStylesheetXPathFactory);

				// The execution context uses the same factory support objects as
				// the processor, since those objects have the same lifetime as
				// other objects created as a result of the execution.
				StylesheetExecutionContextDefault		theExecutionContext(
							theProcessor,
							theXSLTProcessorEnvSupport,
							theDOMSupport,
							theXObjectFactory);

				// Our input file.  The assumption is that the executable will be run
				// from same directory as the input files.
				const XalanDOMString	theXSLFileName("foo.xsl");

				// Our stylesheet input source...
				XSLTInputSource			theStylesheetSource(c_wstr(theXSLFileName));

				// Ask the processor to create a StylesheetRoot for the specified
				// input XSL.  This is the compiled stylesheet.  We don't have to
				// delete it, since it is owned by the StylesheetConstructionContext
				// instance.
				StylesheetRoot* const	theStylesheetRoot =
					theProcessor.processStylesheet(
								theStylesheetSource,
								theConstructionContext);
				assert(theStylesheetRoot != 0);

				for (unsigned int i = 0; i < 10; i++)
				{
					theExecutionContext.setStylesheetRoot(theStylesheetRoot);

					// Buffers passed in to ostrstream.
					char		inBuffer[10];
					char		outBuffer[10];

					// Generate the input and output file names.
					ostrstream	theFormatterIn(inBuffer, sizeof(inBuffer));
					ostrstream	theFormatterOut(outBuffer, sizeof(outBuffer));
					
					theFormatterIn << "foo" << i + 1 << ".xml" << '\0';
					theFormatterOut << "foo" << i + 1 << ".out" << '\0';

					//Generate the XML input and output objects.
					XSLTInputSource		theInputSource(theFormatterIn.str());
					XSLTResultTarget	theResultTarget(XalanDOMString(theFormatterOut.str()));

					// Do the tranformation...
					theProcessor.process(
						 theInputSource,
						 theResultTarget,
						 theExecutionContext);

					// Reset the processor and the execution context
					// so we can perform the next transformation.
					// Reset the parser liaison to clear out the
					// source document we just transformed.
					theProcessor.reset();
					theExecutionContext.reset();
					theParserLiaison.reset();
				}
			}

			// Call the static terminator for Xerces...
			XMLPlatformUtils::Terminate();
		}
		catch(...)
		{
			cerr << "Exception caught!!!"
				 << endl
				 << endl;
		}
	}

	return 0;
}
