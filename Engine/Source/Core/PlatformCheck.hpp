#ifdef _WIN32
	/** Windows x64/x86 */
	#ifdef _WIN64
	/** Windows x64  */
	#define PLATFORM_WINDOWS
	#else
	/** Windows x86 */
	#error "x86 Builds are not supported!"
	#endif
#elif defined(__linux__)
	#define PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	#error "Unknown platform!"
#endif