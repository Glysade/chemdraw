Revvity Open Source ChemDraw File Format
========================================
The Revvity ChemDraw specific code is in the chemdraw directory.
This code relies on expat and expatpp which are included in the expat directory.

Note:
-----
The expatpp used is a relatively old version of expat which is retained for compatibility
reasons.  At somepoint we will investigate upgrading to a more modern and supported
version.

Building
--------

See the RDKit for instructions.  Builds are in rkdit/External/ChemDraw

ChemDraw library
----------------

All files are released under the BSD 3-Clause license, as seen in the license.txt
This license has been added to all headers and CPP files.

A few of the original files have been modified to add missing funcitonality and to
work in the RDKit environment.  The relevant sections are marked in the headers as
"Modified by Glysade"

 * CoreChemistryAPI.h:
   1. reimplemented to use the RDKit's export mechanism
   2. We treat OSX builds like linux builds so we set TARGET_OS_MAC to 0

 * CDObject.cpp:
   The original definitions for replace UTF8 characters were not supplied, these have been
   replaced with the following:
   ```
   // Begin added by Glysade
   namespace cs {
       const char* kSymbolDegree   = u8"\u00B0";  
       const char* kSymbolEllipsis = u8"\u2026";  
       const char* kSymbolEnDash   = u8"\u2013";  
       const char* kSymbolEmDash   = u8"\u2014";  
       const char* kSymbolPlusMinus= u8"\u00B1"; 
       const char* kSymbolBullet   = u8"\u2022";  
       const char* kSymbolCenterDot= u8"\u00B7";  
       const char* kSymbolReg      = u8"\u00AE";  
       const char* kSymbolCopyright= u8"\u00A9";  
       const char* kSymbolAngstrom = u8"\u212B";  
       const char* kSymbolMicro    = u8"\u00B5";  
       const char* kSymbolCent     = u8"\u00A2";  
       const char* kSymbolPound    = u8"\u00A3";  
   }
   // End added by glysade
   ```
 * CDXAttachmentDataHelper.cpp:
    Commented out unused and unimplmented GetTagPositionForObject

Other Reimplemented Headers
---------------------------

In the original code release from Revvity, there were some missing header files.
These are the ones that have either been reimplmented or left as empty if
they were not actually used by the extracted code base.

BasicTypes.h:
  Unused header File, Only provided since it is include somewhere

Base64Converter.h:
  Added csBase64ToBinary and csBinaryToBase64
  
CDMap.h:
  Implements missing CDMap
  
UTF8Iterator.h:
  Implementation of a missing UTF8 iterator that follows the used ChemDraw spec
  
UTF8String.h:
  Unused header File, only provided since it is include elsewhere

cs_assert.h:
  Reimplementation to use generic asserts
  
cs_charUtils.h:
  Only implements IsChargeSign to basic ascii codes
  Unsure if this is completely correct.
  
cs_lock.h:
  Reimplemented to use std::memory locks
  
cs_mapUtils.h:
  Implements missing cdInitMap
  
cs_math.h:
   Implements missing Round function
   
cs_specialchardefs.h:
   Empty header file (will be reinstated with new code given by Revvity)
   
cs_stringUtils.h:
   Implements missing ischar_equals and str_compareNoCase
   
cs_univDefs.h:
   Redefines NEW as new
   
ffMDL.h:
   Unused header file for MDL support, since the RDKit supplies it's own MDL
   support, this is currently empty.
   



 