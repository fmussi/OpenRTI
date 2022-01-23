// rti1516eLv.cpp : shared library to call OpenRTI function from LV.
// [FM-03/01/2022]: first release
#include <iostream>
#include <assert.h>
#include <exception>
#include <typeindex>

// shared lib header
#include "rti1516eLv.h"

// namespaces
using namespace std;
using namespace rti1516e;

namespace rti1516eLv
{   
    // temp LVuser event store
    LVUserEventRef tempUserEvStore;
    
    wstring chararray2wstring(const char charArray[])
    {
        wstring wStringOut;
        // allocate strings
        string sStringTemp(charArray);
        wStringOut.assign(sStringTemp.begin(),sStringTemp.end());

        return wStringOut;
    }

    LStrHandle wstring2LvString(wstring const & theObjectInstanceName)
    {
            const wchar_t *input = theObjectInstanceName.c_str();
            size_t size = (wcslen(input)+1)*sizeof(wchar_t);
            char *buffer = new char[size];
            wcstombs(buffer,input,size);
            // char buffer[] = "Ciao Pirla";
            const int headerSize = 4;
            size = strlen(buffer);

	        // Allocate memory for the LV string
            LStrHandle lSh = (LStrHandle)DSNewHandle(headerSize+size);

            // Write the LV string header
            (*lSh)->cnt = size;
            memcpy( (*lSh)->str,buffer,size);
            

        return lSh;
    }

   // test export functions

    EXTERNC int testFunc()
    {
        return 12345;
    }

    // using struct to test complex data sharing

    EXTERNC MgErr testFireEvent(testEventData *value)
    {
        MgErr status;
        status = PostLVUserEvent(tempUserEvStore,value);
        return status;
    }

    EXTERNC int regObjInstNameResSuccEvent(LVUserEventRef *eventRef)
    {
        tempUserEvStore = *eventRef;
        return 0;
    }

    // helpers - TODO: move to new cpp file

    EXTERNC  int attrHandleValueMapCreate(AttributeHandleValueMap **attrHandleValueMap)
    {
        *attrHandleValueMap = new AttributeHandleValueMap();
        return 0;
    }

    EXTERNC int attrHandleValueMapAddElementString(
        AttributeHandleValueMap * attrHandleValueMap,
        AttributeHandle *attributeHandle,
        const char sElem[])
    {
        wstring wElem = chararray2wstring(sElem);
        HLAunicodeString uElem(wElem);
        (*attrHandleValueMap)[(*attributeHandle)] = uElem.encode();
        
    }

    EXTERNC int attrHandleValueMapGetElementString(
        AttributeHandleValueMap * attrHandleValueMap,
        AttributeHandle *attributeHandle,
        LStrHandle &lSh)
    {
        HLAunicodeString uElem;
        uElem.decode((*attrHandleValueMap)[(*attributeHandle)]);
        lSh = wstring2LvString(wstring(uElem));
        return 0;
    }   

    EXTERNC int attrHandleValueMapNumElements(
        AttributeHandleValueMap * attrHandleValueMap)
    {
        size_t sizeOut = (*attrHandleValueMap).size();
        return (int)sizeOut;
    }

    EXTERNC int attrHandleValueMapDestroy(
        AttributeHandleValueMap * attrHandleValueMap)
    {
        delete attrHandleValueMap;

        return 0;
    }

    EXTERNC int parHandleValueMapCreate(ParameterHandleValueMap **parHandleValueMap)
    {
        *parHandleValueMap = new ParameterHandleValueMap();
        return 0;
    }

    EXTERNC int parHandleValueMapAddElementString(
        ParameterHandleValueMap * parHandleValueMap,
        ParameterHandle * parameterHandle,
        const char sElem[])
    {
        wstring wElem = chararray2wstring(sElem);
        HLAunicodeString uElem(wElem);
        (*parHandleValueMap)[(*parameterHandle)] = uElem.encode();
    }

    EXTERNC int parHandleValueMapGetElementString(
        ParameterHandleValueMap const * parHandleValueMap,
        ParameterHandle *parameterHandle,
        LStrHandle &lSh)
    {
        HLAunicodeString uElem;
        VariableLengthData item;
        try {
            item = (*parHandleValueMap).at(*parameterHandle);
            // item = (*parHandleValueMap)[(*parameterHandle)];
            uElem.decode(item);
            lSh = wstring2LvString(wstring(uElem));
        } catch (out_of_range &oor) {
            return -12345; // test oor error
        }
        //uElem.decode((parHandleValueMap)[(*parameterHandle)]);
        //
        return 23;
    }   

    EXTERNC int parHandleValueMapGetElementValue_vld(
        ParameterHandleValueMap const * parHandleValueMap,
        ParameterHandle *parameterHandle,
        VariableLengthData &vld)
    {
        size_t itemSize;
        try {
            HLAunicodeString uElem;
            VariableLengthData const & item = (*parHandleValueMap).at((*parameterHandle));
            itemSize = item.size();
            vld = item;            
        } catch (out_of_range &oor) {
            return 12345;
        }
       
        //uElem.decode((parHandleValueMap)[(*parameterHandle)]);
        return (int)(sizeof(itemSize));
        //return (int)((*parHandleValueMap).count((*parameterHandle)));
    }  

    EXTERNC int parHandleValueMapNumElements(
        ParameterHandleValueMap * parHandleValueMap)
    {
        size_t sizeOut = (*parHandleValueMap).size();
        return (int)sizeOut;
    }

    EXTERNC int parHandleValueMapDestroy(
        ParameterHandleValueMap * parHandleValueMap)
    {
        //delete parHandleValueMap;
        DSDisposePtr(parHandleValueMap);
        
        return 0;
    } 

    EXTERNC int attrHandleSetCreate(AttributeHandleSet **attrHandleSet)
    {
        *attrHandleSet = new AttributeHandleSet();
        return 0;
    }

    EXTERNC int attrHandleSetInsert(
        AttributeHandleSet *attrHandleSet,
        AttributeHandle *attrHandle
    )
    {
        //AttributeHandle attrHandleVar;
        //attrHandleSet->insert(attrHandleVar);
        //attrHandleSet->insert(*attrHandle);
        (*attrHandleSet).insert(*attrHandle);
        return 0;
    }

    EXTERNC int attrHandleSetDestroy(
        AttributeHandleSet * attrHandleSet
    )
    {
        delete attrHandleSet;
        return 0;
    }

// Variable Data helpers
    EXTERNC int getVariableLenghtDataSize(
        VariableLengthData *dataIn)
    {
        return (*dataIn).size();
    }

    EXTERNC int destroyVariableLenghtDataRef(
        VariableLengthData *dataIn)
    {
        delete dataIn;
        return 0;
    }    

} // namespace rti1516eLv