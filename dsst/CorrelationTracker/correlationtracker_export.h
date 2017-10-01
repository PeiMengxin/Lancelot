#ifndef _CORRELATIONTRACKER_EXPORT_H__
#define _CORRELATIONTRACKER_EXPORT_H__

#ifdef WIN32

#ifdef CORRELATIONTRACKER_EXPORT
#define CORRELATIONTRACKER_API __declspec(dllexport)
#else
#define CORRELATIONTRACKER_API __declspec(dllimport)
#endif // TRACKERAPI_EXPORT

#else
#define CORRELATIONTRACKER_API
#endif // WIN32

#endif // _CORRELATIONTRACKER_EXPORT_H__
