#include <vector>
#include <string>
#include <memory>
#include <cmath>
#include <Windows.h>
#include "FileMapping.h"

namespace filemap
{
	DWORD FileMapping::sPageSize = []() {
		SYSTEM_INFO sysInfo;
		GetSystemInfo(&sysInfo);
		return sysInfo.dwPageSize;
	} ();
	DWORD FileMapping::sLargePageSize = GetLargePageMinimum();

	FileMapping::FileMapping(LPVOID addr, const size_t& length, const FLAGS& prot, const FLAGS& flags, HANDLE file, const off_t& offset) :
		mSharedData(std::make_shared<SharedData>(prot, flags, offset)) {
		mSharedData->mFile = file;
		SetMapLength(length);
		Allocate(addr);
		if (mSharedData->mFlags & Map::READ_AHEAD)
			Load(0, mSharedData->mLength);
	}

	FileMapping::FileMapping(LPVOID addr, const size_t& length, const FLAGS& prot, const FLAGS& flags, const char* fileName, const off_t& offset) :
		mSharedData(std::make_shared<SharedData>(prot, flags, offset)) {
		mSharedData->mFile = CreateFile(
			TEXT(fileName), 
			GetProtectionFlags(), 
			0, 
			NULL, 
			OPEN_ALWAYS, 
			FILE_ATTRIBUTE_NORMAL, 
			NULL
			);
		mSharedData->mFlags |= Inner::CLOSE_FILE_HANDLE;
		SetMapLength(length);
		Allocate(addr);
		if (mSharedData->mFlags & Map::READ_AHEAD)
			Load(0, mSharedData->mLength);
	}

	FileMapping::FileMapping(const FileMapping& other) : 
		mSharedData(other.mSharedData), mMapPointer(other.mMapPointer) {}

	FileMapping::FileMapping(FileMapping&& other) : 
		mSharedData(std::move(other.mSharedData)), mMapPointer(other.mMapPointer) {}

	FileMapping& FileMapping::operator=(const FileMapping& rhs) {
		if (this != &rhs) {
			mSharedData = rhs.mSharedData;
			mMapPointer = rhs.mMapPointer;
		}
		return *this;
	}
	
	FileMapping& FileMapping::operator=(FileMapping&& rhs) {
		if (this != &rhs) {
			std::swap(mSharedData, rhs.mSharedData);
			mMapPointer = rhs.mMapPointer;
		}
		return *this;
	}

	char& FileMapping::operator[](const off_t& offset) {
		if (!(mSharedData->mFlags & Map::READ_AHEAD))
			Load(offset, 1);
		return *(mSharedData->mMap + offset);
	}

	bool FileMapping::Flush(const off_t& offset, const size_t& bytes) const {
		if (mSharedData->mFlags & Prot::READONLY) return false;
		DWORD newFilePointer = SetFilePointer(mSharedData->mFile, mSharedData->mOffset + offset, NULL, FILE_BEGIN);
		DWORD oldFilePointer = SetFilePointer(mSharedData->mFile, 0, NULL, FILE_CURRENT);

		if (newFilePointer == INVALID_SET_FILE_POINTER &&
			GetLastError() != NO_ERROR)
			return false;

		BOOL result = WriteFile(mSharedData->mFile, mSharedData->mMap + offset, bytes, NULL, NULL);
		if (mSharedData->mFlags & Inner::CLOSE_FILE_HANDLE)
			SetFilePointer(mSharedData->mFile, oldFilePointer, NULL, FILE_BEGIN);
		return result;
	}

	void FileMapping::Write(const char* buff, const size_t& bytes) {
		if (!(mSharedData->mFlags & Map::READ_AHEAD))
			Load(mMapPointer - mSharedData->mMap, bytes);
		std::memcpy(mMapPointer, buff, bytes);
		mMapPointer += bytes;
	}

	void FileMapping::Read(char* buff, const size_t& bytes) {
		if (!(mSharedData->mFlags & Map::READ_AHEAD))
			Load(mMapPointer - mSharedData->mMap, bytes);
		std::memcpy(buff, mMapPointer, bytes);
		mMapPointer += bytes;
	}

