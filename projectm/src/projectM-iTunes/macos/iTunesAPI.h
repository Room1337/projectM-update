/*
     File:       iTunesAPI.h
 
     Contains:   iTunes Plug-ins interfaces
 
     Version:    Technology: iTunes
                 Release:    1.1
 
     Copyright:  � 2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __ITUNESAPI__
#define __ITUNESAPI__

#include <Files.h>
#include <MacTypes.h>
#include <MacWindows.h>
#include <OSUtils.h>
#include <Quickdraw.h>
#include <Sound.h>

#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if PRAGMA_STRUCT_ALIGN
    #pragma options align=power
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

enum {
	kITCurrentPluginMajorMessageVersion = 10,
	kITCurrentPluginMinorMessageVersion = 1
};

enum {
	kTrackSupportsID3Tags		= (1L << 0),
	kTrackHasVariableBitRate	= (1L << 1),
	kTrackIsFolder				= (1L << 2),			/* Device tracks only. Information returned is for a folder */
	kTrackIsLocked				= (1L << 3),			/* Device tracks only. Track cannot be deleted or renamed */
	kTrackCanBeDownloaded		= (1L << 4),			/* Device tracks only. Track can be copied from device to desktop. */
	kTrackIsHidden				= (1L << 5)				/* Device tracks only. Track should not be displayed in the device window. */
};
typedef OptionBits	ITTrackAttributes;

enum {
	/*
		These mask values are specified in ITTrackInfo.validFields
		to indicate which fields contain valid data
	*/
	
	kITTINameFieldMask			= (1L << 0),
	kITTIFileNameFieldMask		= (1L << 1),
	kITTIArtistFieldMask		= (1L << 2),
	kITTIAlbumFieldMask			= (1L << 3),
	kITTIGenreFieldMask			= (1L << 4),
	kITTIKindFieldMask			= (1L << 5),
	kITTITrackNumberFieldsMask	= (1L << 6),
	kITTIYearFieldMask			= (1L << 7),
	kITTISoundVolumeFieldMask	= (1L << 8),
	kITTICommentsFieldMask		= (1L << 10),
	kITTITotalTimeFieldMask		= (1L << 11),			
	kITTIStartTimeFieldMask		= (1L << 12),
	kITTIStopTimeFieldMask		= (1L << 13),
	kITTISizeFieldMask			= (1L << 14),
	kITTIBitRateFieldMask		= (1L << 15),
	kITTISampleRateFieldMask	= (1L << 16),
	kITTIAttributesFieldMask	= (1L << 17),
	kITTIFileTypeFieldMask		= (1L << 18),
	kITTIDateFieldMask			= (1L << 19)
};
typedef OptionBits	ITTIFieldMask;

struct ITTrackInfo {
	ITTIFieldMask		validFields;
	UInt32				reserved;						/* Must be zero */
	
	Str255				name;	
	Str255				fileName;
	Str255				artist;
	Str255				album;
		
	Str255				genre;
	Str255				kind;
	
	UInt32				trackNumber;
	UInt32				numTracks;
		
	UInt16				year;
	SInt16				soundVolumeAdjustment;			/* Valid range is -255 to +255 */
	
	Str255				unusedReserved1;				/* Must be empty string */			
	Str255				comments;
	
	UInt32				totalTimeInMS;
	UInt32				startTimeInMS;
	UInt32				stopTimeInMS;

	UInt32				sizeInBytes;

	UInt32				bitRate;
	UInt32				sampleRateFixed;

	OSType				fileType;
	
	UInt32				date;
	UInt32				unusedReserved2;				/* Must be zero */
	
	ITTrackAttributes	attributes;
	ITTrackAttributes	validAttributes;				/* Mask indicating which attributes are applicable */
};
typedef struct ITTrackInfo ITTrackInfo;

enum {
	kCurrentITStreamInfoVersion = 1
};

typedef FSSpec	ITFileSpec;

struct ITFileLocation {
	ITFileSpec			spec;
	FInfo				fInfo;
};
typedef struct ITFileLocation ITFileLocation;


struct ITStreamInfo {
	SInt32				version;
	Str255				streamTitle;
	Str255				streamURL;
	Str255				streamMessage;
};
typedef struct ITStreamInfo ITStreamInfo;

