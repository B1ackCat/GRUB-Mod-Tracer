#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "EFIImageLoader.h"

EFI_STATUS EFIAPI UefiUnload(EFI_HANDLE imageHandle) { return EFI_SUCCESS; }

EFI_STATUS EFIAPI ImgBaseDebugEntryPoint(
	EFI_HANDLE ImageHandle, 
	EFI_SYSTEM_TABLE* SystemTable
) {
	CHAR16* IMAGEPATH = L"\\EFI\\ubuntu\\grubx64.efi";
	LoadImage(IMAGEPATH);
	return EFI_SUCCESS;
}
