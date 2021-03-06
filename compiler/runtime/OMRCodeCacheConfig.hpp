/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2000, 2016
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 *******************************************************************************/

#ifndef OMR_CODECACHECONFIG_INCL
#define OMR_CODECACHECONFIG_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */

#ifndef OMR_CODECACHECONFIG_COMPOSED
#define OMR_CODECACHECONFIG_COMPOSED
namespace OMR { class CodeCacheConfig; }
namespace OMR { typedef CodeCacheConfig CodeCacheConfigConnector; }
#endif

#include "infra/Annotations.hpp"  // for OMR_EXTENSIBLE

#include <stddef.h>  // for size_t
#include <stdint.h>  // for int32_t, uint32_t, uint8_t

class TR_OpaqueMethodBlock;
namespace TR { class CodeCacheConfig; }
namespace TR { class CodeGenerator; }

namespace OMR
{

struct CodeCacheCodeGenCallbacks
   {
   void (*codeCacheConfig) (
         int32_t codeCacheSizeInBytes,
         uint32_t *numTempTrampolines);

   void (*createHelperTrampolines) (
         uint8_t *helperBase,
         int32_t helperCount);

   void (*createMethodTrampoline) (
         void *trampoline,
         void *targetStartPC,
         TR_OpaqueMethodBlock *method);

   int  (*patchTrampoline) (
         void *method,
         void *callingPoint,
         void *currentStartPC,
         void *currentTrampoline,
         void *newStartPC,
         void *extraArg);

   void (*createCCPreLoadedCode) (
         uint8_t *CCPreLoadedCodeBase,
         uint8_t *CCPreLoadedCodeTop,
         void **CCPreLoadedCode,
         TR::CodeGenerator *cg);
   };


class OMR_EXTENSIBLE CodeCacheConfig
   {
   public:

   CodeCacheConfig() :
         _trampolineCodeSize(0),
         _CCPreLoadedCodeSize(0),
         _numOfRuntimeHelpers(0),
         _codeCacheKB(0),
         _codeCacheTotalKB(0),
         _codeCachePadKB(0),
         _codeCacheAlignment(0),
         _codeCacheHelperAlignmentBytes(32),
         _codeCacheTrampolineAlignmentBytes(8),
         _codeCacheMethodBodyAllocRetries(3),
         _codeCacheTempTrampolineSyncArraySize(256),
         _codeCacheHashEntryAllocatorSlabSize(4096),
         _largeCodePageSize(0),
         _largeCodePageFlags(0),
         _allowedToGrowCache(false),
         _needsMethodTrampolines(false),
         _trampolineSpacePercentage(0),
         _lowCodeCacheThreshold(0),
         _maxNumberOfCodeCaches(0),
         _canChangeNumCodeCaches(false),
         _verbosePerformance(false),
         _verboseCodeCache(false),
         _verboseReclamation(false),
         _doSanityChecks(false),
         _codeCacheFreeBlockRecylingEnabled(false),
         _emitElfObject(false)
      {
      #if defined(J9ZOS390)     // EBCDIC
      _warmEyeCatcher[0] = '\xD1';
      _warmEyeCatcher[1] = '\xC9';
      _warmEyeCatcher[2] = '\xE3';
      _warmEyeCatcher[3] = '\xE6';

      _coldEyeCatcher[0] = '\xD1';
      _coldEyeCatcher[1] = '\xC9';
      _coldEyeCatcher[2] = '\xE3';
      _coldEyeCatcher[3] = '\xC3';
      #else                      // ASCII
      _warmEyeCatcher[0] = 'J';
      _warmEyeCatcher[1] = 'I';
      _warmEyeCatcher[2] = 'T';
      _warmEyeCatcher[3] = 'W';

      _coldEyeCatcher[0] = 'J';
      _coldEyeCatcher[1] = 'I';
      _coldEyeCatcher[2] = 'T';
      _coldEyeCatcher[3] = 'C';
      #endif
      }

   TR::CodeCacheConfig * self();

   size_t trampolineCodeSize() const { return _trampolineCodeSize; }
   int32_t trampolineSpacePercentage() const { return _trampolineSpacePercentage; }
   size_t ccPreLoadedCodeSize() const { return _CCPreLoadedCodeSize; }
   int32_t numRuntimeHelpers() const { return _numOfRuntimeHelpers; }
   size_t codeCacheKB() const { return _codeCacheKB; }
   size_t codeCachePadKB() const { return _codeCachePadKB; }
   size_t codeCacheTotalKB() const { return _codeCacheTotalKB; }
   size_t codeCacheAlignment() const { return _codeCacheAlignment; }

