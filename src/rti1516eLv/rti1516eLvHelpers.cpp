// rti1516eLv.cpp : shared library to call OpenRTI function from LV.
// [FM-03/01/2022]: first release
#include <iostream>
#include <assert.h>
#include <exception>
#include <typeindex>
#include <map>

// shared lib header
#include "rti1516eLv.h"

// namespaces
using namespace std;
using namespace rti1516e;

namespace rti1516eLv
{   
    /* data types */
    LVUserEventRef tempUserEvStore;

    /* Test or debug functions */

    EXTERNC int testFunc()
    {
        return 12345;
    }

    // using struct to test complex data sharing

    EXTERNC MgErr testFireEvent(testEventData* value)
    {
        MgErr status;
        status = PostLVUserEvent(tempUserEvStore, value);
        return status;
    }

    EXTERNC int regObjInstNameResSuccEvent(LVUserEventRef* eventRef)
    {
        tempUserEvStore = *eventRef;
        return 0;
    }

    /* Data manipulation */
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

    LStrHandle cArray2LvString(const char* charArray,size_t size)
    {

            const int headerSize = 4;
	        // Allocate memory for the LV string
            LStrHandle lSh = (LStrHandle)DSNewHandle(headerSize+size);
            // Write the LV string header
            (*lSh)->cnt = size;
            memcpy( (*lSh)->str,charArray,size);
            
        return lSh;
    }
    /* Attribute Handle Value Map - Helpers*/
    EXTERNC  int attrHandleValueMapCreate(AttributeHandleValueMap **attrHandleValueMap)
    {
        *attrHandleValueMap = new AttributeHandleValueMap();
        return 0;
    }

    EXTERNC int attrHandleValueMapDestroy(
        AttributeHandleValueMap * attrHandleValueMap)
    {
        delete attrHandleValueMap;

        return 0;
    }

    EXTERNC int attrHandleValueMapGetHandleByIndex(
        AttributeHandleValueMap* attrHandleValueMap,
        const int index,
        AttributeHandle* attributeHandleOut)
    {
        if (index >= (*attrHandleValueMap).size())
            return LV_ERROR_MAPOUTOFRANGE; // index out of range
        else
        {
            auto it = (*attrHandleValueMap).begin();
            advance(it, index);
            (*attributeHandleOut) = it->first;
            return 0;
        }

    }

    EXTERNC int attrHandleValueMapSetHLAunicodeString(
        AttributeHandleValueMap * attrHandleValueMap,
        AttributeHandle *attributeHandle,
        const char sElem[])
    {
        wstring wElem = chararray2wstring(sElem);
        HLAunicodeString uElem(wElem);
        (*attrHandleValueMap)[(*attributeHandle)] = uElem.encode();
        return 0;
    }

    EXTERNC int attrHandleValueMapGetHLAunicodeString(
        AttributeHandleValueMap * attrHandleValueMap,
        AttributeHandle *attributeHandle,
        LStrHandle &lSh)
    {
        HLAunicodeString uElem;
        try {
            uElem.decode((*attrHandleValueMap).at(*attributeHandle));
            lSh = wstring2LvString(wstring(uElem));
        } catch (out_of_range &oor) {
            return LV_ERROR_MAPOUTOFRANGE; // test oor error
        }

        //return (*lSh)->cnt;
        return 0;				 
    }    

    EXTERNC int attrHandleValueMapSetHLAASCIIString(
        AttributeHandleValueMap * attrHandleValueMap,
        AttributeHandle *attributeHandle,
        const char sElem[])
    {
        //wstring wElem = chararray2wstring(sElem);
        HLAASCIIstring aElem(sElem);
        //HLAunicodeString uElem(wElem);
        (*attrHandleValueMap)[(*attributeHandle)] = aElem.encode();
        return 0;
    }

    EXTERNC int attrHandleValueMapGetHLAASCIIString(
        AttributeHandleValueMap * attrHandleValueMap,
        AttributeHandle *attributeHandle,
        LStrHandle &lSh)
    {
        size_t numOfElems;
        HLAASCIIstring sElem;
        Octet* data;
        try {
            numOfElems = ((*attrHandleValueMap).at(*attributeHandle)).size();
            sElem.decode((*attrHandleValueMap).at(*attributeHandle));
            //data = (Octet*)(((*attrHandleValueMap).at(*attributeHandle)).data());
            //lSh = cArray2LvString(data, numOfElems);
            lSh = cArray2LvString(sElem.get().c_str(), numOfElems);
        }
        catch (out_of_range& oor) {
            return LV_ERROR_MAPOUTOFRANGE; // test oor error
        }

        //return (*lSh)->cnt;
        return 0;
    }

