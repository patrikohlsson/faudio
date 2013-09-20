
/*
    FAE
    Copyright (c) DoReMIR Music Research 2012-2013
    All rights reserved.
 */

#include <fae/string.h>
#include <fae/util.h>
#include <fae/dynamic.h>
#include <iconv.h>

#include <CoreFoundation/CoreFoundation.h>

/** Convert a string to a the string representation used by the platform.
    
    * On Mac OS X and iOS, `CFStringRef` is used.
 */
void *fae_string_to_native(fae_string_t str)
{
    char *cstr;
    CFStringRef cfstr;

    cstr    = fae_string_to_utf8(str);
    cfstr   = CFStringCreateWithCString(kCFAllocatorDefault, cstr, kCFStringEncodingUTF8);

    free(cstr);
    return (void *) cfstr;
}

/** Convert a value of the string representation used by the platform to a string.

    * On Mac OS X and iOS, `CFStringRef` is used.
 */
fae_string_t fae_string_from_native(void *cfstr)
{
    CFIndex size;
    char *cstr;
    string_t str;

    if ((cstr = (char *) CFStringGetCStringPtr(cfstr, kCFStringEncodingUTF8))) {
        return fae_string_from_utf8(cstr);
    } else {
        size        = CFStringGetLength(cfstr);
        cstr        = malloc(size + 1);
        cstr[size]  = 0;                     // necesary ?

        CFStringGetCString(cfstr, cstr, size + 1, kCFStringEncodingUTF8);
        str = fae_string_from_utf8(cstr);

        free(cstr);
        return str;
    }
}