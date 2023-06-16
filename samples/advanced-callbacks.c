typedef unsigned long long  UINT64;
typedef long long           INT64;
typedef unsigned int        UINT32;
typedef int                 INT32;
typedef unsigned short      UINT16;
typedef unsigned short      CHAR16;
typedef short               INT16;
typedef unsigned char       BOOLEAN;
typedef unsigned char       UINT8;
typedef char                CHAR8;
typedef char                INT8;
typedef UINT32 UINTN;
#define VOID  void
#define EFIAPI  __attribute__((cdecl))

typedef UINTN RETURN_STATUS;
typedef RETURN_STATUS EFI_STATUS;

//
// Modifiers for Data Types used to self document code.
// This concept is borrowed for UEFI specification.
//

///
/// Datum is passed to the function.
///
#define IN

///
/// Datum is returned from the function.
///
#define OUT

///
/// Passing the datum to the function is optional, and a NULL
/// is passed if the value is not supplied.
///
#define OPTIONAL

typedef struct _EFI_USB_IO_PROTOCOL EFI_USB_IO_PROTOCOL;

// Make the typedef of the callback type generic
typedef
_For_any(T)
EFI_STATUS
(EFIAPI * _Single EFI_ASYNC_USB_TRANSFER_CALLBACK)(
  IN VOID         *Data : byte_count(DataLength),
  IN UINTN        DataLength,
  IN T            * _Single Context,
  IN UINT32       Status
  );

// Make the typedef of a function type for a function that takes an optional
// callback function and data for the callback function generic.
//
// Commented out because it doesn't compile yet.

/*
typedef
_For_any(S)
EFI_STATUS
(EFIAPI * _Single EFI_USB_IO_ASYNC_ISOCHRONOUS_TRANSFER)(
  IN EFI_USB_IO_PROTOCOL              * _Single This,
  IN UINT8                            DeviceEndpoint,
  IN OUT VOID                         * Data : byte_count(DataLength),
  IN     UINTN                        DataLength,
  // TODO: implement type application of typedefs in compiler
  IN EFI_ASYNC_USB_TRANSFER_CALLBACK<S>  IsochronousCallBack,
  IN S                             * _Single Context OPTIONAL
  );
*/

// The previous typedef doesn't compile yet.  This version
// compiles.  It substitutes in the definition of 
// EFI_ASYNC_USB_TRANSFER_CALLBACK at its use.

typedef
_For_any(S)
EFI_STATUS
(EFIAPI *EFI_USB_IO_ASYNC_ISOCHRONOUS_TRANSFER)(
  IN EFI_USB_IO_PROTOCOL              * _Single This,
  IN UINT8                            DeviceEndpoint,
  IN OUT VOID                         *Data,
  IN     UINTN                        DataLength,
  // Substitution of EFI_ASYNC_USB_TRANSFER_CALLBACK
  IN _Ptr<EFI_STATUS EFIAPI(
    IN VOID         *Data : byte_count(DataLength),
    IN UINTN        DataLength,
    IN S            * _Single Context,
    IN UINT32       Status
   )>  IsochronousCallBack,
  IN S                             * _Single Context OPTIONAL
  );
//  */