    EXTERNC int attrHandleValueMapSetRaw(
        AttributeHandleValueMap* attrHandleValueMap,
        AttributeHandle* attributeHandle,
        Octet* data, size_t size)
    {
        VariableLengthData vElem(data, size);
        (*attrHandleValueMap)[(*attributeHandle)] = vElem;
        return 0;
    }

    EXTERNC size_t attrHandleValueMapGetRawSize(
        AttributeHandleValueMap* attrHandleValueMap,
        AttributeHandle* attributeHandle)
    {
        size_t numOfElems;
        try {
            numOfElems = ((*attrHandleValueMap).at(*attributeHandle)).size();
        }
        catch (out_of_range& oor) {
            return LV_ERROR_MAPOUTOFRANGE; // test oor error
        }

        return numOfElems;
    }

    EXTERNC int attrHandleValueMapGetRaw(
        AttributeHandleValueMap* attrHandleValueMap,
        AttributeHandle* attributeHandle,
        Octet* data)
    {
        size_t numOfElems;
        Octet* temp;
        try {
            numOfElems = ((*attrHandleValueMap).at(*attributeHandle)).size();
            temp = (Octet*)(((*attrHandleValueMap).at(*attributeHandle)).data());
            memcpy(data, (Octet*)(((*attrHandleValueMap).at(*attributeHandle)).data()), numOfElems * sizeof(Octet));
        }
        catch (out_of_range& oor) {
            return LV_ERROR_MAPOUTOFRANGE; // test oor error
        }

        return 0;
    }
    EXTERNC int attrHandleValueMapSetElementInt32(
        AttributeHandleValueMap* attrHandleValueMap,
        AttributeHandle* attributeHandle,
        const int iElem)
    {
        (*attrHandleValueMap)[(*attributeHandle)] = ((HLAinteger32BE)iElem).encode();
        return 0;
    }

    EXTERNC int attrHandleValueMapGetElementInt32(
        AttributeHandleValueMap* attrHandleValueMap,
        AttributeHandle* attributeHandle,
        int & iElem)
    {
        HLAinteger32BE hiElem;
        try {
            hiElem.decode((*attrHandleValueMap).at(*attributeHandle));
            iElem = hiElem;
        }
        catch (out_of_range& oor) {
            return LV_ERROR_MAPOUTOFRANGE; // test oor error
        }
        return 0;
    }

    EXTERNC DLLEXPORT int attrHandleValueMapSetElementDouble(
        AttributeHandleValueMap* attrHandleValueMap,
        AttributeHandle* attributeHandle,
        const double dElem)
    {
        (*attrHandleValueMap)[(*attributeHandle)] = ((HLAfloat64BE)dElem).encode();
        return 0;
    }

    EXTERNC int attrHandleValueMapGetElementDouble(
        AttributeHandleValueMap* attrHandleValueMap,
        AttributeHandle* attributeHandle,
        double& dElem)
    {
        HLAfloat64BE hiElem;
        try {
            hiElem.decode((*attrHandleValueMap).at(*attributeHandle));
            dElem = hiElem;
        }
        catch (out_of_range& oor) {
            return LV_ERROR_MAPOUTOFRANGE; // test oor error
        }
        return 0;
    }

    EXTERNC int attrHandleValueMapNumElements(
        AttributeHandleValueMap * attrHandleValueMap)
    {
        size_t sizeOut = (*attrHandleValueMap).size();
        return (int)sizeOut;
    }

