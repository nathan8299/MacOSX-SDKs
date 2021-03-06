/*
        NSSound.h
	Application Kit
	Copyright (c) 1997-2014, Apple Inc.
	All rights reserved.
*/

#import <AppKit/AppKitDefines.h>
#import <AppKit/NSPasteboard.h>
#import <Foundation/NSBundle.h>
#import <Foundation/NSDate.h>

@class NSData, NSURL;
@protocol NSSoundDelegate;

APPKIT_EXTERN NSString * const NSSoundPboardType;

@interface NSSound : NSObject <NSCopying, NSCoding, NSPasteboardReading, NSPasteboardWriting>
{
@private
    id _delegate;
    id _info;
    id _reserved[6];
    uint32_t _sFlags;
}

/* If this finds & creates the sound, only name is saved when archived.
*/
+ (NSSound *)soundNamed:(NSString *)name;

/* When archived, byref ? saves url : saves contents.
*/
- (instancetype)initWithContentsOfURL:(NSURL *)url byReference:(BOOL)byRef;


- (instancetype)initWithContentsOfFile:(NSString *)path byReference:(BOOL)byRef;

/* Whether the data comes in from disk, or via this method, NSSound expects it to have a proper magic number, sound header, and data.  Only uncompressed AIFF data is currently supported.
*/
- (instancetype)initWithData:(NSData *)data;

- (BOOL)setName:(NSString *)string;
@property (readonly, copy) NSString *name;

// Pasteboard support
+ (BOOL)canInitWithPasteboard:(NSPasteboard *)pasteboard;
+ (NSArray*)soundUnfilteredTypes NS_AVAILABLE_MAC(10_5);
- (instancetype)initWithPasteboard:(NSPasteboard *)pasteboard;
- (void)writeToPasteboard:(NSPasteboard *)pasteboard;

// Sound operations
- (BOOL)play;		/* sound is played asynchronously */
- (BOOL)pause;		/* returns NO if sound not paused */
- (BOOL)resume;		/* returns NO if sound not resumed */
- (BOOL)stop;
@property (getter=isPlaying, readonly) BOOL playing;

@property (assign) id<NSSoundDelegate> delegate;


/* Returns the duration of the sound in seconds.
*/
@property (readonly) NSTimeInterval duration NS_AVAILABLE_MAC(10_5);

/* Sets and gets the volume for the sound without affecting the system-wide volume. The valid range is between 0. and 1., inclusive.
*/
@property float volume NS_AVAILABLE_MAC(10_5);

/* Getter: If the sound is playing, currentTime returns the number of  seconds into the sound where playing is occurring.  If the sound is not playing, currentTime returns the number of seconds into the sound where playing would start.
   Setter: Sets the location of the currently playing audio to seconds. If the sound is not playing, this sets the number of seconds into the sound where playing would begin. The currentTime is not archived, copied, or stored on the pasteboard - all new sounds start with a currentTime of 0.
*/
@property NSTimeInterval currentTime NS_AVAILABLE_MAC(10_5);

/* Setter: Sets whether the sound should automatically restart when it is finished playing.  If the sound is currently playing, this takes effect immediately. The default is NO.  A looping sound does not send sound:didFinishPlaying: to its delegate unless it is sent a stop message.
   Getter: Returns whether the sound will automatically restart when it is finished playing.
*/
@property BOOL loops NS_AVAILABLE_MAC(10_5);

/* Setter: Set the UID of the audio device where playback will occur.  Pass nil to play on the default output device.
   Getter: Get the UID of the audio device where playback will occur.  Returns nil if playback tracks the default device, which is the default.
*/
@property (copy) NSString *playbackDeviceIdentifier NS_AVAILABLE_MAC(10_5);

/* Set the channel mapping for the sound.  Pass an array of NSNumbers, which maps sound channels to device channels.  Pass -1 to indicate that a particular sound channel should be ignored.  For any channel, instead of an NSNumber, you may also pass an NSArray of NSNumbers to map a single sound channel to multiple device channels.
*/
- (void)setChannelMapping:(NSArray *)channelMapping NS_DEPRECATED_MAC(10_5, 10_9);

/* Get the channel mapping for the sound.  By default, a stereo sound maps its first and second channels to the left and right device channels, while a mono sound maps its single channel across every device channel.
*/
- (NSArray *)channelMapping NS_DEPRECATED_MAC(10_5, 10_9);


@end

@interface NSSound (NSDeprecated)

/* Methods that were deprecated in Mac OS 10.5. You can now use +soundUnfilteredTypes to get an array of Uniform Type Identifiers (UTIs).
*/
+ (NSArray *)soundUnfilteredFileTypes NS_DEPRECATED_MAC(10_0, 10_5);
+ (NSArray *)soundUnfilteredPasteboardTypes NS_DEPRECATED_MAC(10_0, 10_5);

@end

@protocol NSSoundDelegate <NSObject>
@optional

- (void)sound:(NSSound *)sound didFinishPlaying:(BOOL)aBool;

@end

@interface NSBundle (NSBundleSoundExtensions)

/* May return nil if no file found
*/
- (NSString *)pathForSoundResource:(NSString *)name;

@end

