/************************************************************************************
COPYRIGHT © 2001 Celsis Limited
All International Rights Reserved
(This work is unpublished)

CONFIDENTIAL TRADE SECRET
This material is the property of Celsis Limited and may not be 
used, copied, or distributed in any form without written 
permission from Celsis.

     Celsis Limited
     Cambridge Science Park
     Milton Road, Cambridge, CB4 0FX, UK
     Telephone: +44 (0)1223 426008
     Facsimile:   +44 (0)1223 426003
     Web address: http://www.Celsis.com

FILE STATUS
Design Specification:  887/S/02

Modifications:
(WHO)           DD-MMM-YY   		(REASON)
ITL				01-Sept-01			New documentation base line of version 2.1.7

************************************************************************************/

// CustPal.cpp: implementation of the CCustPal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lucept.h"
#include "CustPal.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
// This structure is equivalent to a 27 color LOGPALLETE
// but must be cast to a standard LOGPALLETE type for use
typedef struct 
{ 
    WORD         palVersion; 
    WORD         palNumEntries; 
    PALETTEENTRY palPalEntry[27]; 
} LOGPAL27; 
 
static LOGPAL27 logPalette27 = 
{
	0x300,		// Windows version of LOGPALETTE structure
	27,
	{
		{255, 255, 255, 0},	//White
		{240, 240, 240, 0},	//Gray1
		{221, 221, 221, 0},	//Gray2
		{204, 204, 204, 0},	//Gray3
		{192, 192, 192, 0},	//Gray4
		{160, 160, 160, 0},	//Gray5
		{153, 153, 153, 0},	//Gray6
		{128, 128, 128, 0},	//Gray7
		{102, 102, 102, 0},	//Gray8
		{ 51,  51,  51, 0},	//Gray9
		{  0,   0,   0, 0},	//Black
		{205, 255, 255, 0},	//Green1
		{150, 204, 204, 0},	//Green2	 
		{102, 153, 153, 0},	//Green3
		{  0, 102, 102, 0},	//Green4
		{204, 204, 255, 0},	//Blue1
		{153, 153, 255, 0},	//Blue2
		{102, 102, 204, 0},	//Blue3
		{255, 251, 240, 0},	//Cream
		{255, 222, 153, 0},	//Yellow
		{255, 172,   0, 0},	//Gold
		{255, 153,   0, 0},	//Orange
		{204, 153,   0, 0},	//Copper
		{255, 149, 149, 0},	//Salmon
		{220,  30,  30, 0},	//Red
		{204,   0, 153, 0},	//Red Purple
		{255, 102, 217, 0}	//Light Red Purple
	}
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustPal::CCustPal()
{
	// Create the pallete
	CreatePalette((LOGPALETTE*)&logPalette27);
}

CCustPal::~CCustPal()
{

}

int CCustPal::AddDIBPalette(HDIB hDIB)
{
	if (hDIB == NULL)
	{
	  return -1;
	}

    // pointer to packed-DIB
	LPSTR lpbi = (LPSTR)::GlobalLock((HGLOBAL) hDIB);        

    // get pointer to BITMAPINFO (Win 3.0) 
    LPBITMAPINFO lpbmi = (LPBITMAPINFO)lpbi;
	// is this a Win 3.0 DIB? 
	if (!IS_WIN30_DIB(lpbi))
	{
		::GlobalUnlock((HGLOBAL) hDIB);
		return -1;
	}

    // get the number of colors in the DIB 
	WORD wNumColors = ::DIBNumColors(lpbi);

	int newColors = 0;
	for (int i = 0; i < wNumColors; i++)
	{
		PALETTEENTRY palEntry;
		COLORREF	color;

		// Read next color from DIB palette
		color = RGB(lpbmi->bmiColors[i].rgbRed,
					lpbmi->bmiColors[i].rgbGreen,
					lpbmi->bmiColors[i].rgbBlue);

		// Check if color already exists
		UINT iEntry = GetNearestPaletteIndex(color);
		VERIFY(GetPaletteEntries(iEntry, 1, &palEntry) == 1);
		bool colorExists = true;
		if (palEntry.peRed != lpbmi->bmiColors[i].rgbRed)
		{
			palEntry.peRed = lpbmi->bmiColors[i].rgbRed;
			colorExists = false;
		}
		if (palEntry.peGreen != lpbmi->bmiColors[i].rgbGreen)
		{
			palEntry.peGreen = lpbmi->bmiColors[i].rgbGreen;
			colorExists = false;
		}
		if (palEntry.peBlue != lpbmi->bmiColors[i].rgbBlue)
		{
			palEntry.peBlue = lpbmi->bmiColors[i].rgbBlue;
			colorExists = false;
		}
		palEntry.peFlags = 0;

		if (!colorExists)
		{	// Add new entry
			VERIFY(ResizePalette(GetEntryCount() + 1));
			newColors++;

			// Add new color
			VERIFY(SetPaletteEntries(GetEntryCount() - 1, 1, &palEntry) == 1);
		}
	}
	GetEntryCount();

	return newColors;
}

