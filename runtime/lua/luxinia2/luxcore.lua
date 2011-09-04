local ffi = require "ffi"
require "luxinia2.luxplatform"

ffi.cdef [[
// lxc | Lux Core
typedef struct lxMemoryAllocator_s * lxMemoryAllocatorPTR ;
typedef void * ( __cdecl * lxMalloc_fn ) ( lxMemoryAllocatorPTR , size_t sz ) ;
typedef void * ( __cdecl * lxMallocAligned_fn ) ( lxMemoryAllocatorPTR , size_t , size_t align ) ;
typedef void * ( __cdecl * lxCalloc_fn ) ( lxMemoryAllocatorPTR , size_t sz , size_t num ) ;
typedef void * ( __cdecl * lxCallocAligned_fn ) ( lxMemoryAllocatorPTR , size_t sz , size_t num , size_t align ) ;
typedef void * ( __cdecl * lxRealloc_fn ) ( lxMemoryAllocatorPTR , void * , size_t sz , size_t oldsz ) ;
typedef void * ( __cdecl * lxReallocAligned_fn ) ( lxMemoryAllocatorPTR , void * , size_t sz , size_t oldsz , size_t align ) ;
typedef void ( __cdecl * lxFree_fn ) ( lxMemoryAllocatorPTR , void * , size_t oldsz ) ;
typedef void ( __cdecl * lxFreeAligned_fn ) ( lxMemoryAllocatorPTR , void * , size_t oldsz ) ;
typedef void * ( __cdecl * lxMallocStats_fn ) ( lxMemoryAllocatorPTR , size_t sz , const char * file , int line ) ;
typedef void * ( __cdecl * lxMallocAlignedStats_fn ) ( lxMemoryAllocatorPTR , size_t , size_t align , const char * file , int line ) ;
typedef void * ( __cdecl * lxCallocStats_fn ) ( lxMemoryAllocatorPTR , size_t sz , size_t num , const char * file , int line ) ;
typedef void * ( __cdecl * lxCallocAlignedStats_fn ) ( lxMemoryAllocatorPTR , size_t sz , size_t num , size_t align , const char * file , int line ) ;
typedef void * ( __cdecl * lxReallocStats_fn ) ( lxMemoryAllocatorPTR , void * , size_t sz , size_t oldsz , const char * file , int line ) ;
typedef void * ( __cdecl * lxReallocAlignedStats_fn ) ( lxMemoryAllocatorPTR , void * , size_t sz , size_t oldsz , size_t align , const char * file , int line ) ;
typedef void ( __cdecl * lxFreeStats_fn ) ( lxMemoryAllocatorPTR , void * , size_t oldsz , const char * file , int line ) ;
typedef void ( __cdecl * lxFreeAlignedStats_fn ) ( lxMemoryAllocatorPTR , void * , size_t oldsz , const char * file , int line ) ;
typedef struct lxMemoryTracker_s
{
    lxMallocStats_fn _malloc ;
    lxCallocStats_fn _calloc ;
    lxReallocStats_fn _realloc ;
    lxFreeStats_fn _free ;
    lxMallocAlignedStats_fn _mallocAligned ;
    lxCallocAlignedStats_fn _callocAligned ;
    lxReallocAlignedStats_fn _reallocAligned ;
    lxFreeAlignedStats_fn _freeAligned ;
}
lxMemoryTracker_t ;
typedef struct lxMemoryAllocator_s
{
    lxMalloc_fn _malloc ;
    lxCalloc_fn _calloc ;
    lxRealloc_fn _realloc ;
    lxFree_fn _free ;
    lxMallocAligned_fn _mallocAligned ;
    lxCallocAligned_fn _callocAligned ;
    lxReallocAligned_fn _reallocAligned ;
    lxFreeAligned_fn _freeAligned ;
    lxMemoryTracker_t * tracker ;
}
lxMemoryAllocator_t ;
typedef struct lxContVector_s
{
    ushort elemsize ;
    ushort alignsize ;
    byte * beg ;
    byte * end ;
    byte * eos ;
    lxMemoryAllocatorPTR allocator ;
}
lxContVector_t ;
typedef struct lxContVector_s * lxContVectorPTR ;
typedef const struct lxContVector_s * lxContVectorCPTR ;
void lxContVector_init ( lxContVectorPTR cv , lxMemoryAllocatorPTR allocator , uint elemsize ) ;
void lxContVector_initAligned ( lxContVectorPTR cv , lxMemoryAllocatorPTR allocator , uint elemsize , uint alignsize ) ;
lxContVector_t lxContVector_new ( uint elemsize , lxMemoryAllocatorPTR allocator ) ;
lxContVector_t lxContVector_copy ( lxContVectorPTR cv ) ;
void * lxContVector_front ( lxContVectorPTR cv ) ;
void * lxContVector_back ( lxContVectorPTR cv ) ;
void * lxContVector_at ( lxContVectorPTR cv , uint idx ) ;
int lxContVector_find ( lxContVectorCPTR cv , const void * val ) ;
uint lxContVector_elemsize ( lxContVectorCPTR cv ) ;
uint lxContVector_capacity ( lxContVectorCPTR cv ) ;
uint lxContVector_size ( lxContVectorCPTR cv ) ;
uint lxContVector_memused ( lxContVectorCPTR cv ) ;
uint lxContVector_memreserved ( lxContVectorCPTR cv ) ;
booln lxContVector_isEmpty ( lxContVectorCPTR cv ) ;
void lxContVector_pushBack ( lxContVectorPTR cv , const void * data ) ;
void lxContVector_popBack ( lxContVectorPTR cv ) ;
void lxContVector_clear ( lxContVectorPTR cv ) ;
void lxContVector_makeEmpty ( lxContVectorPTR cv ) ;
void lxContVector_insert ( lxContVectorPTR cv , uint idx , const void * data ) ;
void lxContVector_insertRepeat ( lxContVectorPTR cv , uint idx , const void * data , uint cnt ) ;
void lxContVector_insertMany ( lxContVectorPTR cv , uint idx , const void * data , uint cnt ) ;
void lxContVector_remove ( lxContVectorPTR cv , uint idx ) ;
void lxContVector_removeMany ( lxContVectorPTR cv , uint idx , uint cnt ) ;
void lxContVector_removeUnsorted ( lxContVectorPTR cv , uint idx ) ;
void lxContVector_shrink ( lxContVectorPTR cv ) ;
void lxContVector_reserve ( lxContVectorPTR cv , uint cnt ) ;
void lxContVector_resize ( lxContVectorPTR cv , uint cnt , const void * fill ) ;
void lxContVector_prepGrowth ( lxContVectorPTR cv , uint delta ) ;
typedef struct lxBitArray_s
{
    uint32 * bits ;
    uint32 num32 ;
}
lxBitArray_t ;
void lxBitArray_set ( lxBitArray_t * ba , uint bit , booln state ) ;
booln lxBitArray_get ( const lxBitArray_t * ba , uint bit ) ;
void lxBitArray_clear ( lxBitArray_t * ba ) ;
void lxBitArray_all ( lxBitArray_t * ba ) ;
booln lxBitArray_any ( const lxBitArray_t * ba ) ;
int32 lxBitArray_getFirst ( const lxBitArray_t * ba , booln state ) ;
typedef struct lxContHash_s * lxContHashPTR ;
typedef const struct lxContHash_s * lxContHashCPTR ;
lxContHashPTR lxContHash_new ( lxMemoryAllocatorPTR allocator , uint numBins , uint valueSize ) ;
void lxContHash_delete ( lxContHashPTR cv ) ;
booln lxContHash_set ( lxContHashPTR cv , uint32 key , const void * val ) ;
booln lxContHash_get ( lxContHashCPTR cv , uint32 key , void * * outval ) ;
booln lxContHash_remove ( lxContHashPTR cv , uint32 key ) ;
booln lxContHash_isEmpty ( lxContHashCPTR cv ) ;
uint lxContHash_getCount ( lxContHashCPTR cv ) ;
void * lxContHash_getNth ( lxContHashCPTR cv , uint n ) ;
uint32 lxContHash_getFirstKey ( lxContHashCPTR cv ) ;
uint32 lxContHash_getNextKey ( lxContHashCPTR cv , uint32 key ) ;
typedef void * ( lxContHash_Iterator_fn ) ( void * fnData , uint32 key , void * val ) ;
void lxContHash_iterate ( lxContHashPTR cv , lxContHash_Iterator_fn * itfunc , void * fnData ) ;
uint lxContHash_shrink ( lxContHashPTR cv ) ;
float lxContHash_memRatio ( lxContHashPTR cv ) ;
typedef struct lxContPtrHash_s * lxContPtrHashPTR ;
typedef const struct lxContPtrHash_s * lxContPtrHashCPTR ;
lxContPtrHashPTR lxContPtrHash_new ( lxMemoryAllocatorPTR allocator , uint numBins , uint valueSize ) ;
void lxContPtrHash_delete ( lxContPtrHashPTR cv ) ;
booln lxContPtrHash_set ( lxContPtrHashPTR cv , void * key , const void * val ) ;
booln lxContPtrHash_get ( lxContPtrHashCPTR cv , void * key , void * * outval ) ;
booln lxContPtrHash_remove ( lxContPtrHashPTR cv , void * key ) ;
booln lxContPtrHash_isEmpty ( lxContPtrHashCPTR cv ) ;
typedef void * ( lxContPtrHash_Iterator_fn ) ( void * fnData , void * key , void * val ) ;
void lxContPtrHash_iterate ( lxContPtrHashPTR cv , lxContPtrHash_Iterator_fn * itfunc , void * fnData ) ;
uint lxContPtrHash_shrink ( lxContPtrHashPTR cv ) ;
void lxContPtrHash_clear ( lxContPtrHashPTR cv ) ;
typedef struct lxContMap_s * lxContMapPTR ;
typedef const struct lxContMap_s * lxContMapCPTR ;
typedef struct lxContMap_s
{
    lxContVector_t keys ;
    lxContVector_t values ;
}
lxContMap_t ;
void lxContMap_init ( lxContMapPTR cv , lxMemoryAllocatorPTR allocator , size_t keysize , size_t valsize ) ;
booln lxContMap_set ( lxContMapPTR cv , void * key , void * val ) ;
booln lxContMap_get ( lxContMapCPTR cv , void * key , void * * outval ) ;
booln lxContMap_remove ( lxContMapPTR cv , void * key ) ;
void lxContMap_clear ( lxContMapPTR cv ) ;
typedef struct lxStrDict_s * lxStrDictPTR ;
typedef const struct lxStrDict_s * lxStrDictCPTR ;
typedef const char * lxStrDictName ;
typedef uint32 lxStrDictKey ;
lxStrDictPTR lxStrDict_new ( lxMemoryAllocatorPTR allocator , uint numBins ) ;
void lxStrDict_delete ( lxStrDictPTR dc ) ;
lxStrDictKey lxStrDict_add ( lxStrDictPTR dc , const char * str ) ;
lxStrDictName lxStrDict_getFromKey ( lxStrDictCPTR dc , lxStrDictKey key ) ;
lxStrDictKey lxStrDict_getKey ( lxStrDictCPTR dc , const char * str ) ;
int lxStrDict_rem ( lxStrDictPTR dc , lxStrDictName str ) ;
int lxStrDict_remKey ( lxStrDictPTR dc , lxStrDictKey key ) ;
typedef struct lxStrMap_s * lxStrMapPTR ;
typedef void * ( lxStrMap_Iterator_fn ) ( void * fnData , const char * , void * browseData ) ;
lxStrMapPTR lxStrMap_new ( lxMemoryAllocatorPTR allocator , uint numBins , uint valueSize , lxStrDictPTR dc ) ;
void lxStrMap_delete ( lxStrMapPTR self , void ( * fnFree ) ( void * ) ) ;
booln lxStrMap_set ( lxStrMapPTR self , const char * key , void * value ) ;
void * lxStrMap_get ( lxStrMapPTR self , const char * key ) ;
void * lxStrMap_getNth ( lxStrMapPTR self , uint n ) ;
uint lxStrMap_getCount ( lxStrMapPTR self ) ;
booln lxStrMap_remove ( lxStrMapPTR self , const char * key ) ;
booln lxStrMap_isSet ( lxStrMapPTR self , const char * key ) ;
void lxStrMap_iterate ( lxStrMapPTR self , lxStrMap_Iterator_fn * itfunc , void * fnData ) ;
typedef struct lxMemoryGenericInfo_s
{
    ptrdiff_t allocs ;
    ptrdiff_t mem ;
}
lxMemoryGenericInfo_t ;
typedef struct lxMemoryGenericDescr_s
{
    void * sys ;
}
lxMemoryGenericDescr_t ;
typedef struct lxMemoryGeneric_s * lxMemoryGenericPTR ;
lxMemoryGenericDescr_t lxMemoryGenericDescr_default ( ) ;
lxMemoryGenericPTR lxMemoryGeneric_new ( lxMemoryGenericDescr_t descr ) ;
void lxMemoryGeneric_delete ( lxMemoryGenericPTR gen ) ;
lxMemoryGenericInfo_t lxMemoryGeneric_getInfo ( lxMemoryGenericPTR gen ) ;
booln lxMemoryGeneric_deinitStats ( lxMemoryGenericPTR gen ) ;
void lxMemoryGeneric_dumpStats ( lxMemoryGenericPTR gen , const char * fname ) ;
lxMemoryAllocatorPTR lxMemoryGeneric_allocator ( lxMemoryGenericPTR gen ) ;
typedef struct lxMemoryList_s * lxMemoryListPTR ;
lxMemoryListPTR lxMemoryList_newBits ( lxMemoryAllocatorPTR allocator , uint sizeminbit , uint sizemaxbit , uint startpagesize , uint alignSize ) ;
lxMemoryListPTR lxMemoryList_new ( lxMemoryAllocatorPTR allocator , uint numSizes , uint * sizes , uint startpagesize , uint alignSize ) ;
void lxMemoryList_delete ( lxMemoryListPTR ) ;
void lxMemoryList_stats ( lxMemoryListPTR , uint * allocTotal , uint * allocPaged , float * pageratio ) ;
void lxMemoryList_freeItem ( lxMemoryListPTR mc , void * ptr , uint oldsize ) ;
void * lxMemoryList_allocItem ( lxMemoryListPTR mc , uint size ) ;
void * lxMemoryList_zallocItem ( lxMemoryListPTR mc , uint size ) ;
void * lxMemoryList_reallocItem ( lxMemoryListPTR mc , void * ptr , uint oldsize , uint newsize ) ;
typedef struct lxMemoryPool_s * lxMemoryPoolPTR ;
typedef const struct lxMemoryPool_s * lxMemoryPoolCPTR ;
lxMemoryPoolPTR lxMemoryPool_new ( lxMemoryAllocatorPTR allocator , uint varSize , uint pageValues , uint alignSize , booln allowMultiPages ) ;
void lxMemoryPool_delete ( lxMemoryPoolPTR mem ) ;
void * lxMemoryPool_allocItem ( lxMemoryPoolPTR mem ) ;
void lxMemoryPool_freeItem ( lxMemoryPoolPTR mem , void * ptr ) ;
uint lxMemoryPool_shrink ( lxMemoryPoolPTR mem ) ;
uint lxMemoryPool_memUsed ( lxMemoryPoolPTR mem ) ;
uint lxMemoryPool_memAllocated ( lxMemoryPoolPTR mem ) ;
float lxMemoryPool_memRatio ( lxMemoryPoolPTR mem ) ;
uint lxMemoryPool_varSize ( lxMemoryPoolCPTR mem ) ;
uint lxMemoryPool_alignSize ( lxMemoryPoolCPTR mem ) ;
typedef struct lxMemoryStack_s * lxMemoryStackPTR ;
lxMemoryStackPTR lxMemoryStack_new ( lxMemoryAllocatorPTR allocator , const char * name , size_t totalbytes ) ;
void lxMemoryStack_delete ( lxMemoryStackPTR mem ) ;
void * lxMemoryStack_zalloc ( lxMemoryStackPTR mem , size_t size ) ;
void * lxMemoryStack_zallocAligned ( lxMemoryStackPTR mem , size_t size , size_t align ) ;
void * lxMemoryStack_alloc ( lxMemoryStackPTR mem , size_t size ) ;
void * lxMemoryStack_current ( lxMemoryStackPTR mem ) ;
booln lxMemoryStack_initMin ( lxMemoryStackPTR mem , size_t totalbytes ) ;
int lxMemoryStack_popResized ( lxMemoryStackPTR mem ) ;
void lxMemoryStack_clear ( lxMemoryStackPTR mem ) ;
void lxMemoryStack_deinit ( lxMemoryStackPTR mem ) ;
booln lxMemoryStack_checkPtr ( lxMemoryStackPTR mem , void * ptr ) ;
void lxMemoryStack_setBegin ( lxMemoryStackPTR mem , void * ptr ) ;
size_t lxMemoryStack_bytesLeft ( lxMemoryStackPTR mem ) ;
size_t lxMemoryStack_bytesUsed ( lxMemoryStackPTR mem ) ;
size_t lxMemoryStack_bytesTotal ( lxMemoryStackPTR mem ) ;
uint32 * lxSortRadixArrayInt ( const uint32 * data , uint size , booln sign , uint32 * indices1 , uint32 * indices2 ) ;
uint32 * lxSortRadixArrayFloat ( const float * data , uint size , uint32 * indices1 , uint32 * indices2 ) ;
typedef uint32 lxHandleID ;
typedef struct lxHandleSys_s * lxHandleSysPTR ;
void lxHandleSys_init ( lxHandleSysPTR sys ) ;
lxHandleID lxHandleSys_add ( lxHandleSysPTR sys , uint32 type , void * data ) ;
booln lxHandleSys_rem ( lxHandleSysPTR sys , lxHandleID id ) ;
booln lxHandleSys_replace ( lxHandleSysPTR sys , lxHandleID id , void * data ) ;
booln lxHandleSys_getSafe ( lxHandleSysPTR sys , lxHandleID id , void * * outval ) ;
void * lxHandleSys_getPtr ( lxHandleSysPTR sys , lxHandleID id ) ;
int lxHandleSys_checkIdx ( lxHandleSysPTR sys , lxHandleID id ) ;
enum
{
    LUX_HANDLE_TYPE = 6 , LUX_HANDLE_IDX = 12 , LUX_HANDLE_COUNTER = 32 - LUX_HANDLE_IDX - LUX_HANDLE_TYPE , LUX_HANDLESYS_MAX = ( 1 << LUX_HANDLE_IDX ) , }
;
typedef struct lxHandle_s
{
    uint type : LUX_HANDLE_TYPE ;
    uint counter : LUX_HANDLE_COUNTER ;
    uint idx : LUX_HANDLE_IDX ;
}
lxHandle_t ;
typedef struct lxHandleEntry_s
{
    lxHandle_t handle ;
    union
    {
        void * data ;
        uint32 nextUnused ;
    }
    ;
}
lxHandleEntry_t ;
typedef struct lxHandleSys_s
{
    uint32 numUsed ;
    uint32 firstUnused ;
    lxHandleEntry_t entries [ LUX_HANDLESYS_MAX ] ;
}
lxHandleSys_t ;
typedef struct lxObjRefSys_s * lxObjRefSysPTR ;
typedef struct lxObjRef_s * lxObjRefPTR ;
typedef void ( lxObjRefDelete_fn ) ( lxObjRefPTR r ) ;
typedef booln ( lxObjRefCheckDelete_fn ) ( lxObjRefPTR r ) ;
enum lxObjRefType_e
{
    LUX_OBJREF_TYPE_DELETED = - 1 , LUX_OBJREF_TYPE_FREEALLOC = 0 , LUX_OBJREF_TYPE_USERSTART , }
;
typedef int32 lxObjRefType_t ;
typedef struct lxObjId_s
{
    void * ptr ;
    lxObjRefType_t type ;
}
lxObjId_t ;
typedef struct lxObjTypeInfo_s
{
    lxObjRefDelete_fn * fnDelete ;
    const char * name ;
}
lxObjTypeInfo_t ;
lxObjRefSysPTR lxObjRefSys_new ( lxMemoryAllocatorPTR allocator ) ;
void lxObjRefSys_delete ( lxObjRefSysPTR sys ) ;
void lxObjRefSys_init ( lxObjRefSysPTR sys , lxMemoryAllocatorPTR allocator ) ;
void lxObjRefSys_deinit ( lxObjRefSysPTR sys , lxObjRefCheckDelete_fn * func ) ;
int32 lxObjRefSys_refCount ( lxObjRefSysPTR sys ) ;
void lxObjRefSys_pushNoDelete ( lxObjRefSysPTR sys ) ;
void lxObjRefSys_popNoDelete ( lxObjRefSysPTR sys ) ;
lxObjTypeInfo_t lxObjTypeInfo_new ( lxObjRefDelete_fn * fnDelete , const char * name ) ;
void lxObjRefSys_register ( lxObjRefSysPTR sys , lxObjRefType_t type , lxObjTypeInfo_t info ) ;
lxObjRefPTR lxObjRefSys_newRef ( lxObjRefSysPTR sys , lxObjRefType_t type , void * ptr ) ;
booln lxObjRefSys_invalidateRef ( lxObjRefSysPTR sys , lxObjRefPTR cref , booln calldestructor ) ;
lxObjId_t * lxObjRef_getId ( lxObjRefPTR cref ) ;
void * lxObjRef_getPtr ( lxObjRefPTR cref ) ;
booln lxObjRef_getSafe ( lxObjRefPTR cref , void * * ptr ) ;
void lxObjRef_addWeak ( lxObjRefPTR cref ) ;
booln lxObjRef_addUser ( lxObjRefPTR cref ) ;
void lxObjRef_releaseWeak ( lxObjRefPTR cref ) ;
booln lxObjRef_releaseUser ( lxObjRefPTR cref ) ;
booln lxObjRef_makeVolatile ( lxObjRefPTR cref ) ;
typedef struct lxObjRef_s
{
    lxObjId_t id ;
    int32 usecounter ;
    int32 weakcounter ;
    lxObjRefSysPTR sys ;
}
lxObjRef_t ;
void lxObjRefSys_deleteRef ( lxObjRefSysPTR sys , lxObjRefPTR cref ) ;
void lxObjRefSys_deleteAlloc ( lxObjRefSysPTR sys , lxObjRefPTR cref ) ;
]]

return ffi.load("luxbackend")