enum {
	/* messages sent to plugin main */
	
	kPluginInitMessage					= 'init',
	kPluginCleanupMessage				= 'clr ',
	
	kPluginIdleMessage					= 'idle'
};


enum {
	/* PluginInitMessage.options */
	
	kPluginWantsIdleMessages	= (1L << 1),	/* Send idle messages to plugin main */
	kPluginWantsToBeLeftOpen	= (1L << 2)		/* Don't close this plugin just because */
												/* it didn't register anyone */
};


enum {
	/* iTunes API messages */

	kPlayerRegisterVisualPluginMessage	= 'rvis',	/* Register a visual plugin */
	kPlayerRegisterDevicePluginMessage	= 'rdev',	/* Register a device plugin */
	
	/* Available for all plugins */

	kPlayerIdleMessage					= 'idle',	/* Give iTunes some time */
	
	kPlayerShowAboutMessage				= 'abou',	/* Show the about box. */
	kPlayerOpenURLMessage				= 'url ',	/* Open a URL */

	kPlayerSetPluginDataMessage			= 'sprf',	/* Set plugin preferences */
	kPlayerGetPluginDataMessage			= 'gprf',	/* Get plugin preferences */
	
	kPlayerSetPluginNamedDataMessage	= 'snpr',	/* Set plugin named preferenes */
	kPlayerGetPluginNamedDataMessage	= 'gnpr',	/* Get plugin named preferenes */
	
	kPlayerHandleMacOSEventMessage		= 'evnt',	/* Tell player to handle unhandled event */
		
	kPlayerGetPluginFileSpecMessage		= 'pspc',	/* Get the location of the plugin executable */

	/* Available for visual plugins */

	kPlayerSetFullScreenMessage			= 'sful',	/* Set full screen mode */
	kPlayerSetFullScreenOptionsMessage	= 'sfop',	/* Set full screen options */
	
	/* Available for device plugins */

	kPlayerSetDeviceSerialNumberMessage = 'dvsn',	/* Set a serial number string for a device. Needed for per-device prefs */
	kPlayerUnregisterPluginMessage		= 'unrg'	/* Unregister the plugin this comes from */
};

typedef OSStatus (*ITAppProcPtr)(void *appCookie, OSType message, struct PlayerMessageInfo *messageInfo);


/*
	Plugin main Messages
*/

struct PluginInitMessage {
	UInt32							majorVersion;			/* Input */
	UInt32							minorVersion;			/* Input */

	void *							appCookie;				/* Input */
	ITAppProcPtr					appProc;				/* Input */
	
	OptionBits						options;				/* Output, see above for values */
	void *							refCon;					/* Output */
};
typedef struct PluginInitMessage PluginInitMessage;

struct PluginMessageInfo {
	union {
		PluginInitMessage			initMessage;
	} u;
};
typedef struct PluginMessageInfo PluginMessageInfo;



/* Plugin main entry point message handler */
typedef OSStatus (*PluginProcPtr)(OSType message, PluginMessageInfo *messageInfo, void *refCon);

/* Visual plugin message handler */
typedef OSStatus (*VisualPluginProcPtr)(OSType message, struct VisualPluginMessageInfo *messageInfo, void *refCon);

/* Device plugin message handler */
typedef OSStatus (*DevicePluginProcPtr)(OSType message, struct DevicePluginMessageInfo *messageInfo, void *refCon);



/*
	Callbacks to iTunes
*/

enum {
	/* PlayerRegisterVisualPluginMessage.options */
	
	kVisualWantsIdleMessages			= (1L << 3),
	kVisualWantsConfigure				= (1L << 5)
};

struct PlayerRegisterVisualPluginMessage {
	/* Input from plugin */
	
	Str63							name;						/* Displayed in the Visual menu */
	OptionBits						options;					/* See above */
	
	OSType							creator;					/* Identifies the plugin */
	
	NumVersion						pluginVersion;				/* Version number of the plugin */

	VisualPluginProcPtr				handler;					/* Handler for the plugin's messages */
	void *							registerRefCon;				/* RefCon for the plugin's handler */

	UInt32							timeBetweenDataInMS;		/* How often to call the plugin (0xFFFFFFFF = as often as possible) */
	UInt32							numWaveformChannels;		/* 0-2 waveforms requested */
	UInt32							numSpectrumChannels;		/* 0-2 spectrums requested */
	
