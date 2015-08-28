#ifndef FILE_MAPPING_H
#define FILE_MAPPING_H

#include <vector>
#include <memory>

namespace filemap
{
	enum Prot {
		READONLY = 0x001, // equivalent to PAGE_READONLY
		READWRITE = 0x002 // equivalent to PAGE_READWRITE
	};

	enum Map {
		LARGE_PAGES = 0x004, // equivalent to SEC_LARGE_PAGES // Large-page memory regions may be difficult to obtain 
		//ANONYMOUS = 0x040, // equivalent to MAP_ANONYMOUS
		READ_AHEAD = 0x080 // reads the whole file into memory on mapping
	};

	enum File {
		BEGIN = 0x001, // equivalent to FILE_BEGIN
		END = 0x002, // equivalent to FILE_END
		CURRENT = 0x004, // equivalent to FILE_CURRENT
	};

	typedef int FLAGS;

	class FileMapping {
	public:
		FileMapping(LPVOID, const size_t&, const FLAGS&, const FLAGS&, HANDLE, const off_t&);
		FileMapping(LPVOID, const size_t&, const FLAGS&, const FLAGS&, const char*, const off_t&);
		FileMapping(const FileMapping&);
		FileMapping(FileMapping&&);

		FileMapping& operator=(FileMapping&&);
		FileMapping& operator=(const FileMapping&);
		char& operator[](const off_t&);

		bool Flush(const off_t&, const size_t&) const;
		void Write(const char*, const size_t&);
		void Read(char*, const size_t&);
		bool Load(const off_t&, const size_t&);
		char* GetMapPointer() const;
		char* SetMapPointer(const ptrdiff_t&, const FLAGS&);

	private:
		struct SharedData;
		std::shared_ptr<SharedData> mSharedData;
		char* mMapPointer;
		
		static DWORD sLargePageSize;
		static DWORD sPageSize;

		inline void SetMapLength(const size_t&);
		inline DWORD GetPageSize() const;
		inline DWORD GetProtectionFlags() const;
		inline DWORD GetPageFlags() const;

		void Allocate(LPVOID);
		//void LoadAnonymous(const off_t&, const size_t&);

		enum Inner {
			CLOSE_FILE_HANDLE = 0x1000
		};

		struct SharedData {
			SharedData(const FLAGS& prot, const FLAGS& flags, const off_t& offset) : 
				mFlags(prot | flags), mOffset(offset) {}

			~SharedData() {
				VirtualFree(mMap, mLength, MEM_RELEASE);
				if (mFlags & Inner::CLOSE_FILE_HANDLE)
					CloseHandle(mFile);
			}

			std::vector<bool> mBitmap;
			HANDLE mFile;
			char* mMap;
			FLAGS mFlags;
			size_t mLength;
			off_t mOffset;
		};
	};
}

#endif