	char* FileMapping::GetMapPointer() const {
		return mMapPointer;
	}

	char* FileMapping::SetMapPointer(const ptrdiff_t& distance, const FLAGS& moveMethod) {
		switch (moveMethod) 
		{
		case (File::BEGIN) : 
			mMapPointer = mSharedData->mMap + distance;
			break;

		case (File::END) : 
			mMapPointer = mSharedData->mMap + mSharedData->mLength - distance;
			break;

		case (File::CURRENT) : 
			mMapPointer += distance; 
			break;
		}
		return mMapPointer;
	}

	inline void FileMapping::SetMapLength(const size_t& requested) {
		mSharedData->mLength = requested > 0 ? requested : GetFileSize(mSharedData->mFile, NULL) - mSharedData->mOffset;
	}

	inline DWORD FileMapping::GetPageFlags() const {
		return (mSharedData->mFlags & Map::LARGE_PAGES ? MEM_LARGE_PAGES : 0);
	}

	inline DWORD FileMapping::GetProtectionFlags() const {
		return (mSharedData->mFlags & Prot::READWRITE ? GENERIC_WRITE : 0) | GENERIC_READ;
	}

	inline DWORD FileMapping::GetPageSize() const {
		return mSharedData->mFlags & Map::LARGE_PAGES ? sLargePageSize : sPageSize;
	}

	/*void FileMapping::LoadAnonymous(const off_t& offset, const size_t& bytes) {
		DWORD pageSize = GetPageSize();
		size_t firstPage = floor(offset / (float)pageSize);
		size_t pages = ceil(bytes / (float)pageSize);

		for (size_t i = firstPage; i < pages; ++i) {
			if (!mSharedData->mBitmap[i]) {
				VirtualAlloc(
					mSharedData->mMap + i * pageSize,
					pageSize,
					GetPageFlags() | MEM_COMMIT,
					PAGE_READWRITE
					);
				mSharedData->mBitmap[i] = true;
			}
		}
	}*/

	bool FileMapping::Load(const off_t& offset, const size_t& bytes) {
		/*if (mSharedData->mFlags & Map::ANONYMOUS) {
			LoadAnonymous(offset, bytes);
			return true;
		}*/
		DWORD oldFilePointer = SetFilePointer(mSharedData->mFile, 0, NULL, FILE_CURRENT);

		DWORD pageSize = GetPageSize();
		size_t firstPage = floor(offset / (float)pageSize);
		size_t pages = ceil(bytes / (float)pageSize);

		for (size_t i = firstPage; i < pages; ++i) {
			if (!mSharedData->mBitmap[i]) {
				DWORD newFilePointer = SetFilePointer(mSharedData->mFile, mSharedData->mOffset + i * pageSize, NULL, FILE_BEGIN);
				if (newFilePointer == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR)
					return false;

				VirtualAlloc(
					mSharedData->mMap + i * pageSize,
					pageSize,
					GetPageFlags() | MEM_COMMIT,
					PAGE_READWRITE
					);

				if (!ReadFile(mSharedData->mFile, mSharedData->mMap + i * pageSize, pageSize, NULL, NULL)) {
					VirtualFree(mSharedData->mMap + i * pageSize, pageSize, MEM_DECOMMIT);
					return false;
				}
				mSharedData->mBitmap[i] = true;
			}
		}

		if (!(mSharedData->mFlags & Inner::CLOSE_FILE_HANDLE))
			SetFilePointer(mSharedData->mFile, oldFilePointer, NULL, FILE_BEGIN);
		return true;
	}

	void FileMapping::Allocate(LPVOID addr) {
		mSharedData->mMap = static_cast<char*>(VirtualAlloc(
			addr,
			mSharedData->mLength,
			GetPageFlags() | MEM_RESERVE,
			PAGE_READWRITE
			));
		mSharedData->mBitmap = std::vector<bool>(ceil(mSharedData->mLength/(float)GetPageSize()));
		mMapPointer = mSharedData->mMap;
	}
}