	SInt16							minWidth;					/* Minimum resizeable width */
	SInt16							minHeight;					/* Minimum resizeable height */
	
	SInt16							maxWidth;					/* Maximum resizeable width */
	SInt16							maxHeight;					/* Maximum resizeable height */
		
	UInt16							minFullScreenBitDepth;		/* 0 = Any */
	UInt16							maxFullScreenBitDepth;		/* 0 = Any */
	
	UInt16							windowAlignmentInBytes;		/* Reserved (should be zero) */
};
typedef struct PlayerRegisterVisualPluginMessage PlayerRegisterVisualPluginMessage;

enum {
	/* PlayerRegisterDevicePluginMessage.options */
	
	kDeviceWantsIdleMessages			= (1L << 0)
};

struct PlayerRegisterDevicePluginMessage {
	/* Input from plugin */
	
	Str63							name;						/* Used internally. Not currently displayed */
	OptionBits						options;					/* See above */
	
	NumVersion						pluginVersion;				/* Version number of the plugin */

	DevicePluginProcPtr				handler;					/* Handler for the plugin's messages */
	void *							registerRefCon;				/* RefCon for the plugin's handler */
	
	UInt32							reserved[2];				/* Must be zero */
};
typedef struct PlayerRegisterDevicePluginMessage PlayerRegisterDevicePluginMessage;

struct PlayerSetFullScreenMessage {
	Boolean							fullScreen;
};
typedef struct PlayerSetFullScreenMessage PlayerSetFullScreenMessage;

struct PlayerSetFullScreenOptionsMessage {
	SInt16							minBitDepth;				/* 0 = Any */
	SInt16							maxBitDepth;				/* 0 = Any */
	SInt16							preferredBitDepth;			/* 0 = Current */
	
	SInt16							desiredWidth;				/* Must be within minWidth & maxWidth */
	SInt16							desiredHeight;				/* Must be within minHeight & maxHeight */
};
typedef struct PlayerSetFullScreenOptionsMessage PlayerSetFullScreenOptionsMessage;

struct PlayerOpenURLMessage {
	SInt8 *							url;
	UInt32							length;
};
typedef struct PlayerOpenURLMessage PlayerOpenURLMessage;

struct PlayerSetPluginDataMessage {
	void *							dataPtr;		/* Input */
	UInt32							dataSize;		/* Input */
};
typedef struct PlayerSetPluginDataMessage PlayerSetPluginDataMessage;

struct PlayerGetPluginDataMessage {
	void *							dataPtr;		/* Input */
	UInt32							dataBufferSize;	/* Input */
	
	UInt32							dataSize;		/* Output */
};
typedef struct PlayerGetPluginDataMessage PlayerGetPluginDataMessage;

struct PlayerSetPluginNamedDataMessage {
	ConstStringPtr					dataName;		/* Input */

	void *							dataPtr;		/* Input */
	UInt32							dataSize;		/* Input */
};
typedef struct PlayerSetPluginNamedDataMessage PlayerSetPluginNamedDataMessage;

struct PlayerGetPluginNamedDataMessage {
	ConstStringPtr					dataName;		/* Input */

	void *							dataPtr;		/* Input */
	UInt32							dataBufferSize;	/* Input */
	
	UInt32							dataSize;		/* Output */
};
typedef struct PlayerGetPluginNamedDataMessage PlayerGetPluginNamedDataMessage;

struct PlayerHandleMacOSEventMessage {
	const EventRecord *				theEvent;		/* Input */
	
	Boolean							handled;		/* Output */
};
typedef struct PlayerHandleMacOSEventMessage PlayerHandleMacOSEventMessage;

struct PlayerGetPluginFileSpecMessage {
	FSSpec *						fileSpec;		/* Output */
};
typedef struct PlayerGetPluginFileSpecMessage PlayerGetPluginFileSpecMessage;

struct PlayerSetDeviceSerialNumberMessage {
	ConstStringPtr					serialNumber;	/* Input */
};
typedef struct PlayerSetDeviceSerialNumberMessage PlayerSetDeviceSerialNumberMessage;