    EXTERNC int attrHandleValueMapGetElement_Coords(
        AttributeHandleValueMap* attrHandleValueMap,
        AttributeHandle* attributeHandle,
        CoordsPtr coords)
    {
        // Specific to CoordinateFixed Record - TODO move to class
        //size_t sizeVld, sizeRec;
        int i;
        HLAfloat64BE hfElem;
        HLAfixedRecord hfr;

        // Declare elements
        HLAfloat64BE ILat = HLAfloat64BE();
        HLAfloat64BE ILong = HLAfloat64BE();
        HLAfloat64BE IAlt = HLAfloat64BE();
        HLAfloat64BE IPitch = HLAfloat64BE();
        HLAfloat64BE IRoll = HLAfloat64BE();
        HLAfloat64BE IHeading = HLAfloat64BE();

        //Update Elements
        hfr.appendElement(ILat);
        hfr.appendElement(ILong);
        hfr.appendElement(IAlt);
        hfr.appendElement(IPitch);
        hfr.appendElement(IRoll);
        hfr.appendElement(IHeading);

        try {
            hfr.decode((*attrHandleValueMap).at(*attributeHandle));
            for (i = 0; i < hfr.size(); i++)
            {
                hfElem.decode(hfr.get(i).encode());
                switch (i)
                {
                    case 0: // Lat
                        coords->Lat = hfElem.get();
                        break;
                    case 1: // Long
                        coords->Lon = hfElem.get();
                        break; 
                    case 2: // Alt
                        coords->Alt = hfElem.get();
                        break;
                    case 3: // Pitch
                        coords->Pitch = hfElem.get();
                        break;
                    case 4: // Roll
                        coords->Roll = hfElem.get();
                        break;
                    case 5: // Heading
                        coords->Heading = hfElem.get();
                        break; 
                }
            }
        }
        catch (out_of_range& oor) {
            return LV_ERROR_MAPOUTOFRANGE; // test oor error
        }
        return 0;
        //const Octet* dataBuffer = (Octet*)(((*attrHandleValueMap).at(*attributeHandle)).data());
        //const HLAoctet disc(1);
        //HLAvariantRecord hvr(disc);

        //AttributeHandleValueMap::const_iterator it((*attrHandleValueMap).begin());
        //advance(it, 0);
        //vld = it->second;
        //sizeVld = vld.size();
        // decode from 

        //sizeRec = hfr.size();
        //HLAfloat64BE eElem;
        //HLAfixedRecord hfr;
        //size_t size = ((*attrHandleValueMap).at(*attributeHandle)).size();
        //const Octet* dataBuffer = (Octet*)(((*attrHandleValueMap).at(*attributeHandle)).data());
        //vector<Octet> vecBuffer(dataBuffer,dataBuffer + size);
        //vecBuffer.size();
        //hfr.decodeFrom(vecBuffer, size);
        //return ((*attrHandleValueMap).at(*attributeHandle)).size();
    }

    /* Parameter Handle Value Map - Helpers*/
    EXTERNC int parHandleValueMapCreate(ParameterHandleValueMap **parHandleValueMap)
    {
        *parHandleValueMap = new ParameterHandleValueMap();
        return 0;
    }

    EXTERNC int parHandleValueMapDestroy(
        ParameterHandleValueMap * parHandleValueMap)
    {
        delete parHandleValueMap;

        return 0;
    }

    EXTERNC int parHandleValueMapGetHandleByIndex(
        ParameterHandleValueMap* parHandleValueMap,
        const int index,
        ParameterHandle* parameterHandleOut)
    {
        if (index >= (*parHandleValueMap).size())
            return LV_ERROR_MAPOUTOFRANGE; // index out of range
        else
        {
            auto it = (*parHandleValueMap).begin();
            advance(it, index);
            (*parameterHandleOut) = it->first;
            return 0;
        }

    }

    EXTERNC int parHandleValueMapSetHLAunicodeString(
        ParameterHandleValueMap * parHandleValueMap,
        ParameterHandle * parameterHandle,
        const char sElem[])
    {
        wstring wElem = chararray2wstring(sElem);
        HLAunicodeString uElem(wElem);
        (*parHandleValueMap)[(*parameterHandle)] = uElem.encode();
        return 0;
    }

    EXTERNC int parHandleValueMapGetHLAunicodeString(
        ParameterHandleValueMap const* parHandleValueMap,
        ParameterHandle* parameterHandle,
        LStrHandle& lSh)
    {
        HLAunicodeString uElem;
        //VariableLengthData item;
        try {
            uElem.decode((*parHandleValueMap).at(*parameterHandle));
            lSh = wstring2LvString(wstring(uElem));
        } catch (out_of_range &oor) {
            return LV_ERROR_MAPOUTOFRANGE; // test oor error
        }

        //return (*lSh)->cnt;
        return 0;			 														  
	}

    EXTERNC int parHandleValueMapSetHLAASCIIString(
        ParameterHandleValueMap * parHandleValueMap,
        ParameterHandle * parameterHandle,
        const char sElem[])
    {
        //wstring wElem = chararray2wstring(sElem);
        HLAASCIIstring aElem(sElem);
        //HLAunicodeString uElem(wElem);
        (*parHandleValueMap)[(*parameterHandle)] = aElem.encode();
        return 0;
    }