   // Alignment for per-code cache helpers
   //
   size_t codeCacheHelperAlignment() const { return _codeCacheHelperAlignmentBytes; }
   void setCodeCacheHelperAlignment(size_t a) { _codeCacheHelperAlignmentBytes = a; }

   size_t codeCacheHelperAlignmentMask() const { return _codeCacheHelperAlignmentBytes-1; }

   //  align trampolines on this byte boundary
   //
   size_t codeCacheTrampolineAlignmentBytes() const { return _codeCacheTrampolineAlignmentBytes; }

   int32_t codeCacheMethodBodyAllocRetries() const { return _codeCacheMethodBodyAllocRetries; }

   size_t codeCacheTempTrampolineSyncArraySize() const { return _codeCacheTempTrampolineSyncArraySize; }

   size_t codeCacheHashEntryAllocatorSlabSize() const { return _codeCacheHashEntryAllocatorSlabSize; }

   size_t largeCodePageSize() const { return _largeCodePageSize; }
   uint32_t largeCodePageFlags() const { return _largeCodePageFlags; }
   bool allowedToGrowCache() const { return _allowedToGrowCache; }
   bool needsMethodTrampolines() const { return _needsMethodTrampolines; }
   size_t lowCodeCacheThreshold() const { return _lowCodeCacheThreshold; }
   int32_t maxNumberOfCodeCaches() const { return _maxNumberOfCodeCaches; }
   bool canChangeNumCodeCaches() const { return _canChangeNumCodeCaches; }
   bool codeCacheFreeBlockRecylingEnabled() const { return _codeCacheFreeBlockRecylingEnabled; }
   bool verbosePerformance() const { return _verbosePerformance; }
   bool verboseCodeCache() const { return _verboseCodeCache; }
   bool verboseReclamation() const { return _verboseReclamation; }
   bool doSanityChecks() const { return _doSanityChecks; }

   CodeCacheCodeGenCallbacks & mccCallbacks() { return _mccCallbacks; }

   bool emitElfObject() const { return _emitElfObject; }

   int32_t _trampolineCodeSize;          /*!< size of the trampoline code in bytes */
   int32_t _CCPreLoadedCodeSize;         /*!< size of the pre-Loaded CodeCache Helpers code in bytes */
   int32_t _numOfRuntimeHelpers;         /*!< number of runtime helpers */

   size_t _codeCacheKB;
   size_t _codeCacheTotalKB;
   size_t _codeCachePadKB;
   size_t _codeCacheAlignment;


   size_t _largeCodePageSize;
   uint32_t _largeCodePageFlags;

   bool _allowedToGrowCache;             /*!< does runtime permit growing the code cache once exhausted? */
   bool _needsMethodTrampolines;         /*!< true if method trampolines are needed */
   uint32_t _trampolineSpacePercentage;
   size_t _lowCodeCacheThreshold;        /*!< threshold to consider available code cache "low" */
   int32_t _maxNumberOfCodeCaches;       /*!< how many code caches allowed to have */
   bool _canChangeNumCodeCaches;
   bool _verbosePerformance;             /*!< should performance related events be reported to the verbose log */
   bool _verboseCodeCache;               /*!< should code cache events be reported to the verbose log */
   bool _verboseReclamation;             /*!< should reclamation events be reported to the verbose log */
   bool _doSanityChecks;
   bool _codeCacheFreeBlockRecylingEnabled;

   CodeCacheCodeGenCallbacks _mccCallbacks;              /*!< codeGen call backs */

   bool _emitElfObject;                  /*!< emit code cache as ELF object on shutdown */

   char * const warmEyeCatcher() { return _warmEyeCatcher; }

   char * const coldEyeCatcher() { return _coldEyeCatcher; }

   protected:

   char _warmEyeCatcher[4];
   char _coldEyeCatcher[4];

   size_t _codeCacheHelperAlignmentBytes;
   size_t _codeCacheTrampolineAlignmentBytes;
   int32_t _codeCacheMethodBodyAllocRetries;
   size_t _codeCacheTempTrampolineSyncArraySize;
   size_t _codeCacheHashEntryAllocatorSlabSize;
   };

} // namespace OMR

#endif // OMR_CODECACHECONFIG_INCL
