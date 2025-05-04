#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DevicePathLib.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>

EFI_STATUS LocateGrub(
    IN CHAR16* ImagePath,
    OUT EFI_DEVICE_PATH** LoaderDevicePath
) {
    EFI_FILE_IO_INTERFACE* ioDevice;
    EFI_HANDLE* handleArr;
    UINTN nbHandle, i;
    EFI_STATUS Status;
    EFI_FILE_PROTOCOL* handleRoots;
    EFI_FILE_PROTOCOL* bootFile;

    *LoaderDevicePath = (EFI_DEVICE_PATH*)NULL;
    Status = gBS->LocateHandleBuffer(ByProtocol, &gEfiSimpleFileSystemProtocolGuid, NULL, &nbHandle, &handleArr);
    if (Status != EFI_SUCCESS)
      return Status;

    for (i = 0; i < nbHandle; i++)
    {
        Status = gBS->HandleProtocol(handleArr[i], &gEfiSimpleFileSystemProtocolGuid, (void**)&ioDevice);
        if (Status != EFI_SUCCESS)
            continue;

        Status = ioDevice->OpenVolume(ioDevice, &handleRoots);
        if (Status != EFI_SUCCESS)
            continue;

        Status = handleRoots->Open(handleRoots, &bootFile, ImagePath, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
        if (Status == EFI_SUCCESS)
        {
            handleRoots->Close(bootFile);
            *LoaderDevicePath = FileDevicePath(handleArr[i], ImagePath);
            break;
        }
    }

    return EFI_SUCCESS;
}

VOID PrintImageBase(
    IN EFI_HANDLE* ImageHandle
) {
    EFI_LOADED_IMAGE_PROTOCOL* imageInfo;

    /* Get Image Info */
    gBS->HandleProtocol(*ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&imageInfo);

    Print(L"[+] GRUB2 ImageBase: 0x%p\n", imageInfo->ImageBase);
    Print(L"Press any key to continue...\n"); 
    gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, NULL);
}

EFI_STATUS LoadImage(IN CHAR16* ImagePath) {
    EFI_DEVICE_PATH* devicePath;
    EFI_STATUS Status;
    EFI_HANDLE ImageHandle = NULL;

    Status = LocateGrub(ImagePath, &devicePath);
    if (EFI_ERROR(Status) || (!devicePath)) {
        Print(L"Failed to find image\n");
        return Status;
    }

    /* Load image in memory */
    Status = gBS->LoadImage(TRUE, gImageHandle, devicePath, NULL, 0, &ImageHandle);
    if (Status != EFI_SUCCESS) {
        Print(L"Failed to load image\n");
        return Status;
    }

    PrintImageBase(&ImageHandle);

    gBS->StartImage(ImageHandle, (UINTN*)NULL, (CHAR16**)NULL);

    return EFI_SUCCESS;
}