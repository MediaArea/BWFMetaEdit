// BWF MetaEdit Common - Common stuff for BWF MetaEdit interfaces
//
// This code was created in 2010 for the Library of Congress and the
// other federal government agencies participating in the Federal Agencies
// Digital Guidelines Initiative and it is in the public domain.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include "Common/Mac_Helpers.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
//---------------------------------------------------------------------------

Ztring makeUniqueFileName()
{
    NSString *unique = [[NSProcessInfo processInfo] globallyUniqueString];
    Ztring toReturn = Ztring().From_UTF8([unique UTF8String]);

    return toReturn;
}

Ztring makeTemporaryDirectoryForFile(const Ztring& path)
{
    Ztring toReturn;
    NSURL *url = [NSURL fileURLWithPath:@(path.To_UTF8().c_str())];

    if(!url)
        return toReturn;

    NSURL *temporaryDirectoryURL = [[NSFileManager defaultManager]
                                     URLForDirectory:NSItemReplacementDirectory
                                     inDomain:NSUserDomainMask
                                     appropriateForURL:url
                                     create:YES
                                     error:nil];

    if(!temporaryDirectoryURL)
        return toReturn;

    if(![temporaryDirectoryURL isFileURL])
        return toReturn;

    NSString *temporaryDirectoryPath = [temporaryDirectoryURL path];

    if(!temporaryDirectoryPath)
        return toReturn;

    toReturn = Ztring().From_UTF8([temporaryDirectoryPath UTF8String]);

    if(toReturn.size() && toReturn[toReturn.size()-1]!=__T('/'))
        toReturn+=__T("/");

    return toReturn;
}

bool deleteTemporaryDirectory(const Ztring& path)
{
    bool toReturn = false;

    NSURL *url = [NSURL fileURLWithPath:@(path.To_UTF8().c_str())];

    if(!url)
        return toReturn;

    toReturn = [[NSFileManager defaultManager] removeItemAtURL:url error:nil];

    return toReturn;
}

void clearNSMenu(void* menu)
{
    [(NSMenu*)menu removeAllItems];
}
