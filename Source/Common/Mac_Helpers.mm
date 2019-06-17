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
//---------------------------------------------------------------------------

string makeUniqueFileName()
{
    NSString *unique = [[NSProcessInfo processInfo] globallyUniqueString];
    string toReturn = string([unique UTF8String]);

    return toReturn;
}

string makeTemporaryDirectoryForFile(const char *path)
{
    string toReturn;
    NSURL *url = [NSURL fileURLWithPath:@(path)];

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

    toReturn = string([temporaryDirectoryPath UTF8String]);

    if(toReturn.size() && toReturn[toReturn.size()-1]!='/')
        toReturn+="/";

    return toReturn;
}

bool deleteTemporaryDirectory(const char *path)
{
    bool toReturn = false;

    NSURL *url = [NSURL fileURLWithPath:@(path)];

    if(!url)
        return toReturn;

    toReturn = [[NSFileManager defaultManager] removeItemAtURL:url error:nil];

    return toReturn;
}