    EXTERNC int parHandleValueMapGetHLAASCIIString(
        ParameterHandleValueMap* parHandleValueMap,
        ParameterHandle* parameterHandle,
        LStrHandle& lSh)
    {
        size_t numOfElems;
        HLAASCIIstring sElem;
        Octet* data;
        try {
            numOfElems = ((*parHandleValueMap).at(*parameterHandle)).size();
            data = (Octet*)(((*parHandleValueMap).at(*parameterHandle)).data());
            lSh = cArray2LvString(data, numOfElems);
            //sElem.decode((*parHandleValueMap).at(*parameterHandle));
            //lSh = cArray2LvString(sElem.get().c_str(), numOfElems);

        }
        catch (out_of_range& oor) {
            return LV_ERROR_MAPOUTOFRANGE; // test oor error
        }

        //return (*lSh)->cnt;
        return 0;
    }

    EXTERNC size_t parHandleValueMapGetRawSize(
        ParameterHandleValueMap const* parHandleValueMap,
        ParameterHandle* parameterHandle)
    {
        size_t numOfElems;
        try {
            numOfElems = ((*parHandleValueMap).at(*parameterHandle)).size();
        }
        catch (out_of_range& oor) {
            return LV_ERROR_MAPOUTOFRANGE; // test oor error
        }

        return numOfElems;
    }

    EXTERNC int parHandleValueMapGetRaw(
        ParameterHandleValueMap const* parHandleValueMap,
        ParameterHandle* parameterHandle,
        Octet *data)
    {
        size_t numOfElems;
        Octet dataElem;
        try {
            numOfElems = ((*parHandleValueMap).at(*parameterHandle)).size();
            memcpy(data,(Octet*)(((*parHandleValueMap).at(*parameterHandle)).data()),numOfElems*sizeof(Octet));            
        } catch (out_of_range &oor) {
            return LV_ERROR_MAPOUTOFRANGE; // test oor error
        }

        return 0;
    }

    EXTERNC int parHandleValueMapSetElementInt32(
        ParameterHandleValueMap* parHandleValueMap,
        ParameterHandle* parameterHandle,
        const int iElem)
    {
        (*parHandleValueMap)[(*parameterHandle)] = ((HLAinteger32BE)iElem).encode();
        return 0;
    }

    EXTERNC int parHandleValueMapGetElementInt32(
        ParameterHandleValueMap* parHandleValueMap,
        ParameterHandle* parameterHandle,
        int& iElem)
    {
        HLAinteger32BE hiElem;
        try {
            hiElem.decode((*parHandleValueMap).at(*parameterHandle));
            iElem = hiElem;
        }
        catch (out_of_range& oor) {
            return LV_ERROR_MAPOUTOFRANGE; // test oor error
        }
        return 0;
    }

    EXTERNC int parHandleValueMapSetElementDouble (
        ParameterHandleValueMap* parHandleValueMap,
        ParameterHandle* parameterHandle,
        const double dElem)
    {
        (*parHandleValueMap)[(*parameterHandle)] = ((HLAfloat64BE)dElem).encode();
        return 0;
    }

    EXTERNC int parHandleValueMapGetElementDouble(
        ParameterHandleValueMap* parHandleValueMap,
        ParameterHandle* parameterHandle,
        double& dElem)
    {
        HLAfloat64BE hiElem;
        try {
            hiElem.decode((*parHandleValueMap).at(*parameterHandle));
            dElem = hiElem;
        }
        catch (out_of_range& oor) {
            return LV_ERROR_MAPOUTOFRANGE; // test oor error
        }
        return 0;
    }

    EXTERNC int parHandleValueMapNumElements(
        ParameterHandleValueMap * parHandleValueMap)
    {
        size_t sizeOut = (*parHandleValueMap).size();
        return (int)sizeOut;
    }

    /* Attribute handle set - Helpers*/
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

    EXTERNC DLLEXPORT int attrHandleSetNumElements(
        AttributeHandleSet * attrHandleSet)
    {
        size_t sizeOut = (*attrHandleSet).size();
        return sizeOut;
    }

    EXTERNC int attrHandleSetDestroy(
        AttributeHandleSet * attrHandleSet
    )
    {
        delete attrHandleSet;
        return 0;
    }  

} // namespace rti1516eLv