struct PlayerMessageInfo {
	UInt32									messageMajorVersion;		/* Should be kITCurrentPluginMajorMessageVersion */
	UInt32									messageMinorVersion;		/* Should be kITCurrentPluginMinorMessageVersion */
	UInt32									messageInfoSize;			/* Should be sizeof(PlayerMessageInfo) */
	
	union {
		PlayerRegisterVisualPluginMessage	registerVisualPluginMessage;
		PlayerRegisterDevicePluginMessage	registerDevicePluginMessage;

		PlayerOpenURLMessage				openURLMessage;
		
		PlayerSetPluginDataMessage			setPluginDataMessage;
		PlayerGetPluginDataMessage			getPluginDataMessage;
		
		PlayerSetPluginNamedDataMessage		setPluginNamedDataMessage;
		PlayerGetPluginNamedDataMessage		getPluginNamedDataMessage;
		
		PlayerHandleMacOSEventMessage		handleMacOSEventMessage;
		
		PlayerGetPluginFileSpecMessage		getPluginFileSpecMessage;

		PlayerSetFullScreenMessage			setFullScreenMessage;
		PlayerSetFullScreenOptionsMessage	setFullScreenOptionsMessage;

		PlayerSetDeviceSerialNumberMessage	setDeviceSerialNumberMessage;
	} u;
};
typedef struct PlayerMessageInfo PlayerMessageInfo;

extern OSStatus ITCallApplication (void *appCookie, ITAppProcPtr appProc, OSType message, PlayerMessageInfo *messageInfo);
extern void		SetNumVersion (NumVersion *numVersion, UInt8 majorRev, UInt8 minorAndBugRev, UInt8 stage, UInt8 nonRelRev);


/* for use from plugin main */

extern OSStatus PlayerRegisterVisualPlugin (void *appCookie, ITAppProcPtr appProc, PlayerMessageInfo *messageInfo);
extern OSStatus PlayerRegisterDevicePlugin (void *appCookie, ITAppProcPtr appProc, PlayerMessageInfo *messageInfo);

/* For all plugins */

extern OSStatus PlayerIdle (void *appCookie, ITAppProcPtr appProc);

extern void		PlayerShowAbout (void *appCookie, ITAppProcPtr appProc);
extern void		PlayerOpenURL (void *appCookie, ITAppProcPtr appProc, SInt8 *string, UInt32 length);

extern OSStatus	PlayerGetPluginData (void *appCookie, ITAppProcPtr appProc, void *dataPtr, UInt32 dataBufferSize, UInt32 *dataSize);
extern OSStatus	PlayerSetPluginData (void *appCookie, ITAppProcPtr appProc, void *dataPtr, UInt32 dataSize);

extern OSStatus	PlayerGetPluginNamedData (void *appCookie, ITAppProcPtr appProc, ConstStringPtr dataName, void *dataPtr, UInt32 dataBufferSize, UInt32 *dataSize);
extern OSStatus	PlayerSetPluginNamedData (void *appCookie, ITAppProcPtr appProc, ConstStringPtr dataName, void *dataPtr, UInt32 dataSize);

extern OSStatus PlayerHandleMacOSEvent (void *appCookie, ITAppProcPtr appProc, const EventRecord *theEvent, Boolean *eventHandled);

extern OSStatus	PlayerGetPluginFileSpec (void *appCookie, ITAppProcPtr appProc, FSSpec *pluginFileSpec);

/* For visual plugins */

extern OSStatus PlayerSetFullScreen (void *appCookie, ITAppProcPtr appProc, Boolean fullScreen);
extern OSStatus PlayerSetFullScreenOptions (void *appCookie, ITAppProcPtr appProc, SInt16 minBitDepth, SInt16 maxBitDepth, SInt16 preferredBitDepth, SInt16 desiredWidth, SInt16 desiredHeight);

/* For device plugins */

extern OSStatus	PlayerSetDeviceSerialNumber (void *appCookie, ITAppProcPtr appProc, ConstStringPtr serialNumber);
extern OSStatus	PlayerUnregisterPlugin (void *appCookie, ITAppProcPtr appProc, PlayerMessageInfo *messageInfo);

#if PRAGMA_STRUCT_ALIGN
    #pragma options align=reset
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(pop)
#elif PRAGMA_STRUCT_PACK
    #pragma pack()
#endif

#ifdef __cplusplus
}
#endif

#endif /* __ITUNESAPI__ */