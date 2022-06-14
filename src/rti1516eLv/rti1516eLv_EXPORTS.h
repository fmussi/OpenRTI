#ifndef RTI1516ELV_EXPORTS_H__
#define RTI1516ELV_EXPORTS_H__

#ifdef _WIN32
    #ifdef RTI1516ELV_EXPORTS
        #define RTI1516ELV_EXPORT __declspec(dllexport)
    #else
        #define RTI1516ELV_EXPORT __declspec(dllimport)
    #endif
#else
    #define RTI1516ELV_EXPORT
#endif

#endif /* _RTI1516ELV_EXPORTS_H__ */
